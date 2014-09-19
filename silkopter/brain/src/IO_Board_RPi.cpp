#include "BrainStdAfx.h"

#ifdef RASPBERRY_PI

#include "IO_Board_RPi.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"
#include <pigpio.h>

#define USE_MPU9250
#define USE_MS5611


#ifdef USE_MPU9250
#   include "chips/MPU9250.h"
#else
#   error "No IMU selected"
#endif

#ifdef USE_MS5611
#   include "chips/MS5611.h"
#else
#   error "No Barometer selected"
#endif

static constexpr std::array<uint32_t, 6> MOTOR_GPIOS = {4, 17, 18, 27, 22, 23};
constexpr uint32_t CAMERA_YAW_GPIO = 0;
constexpr uint32_t CAMERA_PITCH_GPIO = 25;
constexpr uint32_t CAMERA_ROLL_GPIO = 0;
constexpr uint32_t GPIO_PWM_RANGE = 10000;

using namespace silk;
using namespace boost::asio;

struct IO_Board_RPi::Private
{
#ifdef USE_MPU9250
    MPU9250 mpu;
#endif

#ifdef USE_MS5611
    MS5611 baro;
#endif
};

///////////////////////////////////////////////////////////////

IO_Board_RPi::IO_Board_RPi()
{
    load_settings();

    m_p.reset(new Private);
}

IO_Board_RPi::~IO_Board_RPi()
{
    gpioTerminate();
}

auto IO_Board_RPi::load_settings() -> bool
{
    q::data::File_Source fs(q::Path("io_board.cfg"));
    if (!fs.is_open())
    {
        return false;
    }

    auto size = fs.get_size();

    std::vector<char> data(size + 1);
    auto result = fs.read(reinterpret_cast<uint8_t*>(data.data()), size);
    if (result != size)
    {
        SILK_WARNING("Failed to read settings.");
        return false;
    }

    data[size] = 0;
    if (m_settings.document.Parse<0>(data.data()).HasParseError())
    {
        SILK_WARNING("Failed to parse settings: {}:{}", m_settings.document.GetErrorOffset(), m_settings.document.GetParseError());
        return false;
    }

    auto finder = jsonutil::Member_Finder(m_settings.document, "calibration_config");
    if (!finder.is_valid())
    {
        SILK_WARNING("Cannot find root");
        return false;
    }
    auto& calibration_config = finder.get_value();

    {
        auto& json = calibration_config;
        auto abias = jsonutil::get_vec3_value<float>(json, "accelerometer_bias");
        auto ascale = jsonutil::get_vec3_value<float>(json, "accelerometer_scale");
        auto gbias = jsonutil::get_vec3_value<float>(json, "gyroscope_bias");
        auto cbias = jsonutil::get_vec3_value<float>(json, "compass_bias");
        if (!abias || !ascale || !gbias || !cbias)
        {
            SILK_WARNING("Cannot read calibration config");
            return false;
        }
        m_calibration_config.accelerometer_bias = *abias;
        m_calibration_config.accelerometer_scale = *ascale;
        m_calibration_config.gyroscope_bias = *gbias;
        m_calibration_config.compass_bias = *cbias;
    }

    return true;
}
void IO_Board_RPi::save_settings()
{
    TIMED_FUNCTION();
    if (!m_settings.document.IsObject())
    {
        m_settings.document.SetObject();
    }

    auto& allocator = m_settings.document.GetAllocator();

    auto& calibration_config = jsonutil::get_or_add_member(m_settings.document, "calibration_config", rapidjson::kObjectType, allocator);

    {
        auto& json = calibration_config;
        jsonutil::set_vec3_value(json, "accelerometer_bias", m_calibration_config.accelerometer_bias, allocator);
        jsonutil::set_vec3_value(json, "accelerometer_scale", m_calibration_config.accelerometer_scale, allocator);
        jsonutil::set_vec3_value(json, "gyroscope_bias", m_calibration_config.gyroscope_bias, allocator);
        jsonutil::set_vec3_value(json, "compass_bias", m_calibration_config.compass_bias, allocator);
    }

    typedef rapidjson::UTF8<> JSON_Charset;
    typedef rapidjson::GenericStringBuffer<JSON_Charset> JSON_Buffer;
    typedef rapidjson::PrettyWriter<JSON_Buffer> JSON_Writer;

    JSON_Buffer buffer;
    JSON_Writer writer(buffer);
    m_settings.document.Accept(writer);

    q::data::File_Sink fs(q::Path("io_board.cfg"));
    if (!fs.is_open())
    {
        SILK_WARNING("Cannot open file to save settings");
        return;
    }

    fs.write(reinterpret_cast<uint8_t const*>(buffer.GetString()), buffer.GetSize());
}

