#pragma once

#include <string>
#include <vector>

namespace ts
{

class Symbol_Path
{
public:
    Symbol_Path() = default;
    explicit Symbol_Path(std::string const& str);

    auto is_absolute() const -> bool;

    auto get_count() const -> size_t;
    auto get(size_t idx) const -> std::string const&;

    auto empty() const -> bool;

    auto front() const -> std::string const&;
    auto pop_front() -> std::string;
//    void push_front(std::string const& string);

    auto back() const -> std::string const&;
    auto pop_back() -> std::string;
//    void push_back(std::string const& string);

private:
    void parse(char const* str, size_t size);

    bool m_is_absolute = false;
    std::vector<std::string> m_elements;
};

}
