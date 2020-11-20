#include <string.h>
#include <string>
#include <iostream>

using namespace std;

int main() {

    const char * orig = "fserv_basic_info\t{\"total_tousu_num\":0,\"low_star_num\":0,\"cancle_order_num\":0,\"score_diff\":0,\"diff_detail\":[],\"priority\":0.07994548,\"ser_good\":[23,25,24],\"ser_bad\":[12],\"service_score\":67,\"latest_order_num\":300,\"min_normal_score\":60,\"max_normal_score\":85,\"receive_order\":9,\"cancel_point\":0.3160,\"tousu_point\":1.0000,\"comment_point\":0.5260,\"check_status\":3,\"check_begin\":\"00000000\",\"check_end\":\"00000000\",\"only_display\":0,\"remit_detail\":[[35,0],[36,0]],\"history_remit_detail\":[[35,0],[36,0]],\"appealable_order\":{\"cash_complain\":[]},\"appeal_info\":[],\"all_appealable_order\":[],\"score_segment\":[0,60,70,75,80,85,88,91,94,97,100],\"score_segment_priority\":[0.035055645,0.07994548,0.12728232,0.21496987,0.40244493,0.5634439,0.72871923,0.84938496,0.9277857,0.99999]}";
    size_t length = strlen(orig);

    char *pcur = strchr(const_cast<char*>(orig), '\t');

    std::cout << pcur-orig+1 << std::endl;
    int key_len = pcur - orig;
    std::cout << "key:" << string(orig, key_len) << " "
                 "value:"  << string(pcur+1, length- key_len -1) << "\n";

    return 0;
}