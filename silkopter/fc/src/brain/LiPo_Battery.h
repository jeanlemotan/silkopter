#pragma once

#include "common/node/IProcessor.h"
#include "common/stream/IVoltage.h"
#include "common/stream/ICurrent.h"
#include "common/stream/IBattery_State.h"

#include "HAL.h"
#include "utils/Butterworth.h"

namespace silk
{

class LiPo_Battery : q::util::Noncopyable
{
public:
    LiPo_Battery();

    ts::Result<void> init(size_t rate);

    struct Config
    {
        float full_charge = 3;//Ah

    };

    ts::Result<void> set_config(Config const& config);
    void reset();

    auto get_cell_count() const -> boost::optional<uint8_t>;

    auto process(stream::IVoltage::Sample const& v_sample, stream::ICurrent::Sample const& c_sample) -> stream::IBattery_State::Sample const&;

private:
    Config m_config;
    size_t m_rate = 0;
    size_t m_samples_received = 0;

    boost::optional<uint8_t> compute_cell_count();
    boost::optional<uint8_t> m_cell_count;

    stream::IBattery_State::Sample m_battery_state;

    util::Butterworth<stream::IVoltage::Value> m_voltage_filter;
    util::Butterworth<stream::ICurrent::Value> m_current_filter;
};


}