auto IO_Board_RPi::connect() -> Connection_Result
{
    if (!is_disconnected())
    {
        return Connection_Result::OK;
    }

    if (gpioCfgClock(2, 1, 1) < 0)
    {
        SILK_WARNING("Cannot configure gpio clock");
        return Connection_Result::FAILED;
    }

    if (gpioInitialise() < 0)
    {
        SILK_WARNING("Cannot initialize gpio");
        return Connection_Result::FAILED;
    }

    {
        if (m_pwm_frequency >= PWM_Frequency::PWM_1000Hz)
        {
            uint32_t freq = 0;
            switch (m_pwm_frequency)
            {
            case PWM_Frequency::PWM_1000Hz: freq = 1000; break;
            default:
            {
                SILK_WARNING("Cannot recognize pwm frequency {}", static_cast<int>(m_pwm_frequency));
                gpioTerminate();
                return Connection_Result::FAILED;
            }
            }

            for (auto gpio: MOTOR_GPIOS)
            {
                if (gpioSetPWMfrequency(gpio, freq) < 0)
                {
                    SILK_WARNING("Cannot set pwm frequency {} on gpio {}", freq, gpio);
                    gpioTerminate();
                    return Connection_Result::FAILED;
                }
                if (gpioSetPWMrange(gpio, GPIO_PWM_RANGE) < 0)
                {
                    SILK_WARNING("Cannot set pwm range {} on gpio {}", GPIO_PWM_RANGE, gpio);
                    gpioTerminate();
                    return Connection_Result::FAILED;
                }
            }
        }
        else
        {
            SILK_WARNING("Non-pwm ESC support not implemented");
            gpioTerminate();
            return Connection_Result::FAILED;
        }
    }


#ifdef USE_MS5611
    m_p->baro.init("/dev/i2c-1");
#endif

#ifdef USE_MPU9250
    m_p->mpu.init("/dev/i2c-1", MPU9250::Gyroscope_Range::_500_DPS, MPU9250::Accelerometer_Range::_4_G);
#endif

    m_is_connected = true;
    return Connection_Result::OK;
}

void IO_Board_RPi::disconnect()
{
    m_is_connected = false;
}
bool IO_Board_RPi::is_disconnected() const
{
    return !m_is_connected;
}
bool IO_Board_RPi::is_running() const
{
    return m_is_connected;
}

void IO_Board_RPi::set_motor_throttles(float const* throttles, size_t count)
{
    QASSERT(throttles != nullptr);
    if (!throttles)
    {
        return;
    }

    count = math::min(count, MOTOR_GPIOS.size());
    if (count > m_motors.size())
    {
        m_motors.resize(count);
    }

    for (size_t i = 0; i < count; i++)
    {
        auto throttle = math::clamp(throttles[i], 0.f, 1.f);
        m_motors[i].throttle = throttle;

        if (m_pwm_frequency >= PWM_Frequency::PWM_1000Hz)
        {
            uint32_t pulse = throttle * GPIO_PWM_RANGE;
            gpioPWM(MOTOR_GPIOS[i], pulse);
        }
        else
        {
            QASSERT(0);
        }
    }
}

