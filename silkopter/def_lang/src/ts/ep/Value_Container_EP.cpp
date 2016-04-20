#include "ep/Value_Container_EP.h"
#include "IValue.h"

namespace ts
{

Value_Container_EP::~Value_Container_EP()
{

}

Result<void> Value_Container_EP::insert_value(size_t idx, std::shared_ptr<IValue> value)
{
    if (!value)
    {
        return Error("Cannot insert null value");
    }

    if (idx > get_value_count())
    {
        return Error("Cannot insert beyond the end");
    }

    m_values.insert(m_values.begin() + idx, std::move(value));

    return success;
}
Result<void> Value_Container_EP::erase_value(size_t idx)
{
    if (idx >= get_value_count())
    {
        return Error("Cannot erase beyond the end");
    }

    m_values.erase(m_values.begin() + idx);

    return success;
}

size_t Value_Container_EP::get_value_count() const
{
    return m_values.size();
}

std::shared_ptr<const IValue> Value_Container_EP::get_value(size_t idx) const
{
    return m_values[idx];
}
std::shared_ptr<IValue> Value_Container_EP::get_value(size_t idx)
{
    return m_values[idx];
}


}
