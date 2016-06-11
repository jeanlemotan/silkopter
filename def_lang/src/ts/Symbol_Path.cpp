#include "def_lang/Symbol_Path.h"
#include "def_lang/ts_assert.h"

#include <cstring>

namespace ts
{

Symbol_Path::Symbol_Path(std::string const& str)
{
    parse(str.c_str(), str.size());
}

Symbol_Path::Symbol_Path(char const* str)
{
    if (str)
    {
        parse(str, strlen(str));
    }
}

Symbol_Path Symbol_Path::get_path_to(Symbol_Path const& path) const
{
    if (get_count() > path.get_count())
    {
        return path;
    }

    Symbol_Path res;

    for (size_t i = 0; i < get_count(); i++)
    {
        if (get(i) != path.get(i))
        {
            return path;
        }
    }

    for (size_t i = get_count(); i < path.get_count(); i++)
    {
        res.m_elements.push_back(path.m_elements[i]);
    }

    return res;
}

Symbol_Path Symbol_Path::parent() const
{
    Symbol_Path res = *this;
    res.pop_back();
    return res;
}

Symbol_Path Symbol_Path::to_relative() const
{
    Symbol_Path res = *this;
    if (res.is_absolute())
    {
        res.m_is_absolute = false;
    }
    return res;
}

bool Symbol_Path::operator==(Symbol_Path const& other) const
{
    return m_is_absolute == other.m_is_absolute &&
            m_elements == other.m_elements;
}
bool Symbol_Path::operator!=(Symbol_Path const& other) const
{
    return !operator==(other);
}

Symbol_Path Symbol_Path::operator+(Symbol_Path const& other) const
{
    Symbol_Path res = *this;
    for (std::string const& e: other.m_elements)
    {
        res.m_elements.push_back(e);
    }
    return res;
}

bool Symbol_Path::is_absolute() const
{
    return m_is_absolute;
}

size_t Symbol_Path::get_count() const
{
    return m_elements.size();
}
std::string const& Symbol_Path::get(size_t idx) const
{
    return m_elements[idx];
}

bool Symbol_Path::empty() const
{
    return m_elements.empty();
}

std::string const& Symbol_Path::front() const
{
    return m_elements.front();
}
std::string Symbol_Path::pop_front()
{
    std::string element = std::move(m_elements.front());
    m_elements.erase(m_elements.begin());
    m_is_absolute = false;
    return element;
}
//void Symbol_Path::push_front(std::string const& string)
//{
//    m_elements.insert(m_elements.begin(), string);
//}

std::string const& Symbol_Path::back() const
{
    return m_elements.back();
}
std::string Symbol_Path::pop_back()
{
    std::string element = std::move(m_elements.back());
    m_elements.pop_back();
    return element;
}
//void Symbol_Path::push_back(std::string const& string)
//{
//    m_elements.push_back(string);
//}

void Symbol_Path::parse(char const* str, size_t size)
{
    m_is_absolute = false;
    m_elements.clear();

    if (size == 0 || !str || str[0] == 0)
    {
        return;
    }

    const char* start = str;
    do
    {
        size_t count = 0;
        const char* i = strstr(start, "::");
        if (!i)
        {
            count = size - (start - str);
        }
        else if (i == str) //starting with ::
        {
            count = 0;
            m_is_absolute = true;
        }
        else
        {
            count = i - start;
        }

        if (count > 0)
        {
            m_elements.push_back(std::string(start, start + count));
        }

        if (!i)
        {
            break;
        }
        start = i + 2;
    } while (true);
}

std::string Symbol_Path::to_string() const
{
    std::string str;
    if (is_absolute())
    {
        str += "::";
    }

    for (std::string const& element: m_elements)
    {
        str += element;
        if (&element != &m_elements.back())
        {
            str += "::";
        }
    }
    return str;
}

}
