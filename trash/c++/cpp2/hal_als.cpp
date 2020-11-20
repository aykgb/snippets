/*************************************************************************
    > File Name: hal_als.cpp
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Tue 17 May 2016 04:01:30 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;
class AlsPortController {
    typedef std::bitset<ALS::AutomaticLaserShutCondition::ALS_MAX_CONDITION> AlsConditionBit_t;

public:
    AlsPortController():
        port_id_(INVALID_VALUE), als_conditions(INVALID_VALUE), holdoff_conuter(INVALID_VALUE) { }
    AlsPortController(const uint32_t port_id):
        port_id_(port_id),
        als_conditions(0b0010),
        is_laser_opened(false),
        is_laser_enabled(false),
        is_near_end_als_enabled(false),
        is_client_shutdown_enabled(false),
        client_shutdown_holdoff_time(0),
        holdoff_conuter(INVALID_VALUE),
        tx_power(-99.0)
    { }
    virtual ~AlsPortController() { }

    void set_laser_open()
    {
        is_laser_opened = true;
    }
    void set_laser_shut()
    {
        is_laser_opened = false;
    }
    void set_laser_enabled(bool enable)
    {
        is_laser_enabled = enable;
    }
    void set_laser_tx_power(float power)
    {
        tx_power = power;
    }
    void set_near_end_als_enabled(bool enable)
    {
        is_near_end_als_enabled = enable;
    }
    void set_client_shutdown_enabled(bool enable)
    {
        is_client_shutdown_enabled = enable;
    }
    void set_client_shutdown_holdoff_time(int time)
    {
        client_shutdown_holdoff_time = time;
    }
    bool set_als_conditions(
        const AutomaticLaserShutCondition::Enum& condition,
        const bool is_set)
    {
        if(condition != AutomaticLaserShutCondition::ALS_MAX_CONDITION) {
            is_set == true ? als_conditions.set(condition) : als_conditions.reset(condition);
            return true;
        }
        return false;
    }

    bool get_laser_opened() const
    {
        return is_laser_opened;
    }
    bool get_laser_enabled() const
    {
        return is_laser_enabled;
    }
    float get_laser_tx_power() const
    {
        return tx_power;
    }
    bool get_near_end_als_enabled() const
    {
        return is_near_end_als_enabled;
    }
    bool get_client_shutdown_enabled() const
    {
        return is_client_shutdown_enabled;
    }
    int get_client_shutdown_holdoff_time() const
    {
        return client_shutdown_holdoff_time;
    }
    uint32_t get_port_index() const
    {
        return port_id_;
    }
    AlsConditionBit_t get_als_conditions() const
    {
        return als_conditions;
    }

    bool check_near_end_als_conditions() const
    {
        return CONDITION_RANGE_CHECK(
                   als_conditions,
                   AutomaticLaserShutCondition::ALS_NEAR_END_ALS_CONDITIONS_BEGIN,
                   AutomaticLaserShutCondition::ALS_NEAR_END_ALS_CONDITIONS_END
               );
    }
    bool check_client_shutdown_conditions() const
    {
        return CONDITION_RANGE_CHECK(
                   als_conditions,
                   AutomaticLaserShutCondition::ALS_CLIENT_SHUTDOWN_CONDITIONS_BEGIN,
                   AutomaticLaserShutCondition::ALS_CLIENT_SHUTDOWN_CONDITIONS_END
               );
    }
    bool check_configuration_conditions() const
    {
        return CONDITION_RANGE_CHECK(
                   als_conditions,
                   AutomaticLaserShutCondition::ALS_OOS_CONDITIONS_BEGIN,
                   AutomaticLaserShutCondition::ALS_OOS_CONDITIONS_END
               );
    }
    bool check_condition(AutomaticLaserShutCondition::Enum condition) const
    {
        return als_conditions.test(condition);
    }

    bool check_holdoff_counter_zero()
    {
        return (holdoff_conuter == 0);
    }
    bool check_holdoff_counter_invalid()
    {
        return (holdoff_conuter == INVALID_VALUE);
    }
    void reset_holdoff_counter()
    {
        holdoff_conuter = client_shutdown_holdoff_time / HOLDOFF_PERIOD_MS;
    }
    void decrease_holdoff_counter()
    {
        holdoff_conuter--;
    }
    void set_holdoff_invalid()
    {
        holdoff_conuter = INVALID_VALUE;
    }


    bool print_port_als_state(std::ostream& os)
    {
        os.flags(std::ios::right);
        os << std::setw(5)  << port_id_;
        os << std::setw(10) << is_near_end_als_enabled;
        os << std::setw(15) << is_client_shutdown_enabled;
        os << std::setw(15) << is_laser_opened;
        os << std::setw(12) << tx_power;
        os << std::setw(24) << als_conditions << "\n";
        return true;
    }

    void restore_port_als_state(std::string& line)
    {
        std::istringstream iss(line);
        iss >> port_id_;
        iss >> is_near_end_als_enabled;
        iss >> is_client_shutdown_enabled;
        iss >> is_laser_opened;
        iss >> tx_power;
        iss >> als_conditions;
    }

private:
    uint32_t            port_id_;
    /* Records the als shutdown trigering events  */
    AlsConditionBit_t   als_conditions;
    bool                is_laser_opened;
    bool                is_laser_enabled;
    bool                is_near_end_als_enabled;
    bool                is_client_shutdown_enabled;
    uint32_t            client_shutdown_holdoff_time;
    uint32_t            holdoff_conuter;
    float               tx_power;
};