void IO_Board_RPi::set_camera_rotation(math::quatf const& rot)
{
    math::anglef pitch, roll, yaw;
    rot.get_as_euler_xyz(pitch, roll, yaw);

    if (CAMERA_YAW_GPIO > 0)
    {
        //uint32_t pulse = (euler.z * 2500.f) + 1250.f
        //gpioServo(CAMERA_YAW_GPIO, )
    }
    if (CAMERA_PITCH_GPIO > 0)
    {
        auto delta = math::anglef(pitch) - math::anglef(0);
        auto x = math::clamp(delta.radians / math::anglef::pi, -1.f, 1.f);

        uint32_t pulse = static_cast<uint32_t>((x * 2500.f) + 1250.f);
        gpioServo(CAMERA_PITCH_GPIO, pulse);
    }
    if (CAMERA_ROLL_GPIO > 0)
    {
        auto delta = math::anglef(roll) - math::anglef(0);
        auto x = math::clamp(delta.radians / math::anglef::pi, -1.f, 1.f);

        uint32_t pulse = static_cast<uint32_t>((x * 2500.f) + 1250.f);
        gpioServo(CAMERA_ROLL_GPIO, pulse);
    }
}

void IO_Board_RPi::set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale)
{
    m_calibration_config.accelerometer_bias = bias;
    m_calibration_config.accelerometer_scale = scale;
    save_settings();
}
void IO_Board_RPi::get_accelerometer_calibration_data(math::vec3f &bias, math::vec3f &scale) const
{
    bias = m_calibration_config.accelerometer_bias;
    scale = m_calibration_config.accelerometer_scale;
}

void IO_Board_RPi::set_gyroscope_calibration_data(math::vec3f const& bias)
{
    m_calibration_config.gyroscope_bias = bias;
    save_settings();
}
void IO_Board_RPi::get_gyroscope_calibration_data(math::vec3f &bias) const
{
    bias = m_calibration_config.gyroscope_bias;
}

void IO_Board_RPi::set_compass_calibration_data(math::vec3f const& bias)
{
    m_calibration_config.compass_bias = bias;
    save_settings();
}
void IO_Board_RPi::get_compass_calibration_data(math::vec3f &bias) const
{
    bias = m_calibration_config.compass_bias;
}

auto IO_Board_RPi::get_sensor_samples() const -> std::vector<Sensor_Sample> const&
{
    return m_sensor_samples;
}

size_t IO_Board_RPi::get_error_count() const
{
    return m_error_count;
}

void IO_Board_RPi::process()
{
    if (is_disconnected())
    {
        connect();
        return;
    }

    m_sensor_samples.clear();

    auto start = q::Clock::now();

#ifdef USE_MPU9250
    auto a_sample_time = m_p->mpu.get_sample_time();
    auto g_sample_time = a_sample_time;
    m_p->mpu.process();
    auto const& g_samples = m_p->mpu.get_gyroscope_samples();
    auto const& a_samples = m_p->mpu.get_accelerometer_samples();
    auto compass_sample = m_p->mpu.read_compass();
    if (compass_sample)
    {
        m_sensors.compass.value = *compass_sample - m_calibration_config.compass_bias;
        m_sensors.compass.sample_idx++;
    }
#endif

#ifdef USE_MS5611
    m_p->baro.process();
    auto baro_sample = m_p->baro.read_pressure();
    if (baro_sample)
    {
        m_sensors.barometer.value = *baro_sample;
        m_sensors.barometer.sample_idx++;
    }
    auto temp_sample = m_p->baro.read_temperature();
    if (temp_sample)
    {
        m_sensors.thermometer.value = *temp_sample;
        m_sensors.thermometer.sample_idx++;
    }
#endif

    QASSERT(a_samples.size() == g_samples.size());
    m_sensor_samples.resize(a_samples.size());
    for (size_t i = 0; i < a_samples.size(); i++)
    {
        auto& sample = m_sensor_samples[i];

        {
            sample.accelerometer.value.acceleration = (a_samples[i] - m_calibration_config.accelerometer_bias) * m_calibration_config.accelerometer_scale;
            sample.accelerometer.value.dt = a_sample_time;
            sample.accelerometer.sample_idx = m_sensors.accelerometer.sample_idx++;
        }
        {
            sample.gyroscope.value.angular_velocity = g_samples[i] - m_calibration_config.gyroscope_bias;
            sample.gyroscope.value.dt = g_sample_time;
            sample.gyroscope.sample_idx = m_sensors.gyroscope.sample_idx++;
        }

        sample.barometer = m_sensors.barometer;
        sample.thermometer = m_sensors.thermometer;
        sample.compass = m_sensors.compass;
    }

//    auto d = q::Clock::now() - start;
//    SILK_INFO("d = {}, {}", d, m_sensor_samples.size());
}


#endif
