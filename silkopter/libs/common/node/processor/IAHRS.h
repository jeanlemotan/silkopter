#pragma once

#include "common/node/processor/IProcessor.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/IReference_Frame.h"

namespace silk
{
namespace node
{
namespace processor
{

class IAHRS : public IProcessor
{
public:
    virtual auto get_input_angular_velocity_stream(size_t idx) -> stream::IAngular_Velocity& = 0;
    virtual auto get_input_acceleration_stream(size_t idx) -> stream::IAcceleration& = 0;
    virtual auto get_input_magnetic_field_stream(size_t idx) -> stream::IMagnetic_Field& = 0;
    virtual auto get_output_reference_frame_stream() -> stream::IReference_Frame& = 0;

    //constants
    static constexpr math::vec3f k_local_front_vector = math::vec3f(0, 1, 0);
    static constexpr math::vec3f k_local_right_vector = math::vec3f(1, 0, 0);
    static constexpr math::vec3f k_local_up_vector = math::vec3f(0, 0, 1);

    static constexpr math::vec3f k_world_gravity_acceleration = math::vec3f(0, 0, 9.80665f);
};



}
}
}
