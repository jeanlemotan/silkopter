#pragma once

#include "HAL_Motors.h"
#include "Sim_Comms.h"

namespace silk
{
class Sim_Comms;

class HAL_Motors_Sim : public HAL_Motors
{
public:
    HAL_Motors_Sim(Sim_Comms& sim_comms);
    ~HAL_Motors_Sim();

    //----------------------------------------------------------------------
    //motors

    void set_throttles(float const* throttles, size_t count);
    void cut_throttle();

    //----------------------------------------------------------------------
    void process();

private:
    Sim_Comms& m_sim_comms;
};

}
