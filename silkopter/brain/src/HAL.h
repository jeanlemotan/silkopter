#pragma once

#include "HAL_Camera.h"
#include "HAL_Camera_Mount.h"
#include "HAL_Motors.h"
#include "HAL_Sensors.h"

namespace silk
{

class Comms;

struct HAL : q::util::Noncopyable
{
    std::unique_ptr<HAL_Sensors> sensors;
    std::unique_ptr<HAL_Motors> motors;
    std::unique_ptr<HAL_Camera> camera;
    std::unique_ptr<HAL_Camera_Mount> camera_mount;

    auto init(Comms& comms) -> bool;
    void shutdown();
    void process();

private:
    struct Impl;
    std::shared_ptr<Impl> m_impl;
};


}
