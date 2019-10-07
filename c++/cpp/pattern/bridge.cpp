#include <iostream>
#include <string>

/* Implemented interface */
class AbstractInterface {
public:
    virtual void SomeFunctionality() = 0;
};

/* Interface for internal implementation that Bridge uses. */
class ImplementationInterface {
public:
    virtual void AnotherFunctionlity() = 0;
};

/* The Bridge */
class Bridge: public AbstractInterface {
protected:
    ImplementationInterface* implementation;

public:
    Bridge(ImplementationInterface* backend)
    {
        implementation = backend;
    }
};

/* Different special cases of the interface. */
class UseCase1: public Bridge {
public:
    UseCase1(ImplementationInterface* backend):
        Bridge(backend) {}

    void SomeFunctionality()
    {
        std::cout << "UseCase1 on";
        implementation->AnotherFunctionlity();
    }

};

class UseCase2: public Bridge {
public:
    UseCase2(ImplementationInterface* backend)
        : Bridge(backend) {}

    void SomeFunctionality()
    {
        std::cout << "UseCase2 on";
        implementation->AnotherFunctionlity();
    }
};

class Windows: public ImplementationInterface {
public:
    void AnotherFunctionlity()
    {
        std::cout << "Windows :-!" << std::endl;
    }
};

class Linux: public ImplementationInterface {
public:
    void AnotherFunctionlity()
    {
        std::cout << "Linux! :-)" << std::endl;
    }
};

int main()
{
    AbstractInterface *useCase = NULL;
    ImplementationInterface *osWindows = new Windows;
    ImplementationInterface *osLinux = new Linux;

    /* First case */
    useCase = new UseCase1(osWindows);
    useCase->SomeFunctionality();

    useCase = new UseCase1(osLinux);
    useCase->SomeFunctionality();

    /* Second case */
    useCase = new UseCase2(osWindows);
    useCase->SomeFunctionality();

    useCase = new UseCase2(osLinux);
    useCase->SomeFunctionality();

    return 0;
}
