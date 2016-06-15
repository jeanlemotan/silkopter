#pragma once

#include <QObject>
#include "Comms.h"
#include "Comms_Slots.h"

class UAV_QML_Proxy : public QObject, public Comms_Slots
{
    Q_OBJECT
public:
    enum class Type
    {
        NONE = 0,
        MULTIROTOR,
        ROVER,
        BOAT,
        PLANE,
        COPTER
    };
    Q_ENUMS(Type);

    Q_PROPERTY(Type type READ getType NOTIFY typeChanged)
    Q_PROPERTY(QString configJSon READ getConfigJSon WRITE setConfigJSon NOTIFY configJSonChanged)

    explicit UAV_QML_Proxy(QObject *parent = 0);

    QString getConfigJSon() const;
    void setConfigJSon(const QString& json);

    Type getType() const;

    Q_INVOKABLE void createDefaultConfig(Type type) const;

signals:
    void typeChanged();
    void configJSonChanged();

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
//    void slot_node_message_received(std::string const& name, rapidjson::Value const& message) override;
//    void slot_node_config_received(std::string const& name, rapidjson::Value const& json) override;
    void slot_stream_data_received(silk::Comms::IStream_Data const&) override;
//    void slot_uav_config_received(boost::optional<silk::UAV_Config&> config) override;

    Type m_type = Type::NONE;
    QString m_configJSon;
};

