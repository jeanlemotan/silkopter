#pragma once

#include <QObject>
#include "Comms.h"
#include "Comms_Slots.h"

class HAL_QML_Proxy : public QObject, public Comms_Slots
{
    Q_OBJECT
public:
    enum class UAV_Type
    {
        NONE = 0,
        MULTIROTOR_TRI,
        MULTIROTOR_QUAD,
        MULTIROTOR_HEXA,
        MULTIROTOR_HEXA_TRI,
        MULTIROTOR_OCTA,
        MULTIROTOR_OCTA_QUAD,
        MULTIROTOR_CUSTOM,
        ROVER,
        BOAT,
        PLANE,
        COPTER
    };
    Q_ENUMS(UAV_Type);

    Q_PROPERTY(UAV_Type uavType READ getUAVType NOTIFY uavTypeChanged)

    explicit HAL_QML_Proxy(QObject *parent = 0);

    UAV_Type getUAVType() const;

signals:
    void uavTypeChanged();

public slots:

private:
    void slot_clock_received(silk::Manual_Clock::time_point tp) override;
    void slot_reset() override;
    void slot_node_defs_reset() override;
    void slot_node_defs_added(std::vector<silk::Comms::Node_Def> const&) override;
    void slot_nodes_reset() override;
    void slot_nodes_added(std::vector<silk::Comms::Node> const&) override;
    void slot_node_changed(silk::Comms::Node const&) override;
    void slot_node_removed(std::string const& name) override;
    void slot_stream_data_received(silk::Comms::IStream_Data const&) override;
    void slot_uav_descriptor_received(std::shared_ptr<ts::IStruct_Value> descriptor) override;

    UAV_Type m_type = UAV_Type::NONE;
    std::shared_ptr<ts::IStruct_Value> m_uav_descriptor;
};

