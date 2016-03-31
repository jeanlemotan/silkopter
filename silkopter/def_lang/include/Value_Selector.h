#pragma once

#include <vector>

namespace ts
{

class Value_Selector
{
public:

    Value_Selector() = default;
    Value_Selector(std::string const& str);

    struct Element
    {
        enum class Type
        {
            NAME,
            INDEX
        };

        Type type = Type::NAME;

        union Element
        {
            Element() : name() {}

            size_t index;
            std::string name;
        };
    };

    auto get_element_count() const -> size_t;
    auto get_element() const -> Element const&;

    auto empty() const -> bool;

    auto front() const -> Element const&;
    auto pop_front() const -> Element const&;
    void push_front(Element const& element);

    auto back() const -> Element const&;
    auto pop_back() const -> Element const&;
    void push_back(Element const& element);

private:
    std::vector<std::string> m_elements;
};


}
