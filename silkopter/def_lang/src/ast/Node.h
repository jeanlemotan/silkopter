#pragma once

#include "Attribute.h"

namespace ast
{

class Node
{
public:
    enum class Type
    {
        NONE,
        ROOT,
        LIST,
        TYPE,
        LITERAL,
        EXPRESSION,
        IDENTIFIER,

        TEMPLATE_INSTANTIATION,
        TEMPLATE_ARGUMENT,

        NAMESPACE_DECLARATION,
        NAMESPACE_BODY,

        ALIAS_DECLARATION,

        STRUCT_DECLARATION,
        STRUCT_BODY,

        CONSTANT_DECLARATION,

        MEMBER_DECLARATION,

        INITIALIZER_LIST,

        ATTRIBUTE,
    };

    Node() = default;
    Node(Type type);

    auto get_type() const -> Type;

    auto copy_children_from(Node const& node) -> Node&;
    auto move_children_from(Node&& node) -> Node&;

    auto add_child(Node const& node) -> Node&;
    auto get_children() const -> std::vector<Node> const&;

    auto find_first_child_by_type(Type type) const -> Node const*;
    auto get_all_children_of_type(Type type) const -> std::vector<Node>;

    auto add_attribute(Attribute const& att) -> Node&;
    auto get_attributes() const -> std::vector<Attribute> const&;
    auto find_first_attribute_by_name(std::string const& name) const -> Attribute const*;

    auto to_string(size_t ident, bool deep) const -> std::string;

private:
    Type m_type = Type::NONE;
    std::vector<Node> m_children;
    std::vector<Attribute> m_attributes;

};

}
