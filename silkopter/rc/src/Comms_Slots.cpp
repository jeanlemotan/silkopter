#include "Comms_Slots.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

Comms_Slots::Comms_Slots(silk::Comms& comms)
    : m_comms(comms)
{
//    m_connections.reset = m_comms.sig_reset.connect(std::bind(&Comms_Slots::slot_reset, this));
//    m_connections.clock_received = m_comms.sig_clock_received.connect(std::bind(&Comms_Slots::slot_clock_received, this, std::placeholders::_1));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms_Slots::refresh()
{
}


