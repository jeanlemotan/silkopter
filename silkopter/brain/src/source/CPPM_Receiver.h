#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/stream/IPWM.h"

#include "Basic_Output_Stream.h"

#include <atomic>
#include <vector>

namespace silk
{
namespace hal
{
struct CPPM_Receiver_Descriptor;
struct CPPM_Receiver_Config;
}
}



namespace silk
{
namespace node
{

class CPPM_Receiver : public ISource
{
public:
    CPPM_Receiver(HAL& hal);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    ts::Result<void> start(Clock::time_point tp) override;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();

    HAL& m_hal;

    std::shared_ptr<hal::CPPM_Receiver_Descriptor> m_descriptor;
    std::shared_ptr<hal::CPPM_Receiver_Config> m_config;

    Clock::time_point m_last_tp = Clock::now();

    static constexpr size_t MAX_CHANNEL_COUNT = 32;

    struct Callback_Data
    {
        uint32_t gpio = 0;
        bool is_inverted = false;

        enum class State
        {
            WAITING_FOR_SYNC,
            WAITING_FOR_GAP,
            WAITING_FOR_CHANNEL
        } state = State::WAITING_FOR_SYNC;

        enum Error
        {
            ERROR_NONE,
            ERROR_SYNC,
            ERROR_GAP,
            ERROR_CHANNEL,
            ERROR_UNKNOWN
        };

        //this is atomic as we're reading it from the main thread
        std::atomic_int error = { Error::ERROR_NONE };

        size_t channel_idx = 0;
        size_t channel_count = 0;
        std::array<std::atomic_uint, MAX_CHANNEL_COUNT> channel_pulses_us;

        uint32_t frame_length_us = 0;

        uint32_t min_gap_pulse_us = 0;
        uint32_t max_gap_pulse_us = 0;

        uint32_t min_channel_pulse_us = 0;
        uint32_t max_channel_pulse_us = 0;


        uint32_t min_sync_pulse_us = 0;
        uint32_t max_sync_pulse_us = 0;

        uint32_t last_tick = 0;
    } m_callback_data;

    static void gpio_callback_func(int gpio, int level, uint32_t tick, void* userdata);

    void refresh_gpio_callback_data();

    typedef Basic_Output_Stream<stream::IPWM> Output_Stream;
    std::vector<std::shared_ptr<Output_Stream>> m_pwms;
};


}
}
