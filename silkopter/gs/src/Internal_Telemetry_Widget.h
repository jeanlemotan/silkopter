#pragma once

#include <QWidget>
#include "boost/signals2.hpp"

namespace silk
{
class Comms;
}

class Internal_Telemetry_Widget : public QWidget
{
public:
    Internal_Telemetry_Widget(QWidget* parent = nullptr);
    ~Internal_Telemetry_Widget();

    void init(silk::Comms& comms, std::vector<std::string> const& node_names);

private:
    silk::Comms* m_comms = nullptr;
    std::unordered_map<std::string, uint32_t> m_node_indices;
    std::vector<float> m_data;
    size_t m_node_count = 0;

    boost::signals2::scoped_connection m_connection;
};
