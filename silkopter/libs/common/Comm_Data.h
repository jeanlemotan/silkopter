#pragma once

namespace silk
{
namespace comms
{

enum class Telemetry_Message : uint8_t
{
    //------------------------
    //streams
    ACCELERATION_STREAM,
    ADC_VALUE_STREAM,
    ANGULAR_VELOCITY_STREAM,
    BATTERY_STATE_STREAM,
    CARDINAL_POINTS_STREAM,
    CURRENT_STREAM,
    DISTANCE_STREAM,
    LINEAR_ACCELERATION_STREAM,
    LOCATION_STREAM,
    MAGNETIC_FIELD_STREAM,
    PRESSURE_STREAM,
    PWM_VALUE_STREAM,
    REFERENCE_FRAME_STREAM,
    TEMPERATURE_STREAM,
    VOLTAGE_STREAM,
    VIDEO_STREAM,
};

enum class Sim_Message : uint8_t
{
    SENSORS,
    MOTORS,
};


enum class Setup_Message : uint8_t
{
    //nodes
    ENUMERATE_SOURCES,
    ENUMERATE_STREAMS,
    ENUMERATE_PROCESSORS,
    ENUMERATE_SINKS,

    SOURCE_CONFIG,
    STREAM_CONFIG,
    PROCESSOR_CONFIG,
    SINK_CONFIG,

    //injection


    // Multirotor
    MULTIROTOR_MODE,
    REQUEST_MULTIROTOR_INPUT,

    // Plane
    AIRPLANE_MODE,
    REQUEST_AIRPLANE_INPUT,

    // Rover
    ROVER_MODE,
    REQUEST_ROVER_INPUT,

    // Boat
    BOAT_MODE,
    REQUEST_BOAT_INPUT,
};

enum class Input_Message : uint8_t
{
    MULTIROTOR_INPUT,
    AIRPLANE_INPUT,
    ROVER_INPUT,
    BOAT_INPUT,
};



}
}
