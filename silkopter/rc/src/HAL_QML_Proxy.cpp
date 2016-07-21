#include "HAL_QML_Proxy.h"
#include "def_lang/IStruct_Type.h"

extern silk::Comms s_comms;

HAL_QML_Proxy::HAL_QML_Proxy(QObject *parent)
    : QObject(parent)
    , Comms_Slots(s_comms)
{
}

void HAL_QML_Proxy::slot_clock_received(silk::Manual_Clock::time_point tp)
{
}

void HAL_QML_Proxy::slot_reset()
{
    m_type = UAV_Type::NONE;

    emit uavTypeChanged();
}

HAL_QML_Proxy::UAV_Type HAL_QML_Proxy::getUAVType() const
{
    return m_type;
}

