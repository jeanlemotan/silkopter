#include "def_lang/ast/Node.h"
#include "def_lang/ts_assert.h"

namespace ts
{
namespace ast
{

Node::Node(Type type, ts::Source_Location const& loc)
    : m_type(type)
    , m_location(loc)
{
    m_children.reserve(8);
}
Node::Type Node::get_type() const
{
    return m_type;
}

ts::Source_Location const& Node::get_source_location() const
{
    return m_location;
}

Node& Node::copy_children_from(Node const& node)
{
    m_children.reserve(m_children.size() + node.m_children.size());
    std::copy(node.m_children.begin(), node.m_children.end(), std::back_inserter(m_children));
    return *this;
}

Node& Node::move_children_from(Node&& node)
{
    m_children.reserve(m_children.size() + node.m_children.size());
    std::move(node.m_children.begin(), node.m_children.end(), std::back_inserter(m_children));
    return *this;
}

Node& Node::add_child(Node const& node)
{
    m_children.push_back(node);
    return *this;
}

Node& Node::add_child(Node&& node)
{
    m_children.push_back(std::move(node));
    return *this;
}

std::vector<Node> const& Node::get_children() const
{
    return m_children;
}

Node const* Node::find_first_child_by_type(Type type) const
{
    auto it = std::find_if(m_children.begin(), m_children.end(), [&](Node const& child) { return child.get_type() == type; });
    return it != m_children.end() ? &(*it) : nullptr;
}

std::vector<Node> Node::get_all_children_of_type(Type type) const
{
    std::vector<Node> res;
    res.reserve(m_children.size() / 2);
    for (Node const& ch: m_children)
    {
        if (ch.get_type() == type)
        {
            res.push_back(ch);
        }
    }
    return res;
}

Node& Node::add_attribute(Attribute const& att)
{
    m_attributes.push_back(att);
    return *this;
}

Node& Node::add_attribute(Attribute&& att)
{
    m_attributes.push_back(std::move(att));
    return *this;
}

std::vector<Attribute> const& Node::get_attributes() const
{
    return m_attributes;
}

Attribute const* Node::find_first_attribute_by_name(std::string const& name) const
{
    auto it = std::find_if(m_attributes.cbegin(), m_attributes.cend(), [&](Attribute const& attribute)
    {
        return attribute.get_name() == name;
    });

    return it != m_attributes.cend() ? &(*it) : nullptr;
}

static const char* type_to_string(Node::Type type)
{
#define CASE(X) case Node::Type::X: return #X; break;
    switch (type)
    {
    CASE(NONE);
    CASE(ROOT);
    CASE(LIST);
    CASE(TYPE);
    CASE(LITERAL);
    CASE(EXPRESSION);
    CASE(INHERITANCE);
    CASE(IDENTIFIER);
    CASE(TEMPLATE_INSTANTIATION);
    CASE(TEMPLATE_ARGUMENT);
    CASE(NAMESPACE_DECLARATION);
    CASE(NAMESPACE_BODY);
    CASE(ALIAS_DECLARATION);
    CASE(ENUM_DECLARATION);
    CASE(ENUM_BODY);
    CASE(STRUCT_DECLARATION);
    CASE(STRUCT_BODY);
    CASE(CONSTANT_DECLARATION);
    CASE(MEMBER_DECLARATION);
    CASE(INITIALIZER_LIST);
    CASE(INITIALIZER);
    CASE(ATTRIBUTE);
    default: TS_ASSERT(false); return "<unknown>"; break;
    }
#undef CASE
}

std::string Node::to_string(size_t ident_count, bool deep) const
{
    std::string ident(ident_count, ' ');

    std::string str = ident + type_to_string(m_type);

    for (Attribute const& att: m_attributes)
    {
        str += " ";
        str += att.to_string();
    }

    str += "\n";

    if (deep)
    {
        for (Node const& ch: m_children)
        {
            str += ch.to_string(ident_count + 2, deep);
        }
    }

    return str;
}

Result<sz::Value> Node::serialize() const
{
    ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);

    sz_value.add_object_member("type", ts::sz::Value(static_cast<uint8_t>(m_type)));

    if (!m_attributes.empty())
    {
        ts::sz::Value sz_value_attributes(ts::sz::Value::Type::ARRAY);
        for (Attribute const& att: m_attributes)
        {
            auto result = att.serialize();
            if (result != success)
            {
                return result;
            }
            sz_value_attributes.add_array_element(result.extract_payload());
        }
        sz_value.add_object_member("attributes", std::move(sz_value_attributes));
    }

    if (!m_children.empty())
    {
        ts::sz::Value sz_value_children(ts::sz::Value::Type::ARRAY);
        for (Node const& ch: m_children)
        {
            auto result = ch.serialize();
            if (result != success)
            {
                return result;
            }
            sz_value_children.add_array_element(result.extract_payload());
        }
        sz_value.add_object_member("children", std::move(sz_value_children));
    }

    return sz_value;
}

Result<void> Node::deserialize(sz::Value const& sz_value)
{
    if (!sz_value.is_object())
    {
        return Error("Expected an object value while deserializing");
    }

    sz::Value const* sz_type_value = sz_value.find_object_member_by_name("type");
    if (!sz_type_value)
    {
        return Error("Cannot find 'type' member");
    }
    if (!sz_type_value->is_integral_number())
    {
        return Error("Expected integral for the 'type' member");
    }

    m_type = static_cast<Type>(sz_type_value->get_as_integral_number());

    sz::Value const* sz_attributes_value = sz_value.find_object_member_by_name("attributes");
    if (sz_attributes_value)
    {
        if (!sz_attributes_value->is_array())
        {
            return Error("Expected array 'attributes' member");
        }

        for (size_t i = 0; i < sz_attributes_value->get_array_element_count(); i++)
        {
            Attribute att;
            auto result = att.deserialize(sz_attributes_value->get_array_element_value(i));
            if (result != success)
            {
                return result;
            }
            add_attribute(att);
        }
    }
    sz::Value const* sz_children_value = sz_value.find_object_member_by_name("children");
    if (sz_children_value)
    {
        if (!sz_children_value->is_array())
        {
            return Error("Expected array 'children' member");
        }

        for (size_t i = 0; i < sz_children_value->get_array_element_count(); i++)
        {
            Node child;
            auto result = child.deserialize(sz_children_value->get_array_element_value(i));
            if (result != success)
            {
                return result;
            }
            add_child(std::move(child));
        }
    }

    return success;
}

}
}
