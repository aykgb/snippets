#include <iostream>
#include <functional>
#include <sstream>
#include <vector>
#include <memory>

class OpticalModule {
public:
    OpticalModule(): laser_enable(false)
    {
        std::cout << "#### Opticalmodule()...\n";
    }
    OpticalModule(unsigned int port_id): port_id_(port_id), laser_enable(false)
    {
        std::cout << "#### Opticalmodule(" << port_id_ << ")...\n";
    }
    ~OpticalModule()
    {
        std::cout << "#### ~Opticalmodule()...\n";
    }


    OpticalModule(const OpticalModule& obj)
    {
        std::cout << "#### copy constructor...\n";
        port_id_ = obj.port_id_;
        laser_enable = obj.laser_enable;
    }

    OpticalModule& operator=(const OpticalModule& obj)
    {
        std::cout << "#### copy asignment operator...\n";
        port_id_ = obj.port_id_;
        laser_enable = obj.laser_enable;
        return *this;
    }

    void SetQsfpLaserEnable(bool enable)
    {
        laser_enable = enable;
        std::cout << "#### Laser Open...\n";
    }

    bool SetCfpLaserEnable(unsigned int port_id, bool enable)
    {
        port_id_ = port_id;
        laser_enable = enable;
        std::cout << "#### PortId:" << port_id << " Laser Open...\n";
        return true;
    }

private:
    unsigned int port_id_;
    bool laser_enable;
};

int main()
{
    //  OpticalModule qsfp = OpticalModule(0);
    OpticalModule qsfp0(0);
    OpticalModule qsfp1 = OpticalModule(1);
    OpticalModule qsfp1_1;
    qsfp1_1 = qsfp1;
    //  OpticalModule *qsfp = new OpticalModule(0);
    std::function<void(OpticalModule& ,const bool)> cfp_laser_enable_call_back = &OpticalModule::SetQsfpLaserEnable;

    /* It will call the copy constructor twice. */
    // std::function<void(OpticalModule ,const bool)> cfp_laser_enable_call_back = &OpticalModule::SetQsfpLaserEnable;

    cfp_laser_enable_call_back(qsfp0, true);

    //auto cfp_laser_enable_call_back = std::bind(&OpticalModule::SetQsfpLaserEnable, std::placeholders::_1);

    return 0;
}
