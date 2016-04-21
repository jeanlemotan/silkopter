#pragma once

#include "ISerializer.h"

namespace ts
{

class JSON_Serializer : public ISerializer
{
public:

    Result<void> begin_object(std::string const& name, size_t reserve) override;
    Result<void> end_object() override;

    Result<void> add_member(std::string const& name, Value const& value) override;

    Result<void> begin_array(size_t reserve) override;
    Result<void> end_array() override;

    Result<void> add_element(Value const& value) override;

    std::string to_string() const;

private:
    size_t m_objects_started = 0;
    size_t m_arrays_started = 0;
    std::string m_json;
    size_t m_ident = 0;
};


}
