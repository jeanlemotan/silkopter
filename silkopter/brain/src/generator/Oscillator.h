#pragma once

#include "HAL.h"
#include "common/stream/IFloat.h"
#include "common/node/IGenerator.h"

#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
{
struct Oscillator_Descriptor;
struct Oscillator_Config;
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

    bool init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    bool set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<hal::Oscillator_Descriptor> m_descriptor;
    std::shared_ptr<hal::Oscillator_Config> m_config;

    float m_period = 0;
    std::uniform_real_distribution<float> m_rnd_distribution;
    std::mt19937 m_rnd_engine;
    bool m_has_noise = false;

    typedef Basic_Output_Stream<stream::IFloat> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


}
}

