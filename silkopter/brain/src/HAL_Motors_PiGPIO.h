#pragma once

namespace silk
{

class HAL_Motors_PiGPIO : q::util::Noncopyable
{
public:
    HAL_Motors_PiGPIO(PiGPIO& pigpio);
    ~HAL_Motors_PiGPIO();

    enum class PWM_Frequency : uint8_t
    {
        SERVO_50HZ,
        SERVO_100HZ,
        SERVO_250HZ,
        SERVO_500HZ,
        PWM_1000Hz,
    };

    struct Params
    {
        size_t count = 4;
        PWM_Frequency frequency = PWM_Frequency::PWM_1000Hz;
    };

    auto init() -> Result;
    void shutdown();

    //----------------------------------------------------------------------
    //motors

    auto get_count() const -> size_t;
    void set_throttles(float const* throttles, size_t count);
    void cut_throttle();

    //----------------------------------------------------------------------
    void process();

private:
    PiGPIO& m_pigpio;

    PWM_Frequency m_pwm_frequency = PWM_Frequency::PWM_1000Hz;
    struct Motor
    {
        float throttle = 0;
    };
    std::vector<Motor> m_motors;

    bool m_is_connected = false;
    size_t m_error_count = 0;

    struct Impl;
    std::shared_ptr<Impl> m_impl;

    struct Settings
    {
        rapidjson::Document document;
    } m_settings;

    auto load_settings() -> bool;
    void save_settings();

    
};

}
