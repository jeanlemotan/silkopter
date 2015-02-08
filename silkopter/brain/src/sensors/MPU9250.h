#pragma once

#include "i2c.h"
#include "spi.h"

namespace silk
{


class MPU9250 : public q::util::Noncopyable
{
public:
    virtual ~MPU9250();

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

    auto init(Gyroscope_Range gr, Accelerometer_Range ar) -> bool;

    void process();

    auto get_compass_samples() const -> std::vector<math::vec3f> const&;
    auto get_gyroscope_samples() const -> std::vector<math::vec3f> const&;
    auto get_accelerometer_samples() const -> std::vector<math::vec3f> const&;

    auto get_compass_sample_time() const -> q::Clock::duration;
    auto get_gyroscope_sample_time() const -> q::Clock::duration;
    auto get_accelerometer_sample_time() const -> q::Clock::duration;

protected:

    virtual auto mpu_read(uint8_t reg, uint8_t* data, uint32_t size) -> bool = 0;
    virtual auto mpu_read_u8(uint8_t reg, uint8_t& dst) -> bool = 0;
    virtual auto mpu_read_u16(uint8_t reg, uint16_t& dst) -> bool = 0;
    virtual auto mpu_write_u8(uint8_t reg, uint8_t const& t) -> bool = 0;
    virtual auto mpu_write_u16(uint8_t reg, uint16_t const& t) -> bool = 0;

    virtual auto akm_read(uint8_t reg, uint8_t* data, uint32_t size) -> bool = 0;
    virtual auto akm_read_u8(uint8_t reg, uint8_t& dst) -> bool = 0;
    virtual auto akm_read_u16(uint8_t reg, uint16_t& dst) -> bool = 0;
    virtual auto akm_write_u8(uint8_t reg, uint8_t const& t) -> bool = 0;
    virtual auto akm_write_u16(uint8_t reg, uint16_t const& t) -> bool = 0;

    void reset_fifo();

    auto setup_compass() -> bool;
    void set_bypass(bool on);
    void process_compass();

    uint8_t m_akm_address = 0;

private:
    mutable std::vector<uint8_t> m_fifo_buffer;

    size_t m_fifo_sample_size = 999999;

    Gyroscope_Range m_gyroscope_rate;
    Accelerometer_Range m_accelerometer_range;

    struct Samples
    {
        std::vector<math::vec3f> gyroscope;
        std::vector<math::vec3f> accelerometer;
        std::vector<math::vec3f> compass;
    } m_samples;

    float m_accelerometer_scale_inv = 1.f;
    float m_gyroscope_scale_inv = 1.f;
    float m_magnetic_adj[3];

    uint32_t m_sample_rate = 1000;
    q::Clock::duration m_sample_time;

    q::Clock::time_point m_last_compass_timestamp;
    q::Clock::duration m_compass_sample_time;
};

}
