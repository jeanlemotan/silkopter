#pragma once

#include "common/node/processor/IProcessor.h"
#include "common/node/stream/IReference_Frame.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/ICardinal_Points.h"

namespace silk
{
namespace node
{
namespace processor
{

class ICompass : public IProcessor
{
public:
    virtual auto get_input_reference_frame_stream() -> stream::IReference_Frame& = 0;
    virtual auto get_input_magnetic_field_stream() -> stream::IMagnetic_Field& = 0;

    virtual auto get_output_cardinal_points_stream() -> stream::ICardinal_Points& = 0;
};


}
}
}
