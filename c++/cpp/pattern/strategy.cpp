#include <iostream>
using namespace std;

class Strategy {
public:
    virtual void AlgorithmInterface() = 0;
};

class ConcreteStrategyA: public Strategy {
public:
    virtual ~ConcreteStrategyA() {  }
    void AlgorithmInterface()
    {
        std::cout << "I am from ConcreteStrategyA." << std::endl;
    }
};

class ConcreteStrategyB: public Strategy {
public:
    virtual ~ConcreteStrategyB() {  }
    void AlgorithmInterface()
    {
        std::cout << "I am from ConcreteStrategyB." << std::endl;
    }
};

class Context {
public:
    Context(Strategy *pStrategyArg): pStrategy(pStrategyArg)
    {  }

    void ContextInterface()
    {
        pStrategy->AlgorithmInterface();
    }

private:
    Strategy *pStrategy;
};

int main()
{
    Strategy *pStrategyA = new ConcreteStrategyA;
    Strategy *pStrategyB = new ConcreteStrategyB;

    Context *pContextA = new Context(pStrategyA);
    Context *pContextB = new Context(pStrategyB);

    pContextA->ContextInterface();
    pContextB->ContextInterface();

    if(pStrategyA) delete pStrategyA;
    if(pStrategyB) delete pStrategyB;
}
