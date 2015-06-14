#pragma once

#include "HAL.h"
#include "common/node/stream/IThrottle.h"
#include "common/node/stream/IPWM.h"
#include "common/node/IProcessor.h"

#include "Basic_Output_Stream.h"

namespace sz
{
namespace Throttle_PWM
{
struct Init_Params;
struct Config;
}
}

namespace silk
{
namespace node
{

class Throttle_PWM : public IProcessor
{
public:
    Throttle_PWM(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void set_stream_input_path(size_t idx, q::Path const& path);
    auto get_stream_inputs() const -> std::vector<Stream_Input>;
    auto get_stream_outputs() const -> std::vector<Stream_Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::Throttle_PWM::Init_Params> m_init_params;
    std::shared_ptr<sz::Throttle_PWM::Config> m_config;

    IThrottle_wptr m_input_stream;

    typedef Basic_Output_Stream<stream::IPWM> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


}
}

