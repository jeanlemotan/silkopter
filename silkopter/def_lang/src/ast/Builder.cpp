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

static auto get_type_name(Node const& node) -> boost::optional<std::string>
{
    Node const* type = node.find_first_child_by_type(Node::Type::TYPE);
    if (!type)
    {
        return boost::none;
    }
    return get_name_identifier(*type);
}


static auto create_namespace(ts::IDeclaration_Scope& scope, Node const& node) -> bool
{
    assert(node.get_type() == Node::Type::NAMESPACE_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << "Cannot find namespace name identifier";
        return false;
    }

    ts::Namespace* ns = new ts::Namespace(*name);

    //add it to the typesystem so we can search for types
    scope.add_symbol(std::unique_ptr<ts::ISymbol>(ns));


    Node const* body = node.find_first_child_by_type(Node::Type::NAMESPACE_BODY);
    if (body)
    {
        for (Node const& ch: body->get_children())
        {
            if (!populate_declaration_scope(*ns, ch))
            {
                return false;
            }
        }
    }

    return true;
}

static auto create_member_def(ts::IDeclaration_Scope& scope, Node const& node) -> std::unique_ptr<ts::Member_Def>
{
    assert(node.get_type() == Node::Type::MEMBER_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << "Cannot find member name identifier";
        return nullptr;
    }

    boost::optional<std::string> type_name = get_type_name(node);
    if (!type_name)
    {
        std::cerr << "Cannot find member type name";
        return nullptr;
    }

    std::shared_ptr<ts::IType> type = scope.find_specialized_symbol_by_path<ts::IType>(ts::Symbol_Path(*type_name));
    if (!type)
    {
        std::cerr << "Cannot find type " << *type_name;
        return nullptr;
    }

    std::unique_ptr<ts::Member_Def> def;
    def.reset(new ts::Member_Def(*name, type, nullptr));
    return std::move(def);
}

static auto create_alias(ts::IDeclaration_Scope& scope, Node const& node) -> bool
{
    assert(node.get_type() == Node::Type::ALIAS_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << "Cannot find namespace name identifier";
        return false;
    }

    boost::optional<std::string> type_name = get_type_name(node);
    if (!type_name)
    {
        std::cerr << "Cannot find alias type name";
        return false;
    }

    std::shared_ptr<ts::IType> type = scope.find_specialized_symbol_by_path<ts::IType>(ts::Symbol_Path(*type_name));
    if (!type)
    {
        std::cerr << "Cannot find type " << *type_name;
        return false;
    }

    std::unique_ptr<ts::IType> aliased_type = type->clone(*name);

    scope.add_symbol(std::move(aliased_type));

    return true;
}

static auto create_struct_type(ts::IDeclaration_Scope& scope, Node const& node) -> bool
{
    assert(node.get_type() == Node::Type::STRUCT_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << "Cannot find namespace name identifier";
        return false;
    }

    ts::Struct_Type* type = new ts::Struct_Type(*name);

    //add it to the typesystem so we can search for types
    scope.add_symbol(std::unique_ptr<ts::ISymbol>(type));

    Node const* body = node.find_first_child_by_type(Node::Type::STRUCT_BODY);
    if (body)
    {
        for (Node const& ch: body->get_children())
        {
            if (ch.get_type() == Node::Type::STRUCT_DECLARATION)
            {
                if (!create_struct_type(*type, ch))
                {
                    return false;
                }
            }
            else if (ch.get_type() == Node::Type::ALIAS_DECLARATION)
            {
                if (!create_alias(*type, ch))
                {
                    return false;
                }
            }
            else if (ch.get_type() == Node::Type::MEMBER_DECLARATION)
            {
                std::unique_ptr<ts::Member_Def> t = create_member_def(*type, ch);
                if (!t)
                {
                    return false;
                }
                if (!type->add_member_def(std::move(t)))
                {
                    return false;
                }
            }
            else
            {
                std::cerr << "Illegal node type in struct";
                return false;
            }
        }
    }

    return true;
}

static auto create_symbol(ts::IDeclaration_Scope& scope, Node const& node) -> bool
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
        return create_alias(scope, node);
    }

    return false;
}

static auto populate_declaration_scope(ts::IDeclaration_Scope& scope, Node const& node) -> bool
{
    return create_symbol(scope, node);
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
