#pragma once

#include <string>
#include <stdint.h>

namespace ts
{
//////////////////////////////////////////////////////////////////////////

class Source_Location
{
public:
    Source_Location() = default;
    Source_Location(std::string const&, uint32_t line, uint32_t column);

    void set_line(std::uint32_t);
    void set_column(std::uint32_t);
    void set_file_path(std::string const&);

    uint32_t get_line() const;
    uint32_t get_column() const;
    auto get_file_path() const -> std::string const&;

    auto to_string() const -> std::string;

    bool operator==(const Source_Location& other) const;
    bool operator!=(const Source_Location& other) const;

	//////////////////////////////////////////////////////////////////////////

    static Source_Location from_cpp_source(const char* file, uint32_t line);

	//////////////////////////////////////////////////////////////////////////
private:
    std::string m_file_path;
    uint32_t m_line = 0;
    uint32_t m_column = 0;
};

//////////////////////////////////////////////////////////////////////////
} // namespace ast


