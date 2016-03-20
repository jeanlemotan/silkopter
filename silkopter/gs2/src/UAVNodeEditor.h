#pragma once

#include <QQuickPaintedItem>
#include <QQuickView>
#include <QQmlComponent>
#include "Comms_Slots.h"

namespace detail
{

class NodeDefObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName)
public:
    NodeDefObject(const QString& name,  QObject* parent)
        : QObject(parent)
        , m_name(name)
    {}

    const QString& getName() const { return m_name; }
private:
    QString m_name;
};

}

class UAVNodeEditor : public QQuickPaintedItem, public Comms_Slots
{
    Q_OBJECT

    Q_PROPERTY(QVariant nodeDefModel READ getNodeDefModel NOTIFY nodeDefModelChanged)

public:
    explicit UAVNodeEditor(QQuickItem* parent = 0);


    QVariant getNodeDefModel() const;

signals:
    void nodeDefModelChanged();

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
    void slot_node_message_received(std::string const& name, rapidjson::Value const& message) override;
    void slot_node_config_received(std::string const& name, rapidjson::Value const& json) override;
    void slot_stream_data_received(silk::Comms::IStream_Data const&) override;
    void slot_uav_config_received(boost::optional<silk::UAV_Config&> config) override;

    void paint(QPainter* painter);

    QQuickView* m_view = nullptr;
    std::unique_ptr<QQmlComponent> m_nodeComponent;

    QVariant m_nodeDefModel;
};

