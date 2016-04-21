#include "Value_Selector.h"
#include "ts_assert.h"

namespace ts
{

Value_Selector::Element::Element()
    : type(Type::NAME)
    , name()
{
}

Value_Selector::Element::Element(size_t index)
    : type(Type::INDEX)
    , index(index)
{
}

Value_Selector::Element::Element(std::string const& name)
    : type(Type::NAME)
    , name(name)
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

Value_Selector::Value_Selector(char const* str)
{
    parse(std::string(str));
}

void Value_Selector::parse(std::string const& str)
{
    const char* sep = ".[";

    size_t start = 0;
    size_t off = 0;
    do
    {
        off = str.find_first_of(sep, start);
        if (off == std::string::npos)
        {
            off = str.size();
        }
        if (off > start)
        {
            Element element(str.substr(start, off - start));
            push_back(element);
        }

        if (str[off] == '[')
        {
            size_t end = str.find(']', off);
            if (end == std::string::npos)
            {
                TS_ASSERT(false);
                break;
            }

            int index = atoi(str.data() + off + 1);
            Element element(static_cast<size_t>(index));
            push_back(element);

            start = end + 1;
        }
        else
        {
            start = off + 1;
        }
    } while (start < str.size());
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

std::string Value_Selector::to_string() const
{
    std::string str;
    for (Element const& element: m_elements)
    {
        if (element.type == Element::Type::INDEX)
        {
            str += "[" + std::to_string(element.index) + "]";
        }
        else
        {
            if (!str.empty())
            {
                str += ".";
            }
            str += element.name;
        }
    }
    return str;
}

}
