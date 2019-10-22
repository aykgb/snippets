#pragma once

#include <string.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "rapid_json_parser.h"
#include "rapidjson/error/en.h"
#include "rapidjson/error/error.h"
#include "rapidjson/reader.h"

using namespace std;
using namespace rapidjson;

#if defined(__GNUC__)
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(effc++)
#endif

#ifdef __clang__
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(switch - enum)
#endif

struct default_filter {
  typedef bool result_type;
  result_type operator()(string const&) const { return false; }
};

typedef std::pair<string, string> return_type;
inline return_type split_with_last_of_delimiter(const char* chars, size_t len,
                                                char delim) {
  //! point to the position of delimiter if it exists.
  char* pcur = strrchr(const_cast<char*>(chars), delim);
  //! point to the next position of the last character.
  char* pend = const_cast<char*>(chars) + len;
  if (pcur == nullptr || pcur == chars || pend == pcur + 1) {
    return return_type();
  }
  return return_type(string(const_cast<char*>(chars), pcur),
                     string(pcur + 1, pend));
}

/**
 *  {
 *      "tablename": "gs_driver",
 *      "key": "WL_DRIVER_xxxxxxxxxx",
 *      "fieldvalues": [
 *          "key\tvalue",
 *          "key\tvalue",
 *          ......
 *      ],
 *      "ttl": "1234"
 *  }
 */
struct GizzleFormatHandler
    : public BaseReaderHandler<UTF8<>, GizzleFormatHandler> {
  GizzleFormatHandler()
      : filter(default_filter()), state_(kExpectObjectStart) {}

  bool StartObject() {
    switch (state_) {
      case kExpectObjectStart:
        state_ = kExpectNameOrObjectEnd;
        return true;
      default:
        err_code = 1;
        return false;
    }
  }

  bool String(const char* str, SizeType length, bool) {
    switch (state_) {
      case kExpectNameOrObjectEnd:
        name_ = string(str, length);
        state_ = kExpectValue;
        return true;
      case kExpectValue:
        if (name_ == "key") {
          parse_flag_ |= HAS_KEY_;
          row_key.assign(str, length);
          if (filter(row_key)) {
            //! match the filter, then stop parsing
            err_code = 2;
            return false;
          }
        } else if (name_ == "ttl") {
          parse_flag_ |= HAS_TTL_;
          ttl = std::stoll(string(str, length));
        } else {
        }
        state_ = kExpectNameOrObjectEnd;
        return true;
      case kExpectValueArray:
        if (name_ != "fieldvalues") {
          parse_flag_ &= ~HAS_FVS_;
          err_code = 3;
          return false;
        }
        //! find first of '\t'
        {
          char* pcur = strchr(const_cast<char*>(str), '\t');
          char* pend = const_cast<char*>(str) + length;
          if (pcur == nullptr || pcur == str /*|| pend == pcur + 1*/) {
            err_code = 4;
            return false;
          }
          field_values.emplace(string(const_cast<char*>(str), pcur),
                               string(pcur + 1, pend));
        }
        return true;
      default:
        err_code = 5;
        return false;
    }
  }

  bool EndObject(SizeType) { return state_ == kExpectNameOrObjectEnd; }

  bool StartArray() {
    switch (state_) {
      case kExpectValue:
        state_ = kExpectValueArray;
        return true;
      default:
        err_code = 6;
        return false;
    }
  }

  bool EndArray(SizeType) {
    switch (state_) {
      case kExpectValueArray:
        state_ = kExpectNameOrObjectEnd;
        return true;
      default:
        err_code = 7;
        return false;
    }
  }

  bool Default() {
    err_code = 8;
    return false;
  }  // All other events are invalid.

  /*
   * support user-defined filter
   * if filter(key) == true, the parsing process shall be terminated.
   */
  std::function<bool(std::string&)> filter;

  // MessageMap messages_;
  // ArrayMap arrays_;
  enum State {
    kExpectObjectStart,
    kExpectNameOrObjectEnd,
    kExpectValue,
    kExpectValueArray
  } state_;
  std::string name_;
  std::string row_key;
  std::map<string, string> field_values;
  int64_t ttl = -1;

  constexpr static uint32_t HAS_KEY_ = 0b0001;
  constexpr static uint32_t HAS_TTL_ = 0b0010;
  constexpr static uint32_t HAS_FVS_ = 0b0100;
  constexpr static uint32_t HAS_ALL_ = 0b0111;

  uint32_t parse_flag_ = HAS_FVS_;
  bool IsValid() { return parse_flag_ == HAS_ALL_; }

  int32_t err_code = 0;
  void Reset() {
    err_code = 0;
    field_values.clear();
    parse_flag_ = HAS_FVS_;
    state_ = kExpectObjectStart;
  }
};

#if defined(__GNUC__)
RAPIDJSON_DIAG_POP
#endif

#ifdef __clang__
RAPIDJSON_DIAG_POP
#endif

class RapidJsonParser {
  uint64_t parse_err_nums = 0;

 public:
  RapidJsonParser(){};
  virtual ~RapidJsonParser(){};
  //解析成key,value
  virtual bool Decode(const string& info, string& key, string& val);
  //解析成map <key，value>结构
  virtual bool MDecode(const string& info, map<string, string>& kvs);
  //解析成hash结构
  virtual bool HDecode(const string& info, string& key,
                       map<string, string>& fvs);
  //解析成hash结构
  virtual bool HDecodeWithTTL(const string& info, string& key,
                              map<string, string>& fvs, int64_t& ttl);
};