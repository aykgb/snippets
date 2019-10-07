#include <iostream>
#include <bitset>
#include <stdio.h>

using namespace std;

const unsigned int numTraceItems = 32;

class SdkTrace {
public:
    SdkTrace()
    {
        std::cout << "SdkTrace()" << std::endl;
    }

    virtual ~SdkTrace()
    {
        //        std::cout << "~SdkTrace()" << std::endl;
    }

    void print()
    {
        std::cout << myTraceWord << std::endl;
        printf("%p\n", this);
    }

    std::bitset<numTraceItems> myTraceWord;
};

class Pm5990SdkTrace {
public:
    Pm5990SdkTrace()
    {
        std::cout << "Pm5990SdkTrace()" << std::endl;
    }
    virtual ~Pm5990SdkTrace()
    {
        //        std::cout << "~Pm5990SdkTrace()" << std::endl;
    }

    static void print()
    {
        //        std::cout << mySdkTrace.myTraceWord << std::endl;
        mySdkTrace.print();
    }

    static SdkTrace  mySdkTrace;
};

SdkTrace Pm5990SdkTrace::mySdkTrace;

class Pm5990OduGenericDevice : public Pm5990SdkTrace {
public:
    Pm5990OduGenericDevice()
    {
        std::cout << "Pm5990OduGenericDevice()" << std::endl;
    }

    virtual ~Pm5990OduGenericDevice()
    {
        //        std::cout << "~Pm5990OduGenericDevice()" << std::endl;
    }

    void traceSet(unsigned int i)
    {
        if(i < numTraceItems) {
            mySdkTrace.myTraceWord.set(i);
        } else {
            std::cout << "Wrong Parameter!" << std::endl;
        }

    }

    void print()
    {
        mySdkTrace.print();
    }
};

class Pm5990PortGenericDevice : public Pm5990SdkTrace {
public:
    Pm5990PortGenericDevice()
    {
        std::cout << "Pm5990PortGenericDevice()" << std::endl;
    }

    virtual ~Pm5990PortGenericDevice()
    {
        //        std::cout << "~Pm5990PortGenericDevice()" << std::endl;
    }

    void traceSet(unsigned int i)
    {
        if(i < numTraceItems) {
            mySdkTrace.myTraceWord.set(i);
        } else {
            std::cout << "Wrong Parameter!" << std::endl;
        }

    }

    void print()
    {
        mySdkTrace.print();
    }
};

class Pm5990GenericDevice : public Pm5990SdkTrace {
public:
    virtual ~Pm5990GenericDevice() {}

    void traceSet(unsigned int i)
    {
        if(i < numTraceItems) {
            mySdkTrace.myTraceWord.set(i);
        } else {
            std::cout << "Wrong Parameter!" << std::endl;
        }

    }

    void traceSet(unsigned int portId, unsigned int i)
    {
        if(i < numTraceItems) {
            myPorts[portId].traceSet(i);
        } else {
            std::cout << "Wrong Parameter!" << std::endl;
        }

    }

    void print()
    {
        mySdkTrace.print();
    }

private:
    Pm5990PortGenericDevice myPorts[182];
    Pm5990OduGenericDevice myOdus[32];
};

class Pm5990SpecializedDevice : public Pm5990GenericDevice {
public:
    virtual ~Pm5990SpecializedDevice() {}

    void traceSet(unsigned int i)
    {
        if(i < numTraceItems) {
            mySdkTrace.myTraceWord.set(i);
        } else {
            std::cout << "Wrong Parameter!" << std::endl;
        }

    }
};

int main()
{
    // Pm5990PortGenericDevice myPorts[182];
    // Pm5990OduGenericDevice myOdus[32];

    // myOdus[0].traceSet(0);
    // myOdus[1].traceSet(1);
    // Pm5990SdkTrace::print();

    // myPorts[2].traceSet(2);
    // myPorts[3].traceSet(3);

    //  Pm5990GenericDevice  myGenDevices[2];
    Pm5990SpecializedDevice mySpeDevices[2];
    mySpeDevices[0].traceSet(0);
    mySpeDevices[2].traceSet(1);
    Pm5990SdkTrace::print();

    //  printf("%p", &())

    return 0;
}
