#include <iostream>
#include <string>
#include <memory>

class Base {
    public:
        virtual void func() = 0;
};

class Derived : public Base {
    public:
        virtual void func() override {
            std::cout << "Derived::func" << std::endl;
        }

        void run() {
            func();
        }
}; 

class Concrete : public Derived {
    public:
        virtual void func() override {
            std::cout << "Concrete::func" << std::endl;
        }
};

int main() {
    std::shared_ptr<Base> b = std::make_shared<Concrete>();
    b->func();
    return 0;
}


