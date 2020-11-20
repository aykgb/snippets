#include <string.h>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

using namespace std;

typedef std::pair<string, string> return_type;
inline return_type split_with_last_of_delim(const char *chars, size_t len,
                                            char delim) {
    //! point to the position of delimiter if it exists.
    char *pcur = strrchr(const_cast<char *>(chars), delim);
    //! point to the next position of the last character.
    char *pend = const_cast<char *>(chars) + len;
    if (pcur != nullptr) {
        if (pcur != chars) {
            // std::cout << "|" << string(const_cast<char*>(chars), pcur) << "|"
            // << std::endl;
            /*
            if(pcur + 1 != pend) {
                // std::cout << "|" << string(pcur + 1, pend) << "|" <<
            std::endl; return return_type( string(const_cast<char*>(chars),
            pcur), string(pcur + 1, pend)
                );
            } else {
                // std::cout << "field value empty.\n";
            }
            */
            return return_type(string(const_cast<char *>(chars), pcur),
                               string(pcur + 1, pend));
        } else {
            // std::cout << "field key empty.\n";
        }
    } else {
        // std::cout << "not found\n";
    }
    return return_type();
}

typedef std::pair<string, string> return_type;
inline return_type split_with_first_of_delim(const char *chars, size_t len,
                                             char delim) {
    //! point to the position of delimiter if it exists.
    char *pcur = strchr(const_cast<char *>(chars), delim);
    //! point to the next position of the last character.
    char *pend = const_cast<char *>(chars) + len;
    if (pcur != nullptr) {
        if (pcur != chars) {
            // std::cout << "|" << string(const_cast<char*>(chars), pcur) << "|"
            // << std::endl;
            /*
            if(pcur + 1 != pend) {
                // std::cout << "|" << string(pcur + 1, pend) << "|" <<
            std::endl; return return_type( string(const_cast<char*>(chars),
            pcur), string(pcur + 1, pend)
                );
            } else {
                // std::cout << "field value empty.\n";
            }
            */
            return return_type(string(const_cast<char *>(chars), pcur),
                               string(pcur + 1, pend));
        } else {
            // std::cout << "field key empty.\n";
        }
    } else {
        // std::cout << "not found\n";
    }
    return return_type();
}

int main() {
    auto split = [](const char *chars, size_t len, char delim) {
        std::cout << "-----------------------------------\n";
        char *pcur = strrchr(const_cast<char *>(chars), delim);
        char *pend = const_cast<char *>(chars) +
                     len;  //! Point to the next position of the last character.
        std::cout << "chars=" << string(const_cast<char *>(chars), pend)
                  << "|\n";
        if (pcur != nullptr) {
            if (pcur != chars) {
                std::cout << "|" << string(const_cast<char *>(chars), pcur)
                          << "|" << std::endl;
                if (pcur + 1 != pend) {
                    std::cout << "|" << string(pcur + 1, pend) << "|"
                              << std::endl;
                } else {
                    std::cout << "field value empty.\n";
                }
            } else {
                std::cout << "field key empty.\n";
            }
        } else {
            std::cout << "not found\n";
        }
    };

    split("aaaa\tbbbb", 9, '\t');
    split("\taaaabbbb", 9, '\t');
    split("aaaabbbb\t", 9, '\t');
    split("aaaacbbbbd", 9, '\t');

    std::cout << "1-----------------------------------\n";
    auto ret = split_with_first_of_delim("aaaa\tbbbb", 9, '\t');
    std::cout << "|" << ret.first << "|" << ret.second << "|" << std::endl;
    std::cout << "2-----------------------------------\n";
    ret = split_with_first_of_delim("aaaabbbb", 8, '\t');
    std::cout << "|" << ret.first << "|" << ret.second << "|" << std::endl;
    std::cout << "3-----------------------------------\n";
    ret = split_with_first_of_delim("aaaabbbb\t", 9, '\t');
    std::cout << "|" << ret.first << "|" << ret.second << "|" << std::endl;
    std::cout << "4-----------------------------------\n";
    ret = split_with_first_of_delim("\taaaabbbb", 9, '\t');
    std::cout << "|" << ret.first << "|" << ret.second << "|" << std::endl;
    std::cout << "5-----------------------------------\n";
    ret = split_with_first_of_delim("\taaa\tabbbb", 10, '\t');
    std::cout << "|" << ret.first << "|" << ret.second << "|" << std::endl;

    std::vector<string> vstrs;
    auto boost_split = [&](std::string sstr) {
        boost::split(vstrs, sstr, boost::is_any_of("\t"));
        std::cout << "----------------- boost split ------------------\n";
        if (vstrs.empty()) {
            std::cout << "split |" << sstr << "| fail." << std::endl;
        } else {
            for (auto &strs : vstrs) {
                std::cout << "|" << strs;
            }
            std::cout << "|" << std::endl;
        }
    };

    boost_split("aaaa\tbbbb");
    boost_split("aaaabbbbb");
    boost_split("\taaaaaabbbbb");
    boost_split("aaaaaabbbbb\t");
    boost_split("aaaaa\tabbbbb\t");
    boost_split("\taaaaa\tabbbbb\t");
    boost_split("\taaaaa\tabbbbb");

    std::map<string, string> fvs;
    fvs[""] = "empty string";
    fvs["key1"] = "";

    auto found = fvs.find("");
    if (found != fvs.end()) {
        std::cout << "founnd value=" << found->second << std::endl;
    }

    return 0;
}
