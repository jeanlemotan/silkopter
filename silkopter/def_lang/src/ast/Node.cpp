#include "Node.h"
#include "ts_assert.h"

namespace ast
{

Node::Node(Type type)
    : m_type(type)
{
    m_children.reserve(8);
}
auto Node::get_type() const -> Type
{
    return m_type;
}

auto Node::copy_children_from(Node const& node) -> Node&
{
    m_children.reserve(m_children.size() + node.m_children.size());
    std::copy(node.m_children.begin(), node.m_children.end(), std::back_inserter(m_children));
    return *this;
}

auto Node::move_children_from(Node&& node) -> Node&
{
    m_children.reserve(m_children.size() + node.m_children.size());
    std::move(node.m_children.begin(), node.m_children.end(), std::back_inserter(m_children));
    return *this;
}

auto Node::add_child(Node const& node) -> Node&
{
    m_children.push_back(node);
    return *this;
}

auto Node::get_children() const -> std::vector<Node> const&
{
    return m_children;
}

auto Node::find_first_child_by_type(Type type) const -> Node const*
{
    auto it = std::find_if(m_children.begin(), m_children.end(), [&](Node const& child) { return child.get_type() == type; });
    return it != m_children.end() ? &(*it) : nullptr;
}

auto Node::get_all_children_of_type(Type type) const -> std::vector<Node>
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

auto Node::add_attribute(Attribute const& att) -> Node&
{
    m_attributes.push_back(att);
    return *this;
}

auto Node::get_attributes() const -> std::vector<Attribute> const&
{
    return m_attributes;
}

auto Node::find_first_attribute_by_name(std::string const& name) const -> Attribute const*
{
    auto it = std::find_if(m_attributes.cbegin(), m_attributes.cend(), [&](Attribute const& attribute)
    {
        return attribute.get_name() == name;
    });

    return it != m_attributes.cend() ? &(*it) : nullptr;
}

auto Node::to_string(size_t ident_count, bool deep) const -> std::string
{
    std::string ident(ident_count, ' ');

    std::string str = ident;// + "Type: ";
#define CASE(X) case Type::X: str += #X; break;
    switch (m_type)
    {
    CASE(NONE);
    CASE(ROOT);
    CASE(LIST);
    CASE(TYPE);
    CASE(LITERAL);
    CASE(EXPRESSION);
    CASE(IDENTIFIER);
    CASE(TEMPLATE_INSTANTIATION);
    CASE(TEMPLATE_ARGUMENT);
    CASE(NAMESPACE_DECLARATION);
    CASE(NAMESPACE_BODY);
    CASE(ALIAS_DECLARATION);
    CASE(STRUCT_DECLARATION);
    CASE(STRUCT_BODY);
    CASE(CONSTANT_DECLARATION);
    CASE(MEMBER_DECLARATION);
    CASE(INITIALIZER_LIST);
    CASE(INITIALIZER);
    CASE(ATTRIBUTE);
    default: TS_ASSERT(false); str += "<unknown>"; break;
    }
#undef CASE


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

}
