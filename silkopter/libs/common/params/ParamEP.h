#pragma once

#include "IParam.h"

namespace silk
{
namespace params
{

class ParamEP : public IParam
{
public:
	ParamEP(q::Path const& path,
			std::string const& ui_prefix,
			std::string const& ui_suffix,
			std::string const& ui_name);

	q::Path const& get_path() const override;

	std::string const& get_ui_prefix() const override;
	std::string const& get_ui_suffix() const override;
	std::string const& get_ui_name() const override;

	void set_enabled(bool enabled) override;
	bool is_enabled() const override;

	bool is_value_compatible(Value const& value) const override;

private:
	q::Path m_path;
	std::string m_ui_prefix;
	std::string m_ui_suffix;
	std::string m_ui_name;
	bool m_is_enabled = true;
}

}
}
