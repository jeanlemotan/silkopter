#include "UAVNodeEditor.h"
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QtQml>

extern silk::Comms s_comms;


UAVNodeEditor::UAVNodeEditor(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , Comms_Slots(s_comms)
{
    setFlag(ItemHasContents);

    refresh();
}

void UAVNodeEditor::slot_clock_received(silk::Manual_Clock::time_point tp)
{

}

void UAVNodeEditor::slot_reset()
{
}

void UAVNodeEditor::slot_node_defs_reset()
{
    QQmlEngine* engine = qmlEngine(this);
    m_nodeComponent.reset(new QQmlComponent(engine, QUrl("qrc:/qml/nodes/Node.qml")));

    QQuickItem* node = qobject_cast<QQuickItem*>(m_nodeComponent->create());
    node->setParentItem(this);

    m_nodeDefModel = QVariant();
    emit nodeDefModelChanged();
}

void UAVNodeEditor::slot_node_defs_added(std::vector<silk::Comms::Node_Def> const& defs)
{
    QList<QObject*> list;
    for (silk::Comms::Node_Def const& def: defs)
    {
        list.append(new detail::NodeDefObject(def.name.c_str(), this));
    }

    m_nodeDefModel = QVariant::fromValue(list);
    emit nodeDefModelChanged();
}

void UAVNodeEditor::slot_nodes_reset()
{

}

void UAVNodeEditor::slot_nodes_added(std::vector<silk::Comms::Node> const& nodes)
{

}

void UAVNodeEditor::slot_node_changed(silk::Comms::Node const& node)
{

}

void UAVNodeEditor::slot_node_removed(std::string const& name)
{

}

void UAVNodeEditor::slot_node_message_received(std::string const& name, rapidjson::Value const& message)
{

}

void UAVNodeEditor::slot_node_config_received(std::string const& name, rapidjson::Value const& json)
{

}

void UAVNodeEditor::slot_stream_data_received(silk::Comms::IStream_Data const&)
{

}

void UAVNodeEditor::slot_uav_config_received(boost::optional<silk::UAV_Config&> config)
{

}

void UAVNodeEditor::paint(QPainter* painter)
{
    //QQuickPaintedItem::paint(painter);

}

QVariant UAVNodeEditor::getNodeDefModel() const
{
    return m_nodeDefModel;
}
