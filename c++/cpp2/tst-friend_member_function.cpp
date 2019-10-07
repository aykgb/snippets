#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Controller;

class Agent {
public:
    Agent(const int &num_constrollers);
    ~Agent()
    {
        vector<Controller* >::iterator it = controllers.begin();
        for(; it != controllers.end(); )
            controllers.erase(it++);
    }

    Agent& handler(const int&);

private:
    unsigned int          current_id;
    vector<Controller *>  controllers;
};

class Controller {
    friend Agent& Agent::handler(const int&);

public:
    Controller() {}
    ~Controller() {}

private:
    bool set_flag()
    {
        flag = true;
        return flag;
    }
    void reset_flag()
    {
        flag = false;
    }

private:
    bool flag = false;
};

Agent::Agent(const int &num_constrollers)
{
    for(int i = 0; i < num_constrollers; ++i) {
        controllers.push_back(new Controller());
    }
}

Agent& Agent::handler(const int& id)
{
    current_id = id;
    std::cout << controllers[id]->set_flag() << std::endl;

    return *this;
}

int main()
{
    Agent* agent =new Agent(12);
    agent->handler(0);

    return 0;
}
