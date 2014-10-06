#pragma once

namespace silk
{

class Sensor_Interface;
class Motor_Interface;
class Camera_Interface;


struct HW_Interfaces
{
    std::shared_ptr<Sensor_Interface> sensor_interface;
    std::shared_ptr<Motor_Interface> motor_interface;
    std::shared_ptr<Camera_Interface> camera_interface;

    HW_Interfaces();

    enum class Result
    {
        OK,
        FAILED
    };

    auto init() -> Result;
    void shutdown();
};


}
