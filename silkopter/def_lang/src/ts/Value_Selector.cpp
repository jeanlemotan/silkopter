#include "Value_Selector.h"

namespace ts
{

Value_Selector::Element::Element()
    : type(Type::NAME)
    , name()
{
}

Value_Selector::Element::~Element()
{
    if (type == Type::NAME)
    {
        typedef std::string string_type;
        name.~string_type();
    }
}

Value_Selector::Element::Element(Element const& other)
    : type(other.type)
    , name()
{
    if (type == Type::NAME)
    {
        name = other.name;
    }
    else
    {
        index = other.index;
    }
}

Value_Selector::Element::Element(Element&& other)
    : type(other.type)
    , name()
{
    if (type == Type::NAME)
    {
        name = std::move(other.name);
    }
    else
    {
        index = other.index;
    }
}

Value_Selector::Element& Value_Selector::Element::operator=(Element const& other)
{
    if (type == Type::NAME)
    {
        typedef std::string string_type;
        name.~string_type();
    }
    type = other.type;
    if (type == Type::NAME)
    {
        name = other.name;
    }
    else
    {
        index = other.index;
    }
    return *this;
}

Value_Selector::Element& Value_Selector::Element::operator=(Element&& other)
{
    if (type == Type::NAME)
    {
        typedef std::string string_type;
        name.~string_type();
    }
    type = other.type;
    if (type == Type::NAME)
    {
        name = std::move(other.name);
    }
    else
    {
        index = other.index;
    }
    return *this;
}




Value_Selector::Value_Selector(std::string const& str)
{
    parse(str);
}

void Value_Selector::parse(std::string const& str)
{

}

size_t Value_Selector::get_element_count() const
{
    return m_elements.size();
}
Value_Selector::Element const& Value_Selector::get_element(size_t idx) const
{
    return m_elements[idx];
}

bool Value_Selector::empty() const
{
    return m_elements.empty();
}

Value_Selector::Element const& Value_Selector::front() const
{
    return m_elements.front();
}
Value_Selector::Element Value_Selector::pop_front()
{
    Element element = m_elements.front();
    m_elements.erase(m_elements.begin());
    return element;
}
void Value_Selector::push_front(Element const& element)
{
    m_elements.insert(m_elements.begin(), element);
}

Value_Selector::Element const& Value_Selector::back() const
{
    return m_elements.back();
}
Value_Selector::Element Value_Selector::pop_back()
{
    Element element = m_elements.back();
    m_elements.pop_back();
    return element;
}
void Value_Selector::push_back(Element const& element)
{
    m_elements.push_back(element);
}


}
