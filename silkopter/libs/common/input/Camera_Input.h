#pragma once

namespace silk
{

namespace camera_input
{
    enum class Input : uint8_t
    {
        STREAM_QUALITY,
        START_RECORDING,
        STOP_RECORDING,
        SHUTTER_SPEED,
        ISO,
    };

    typedef q::Clock::duration Shutter_Speed; //zero means auto
    typedef uint32_t Iso; //zero means auto

    enum class Stream_Quality : uint8_t
    {
        HIGH,
        MEDIUM,
        LOW
    };

}
}
