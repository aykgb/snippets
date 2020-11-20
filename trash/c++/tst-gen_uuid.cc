#include <sys/types.h>
#include <uuid/uuid.h>

#include <iostream>
#include <string>

using namespace std;

uint64_t gen_uuid() {
    uuid_t src_uid;
    uuid_generate(src_uid);
    std::cout << "original uuid:" << src_uid << std::endl;
    uint64_t* p_uid = (uint64_t*)src_uid;
    return p_uid[0];
}

int main() {
    for (int i = 0; i < 10; i++) {
        uint64_t uuid = gen_uuid();
        std::cout << "uint64_t type uuid:" << uuid << std::endl;
    }
    return 0;
}
