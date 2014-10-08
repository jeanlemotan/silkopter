#pragma once

namespace silk
{

class PiGPIO;

class HAL_Camera_Mount_PiGPIO : q::util::Noncopyable
{
public:
    HAL_Camera_Mount_PiGPIO(PiGPIO& pigpio);
    ~HAL_Camera_Mount_PiGPIO();

    //----------------------------------------------------------------------
    //mount

    void set_rotation(math::quatf const& rot);

    //----------------------------------------------------------------------
    void process();

private:
    PiGPIO& m_pigpio;
};

}
