#pragma once

namespace silk
{
namespace params
{

class IParam
{
	virtual ~IParam() = default;

	virtual q::Path const& get_path() const = 0;

	virtual std::string const& get_ui_prefix() const = 0;
	virtual std::string const& get_ui_suffix() const = 0;
	virtual std::string const& get_ui_name() const = 0;

	virtual void set_enabled(bool enabled) = 0;
	virtual bool is_enabled() const = 0;

	boost::variant<boo, int32_t, float, std::string> Value;
	virtual Value get_value() const = 0;
	virtual void set_value(Value const& value) = 0;
	virtual bool is_value_compatible(Value const& value) const = 0;
	virtual bool is_value_valid(Value const& value) const = 0;
}

}
}
