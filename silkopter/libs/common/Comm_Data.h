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


enum class Setup_Message : uint8_t
{
    //returns all the sources in the system
    //REQ:
    //  uint32_t req_id
    //RES:
    //  uint32_t req_id
    //  uint32_t source_count - array size
    //      string - source name
    //      uint32_t output_count - array size
    //          type - type of output stream
    //          string - name of output stream
    ENUMERATE_SOURCES,

    //returns all the streams in the system
    //REQ:
    //  uint32_t req_id
    //RES:
    //  uint32_t req_id
    //  uint32_t stream_count - array size
    //      type - type of stream
    //      string - name of stream
    //      rate - frequency of the stream samples
    ENUMERATE_STREAMS,

    //returns all the processors in the system
    //REQ:
    //  uint32_t req_id
    //RES:
    //  uint32_t req_id
    //  uint32_t processor_count - array size
    //      string - processor name
    //      uint32_t input_count - array size
    //          type - type of input stream
    //          string - name of input stream
    //      uint32_t output_count - array size
    //          type - type of output stream
    //          string - name of output stream
    ENUMERATE_PROCESSORS,

    //returns all the sinks in the system
    //REQ:
    //  uint32_t req_id
    //RES:
    //  uint32_t req_id
    //  uint32_t sink_count - array size
    //      string - processor name
    //      uint32_t input_count - array size
    //          type - type of input stream
    //          string - name of input stream
    ENUMERATE_SINKS,

    //sets/gets a node config as a json string
    //REQ:
    //  uint32_t req_id
    //  string - node name
    //  [optional] string - if present, the config to set. If not set - it's a request
    //RES:
    //  uint32_t req_id
    //  string - node name
    //  string - the config
    SOURCE_CONFIG,
    STREAM_CONFIG,
    PROCESSOR_CONFIG,
    SINK_CONFIG,

    //start or stop telemetry streaming.
    //REQ:
    //  uint32_t req_id
    //  [optional] uint32_t size - array size - if present it's a setter. If not, it's a request
    //  [optional] string[size] - names of the streams
    //RES:
    //  uint32_t req_id
    //  uint32_t size - array size
    //  string[size] - names of the streams that will be sent as telemetry
    TELEMETRY_STREAMS,

    //-----------------------------
    // Multirotor

    //Set/Get the mode
    //REQ:
    //  uint32_t req_id
    //  string name - the multirotor name
    //  [optional] Multirotor::Mode - if present, the mode to change to. If not, it's a getter
    //REQ:
    //  uint32_t req_id
    //  Multirotor::Mode - the current mode
    MULTIROTOR_MODE,

    //Get the input
    //REQ:
    //  uint32_t req_id
    //  string name - the multirotor name
    //REQ:
    //  uint32_t req_id
    //  Multirotor::Input - the current input values
    MULTIROTOR_INPUT_REQUEST,

//    // Plane
//    AIRPLANE_MODE,
//    REQUEST_AIRPLANE_INPUT,

//    // Rover
//    ROVER_MODE,
//    REQUEST_ROVER_INPUT,

//    // Boat
//    BOAT_MODE,
//    REQUEST_BOAT_INPUT,

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
