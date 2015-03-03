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
    //returns all the possible nodes in the system
    //REQ:
    //  uint32_t req_id
    //RES:
    //  uint32_t req_id
    //  uint32_t source_count
    //  uint32_t sink_count
    //  uint32_t processor_count
    //      sources[source_count]
    //          string - name
    //          class_id - type
    //          uint32_t - output count
    //              class_id - output type
    //              string - name
    //          string - default init prams
    //          string - default config
    //      sinks[sink_count]
    //          string - name
    //          class_id - type
    //          uint32_t - input count
    //              class_id - input type
    //              string - name
    //          string - default init prams
    //          string - default config
    //      processors[processor_count]
    //          string - name
    //          class_id - type
    //          uint32_t - input count
    //              class_id - input type
    //              string - name
    //          uint32_t - output count
    //              class_id - output type
    //              string - name
    //          string - default init prams
    //          string - default config
    ENUMERATE_NODE_DEFS,

    //returns all the nodes in the system
    //REQ:
    //  uint32_t req_id
    //RES:
    //  uint32_t req_id
    //  uint32_t stream_count
    //  uint32_t source_count
    //  uint32_t sink_count
    //  uint32_t processor_count
    //      streams[stream_count]
    //          string - name
    //          class_id - type
    //          uint32_t - sample rate
    //      sources[source_count]
    //          string - name
    //          class_id - type
    //          uint32_t output_count
    //              string - name of output stream
    //          string - init prams
    //          string - config
    //      sinks[sink_count]
    //          string - name
    //          class_id - type
    //          uint32_t input_count - array size
    //              string - name of input stream
    //          string - init prams
    //          string - config
    //      processors[processor_count]
    //          string - name
    //          class_id - type
    //          uint32_t input_count - array size
    //              string[input_count] - name of the stream
    //          uint32_t output_count - array size
    //              string[output_count] - name of the stream
    //          string - init prams
    //          string - config
    ENUMERATE_NODES,

    //sets/gets a node init params as a json string
    //REQ:
    //  uint32_t req_id
    //  string - node name
    //  [optional] string - if present, the init params to set. If not set - it's a request. NOTE - settinng this triggers a reboot
    //RES:
    //  uint32_t req_id
    //  string - node name
    //  string - the init params
    SOURCE_INIT_PARAMS,
    STREAM_INIT_PARAMS,
    PROCESSOR_INIT_PARAMS,
    SINK_INIT_PARAMS,

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
