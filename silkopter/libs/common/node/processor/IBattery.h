#pragma once

#include "common/node/processor/IProcessor.h"
#include "common/node/stream/IVoltage.h"
#include "common/node/stream/ICurrent.h"
#include "common/node/stream/IBattery_State.h"

namespace silk
{
namespace node
{
namespace processor
{

class IBattery : public IProcessor
{
public:
    virtual auto get_input_voltage_stream(size_t idx) -> stream::IVoltage& = 0;
    virtual auto get_input_current_stream(size_t idx) -> stream::ICurrent& = 0;

    virtual auto get_output_stream(size_t idx) -> stream::IBattery_State& = 0;
};


}
}
}
