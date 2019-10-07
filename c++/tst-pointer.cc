#include <iostream>

class Header {
   public:
    int ver = 1;
};

class Req {
   public:
    Header header;
};

int main() {
    Req* pr = new Req;

    std::cout << "&req " << pr << "\n";
    std::cout << "(int)pr->header.ver " << (int)pr->header.ver << "\n";

    delete pr;

    return 0;
}
