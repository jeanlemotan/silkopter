#include "HALNodeEditor.h"
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QtQml>

extern silk::Comms s_comms;
extern ts::Type_System s_type_system;

HALNodeEditor::HALNodeEditor(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , Comms_Slots(s_comms, s_type_system)
{
    setFlag(ItemHasContents);

    refresh();
}

void HALNodeEditor::slot_clock_received(silk::Manual_Clock::time_point tp)
{

}

void HALNodeEditor::slot_reset()
{
}

void HALNodeEditor::slot_node_defs_reset()
{
    QQmlEngine* engine = qmlEngine(this);
    m_nodeComponent.reset(new QQmlComponent(engine, QUrl("qrc:/NodeEditor/qml/Node.qml")));

//    QQuickItem* node = qobject_cast<QQuickItem*>(m_nodeComponent->create());
//    node->setParentItem(this);

    m_nodeDefModel.reset(new NodeDefModel());
    emit nodeDefModelChanged();
}

void HALNodeEditor::slot_node_defs_added(std::vector<silk::Comms::Node_Def> const& defs)
{
    m_nodeDefs = defs;

    m_nodeDefModel.reset(new NodeDefModel());
    m_nodeDefModel->init(defs);

    emit nodeDefModelChanged();
}

void HALNodeEditor::slot_nodes_reset()
{

}

void HALNodeEditor::slot_nodes_added(std::vector<silk::Comms::Node> const& nodes)
{

}

void HALNodeEditor::slot_node_changed(silk::Comms::Node const& node)
{

}

void HALNodeEditor::slot_node_removed(std::string const& name)
{

}

//void UAVNodeEditor::slot_node_message_received(std::string const& name, rapidjson::Value const& message)
//{

//}

//void UAVNodeEditor::slot_node_config_received(std::string const& name, rapidjson::Value const& json)
//{

//}

void HALNodeEditor::slot_stream_data_received(silk::Comms::IStream_Data const&)
{

}

void HALNodeEditor::slot_uav_descriptor_received(std::shared_ptr<ts::IStruct_Value> descriptor)
{

}

void HALNodeEditor::paint(QPainter* painter)
{
    //QQuickPaintedItem::paint(painter);

}

QAbstractItemModel* HALNodeEditor::getNodeDefModel() const
{
    return m_nodeDefModel.get();
}
