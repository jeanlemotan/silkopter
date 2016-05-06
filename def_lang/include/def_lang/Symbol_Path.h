#pragma once

#include <string>
#include <vector>

namespace ts
{

class Symbol_Path
{
public:
    Symbol_Path() = default;
    Symbol_Path(std::string const& str);
    Symbol_Path(char const* str);

    Symbol_Path get_path_to(Symbol_Path const& path) const;

    bool operator==(Symbol_Path const& other) const;
    bool operator!=(Symbol_Path const& other) const;

    Symbol_Path operator+(Symbol_Path const& other) const;

    Symbol_Path parent() const;

    bool is_absolute() const;

    size_t get_count() const;
    std::string const& get(size_t idx) const;

    bool empty() const;

    std::string const& front() const;
    std::string pop_front();
//    void push_front(std::string const& string);

    std::string const& back() const;
    std::string pop_back();
//    void push_back(std::string const& string);

    std::string to_string() const;

private:
    void parse(char const* str, size_t size);

    bool m_is_absolute = false;
    std::vector<std::string> m_elements;
};

}
