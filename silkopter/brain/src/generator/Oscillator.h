#pragma once

#include "HAL.h"
#include "common/node/stream/IFloat.h"
#include "common/node/IGenerator.h"

#include "Basic_Output_Stream.h"


namespace sz
{
namespace Oscillator
{
struct Init_Params;
struct Config;
}
}

namespace silk
{
namespace node
{

class Oscillator : public IGenerator
{
public:
    Oscillator(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_stream_inputs() const -> std::vector<Stream_Input>;
    auto get_stream_outputs() const -> std::vector<Stream_Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::Oscillator::Init_Params> m_init_params;
    std::shared_ptr<sz::Oscillator::Config> m_config;

    float m_period = 0;
    std::uniform_real_distribution<float> m_rnd_distribution;
    std::mt19937 m_rnd_engine;

    typedef Basic_Output_Stream<stream::IFloat> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


}
}

