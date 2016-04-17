#include "Source_Location.h"

namespace ts
{
//////////////////////////////////////////////////////////////////////////

Source_Location::Source_Location(std::string const& file_path, uint32_t line, uint32_t column)
    : m_file_path(file_path)
    , m_line(line)
    , m_column(column)
{
}

//////////////////////////////////////////////////////////////////////////

void Source_Location::set_line(uint32_t line)
{
	m_line = line;
}

uint32_t Source_Location::get_line() const
{
	return m_line;
}

uint32_t Source_Location::get_column() const
{
	return(m_column);
}

//////////////////////////////////////////////////////////////////////////

void Source_Location::set_column(uint32_t column)
{
	m_column = column;
}

//////////////////////////////////////////////////////////////////////////

void Source_Location::set_file_path(std::string const& file)
{
    m_file_path = file;
}

//////////////////////////////////////////////////////////////////////////

std::string const& Source_Location::get_file_path() const
{
    return m_file_path;
}

//////////////////////////////////////////////////////////////////////////

Source_Location Source_Location::from_cpp_source(const char *file, uint32_t line)
{
    return Source_Location(std::string(file), line, 0);
}

//////////////////////////////////////////////////////////////////////////

bool Source_Location::operator==(const Source_Location& other) const
{
    return m_file_path == other.m_file_path && m_line == other.m_line && m_column == other.m_column;
}

//////////////////////////////////////////////////////////////////////////

bool Source_Location::operator!=(const Source_Location& other) const
{
    return m_file_path != other.m_file_path || m_line != other.m_line || m_column != other.m_column;
}

//////////////////////////////////////////////////////////////////////////


std::string Source_Location::to_string() const
{
    return get_file_path() + ":" + std::to_string(get_line()) + ":" + std::to_string(get_column()) + ": ";
}

} // namespace ast
