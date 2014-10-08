#pragma once

#include "i2c.h"

namespace silk
{


class MPU9250 : public q::util::Noncopyable
{
public:
    enum class Result
    {
        OK,
        FAILED
    };

    enum class Gyroscope_Range
    {
        _250_DPS = 250,
        _500_DPS = 500,
        _1000_DPS = 1000,
        _2000_DPS = 2000,
    };
    enum class Accelerometer_Range
    {
        _2_G = 2,
        _4_G = 4,
        _8_G = 8,
        _16_G = 16,
    };

    auto init(q::String const& device, Gyroscope_Range gr, Accelerometer_Range ar) -> Result;

    void process();

    auto read_compass() -> boost::optional<math::vec3f>;

    auto get_gyroscope_samples() const -> std::vector<math::vec3f> const&;
    auto get_accelerometer_samples() const -> std::vector<math::vec3f> const&;

    auto get_sample_time() const -> q::Clock::duration;

private:
    void reset_fifo();

    auto setup_compass() -> Result;
    void mpu_set_bypass(bool on);
    void process_compass();

    i2c m_i2c;

    uint8_t m_compass_addr = 0;

    mutable std::vector<uint8_t> m_fifo_buffer;

    size_t m_fifo_sample_size = 999999;

    Gyroscope_Range m_gyroscope_rate;
    Accelerometer_Range m_accelerometer_range;

    struct Samples
    {
        std::vector<math::vec3f> gyroscope;
        std::vector<math::vec3f> accelerometer;
        boost::optional<math::vec3f> compass;
    } m_samples;

    float m_accelerometer_scale_inv = 1.f;
    float m_gyroscope_scale_inv = 1.f;
    float m_magnetic_adj[3];

    uint32_t m_sample_rate = 1000;
    q::Clock::duration m_sample_time;
    q::Clock::time_point m_last_compass_timestamp;
};

}
