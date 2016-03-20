#include "UAV_QMLProxy.h"
#include "utils/RCP_RFMON_Socket.h"

UAV_QMLProxy::UAV_QMLProxy(QObject *parent)
    : QObject(parent)
{

}

void UAV_QMLProxy::init(silk::Comms& comms)
{
    m_comms = &comms;


    m_connections.reset = m_comms->sig_reset.connect(std::bind(&UAV_QMLProxy::slot_reset, this));
    m_connections.clock_received = m_comms->sig_clock_received.connect(std::bind(&UAV_QMLProxy::slot_clock_received, this, std::placeholders::_1));
    m_connections.node_defs_reset = m_comms->sig_node_defs_reset.connect(std::bind(&UAV_QMLProxy::slot_node_defs_reset, this));
    m_connections.node_defs_added = m_comms->sig_node_defs_added.connect(std::bind(&UAV_QMLProxy::slot_node_defs_added, this, std::placeholders::_1));
    m_connections.nodes_reset = m_comms->sig_nodes_reset.connect(std::bind(&UAV_QMLProxy::slot_nodes_reset, this));
    m_connections.nodes_added = m_comms->sig_nodes_added.connect(std::bind(&UAV_QMLProxy::slot_nodes_added, this, std::placeholders::_1));
    m_connections.node_changed = m_comms->sig_node_changed.connect(std::bind(&UAV_QMLProxy::slot_node_changed, this, std::placeholders::_1));
    m_connections.node_removed = m_comms->sig_node_removed.connect(std::bind(&UAV_QMLProxy::slot_node_removed, this, std::placeholders::_1));
    m_connections.node_message_received = m_comms->sig_node_message_received.connect(std::bind(&UAV_QMLProxy::slot_node_message_received, this, std::placeholders::_1, std::placeholders::_2));
    m_connections.node_config_received = m_comms->sig_node_config_received.connect(std::bind(&UAV_QMLProxy::slot_node_config_received, this, std::placeholders::_1, std::placeholders::_2));
    m_connections.stream_data_received = m_comms->sig_stream_data_received.connect(std::bind(&UAV_QMLProxy::slot_stream_data_received, this, std::placeholders::_1));
    m_connections.uav_config_received = m_comms->sig_uav_config_received.connect(std::bind(&UAV_QMLProxy::slot_uav_config_received, this, std::placeholders::_1));
}

void UAV_QMLProxy::slot_clock_received(silk::Manual_Clock::time_point tp)
{

}


void UAV_QMLProxy::slot_reset()
{

}

void UAV_QMLProxy::slot_node_defs_reset()
{

}

void UAV_QMLProxy::slot_node_defs_added(std::vector<silk::Comms::Node_Def> const& defs)
{

}

void UAV_QMLProxy::slot_nodes_reset()
{

}

void UAV_QMLProxy::slot_nodes_added(std::vector<silk::Comms::Node> const& nodes)
{

}

void UAV_QMLProxy::slot_node_changed(silk::Comms::Node const& node)
{

}

void UAV_QMLProxy::slot_node_removed(std::string const& name)
{

}

void UAV_QMLProxy::slot_node_message_received(std::string const& name, rapidjson::Value const& message)
{

}

void UAV_QMLProxy::slot_node_config_received(std::string const& name, rapidjson::Value const& json)
{

}

void UAV_QMLProxy::slot_stream_data_received(silk::Comms::IStream_Data const& stream_data)
{

}

void UAV_QMLProxy::slot_uav_config_received(boost::optional<silk::UAV_Config&> _config)
{
//    if (!_config)
//    {
//        m_configType = ConfigType::NONE;
//        emit configTypeChanged(m_configType);
//        return;
//    }

//    silk::UAV_Config* config = _config.get_ptr();
//    if (silk::Multirotor_Config* multirotor_config = dynamic_cast<silk::Multirotor_Config>(config))
//    {
//        m_configType = ConfigType::MULTIROTOR;
//        emit configTypeChanged(m_configType);
//    }
//    else
//    {
//        m_configType = ConfigType::NONE;
//        emit configTypeChanged(m_configType);
//    }
}

void UAV_QMLProxy::start()
{
    m_comms->request_all_data();
}


