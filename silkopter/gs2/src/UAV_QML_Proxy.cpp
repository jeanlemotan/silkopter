#include "UAV_QML_Proxy.h"

extern silk::Comms s_comms;

UAV_QML_Proxy::UAV_QML_Proxy(QObject *parent)
    : QObject(parent)
    , Comms_Slots(s_comms)
{
}

void UAV_QML_Proxy::slot_clock_received(silk::Manual_Clock::time_point tp)
{
}

void UAV_QML_Proxy::slot_reset()
{
    m_type = Type::NONE;
    emit typeChanged(m_type);
}

void UAV_QML_Proxy::slot_node_defs_reset()
{
}

void UAV_QML_Proxy::slot_node_defs_added(std::vector<silk::Comms::Node_Def> const& defs)
{
}

void UAV_QML_Proxy::slot_nodes_reset()
{

}

void UAV_QML_Proxy::slot_nodes_added(std::vector<silk::Comms::Node> const& nodes)
{

}

void UAV_QML_Proxy::slot_node_changed(silk::Comms::Node const& node)
{

}

void UAV_QML_Proxy::slot_node_removed(std::string const& name)
{

}

void UAV_QML_Proxy::slot_node_message_received(std::string const& name, rapidjson::Value const& message)
{

}

void UAV_QML_Proxy::slot_node_config_received(std::string const& name, rapidjson::Value const& json)
{

}

void UAV_QML_Proxy::slot_stream_data_received(silk::Comms::IStream_Data const&)
{

}

void UAV_QML_Proxy::slot_uav_config_received(boost::optional<silk::UAV_Config&> config)
{
    if (!config)
    {
        m_type = Type::NONE;
        emit typeChanged(m_type);
    }
    else if (silk::Multirotor_Config* multirotor_config = dynamic_cast<silk::Multirotor_Config*>(config.get_ptr()))
    {
        m_type = Type::MULTIROTOR;
        emit typeChanged(m_type);
    }
    else
    {
        m_type = Type::NONE;
        emit typeChanged(m_type);
    }
}

UAV_QML_Proxy::Type UAV_QML_Proxy::getType() const
{
    return m_type;
}

