#pragma once

//#include "IGimbal.h"

//namespace silk
//{
//namespace actuators
//{

//class Gimbal_Servo_PiGPIO : public IGimbal
//{
//public:
//    Gimbal_Servo_PiGPIO();
//    ~Gimbal_Servo_PiGPIO();

//    auto get_name() const -> q::String const&;

//    auto init() -> bool;
//    void shutdown();

//    //----------------------------------------------------------------------
//    //mount

//    void set_rotation(math::vec3f const& rot); //radians for each axis

//    //----------------------------------------------------------------------
//    void process();

//private:
//    enum class PWM_Frequency : uint8_t
//    {
//        SERVO_50HZ,
//        SERVO_100HZ,
//        SERVO_200HZ,
//        SERVO_400HZ,
//    };

//    struct Settings
//    {
//        struct End
//        {
//            float angle;
//            uint32_t pulse;
//        };
//        struct Range
//        {
//            End min { -math::anglef::pi, 1000 };
//            End max { math::anglef::pi, 2000 };
//        };

//        Range x_range;
//        Range y_range;
//        Range z_range;

//        PWM_Frequency frequency = PWM_Frequency::SERVO_200HZ;
//    } m_settings;

//    math::vec3f m_rotation;
//    math::vec3f m_last_rotation;
//    q::Clock::time_point m_last_time_point;

//    static auto compute_pulse(float angle, Settings::Range const& range) -> uint32_t;

//    bool m_is_initialized = false;

//    auto load_settings() -> bool;
//    void save_settings();
//};

//DECLARE_CLASS_PTR(Gimbal_Servo_PiGPIO);

//}
//}
