#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Component {
public:
    Component(string name) : m_strCompname(name) {}
    virtual ~Component() {}
    virtual void Operation() = 0;
    virtual void Add(Component *) = 0;
    virtual void Remove(Component *) = 0;
    virtual Component *GetChild(int) = 0;
    virtual string GetName()
    {
        return m_strCompname;
    }
    virtual void Print() = 0;

protected:
    string m_strCompname;
};

class Leaf: public Component {
public:
    Leaf(string name): Component(name) {}
    void Operation()
    {
        std::cout << "I'am " << m_strCompname << std::endl;
    }

    void Add(Component *pComponent) {}
    void Remove(Component *pComponent) {}
    Component *GetChild(int index)
    {
        return NULL;
    }
    void Print()
    {
        std::cout << this->GetName() << std::endl;
    }
};

class Composite: public Component {
public:
    Composite(string name): Component(name)
    {}
    ~Composite()
    {
        vector<Component *>::iterator it = m_vecComp.begin();
        while(it != m_vecComp.end()) {
            if(*it != NULL) {
                std::cout << "----delete " << (*it)->GetName() << "----" << std::endl;
                *it = NULL;
            }
            m_vecComp.erase(it);
            it = m_vecComp.begin();
        }
    }

    void Operation()
    {
        std::cout << "I'm " << m_strCompname << std::endl;
    }

    void Add(Component *pComponent)
    {
        m_vecComp.push_back(pComponent);
    }

    void Remove(Component *pComponent)
    {
        for(vector<Component *>::iterator it= m_vecComp.begin(); it != m_vecComp.end(); ++it) {
            if((*it)->GetName() == pComponent->GetName()) {
                if(*it != NULL) {
                    delete *it;
                    *it = NULL;
                }
                m_vecComp.erase(it);
                break;
            }
        }
    }

    Component *GetChild(int index)
    {
        if(index > m_vecComp.size()) {
            return NULL;
        }

        return m_vecComp[index - 1];
    }

    void Print()
    {
        std::cout << m_strCompname << std::endl;
        for(vector<Component *>::iterator it = m_vecComp.begin(); it != m_vecComp.end(); ++it) {
            (*it)->Print();
        }
    }

private:
    vector<Component *> m_vecComp;
};

int main(int argc, char *argv[])
{
    Component *pNode = new Composite("Beijing Head Office");
    Component *pNodeHr = new Leaf("Beijing Human Resource Department");
    Component *pSubNodeSh = new Composite("Shanghai Branch");
    Component *pSubNodeCd = new Composite("Chengdu Branch");
    Component *pSubNodeBt = new Composite("Baotou Branch");
    pNode->Add(pNodeHr);
    pNode->Add(pSubNodeSh);
    pNode->Add(pSubNodeCd);
    pNode->Add(pSubNodeBt);
    pNode->Print();

    Component *pSubNodeShHr = new Leaf("Shanghai Human Resources Department");
    Component *pSubNodeShCg = new Leaf("Shanghai Purchasing Department");
    Component *pSubNodeShXs = new Leaf("Shanghai Sales Department");
    Component *pSubNodeShZb = new Leaf("Shanghai Quality Supervision Department");
    pSubNodeSh->Add(pSubNodeShHr);
    pSubNodeSh->Add(pSubNodeShCg);
    pSubNodeSh->Add(pSubNodeShXs);
    pSubNodeSh->Add(pSubNodeShZb);
    pNode->Print();

    std::cout << "============================" << std::endl;
    pSubNodeSh->Remove(pSubNodeShZb);
    if(pNode != NULL) {
        delete pNode;
        pNode = NULL;
    }

    return 0;
}
