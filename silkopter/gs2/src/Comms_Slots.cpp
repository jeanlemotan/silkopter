#include "Comms_Slots.h"

Comms_Slots::Comms_Slots(silk::Comms& comms)
    : m_comms(comms)
{
    m_connections.reset = m_comms.sig_reset.connect(std::bind(&Comms_Slots::slot_reset, this));
    m_connections.clock_received = m_comms.sig_clock_received.connect(std::bind(&Comms_Slots::slot_clock_received, this, std::placeholders::_1));
    m_connections.node_defs_reset = m_comms.sig_node_defs_reset.connect(std::bind(&Comms_Slots::slot_node_defs_reset, this));
    m_connections.node_defs_added = m_comms.sig_node_defs_added.connect(std::bind(&Comms_Slots::slot_node_defs_added, this, std::placeholders::_1));
    m_connections.nodes_reset = m_comms.sig_nodes_reset.connect(std::bind(&Comms_Slots::slot_nodes_reset, this));
    m_connections.nodes_added = m_comms.sig_nodes_added.connect(std::bind(&Comms_Slots::slot_nodes_added, this, std::placeholders::_1));
    m_connections.node_changed = m_comms.sig_node_changed.connect(std::bind(&Comms_Slots::slot_node_changed, this, std::placeholders::_1));
    m_connections.node_removed = m_comms.sig_node_removed.connect(std::bind(&Comms_Slots::slot_node_removed, this, std::placeholders::_1));
    //m_connections.node_message_received = m_comms.sig_node_message_received.connect(std::bind(&Comms_Slots::slot_node_message_received, this, std::placeholders::_1, std::placeholders::_2));
    //m_connections.node_config_received = m_comms.sig_node_config_received.connect(std::bind(&Comms_Slots::slot_node_config_received, this, std::placeholders::_1, std::placeholders::_2));
    m_connections.stream_data_received = m_comms.sig_stream_data_received.connect(std::bind(&Comms_Slots::slot_stream_data_received, this, std::placeholders::_1));
    //m_connections.uav_config_received = m_comms.sig_uav_config_received.connect(std::bind(&Comms_Slots::slot_uav_config_received, this, std::placeholders::_1));
}

void Comms_Slots::refresh()
{
    m_comms.request_all_data();
}


