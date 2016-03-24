#include "autojsoncxx/autojsoncxx.hpp"
#include "common/config/Multirotor_Config.h"
#include "sz_math.hpp"
#include "sz_Multirotor_Config.hpp"

#include "UAV_QML_Proxy.h"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

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

template<class T>
QString config_to_string(T& config)
{
    rapidjson::Document json;
    autojsoncxx::error::ErrorStack result;

    if (!autojsoncxx::from_value(config, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Req Id: {} - Cannot convert config to json: {}", ss.str());
        return QString();
    }

    rapidjson::StringBuffer str_buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(str_buf);
    json.Accept(writer);
    QString str(str_buf.GetString());
    return str;
}

QString UAV_QML_Proxy::createNewUAVConfigJSon(Type type) const
{
    if (type == Type::NONE)
    {
        return QString();
    }
    else if (type == Type::MULTIROTOR)
    {
        int config;
        return config_to_string(config);
    }
    else
    {
        return QString();
    }
}
void UAV_QML_Proxy::setUAVConfigJSon(Type type, const QString& json) const
{

}
