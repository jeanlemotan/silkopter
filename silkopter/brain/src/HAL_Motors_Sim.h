#pragma once

namespace silk
{
class Sim_Comms;

class HAL_Motors_Sim : q::util::Noncopyable
{
public:
    HAL_Motors_Sim(Sim_Comms& sim_comms);
    ~HAL_Motors_Sim();

    //----------------------------------------------------------------------
    //motors

    auto get_count() const -> size_t;
    void set_throttles(float const* throttles, size_t count);

    //----------------------------------------------------------------------
    void process();

private:
    Sim_Comms& m_sim_comms;
};

}
