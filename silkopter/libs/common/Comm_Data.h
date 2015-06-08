#pragma once

namespace silk
{
namespace comms
{


enum class Video_Message : uint8_t
{
    FRAME_DATA,
    COUNT //keep last
};

enum class Telemetry_Message : uint8_t
{
    STREAM_DATA,
    HAL_DATA,

    COUNT //keep last
};

enum class Setup_Message : uint8_t
{
    CLOCK,

    MULTI_CONFIG,
//    COPTER_CONFIG,
//    PLANE_CONFIG,
//    ROVER_CONFIG,
//    BOAT_CONFIG,

    ENUMERATE_NODE_DEFS,
    ENUMERATE_NODES,

    ADD_NODE,
    REMOVE_NODE,
    NODE_CONFIG,
    NODE_MESSAGE,
    NODE_DATA,

    STREAM_TELEMETRY_ACTIVE,
    HAL_TELEMETRY_ACTIVE,

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
