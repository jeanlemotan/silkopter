#include "HAL_QML_Proxy.h"
#include "def_lang/IStruct_Type.h"

extern silk::Comms s_comms;
extern ts::Type_System s_type_system;

HAL_QML_Proxy::HAL_QML_Proxy(QObject *parent)
    : QObject(parent)
    , Comms_Slots(s_comms, s_type_system)
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

void HAL_QML_Proxy::slot_node_defs_reset()
{
}

void HAL_QML_Proxy::slot_node_defs_added(std::vector<silk::Comms::Node_Def> const& defs)
{
}

void HAL_QML_Proxy::slot_nodes_reset()
{

}

void HAL_QML_Proxy::slot_nodes_added(std::vector<silk::Comms::Node> const& nodes)
{

}

void HAL_QML_Proxy::slot_node_changed(silk::Comms::Node const& node)
{

}

void HAL_QML_Proxy::slot_node_removed(std::string const& name)
{

}

void HAL_QML_Proxy::slot_stream_data_received(silk::Comms::IStream_Data const&)
{

}

void HAL_QML_Proxy::slot_uav_descriptor_received(std::shared_ptr<ts::IStruct_Value> descriptor)
{
    m_uav_descriptor = descriptor;

    if (!descriptor)
    {
        m_type = UAV_Type::NONE;

        emit uavTypeChanged();
    }
    else
    {
        m_type = UAV_Type::NONE;

        std::shared_ptr<const ts::IStruct_Type> descriptor_type = m_ts.get_root_scope()->find_specialized_symbol_by_path<ts::IStruct_Type>("Tri_Descriptor");
        if (descriptor_type && descriptor_type->is_base_of(*descriptor->get_specialized_type()))
        {
            m_type = UAV_Type::MULTIROTOR_TRI;
        }
        if (m_type == UAV_Type::NONE)
        {
            descriptor_type = m_ts.get_root_scope()->find_specialized_symbol_by_path<ts::IStruct_Type>("Quad_Descriptor");
            if (descriptor_type && descriptor_type->is_base_of(*descriptor->get_specialized_type()))
            {
                m_type = UAV_Type::MULTIROTOR_QUAD;
            }
        }
        if (m_type == UAV_Type::NONE)
        {
            descriptor_type = m_ts.get_root_scope()->find_specialized_symbol_by_path<ts::IStruct_Type>("Hexa_Descriptor");
            if (descriptor_type && descriptor_type->is_base_of(*descriptor->get_specialized_type()))
            {
                m_type = UAV_Type::MULTIROTOR_HEXA;
            }
        }
        if (m_type == UAV_Type::NONE)
        {
            descriptor_type = m_ts.get_root_scope()->find_specialized_symbol_by_path<ts::IStruct_Type>("Hexatri_Descriptor");
            if (descriptor_type && descriptor_type->is_base_of(*descriptor->get_specialized_type()))
            {
                m_type = UAV_Type::MULTIROTOR_HEXA_TRI;
            }
        }
        if (m_type == UAV_Type::NONE)
        {
            descriptor_type = m_ts.get_root_scope()->find_specialized_symbol_by_path<ts::IStruct_Type>("Octa_Descriptor");
            if (descriptor_type && descriptor_type->is_base_of(*descriptor->get_specialized_type()))
            {
                m_type = UAV_Type::MULTIROTOR_OCTA;
            }
        }
        if (m_type == UAV_Type::NONE)
        {
            descriptor_type = m_ts.get_root_scope()->find_specialized_symbol_by_path<ts::IStruct_Type>("Octaquad_Descriptor");
            if (descriptor_type && descriptor_type->is_base_of(*descriptor->get_specialized_type()))
            {
                m_type = UAV_Type::MULTIROTOR_OCTA_QUAD;
            }
        }
        if (m_type == UAV_Type::NONE)
        {
            descriptor_type = m_ts.get_root_scope()->find_specialized_symbol_by_path<ts::IStruct_Type>("Custom_Multirotor_Descriptor");
            if (descriptor_type && descriptor_type->is_base_of(*descriptor->get_specialized_type()))
            {
                m_type = UAV_Type::MULTIROTOR_CUSTOM;
            }
        }

        if (m_type == UAV_Type::NONE)
        {
            descriptor_type = m_ts.get_root_scope()->find_specialized_symbol_by_path<ts::IStruct_Type>("Multirotor_Descriptor");
            if (descriptor_type && descriptor_type->is_base_of(*descriptor->get_specialized_type()))
            {
                QLOGE("Unrecognized multirotor descriptor");
            }
        }

        emit uavTypeChanged();
    }
}

HAL_QML_Proxy::UAV_Type HAL_QML_Proxy::getUAVType() const
{
    return m_type;
}

