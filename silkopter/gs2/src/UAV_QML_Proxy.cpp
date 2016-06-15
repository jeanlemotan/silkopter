#include "UAV_QML_Proxy.h"

extern silk::Comms s_comms;


template<class T>
QString config_to_string(T& config)
{
//    rapidjson::Document json;
//    json.SetObject();
//    autojsoncxx::to_document(config, json);

//    rapidjson::StringBuffer str_buf;
//    rapidjson::Writer<rapidjson::StringBuffer> writer(str_buf);
//    json.Accept(writer);
//    QString str(str_buf.GetString());
//    return str;
}




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
    emit typeChanged();
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

//void UAV_QML_Proxy::slot_node_message_received(std::string const& name, rapidjson::Value const& message)
//{

//}

//void UAV_QML_Proxy::slot_node_config_received(std::string const& name, rapidjson::Value const& json)
//{

//}

void UAV_QML_Proxy::slot_stream_data_received(silk::Comms::IStream_Data const&)
{

}

//void UAV_QML_Proxy::slot_uav_config_received(boost::optional<silk::UAV_Config&> config)
//{
//    if (!config)
//    {
//        m_type = Type::NONE;
//        emit typeChanged();
//        m_configJSon = QString();
//        emit configJSonChanged();
//    }
//    else if (silk::Multirotor_Config* multirotor_config = dynamic_cast<silk::Multirotor_Config*>(config.get_ptr()))
//    {
//        m_type = Type::MULTIROTOR;
//        emit typeChanged();
//        m_configJSon = config_to_string(*multirotor_config);
//        emit configJSonChanged();
//    }
//    else
//    {
//        m_type = Type::NONE;
//        emit typeChanged();
//        m_configJSon = QString();
//        emit configJSonChanged();
//    }
//}

UAV_QML_Proxy::Type UAV_QML_Proxy::getType() const
{
    return m_type;
}
void UAV_QML_Proxy::createDefaultConfig(Type type) const
{
//    if (type == Type::NONE)
//    {
//        s_comms.send_uav_config(boost::none);
//    }
//    else if (type == Type::MULTIROTOR)
//    {
//        silk::Multirotor_Config config;
//        config.motors.resize(4);
//        config.motors[0].position.set( 0.26f,  0.26f, 0.f);
//        config.motors[0].clockwise = false;
//        config.motors[1].position.set(-0.26f, -0.26f, 0.f);
//        config.motors[1].clockwise = false;
//        config.motors[2].position.set(-0.26f,  0.26f, 0.f);
//        config.motors[2].clockwise = true;
//        config.motors[3].position.set( 0.26f, -0.26f, 0.f);
//        config.motors[3].clockwise = true;
//        s_comms.send_uav_config(config);
//    }
//    else
//    {
//        s_comms.send_uav_config(boost::none);
//    }
}
QString UAV_QML_Proxy::getConfigJSon() const
{
    return m_configJSon;
}

void UAV_QML_Proxy::setConfigJSon(const QString& json)
{
    if (m_type == Type::NONE)
    {
        m_configJSon = QString();
        emit configJSonChanged();
    }
    else if (m_type == Type::MULTIROTOR)
    {
//        rapidjson::Document configj;
//        configj.SetObject();
//        if (json.isEmpty() || configj.Parse(json.toLatin1().data()).HasParseError())
//        {
//            QLOGE("Failed to parse json: {}:{}", configj.GetParseError(), configj.GetErrorOffset());
//            m_configJSon = QString();
//            emit configJSonChanged();
//            return;
//        }

//        silk::Multirotor_Config config;
//        autojsoncxx::error::ErrorStack result;
//        if (!autojsoncxx::from_value(config, configj, result))
//        {
//            QLOGE("Cannot convert string to multi config: {}", json.toLatin1().data());
//            m_configJSon = QString();
//            emit configJSonChanged();
//            return;
//        }

//        s_comms.send_uav_config(config);
    }
    else
    {
        m_configJSon = QString();
        emit configJSonChanged();
    }
}


