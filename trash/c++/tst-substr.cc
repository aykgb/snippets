#include <string>
#include <iostream>

using namespace std;

int main() {
    string md5_path = "/user/engine_arch/wangchenglin/dict_popup_accept_rate_64.gz.md5";
    string md5_path_local = "/home/xiaoju/dc_build_data/local_data/md5//dict_popup_accept_rate_64.gz_5.md5";

    std::cout << md5_path.substr(md5_path.find_last_of("/") + 1) << std::endl;
    std::cout << md5_path_local.substr(0, md5_path_local.find_last_of("/")) << std::endl;

    return 0;
}
