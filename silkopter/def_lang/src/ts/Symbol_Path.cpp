#include "Symbol_Path.h"

#include <cstring>

namespace ts
{

Symbol_Path::Symbol_Path(std::string const& str)
{
    parse(str.c_str(), str.size());
}

auto Symbol_Path::is_absolute() const -> bool
{
    return m_is_absolute;
}

auto Symbol_Path::get_count() const -> size_t
{
    return m_elements.size();
}
auto Symbol_Path::get(size_t idx) const -> std::string const&
{
    return m_elements[idx];
}

auto Symbol_Path::empty() const -> bool
{
    return m_elements.empty();
}

auto Symbol_Path::front() const -> std::string const&
{
    return m_elements.front();
}
auto Symbol_Path::pop_front() -> std::string
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

auto Symbol_Path::back() const -> std::string const&
{
    return m_elements.back();
}
auto Symbol_Path::pop_back() -> std::string
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

}
