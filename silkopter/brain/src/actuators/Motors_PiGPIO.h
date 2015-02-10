#pragma once

#include "IMotors.h"

namespace silk
{
namespace actuators
{

class Motors_PiGPIO : public IMotors
{
public:
    Motors_PiGPIO();
    ~Motors_PiGPIO();

    auto init() -> bool;
    void shutdown();

    //----------------------------------------------------------------------
    //motors

    void set_throttles(float const* throttles, size_t count);
    void cut_throttle();

    //----------------------------------------------------------------------
    void process();

private:
    enum class PWM_Frequency : uint8_t
    {
        SERVO_50HZ,
        SERVO_100HZ,
        SERVO_200HZ,
        SERVO_400HZ,
        PWM_1000HZ, //don't use as the RPI puts the pins high when shutting down!!! This will cause motors to spin
    };

    bool m_is_initialized = false;

    struct Motor
    {
        float throttle = 0;
    };
    std::vector<Motor> m_motors;

    struct Impl;
    std::shared_ptr<Impl> m_impl;

    auto get_pulse_range() const -> std::pair<int, int>;

    struct Settings
    {
        size_t count = 4;
        //PWM_Frequency frequency = PWM_Frequency::SERVO_50HZ;
        PWM_Frequency frequency = PWM_Frequency::PWM_1000HZ;
//        rapidjson::Document document;
    } m_settings;

    auto load_settings() -> bool;
    void save_settings();

    
};

DECLARE_CLASS_PTR(Motors_PiGPIO);

}
}
