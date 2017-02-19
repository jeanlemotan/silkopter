#include "ParamEP.h"

namespace silk
{
namespace params
{

ParamEP::ParamEP(q::Path const& path,
		std::string const& ui_prefix,
		std::string const& ui_suffix,
		std::string const& ui_name)
	: m_path(path)
	, m_ui_prefix(ui_prefix)
	, m_ui_suffix(ui_suffix)
	, m_ui_name(ui_name)
{
}

q::Path const& ParamEP::get_path() const
{
	return m_path;
}

std::string const& ParamEP::get_ui_prefix() const
{
	return m_ui_prefix;
}
std::string const& ParamEP::get_ui_suffix() const
{
	return m_ui_suffix;
}
std::string const& ParamEP::get_ui_name() const
{
	return m_ui_name;
}

void ParamEP::set_enabled(bool enabled)
{
	m_is_enabled = enabled;
}
bool ParamEP::is_enabled() const
{
	return m_is_enabled;
}

bool ParamEP::is_value_compatible(Value const& value) const
{
	get_value().which() == value.which();
}

}
}
