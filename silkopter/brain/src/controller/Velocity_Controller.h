#pragma once

#include "common/node/IController.h"
#include "common/node/stream/IVelocity.h"
#include "common/node/stream/IFrame.h"
#include "common/node/stream/IForce.h"
#include "HAL.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace sz
{
namespace Velocity_Controller
{
struct Init_Params;
struct Config;
}
}



namespace silk
{
namespace node
{

class Velocity_Controller : public IController
{
public:
    Velocity_Controller(HAL& hal);

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

    std::shared_ptr<sz::Velocity_Controller::Init_Params> m_init_params;
    std::shared_ptr<sz::Velocity_Controller::Config> m_config;

    Sample_Accumulator<stream::IVelocity, stream::IVelocity> m_accumulator;

    typedef Basic_Output_Stream<stream::IFrame> Output_Frame_Stream;
    mutable std::shared_ptr<Output_Frame_Stream> m_output_frame_stream;

    typedef Basic_Output_Stream<stream::IForce> Output_Force_Stream;
    mutable std::shared_ptr<Output_Force_Stream> m_output_force_stream;
};



}
}
