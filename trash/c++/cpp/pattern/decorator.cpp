#include <iostream>

using namespace std;

class Component {
public:
    virtual void Operation() = 0;
};

class ConcreteComponent: public Component {
public:
    virtual ~ConcreteComponent() {  }
    void Operation()
    {
        std::cout << "I am no decoratored ConcreteComponent." << std::endl;
    }
};

class Decorator: public Component {
public:
    Decorator(Component *pComponent): m_pComponentObj(pComponent) {}
    void Operation()
    {
        if(m_pComponentObj != NULL) {
            m_pComponentObj->Operation();
        }
    }

protected:
    Component *m_pComponentObj;
};

class ConcreteDecoratorA: public Decorator {
public:
    ConcreteDecoratorA(Component *pDecorator): Decorator(pDecorator) {  }
    void Operation()
    {
        AddedBehavior();
        Decorator::Operation();
    }

    void AddedBehavior()
    {
        std::cout << "This is added behavior A." << std::endl;
    }
};

class ConcreteDecoratorB: public Decorator {
public:
    ConcreteDecoratorB(Component *pDecorator): Decorator(pDecorator) {  }
    void Operation()
    {
        AddedBehavior();
        Decorator::Operation();
    }

    void AddedBehavior()
    {
        std::cout << "This is added behavior B." << std::endl;
    }
};

int main()
{
    Component *pComponentObj = new ConcreteComponent();
    Decorator *pDecoratorAObj = new ConcreteDecoratorA(pComponentObj);
    pDecoratorAObj->Operation();
    std::cout << "===================================" << std::endl;
    Decorator *pDecoratorBObj = new ConcreteDecoratorB(pComponentObj);
    pDecoratorBObj->Operation();
    std::cout << "===================================" << std::endl;
    Decorator *pDecoratorBAObj = new ConcreteDecoratorB(pDecoratorAObj);
    pDecoratorBAObj->Operation();
    std::cout << "===================================" << std::endl;

    delete pDecoratorBAObj;
    pDecoratorBAObj = NULL;

    delete pDecoratorBObj;
    pDecoratorBObj = NULL;

    delete pDecoratorAObj;
    pDecoratorAObj = NULL;

    delete pComponentObj;
    pComponentObj = NULL;

    return 0;
}
