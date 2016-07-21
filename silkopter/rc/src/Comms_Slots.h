#pragma once

#include "Comms.h"

class Comms_Slots
{
public:
    Comms_Slots(silk::Comms& comms);

    virtual void refresh();

private:
    virtual void slot_clock_received(silk::Manual_Clock::time_point tp) = 0;
    virtual void slot_reset() = 0;

    struct Connections
    {
        q::util::Scoped_Connection clock_received;
        q::util::Scoped_Connection reset;
    } m_connections;

protected:
    silk::Comms& m_comms;
};

