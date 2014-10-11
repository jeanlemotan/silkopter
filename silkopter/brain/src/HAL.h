#pragma once

#include "HAL_Camera.h"
#include "HAL_Motors.h"
#include "HAL_Sensors.h"

namespace silk
{


struct HAL : q::util::Noncopyable
{
    std::unique_ptr<HAL_Sensors> sensors;
    std::unique_ptr<HAL_Motors> motors;
    std::unique_ptr<HAL_Camera> camera;

    auto init(boost::asio::io_service& io_service) -> bool;
    void process();

private:
    struct Impl;
    std::shared_ptr<Impl> m_impl;
};


}
