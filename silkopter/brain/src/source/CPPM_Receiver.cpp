#include "BrainStdAfx.h"
#include "CPPM_Receiver.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "hal.def.h"

#ifdef RASPBERRY_PI

extern "C"
{
    #include "utils/hw/pigpio.h"
}
extern std::chrono::microseconds PIGPIO_PERIOD;

#endif

namespace silk
{
namespace node
{

CPPM_Receiver::CPPM_Receiver(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::CPPM_Receiver_Descriptor())
    , m_config(new hal::CPPM_Receiver_Config())
{
    refresh_gpio_callback_data();
}

auto CPPM_Receiver::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs;
    for (size_t i = 0; i < m_pwms.size(); i++)
    {
        outputs.push_back({q::util::format<std::string>("pwm_{}", i), m_pwms[i]});
    }
    return outputs;
}
ts::Result<void> CPPM_Receiver::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("CPPM_Receiver::init");

    auto specialized = dynamic_cast<hal::CPPM_Receiver_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

ts::Result<void> CPPM_Receiver::init()
{
    if (m_descriptor->get_channel_count() >= MAX_CHANNEL_COUNT)
    {
        return make_error("Too many channels. Max is " + std::to_string(MAX_CHANNEL_COUNT));
    }

#ifdef RASPBERRY_PI

    m_pwms.resize(m_descriptor->get_channel_count());
    for (size_t i = 0; i < m_pwms.size(); i++)
    {
        m_pwms[i] = std::make_shared<Output_Stream>();
        m_pwms[i]->set_rate(m_descriptor->get_rate());
    }

    refresh_gpio_callback_data();

    uint32_t gpio = static_cast<uint32_t>(m_descriptor->get_gpio());
    m_callback_data.gpio = gpio;

    QLOGI("Installing alert callback on gpio {}", gpio);
    int res = gpioSetAlertFuncEx(gpio, &CPPM_Receiver::gpio_callback_func, &m_callback_data);
    if (res != 0)
    {
        return make_error("Cannot install gpio alert callback: {}", res);
    }

    return ts::success;

#else
    return make_error("PIGPIO only supported on the raspberry pi");
#endif
}

ts::Result<void> CPPM_Receiver::start(q::Clock::time_point tp)
{
    m_last_tp = tp;
    for (auto& pwm: m_pwms)
    {
        pwm->set_tp(tp);
    }
    return ts::success;
}

void CPPM_Receiver::gpio_callback_func(int gpio, int level, uint32_t tick, void* userdata)
{
    if (userdata == nullptr)
    {
        QASSERT(false);
        return;
    }

    Callback_Data& data = *reinterpret_cast<Callback_Data*>(userdata);
    if (static_cast<uint32_t>(gpio) != data.gpio)
    {
        QASSERT(false);
        return;
    }

    if (data.is_inverted)
    {
        level = level == 0 ? 1 : 0;
    }

    uint32_t delta = tick - data.last_tick;

    switch (data.state)
    {
    case Callback_Data::State::WAITING_FOR_SYNC:
    {
        //expecting a low level of sync_length duration
        if (level == 1 || delta < data.min_sync_pulse_us || delta > data.max_sync_pulse_us)
        {
            QLOGI("%%%Sync error, {}, {}", level, delta);
            data.error = Callback_Data::Error::ERROR_SYNC;
            break;
        }

//        QLOGI("*** Synced, {}, {}", level, delta);
        data.channel_idx = 0;
        data.state = Callback_Data::State::WAITING_FOR_GAP;
        break;
    }
    case Callback_Data::State::WAITING_FOR_GAP:
    {
        //expecting a high level of sync_length duration
        if (level == 0 || delta < data.min_gap_pulse_us || delta > data.max_gap_pulse_us)
        {
            QLOGI("%%%Gap error, {}, {}", level, delta);
            data.state = Callback_Data::State::WAITING_FOR_SYNC;
            data.error = Callback_Data::Error::ERROR_GAP;
            break;
        }

        if (data.channel_idx >= data.channel_count)
        {
            data.state = Callback_Data::State::WAITING_FOR_SYNC;
        }
        else
        {
            data.state = Callback_Data::State::WAITING_FOR_CHANNEL;
        }
        break;
    }
    case Callback_Data::State::WAITING_FOR_CHANNEL:
    {
        //expecting a high level of sync_length duration
        if (level == 1 || delta < data.min_channel_pulse_us || delta > data.max_channel_pulse_us)
        {
            QLOGI("%%%Channel error, {}, {}", level, delta);
            data.state = Callback_Data::State::WAITING_FOR_SYNC;
            data.error = Callback_Data::Error::ERROR_CHANNEL;
            break;
        }

        data.channel_pulses_us[data.channel_idx++] = delta;
        data.state = Callback_Data::State::WAITING_FOR_GAP;
        break;
    }
    default:
        QASSERT(0);
        data.state = Callback_Data::State::WAITING_FOR_SYNC;
        data.error = Callback_Data::Error::ERROR_UNKNOWN;
        break;
    }

    data.last_tick = tick;
}


void CPPM_Receiver::process()
{
    QLOG_TOPIC("CPPM_Receiver::process");

    for (auto& pwm: m_pwms)
    {
        pwm->clear();
    }

    if (m_pwms.empty())
    {
        return;
    }

    auto now = q::Clock::now();
    if (now - m_last_tp < m_pwms[0]->get_dt())
    {
        return;
    }

    //first make a copy of the pulses - to avoid them changing while working with them
    //note that the pulses array in the gpio_callback_data is atomic so this operation is safe
    int error = m_callback_data.error;
    std::array<uint32_t, MAX_CHANNEL_COUNT> channel_pulses_us;
    for (size_t i = 0; i < m_pwms.size(); i++)
    {
        channel_pulses_us[i] = m_callback_data.channel_pulses_us[i];
    }

    float min_channel_pulse_length_ms = m_config->get_min_pulse_length();
    float max_channel_pulse_length_ms = m_config->get_max_pulse_length();

    for (size_t i = 0; i < m_pwms.size(); i++)
    {
        Output_Stream& pwm = *m_pwms[i];
        size_t samples_needed = pwm.compute_samples_needed();
        if (samples_needed > 0)
        {
            float channel_value = ((static_cast<float>(channel_pulses_us[i]) / 1000.f) - min_channel_pulse_length_ms) / (max_channel_pulse_length_ms - min_channel_pulse_length_ms);
            channel_value = math::clamp(channel_value, 0.f, 1.f);

            for (size_t s = 0; s < samples_needed; s++)
            {
                pwm.push_sample(channel_value, error == Callback_Data::Error::ERROR_NONE);
            }
        }
    }

    //reset the error
    m_callback_data.error = Callback_Data::Error::ERROR_NONE;

    m_last_tp = now;
}

ts::Result<void> CPPM_Receiver::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("CPPM_Receiver::set_config");

