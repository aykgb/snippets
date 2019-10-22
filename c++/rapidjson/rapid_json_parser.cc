// Reading a message JSON with Reader (SAX-style API).
// The JSON should be an object with key-string pairs.
#include "rapid_json_parser.h"
#include <thread>

thread_local Reader reader;
thread_local GizzleFormatHandler handler;
bool RapidJsonParser::Decode(const string& info, string& key, string& val) {
  return false;
}
bool RapidJsonParser::MDecode(const string& info, map<string, string>& kvs) {
  return false;
}
bool RapidJsonParser::HDecode(const string& info, string& key,
                              map<string, string>& fvs) {
  return false;
}
bool RapidJsonParser::HDecodeWithTTL(const string& json, std::string& key,
                                     std::map<string, string>& fvs,
                                     int64_t& ttl) {
  if (json.size() == 0) {
    log_error("HDecodeWithTTL input is empty.");
    return false;
  }

  StringStream ss(json.c_str());
  if (!reader.Parse(ss, handler)) {
    parse_err_nums++;
    ParseErrorCode e = reader.GetParseErrorCode();
    size_t o = reader.GetErrorOffset();
    if (parse_err_nums % 10000 == 0 || parse_err_nums < 1000) {
      log_error(
          "HDecodeWithTTL err_code %d json %s parse failed, inner err_code %d "
          "err %s at offset %lu near '%s...' total parsing error nums %ld",
          handler.err_code, json.c_str(), e, GetParseError_En(e), o,
          json.substr(o, 10).c_str(), parse_err_nums);
    }
    handler.Reset();
    return false;
  }
  if (!handler.IsValid()) {
    log_error("HDecodeWithTTL json %s format is not valid, flag %d",
              json.c_str(), handler.parse_flag_);
    handler.Reset();
    return false;
  }

  key.swap(handler.row_key);
  fvs.swap(handler.field_values);
  ttl = handler.ttl;
  handler.Reset();

  return true;
}

/*
 * demo of using rapidjson parse dict json file.
 */
typedef map<string, string> MessageMap;
typedef map<string, vector<string>> ArrayMap;

#if defined(__GNUC__)
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(effc++)
#endif

#ifdef __clang__
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(switch - enum)
#endif

struct MessageHandler : public BaseReaderHandler<UTF8<>, MessageHandler> {
  MessageHandler() : messages_(), state_(kExpectObjectStart), name_() {}

  bool StartObject() {
    switch (state_) {
      case kExpectObjectStart:
        state_ = kExpectNameOrObjectEnd;
        return true;
      default:
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
        messages_.insert(MessageMap::value_type(name_, string(str, length)));
        state_ = kExpectNameOrObjectEnd;
        return true;
      case kExpectValueArray:
        arrays_[name_].emplace_back(string(str, length));
        return true;
      default:
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
        return false;
    }
  }

  bool EndArray(SizeType) {
    switch (state_) {
      case kExpectValueArray:
        state_ = kExpectNameOrObjectEnd;
        return true;
      default:
        return false;
    }
  }

  bool Default() { return false; }  // All other events are invalid.

  MessageMap messages_;
  ArrayMap arrays_;
  enum State {
    kExpectObjectStart,
    kExpectNameOrObjectEnd,
    kExpectValue,
    kExpectValueArray
  } state_;
  std::string name_;

  void Reset() {
    state_ = kExpectObjectStart;
    messages_.clear();
    arrays_.clear();
  }
};

#if defined(__GNUC__)
RAPIDJSON_DIAG_POP
#endif

#ifdef __clang__
RAPIDJSON_DIAG_POP
#endif

static Reader reader;
static MessageHandler handler;
static void ParseMessages(const char* json, MessageMap& messages,
                          ArrayMap& arrays) {
  StringStream ss(json);
  if (reader.Parse(ss, handler)) {
    messages.swap(handler.messages_);  // Only change it if success.
    arrays.swap(handler.arrays_);
  } else {
    ParseErrorCode e = reader.GetParseErrorCode();
    size_t o = reader.GetErrorOffset();
    cout << "Error: " << GetParseError_En(e) << endl;
    ;
    cout << " at offset " << o << " near '" << string(json).substr(o, 10)
         << "...'" << endl;
  }
  handler.Reset();
}

int TEST_RapidJsonParser() {
  MessageMap messages;
  ArrayMap arrays;

  const char* json1 =
      "{ \"greeting\" : \"Hello!\", \"farewell\" : \"bye-bye!\" }";
  cout << json1 << endl;
  ParseMessages(json1, messages, arrays);

  for (MessageMap::const_iterator itr = messages.begin(); itr != messages.end();
       ++itr)
    cout << itr->first << ": " << itr->second << endl;

  cout << endl << "Parse a JSON with invalid schema." << endl;
  const char* json2 =
      "{ \"greeting\" : \"Hello!\", \"farewell\" : \"bye-bye!\", \"foo\" : {} "
      "}";
  cout << json2 << endl;
  ParseMessages(json2, messages, arrays);

  ifstream ifs;
  ifs.open("./dict_safe_driver_profiling");
  string json3;
  std::getline(ifs, json3);

  cout << json3 << endl;
  ParseMessages(json3.c_str(), messages, arrays);
  for (auto& itr : messages) {
    cout << itr.first << ": " << itr.second << endl;
  }
  for (auto& itr : arrays) {
    cout << itr.first << ":\n";
    for (auto& v : itr.second) {
      cout << "   ==> " << v << "\n";
    }
  }

  return 0;
}
