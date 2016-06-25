#include "Config_Widget.h"
#include "Properties_Browser.h"

void Config_Widget::init(ts::Type_System& ts, Properties_Browser& browser)
{
    QASSERT(m_ts == nullptr);
    m_ts = &ts;
    m_browser = &browser;

    refresh();
}

void Config_Widget::refresh()
{
    std::shared_ptr<const ts::IStruct_Type> descriptor_type = m_ts->get_root_scope()->find_specialized_symbol_by_path<ts::IStruct_Type>("Multirotor_Brain_Config");
    if (!descriptor_type)
    {
        return;
    }

    std::shared_ptr<ts::IStruct_Value> descriptor_value = descriptor_type->create_specialized_value();
    QASSERT(descriptor_value);
    auto construction_result = descriptor_value->construct();
    if (construction_result != ts::success)
    {
        return;
    }

    m_browser->set_value(descriptor_value);
}
