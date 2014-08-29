#pragma once

struct Config
{
    struct Motor
    {
        math::vec2f position;
        bool clockwise = true;
        float drag = 0;
        float max_rpm = 0;
        float acceleration = 0;
        float decceleration = 0;
        std::vector<std::pair<float, float>> thrust_curve;
        std::vector<std::pair<float, float>> amps_curve;
    };

    struct UAV
    {
        float radius = 0.25f;
        float height = 0.3f;
        float mass = 1.f;
        std::vector<Motor> motors;
    } uav;

    struct Environment
    {
        bool is_simulation_enabled = true;
        bool is_ground_enabled = true;
        bool is_gravity_enabled = true;
    } environment;

    struct Accelerometer
    {
        q::Clock::duration sample_period{std::chrono::milliseconds(2)};
        float range = 8.f; //
        float noise = 0.1f;
        math::vec3f bias;
        math::vec3f scale = math::vec3f::one;
    } accelerometer;

    struct Gyroscope
    {
        q::Clock::duration sample_period{std::chrono::milliseconds(2)};
        float range_degrees = 1000.f;
        float noise_degrees = 0.1f;
        math::vec3f bias_degrees;
    } gyroscope;

    struct Compass
    {
        q::Clock::duration sample_period{std::chrono::milliseconds(15)};
        float noise = 1.f;
        math::vec3f bias;
    } compass;

    struct Barometer
    {
        q::Clock::duration sample_period{std::chrono::milliseconds(20)};
        float noise = 100.f;
        bool is_enabled = true;
    } barometer;

    struct Thermometer
    {
        q::Clock::duration sample_period{std::chrono::milliseconds(100)};
        float noise = 0.5f;
    } thermometer;

    struct Sonar
    {
        q::Clock::duration sample_period{std::chrono::milliseconds(100)};
        float noise = 0.02f;
        float range = 4.f;
        bool is_enabled = true;
    } sonar;
};
