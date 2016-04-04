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

    auto get_count() const -> size_t;
    auto get(size_t idx) const -> std::string const&;

    auto empty() const -> bool;

    auto front() const -> std::string const&;
    auto pop_front() const -> std::string const&;
    void push_front(std::string const& string);

    auto back() const -> std::string const&;
    auto pop_back() const -> std::string const&;
    void push_back(std::string const& string);

private:
    std::vector<std::string> m_elements;

};

}
