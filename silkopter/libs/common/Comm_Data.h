#pragma once

#include <cstdint>

namespace silk
{
namespace rc_comms
{

struct Video_Header
{
    uint32_t width : 11;
    uint32_t height : 11;
    uint32_t fec_k : 4;
    uint32_t fec_n : 5;
};

enum class Video_Message : uint8_t
{
    FRAME_DATA,
    COUNT //keep last
};

enum class Telemetry_Message : uint8_t
{
    STREAM_DATA,
    UAV_DATA,

    COUNT //keep last
};

//enum class Setup_Message : uint8_t
//{
//    CLOCK,

//    UAV_DESCRIPTOR,

//    ENUMERATE_NODE_DEFS,
//    ENUMERATE_NODES,
//    GET_NODE_DATA,

//    ADD_NODE,
//    REMOVE_NODE,
//    NODE_CONFIG,
//    NODE_MESSAGE,
//    NODE_INPUT_STREAM_PATH,

//    STREAM_TELEMETRY_ACTIVE,
//    UAV_TELEMETRY_ACTIVE,

//    COUNT //keep last
//};

enum class Pilot_Message : uint8_t
{
    MULTIROTOR_COMMANDS,
    MULTIROTOR_STATE,

    AIRPLANE_COMMANDS,
    AIRPLANE_STATE,

    ROVER_COMMANDS,
    ROVER_STATE,

    BOAT_COMMANDS,
    BOAT_STATE,

    COUNT //keep last
};


enum class Packet_Type : uint8_t
{
    //RC_CONNECTED,

    MULTIROTOR_COMMANDS,
    CAMERA_COMMANDS,
    MULTIROTOR_STATE,
    VIDEO
};

}
}
