#pragma once

#include "UAV.h"
#include "common/stream/IFloat.h"
#include "common/node/IGenerator.h"

#include "Basic_Output_Stream.h"


namespace silk
{
struct Oscillator_Descriptor;
struct Oscillator_Config;
}

namespace silk
{
namespace node
{

class Oscillator : public IGenerator
{
public:
    Oscillator(UAV& uav);

    bool init(std::shared_ptr<Node_Descriptor_Base> descriptor) override;
    std::shared_ptr<Node_Descriptor_Base> get_descriptor() const override;

    bool set_config(std::shared_ptr<Node_Config_Base> config) override;
    std::shared_ptr<Node_Config_Base> get_config() const override;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

    std::shared_ptr<Oscillator_Descriptor> m_descriptor;
    std::shared_ptr<Oscillator_Config> m_config;

    float m_period = 0;
    std::uniform_real_distribution<float> m_rnd_distribution;
    std::mt19937 m_rnd_engine;
    bool m_has_noise = false;

    typedef Basic_Output_Stream<stream::IFloat> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


}
}