    auto specialized = dynamic_cast<hal::CPPM_Receiver_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }

#ifdef RASPBERRY_PI
    float pigpio_period_ms = std::chrono::duration_cast<std::chrono::microseconds>(PIGPIO_PERIOD).count() / 1000.f;
#else
    float pigpio_period_ms = 0.005f;
#endif

    float frame_length_ms = specialized->get_frame_length();
    float min_channel_pulse_ms = specialized->get_min_pulse_length();
    float max_channel_pulse_ms = specialized->get_max_pulse_length();
    if (min_channel_pulse_ms >= max_channel_pulse_ms)
    {
        return make_error("Min pulse length is >= max pulse length");
    }
    float gap_pulse_ms = specialized->get_gap_pulse_length();

    float min_sync_pulse_ms = (max_channel_pulse_ms + gap_pulse_ms) * 2;

    float min_frame_length_ms = (max_channel_pulse_ms + gap_pulse_ms) * m_descriptor->get_channel_count() + min_sync_pulse_ms;
    if (min_frame_length_ms > frame_length_ms)
    {
        return make_error("Frame length is too small. The minimum is " + std::to_string(min_frame_length_ms) + "ms");
    }

    //check pulse resolution
    size_t pulse_steps = static_cast<size_t>((max_channel_pulse_ms - min_channel_pulse_ms) / pigpio_period_ms);
    if (pulse_steps < 50)
    {
        return make_error("Not enough pulse resolution. The detection resolution is " + std::to_string(pigpio_period_ms) + "ms");
    }

    //check gap resolution
    size_t gap_steps = static_cast<size_t>(gap_pulse_ms / pigpio_period_ms);
    if (gap_steps < 20)
    {
        return make_error("Not enough gap resolution. The detection resolution is " + std::to_string(pigpio_period_ms) + "ms");
    }

    *m_config = *specialized;

    refresh_gpio_callback_data();

    return ts::success;
}

void CPPM_Receiver::refresh_gpio_callback_data()
{
    float frame_length_ms = m_config->get_frame_length();
    float min_channel_pulse_ms = m_config->get_min_pulse_length() * 0.9f;
    float max_channel_pulse_ms = m_config->get_max_pulse_length() * 1.1f;

    float gap_pulse_ms = m_config->get_gap_pulse_length();
    float min_gap_pulse_ms = gap_pulse_ms * 0.9f;
    float max_gap_pulse_ms = gap_pulse_ms * 1.1f;

    float min_sync_pulse_ms = (max_channel_pulse_ms + gap_pulse_ms) * 2;
    float max_sync_pulse_ms = frame_length_ms - (min_channel_pulse_ms + gap_pulse_ms) * m_descriptor->get_channel_count();

    m_callback_data.is_inverted = m_config->get_inverted();

    m_callback_data.channel_count = m_pwms.size();

    m_callback_data.frame_length_us = static_cast<uint32_t>(frame_length_ms * 1000.f);
    QLOGI("Frame length: {}us", m_callback_data.frame_length_us);

    m_callback_data.min_gap_pulse_us = static_cast<uint32_t>(min_gap_pulse_ms * 1000.f);
    m_callback_data.max_gap_pulse_us = static_cast<uint32_t>(max_gap_pulse_ms * 1000.f);
    QLOGI("Gap length: {}us - {}us", m_callback_data.min_gap_pulse_us, m_callback_data.max_gap_pulse_us);

    m_callback_data.min_channel_pulse_us = static_cast<uint32_t>(min_channel_pulse_ms * 1000.f);
    m_callback_data.max_channel_pulse_us = static_cast<uint32_t>(max_channel_pulse_ms * 1000.f);
    QLOGI("Channel length: {}us - {}us", m_callback_data.min_channel_pulse_us, m_callback_data.max_channel_pulse_us);

    m_callback_data.min_sync_pulse_us = static_cast<uint32_t>(min_sync_pulse_ms * 1000.f);
    m_callback_data.max_sync_pulse_us = static_cast<uint32_t>(max_sync_pulse_ms * 1000.f);
    QLOGI("Sync length: {}us - {}us", m_callback_data.min_sync_pulse_us, m_callback_data.max_sync_pulse_us);
}

auto CPPM_Receiver::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto CPPM_Receiver::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

//auto CPPM_Receiver::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
