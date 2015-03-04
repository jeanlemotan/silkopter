#include "BrainStdAfx.h"
#include "Inertial.h"
#include "physics/constants.h"

#include "sz_math.hpp"
#include "sz_Inertial.hpp"

namespace silk
{
namespace node
{
namespace processor
{

Inertial::Inertial(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Inertial::Init_Params())
    , m_config(new sz::Inertial::Config())
{
}

auto Inertial::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("inertial::init");

    sz::Inertial::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Inertial data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto Inertial::init() -> bool
{
    m_stream = std::make_shared<Stream>();
    return true;
}

auto Inertial::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(2);
    inputs[0].class_id = q::rtti::get_class_id<stream::IReference_Frame>();
    inputs[0].name = "Reference_ Frame";
    inputs[0].stream = m_config->inputs.reference_frame;
    inputs[1].class_id = q::rtti::get_class_id<stream::IAcceleration>();
    inputs[1].name = "Acceleration";
    inputs[1].stream = m_config->inputs.acceleration;
    return inputs;
}
auto Inertial::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].class_id = q::rtti::get_class_id<stream::ILinear_Acceleration>();
    outputs[0].name = "Linear Acceleration";
    outputs[0].stream = m_stream;
    return outputs;
}

void Inertial::process()
{
    m_stream->samples.clear();

    auto reference_frame_stream = m_reference_frame_stream.lock();
    auto acceleration_stream = m_acceleration_stream.lock();
    if (!reference_frame_stream || !acceleration_stream)
    {
        return;
    }

    //accumulate the input streams
    {
        auto const& samples = reference_frame_stream->get_samples();
        m_reference_frame_samples.reserve(m_reference_frame_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_reference_frame_samples));
    }
    {
        auto const& samples = acceleration_stream->get_samples();
        m_acceleration_samples.reserve(m_acceleration_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_acceleration_samples));
    }

    //TODO add some protecton for severely out-of-sync streams

    size_t count = std::min(m_reference_frame_samples.size(), m_acceleration_samples.size());
    if (count == 0)
    {
        return;
    }

    m_stream->samples.resize(count);

    for (size_t i = 0; i < count; i++)
    {
        m_stream->last_sample.dt = m_dt;
        m_stream->last_sample.sample_idx++;

        auto w2l = math::inverse(m_reference_frame_samples[i].value.local_to_world);
        auto gravity_local = math::rotate(w2l, physics::constants::world_gravity);
        m_stream->last_sample.value = m_acceleration_samples[i].value - gravity_local;
        m_stream->samples[i] = m_stream->last_sample;
    }

    //consume processed samples
    m_reference_frame_samples.erase(m_reference_frame_samples.begin(), m_reference_frame_samples.begin() + count);
    m_acceleration_samples.erase(m_acceleration_samples.begin(), m_acceleration_samples.begin() + count);
}

auto Inertial::set_config(rapidjson::Value const& json) -> bool
{
    sz::Inertial::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Inertial config data: {}", ss.str());
        return false;
    }

    auto reference_frame_stream = m_hal.get_streams().find_by_name<stream::IReference_Frame>(sz.inputs.reference_frame);
    auto acceleration_stream = m_hal.get_streams().find_by_name<stream::IAcceleration>(sz.inputs.acceleration);
    if (!reference_frame_stream || reference_frame_stream->get_rate() == 0)
    {
        QLOGE("No input angular velocity stream specified");
        return false;
    }
    if (!acceleration_stream || acceleration_stream->get_rate() == 0)
    {
        QLOGE("No input acceleration stream specified");
        return false;
    }
    if (acceleration_stream->get_rate() != reference_frame_stream->get_rate())
    {
        QLOGE("Reference Frame and Acceleration streams have different rates: {} != {}",
              reference_frame_stream->get_rate(),
              acceleration_stream->get_rate());
        return false;
    }

    if (m_stream->rate != 0 && m_stream->rate != acceleration_stream->get_rate())
    {
        QLOGE("Input streams rate has changed: {} != {}",
              acceleration_stream->get_rate(),
              m_stream->rate);
        return false;
    }

    m_dt = std::chrono::microseconds(1000000 / m_stream->get_rate());

    m_reference_frame_stream = reference_frame_stream;
    m_acceleration_stream = acceleration_stream;
    m_stream->rate = reference_frame_stream->get_rate();

    *m_config = sz;
    return true;
}
auto Inertial::get_config() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Inertial::get_init_params() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}


}
}
}
