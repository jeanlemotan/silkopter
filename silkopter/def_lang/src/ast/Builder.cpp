#include "Builder.h"
#include "Lexer.h"

#include <boost/optional.hpp>

#include "Namespace.h"
#include "types/Struct_Type.h"
#include "Member_Def.h"
#include "values/IValue.h"

namespace ast
{

Builder::Builder()
    : m_root_node(Node::Type::ROOT)
{
    m_lexer.reset(new Lexer(*this));
}

Builder::~Builder()
{
}

auto Builder::start_file(std::string const& filename) -> bool
{
    std::unique_ptr<std::ifstream> fs(new std::ifstream(filename));
    if (!fs->is_open())
    {
        return false;
    }

    yy_buffer_state* buffer = m_lexer->yy_create_buffer(*fs, 32);
    if (!buffer)
    {
        return false;
    }

    m_imported_files.push_back(std::move(fs));

    m_lexer->yypush_buffer_state(buffer);

    return true;
}

auto Builder::end_file() -> bool
{
    if (m_imported_files.empty())
    {
        return false;
    }
    m_lexer->yypop_buffer_state();
    m_imported_files.pop_back();

    if (m_imported_files.empty())
    {
        return false;
    }
    return true;
}

auto Builder::get_root_node() -> Node&
{
    return m_root_node;
}

auto Builder::get_root_node() const -> Node const&
{
    return m_root_node;
}

auto Builder::get_filename() const -> std::string
{
    return m_filename;
}

auto Builder::get_lexer() -> Lexer&
{
    return *m_lexer;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////

static auto populate_declaration_scope(ts::IDeclaration_Scope& scope, Node const& node) -> bool;


static auto get_name_identifier(Node const& node) -> boost::optional<std::string>
{
    Node const* identifier = node.find_first_child_by_type(Node::Type::IDENTIFIER);
    if (!identifier)
    {
        return boost::none;
    }
    Attribute const* identifier_value = identifier->find_first_attribute_by_name("value");
    if (!identifier_value)
    {
        return boost::none;
    }
    if (identifier_value->get_type() != Attribute::Type::STRING)
    {
        return boost::none;
    }
    return identifier_value->get_as_string();
}

static auto create_namespace(ts::IDeclaration_Scope& scope, Node const& node) -> std::unique_ptr<ts::Namespace>
{
    assert(node.get_type() == Node::Type::NAMESPACE_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << "Cannot find namespace name identifier";
        return nullptr;
    }

    std::unique_ptr<ts::Namespace> ns;
    ns.reset(new ts::Namespace(*name));

    Node const* body = node.find_first_child_by_type(Node::Type::NAMESPACE_BODY);
    if (body)
    {
        for (Node const& ch: body->get_children())
        {
            if (!populate_declaration_scope(*ns, ch))
            {
                return nullptr;
            }
        }
    }

    return std::move(ns);
}

static auto create_member_def(ts::IDeclaration_Scope& scope, Node const& node) -> std::unique_ptr<ts::Member_Def>
{
    assert(node.get_type() == Node::Type::MEMBER_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << "Cannot find namespace name identifier";
        return nullptr;
    }

    std::unique_ptr<ts::Member_Def> def;
    def.reset(new ts::Member_Def(*name, nullptr, nullptr));
    return std::move(def);
}

static auto create_struct_type(ts::IDeclaration_Scope& scope, Node const& node) -> std::unique_ptr<ts::IStruct_Type>
{
    assert(node.get_type() == Node::Type::STRUCT_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << "Cannot find namespace name identifier";
        return nullptr;
    }

    std::unique_ptr<ts::Struct_Type> type;
    type.reset(new ts::Struct_Type(*name));

    Node const* body = node.find_first_child_by_type(Node::Type::STRUCT_BODY);
    if (body)
    {
        for (Node const& ch: body->get_children())
        {
            if (ch.get_type() == Node::Type::STRUCT_DECLARATION)
            {
                std::unique_ptr<ts::IStruct_Type> t = create_struct_type(*type, ch);
                if (!t)
                {
                    return nullptr;
                }
                if (!type->add_symbol(std::move(t)))
                {
                    return nullptr;
                }
            }
            else if (ch.get_type() == Node::Type::ALIAS_DECLARATION)
            {
//                std::unique_ptr<ts::IStruct_Type> t = create_alias(*type, ch);
//                if (!t)
//                {
//                    return nullptr;
//                }
//                if (!type->add_symbol(std::move(t)))
//                {
//                    return nullptr;
//                }
            }
            else if (ch.get_type() == Node::Type::MEMBER_DECLARATION)
            {
                std::unique_ptr<ts::Member_Def> t = create_member_def(*type, ch);
                if (!t)
                {
                    return nullptr;
                }
                if (!type->add_member_def(std::move(t)))
                {
                    return nullptr;
                }
            }
            else
            {
                std::cerr << "Illegal node type in struct";
                return nullptr;
            }
        }
    }

    return std::move(type);
}

static auto create_symbol(ts::IDeclaration_Scope& scope, Node const& node) -> std::unique_ptr<ts::ISymbol>
{
    if (node.get_type() == Node::Type::NAMESPACE_DECLARATION)
    {
        return create_namespace(scope, node);
    }
    else if (node.get_type() == Node::Type::STRUCT_DECLARATION)
    {
        return create_struct_type(scope, node);
    }
    else if (node.get_type() == Node::Type::ALIAS_DECLARATION)
    {
//        return create_alias(scope, node);
    }

    return nullptr;
}

static auto populate_declaration_scope(ts::IDeclaration_Scope& scope, Node const& node) -> bool
{
    std::unique_ptr<ts::ISymbol> symbol = create_symbol(scope, node);
    if (!symbol)
    {
        return false;
    }

    scope.add_symbol(std::move(symbol));
    return true;
}


auto Builder::populate_type_system(ts::Type_System& ts) -> bool
{
    if (m_root_node.get_children().empty())
    {
        return true;
    }

    for (Node const& ch: m_root_node.get_children())
    {
        if (!populate_declaration_scope(ts, ch))
        {
            return false;
        }
    }

    return true;
}

}
