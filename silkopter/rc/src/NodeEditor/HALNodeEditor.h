#pragma once

#include <QQuickPaintedItem>
#include <QQuickView>
#include <QQmlComponent>
#include "Comms_Slots.h"
#include "NodeDefModel.h"

namespace detail
{

class NodeDefObject : public QObject
{
    Q_OBJECT

    typedef silk::node::Type Type;
    Q_ENUMS(Type);

    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(Type type READ getType)
    Q_PROPERTY(QString description READ getDescription)
public:
    NodeDefObject(const silk::Comms::Node_Def& def, QObject* parent)
        : QObject(parent)
        , m_def(def)
    {}

    QString getName() const { return QString(m_def.name.c_str()); }
    Type getType() const { return m_def.type; }
    QString getDescription() const { return QString(silk::node::get_as_string(m_def.type).c_str()); }

private:
    silk::Comms::Node_Def m_def;
};

}

class HALNodeEditor : public QQuickPaintedItem, public Comms_Slots
{
    Q_OBJECT

    Q_PROPERTY(QAbstractItemModel* nodeDefModel READ getNodeDefModel NOTIFY nodeDefModelChanged)

public:
    explicit HALNodeEditor(QQuickItem* parent = 0);


    QAbstractItemModel* getNodeDefModel() const;

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
    //void slot_node_message_received(std::string const& name, rapidjson::Value const& message) override;
    //void slot_node_config_received(std::string const& name, rapidjson::Value const& json) override;
    void slot_stream_data_received(silk::Comms::IStream_Data const&) override;
    void slot_uav_descriptor_received(std::shared_ptr<ts::IStruct_Value> descriptor) override;

    void paint(QPainter* painter);

    QQuickView* m_view = nullptr;
    std::unique_ptr<QQmlComponent> m_nodeComponent;

    std::vector<silk::Comms::Node_Def> m_nodeDefs;
    std::unique_ptr<NodeDefModel> m_nodeDefModel;
};

