#pragma once

namespace silk
{
namespace comms
{


enum class Sim_Message : uint8_t
{
    SENSORS,
    MOTORS,
};

enum class Telemetry_Message : uint8_t
{
    STREAM_DATA,

    COUNT //keep last
};

enum class Setup_Message : uint8_t
{
    ENUMERATE_NODE_DEFS,
    ENUMERATE_NODES,

    ADD_NODE,
    REMOVE_NODE,
    NODE_CONFIG,
    NODE_DATA,

    STREAM_TELEMETRY_ACTIVE,

    COUNT //keep last
};

enum class Input_Message : uint8_t
{
    MULTIROTOR_INPUT,
    AIRPLANE_INPUT,
    ROVER_INPUT,
    BOAT_INPUT,

    COUNT //keep last
};



}
}
