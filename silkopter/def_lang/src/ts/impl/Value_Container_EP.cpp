#include "impl/Value_Container_EP.h"
#include "values/IValue.h"

namespace ts
{

Value_Container_EP::~Value_Container_EP()
{

}

auto Value_Container_EP::insert_value(size_t idx, std::unique_ptr<IValue>&& value) -> Result<void>
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
auto Value_Container_EP::erase_value(size_t idx) -> Result<void>
{
    if (idx >= get_value_count())
    {
        return Error("Cannot erase beyond the end");
    }

    m_values.erase(m_values.begin() + idx);

    return success;
}

auto Value_Container_EP::get_value_count() const -> size_t
{
    return m_values.size();
}

auto Value_Container_EP::get_value(size_t idx) const -> IValue const&
{
    return *m_values[idx];
}
auto Value_Container_EP::get_value(size_t idx) -> IValue&
{
    return *m_values[idx];
}


}
