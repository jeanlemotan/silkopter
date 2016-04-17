#pragma once

#include <vector>

namespace ts
{

class Value_Selector final
{
public:

    Value_Selector() = default;
    explicit Value_Selector(std::string const& str);

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

    size_t get_element_count() const;
    Element const& get_element() const;

    bool empty() const;

    Element const& front() const;
    Element pop_front() const;
    void push_front(Element const& element);

    Element const& back() const;
    Element pop_back() const;
    void push_back(Element const& element);

private:
    std::vector<std::string> m_elements;
};


}
