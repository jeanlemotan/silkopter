#pragma once

#include "ParamEP.h"

namespace silk
{
namespace params
{

class Integral_Param : public ParamEP
{
public:
	typedef int32_t value_t;

	Integral_Param(q::Path const& path,
					std::string const& ui_prefix,
					std::string const& ui_suffix,
					std::string const& ui_name,
					value_t value = 0,
					value_t min = std::numeric_limits<value_t>::lowest(),
					value_t max = std::numeric_limits<value_t>::max());

	Value get_value() const override;
	bool is_value_valid(Value const& value) const override;

	value_t get() const;
	value_t get_min() const;
	value_t get_max() const;

private:
	value_t m_value = 0;
	value_t m_min = std::numeric_limits<value_t>::lowest();
	value_t m_max = std::numeric_limits<value_t>::max();
}

}
}
