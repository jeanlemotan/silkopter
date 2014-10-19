#include "BrainStdAfx.h"
#include "HAL_Motors_Sim.h"
#include "utils/Json_Helpers.h"

#ifndef RASPBERRY_PI

using namespace silk;
using namespace boost::asio;

HAL_Motors_Sim::HAL_Motors_Sim(Sim_Comms& sim_comms)
    : m_sim_comms(sim_comms)
{
}

void HAL_Motors_Sim::cut_throttle()
{
    static std::array<float, 255> zero_throttles = {0};
    set_throttles(zero_throttles.data(), zero_throttles.size());
}

void HAL_Motors_Sim::set_throttles(float const* throttles, size_t count)
{
    QASSERT(throttles != nullptr);
    if (!throttles)
    {
        return;
    }
    QASSERT(count < 256);
    m_sim_comms.m_channel.begin_pack();
    m_sim_comms.m_channel.pack_param(static_cast<uint8_t>(count));
    for (size_t i = 0; i < count; i++)
    {
       auto v = math::clamp(throttles[i], 0.f, 1.f);
       m_sim_comms.m_channel.pack_param(v);
    }
    m_sim_comms.m_channel.end_pack(Sim_Comms::Message::MOTOR_OUTPUTS);
}

void HAL_Motors_Sim::process()
{
    if (!m_sim_comms.is_connected())
    {
        m_sim_comms.connect();
    }
    else
    {
        m_sim_comms.process();
    }
}


#endif
