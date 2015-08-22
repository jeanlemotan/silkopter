#pragma once

#include "common/node/IProcessor.h"
#include "common/stream/IAngular_Velocity.h"
#include "common/stream/IAcceleration.h"
#include "common/stream/IMagnetic_Field.h"
#include "common/stream/IFrame.h"
#include "HAL.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace sz
{
namespace Comp_AHRS
{
struct Init_Params;
struct Config;
}
}



namespace silk
{
namespace node
{

class Comp_AHRS : public IProcessor
{
public:
    Comp_AHRS(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::Comp_AHRS::Init_Params> m_init_params;
    std::shared_ptr<sz::Comp_AHRS::Config> m_config;

    Sample_Accumulator<stream::IAngular_Velocity, stream::IAcceleration, stream::IMagnetic_Field> m_accumulator;

    math::vec3f m_noisy_front_w;
    math::vec3f m_noisy_right_w;
    math::vec3f m_noisy_up_w;

    typedef Basic_Output_Stream<stream::IFrame> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};



}
}
