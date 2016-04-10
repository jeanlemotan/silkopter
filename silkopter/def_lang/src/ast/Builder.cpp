#include "Builder.h"
#include "Lexer.h"

#include <boost/optional.hpp>

#include "Namespace.h"
#include "types/Struct_Type.h"
#include "Member_Def.h"
#include "values/IValue.h"
#include "IInitializer.h"
#include "Initializer_List.h"
#include "expression/IExpression.h"
#include "expression/Literal_Expression.h"
#include "ILiteral.h"
#include "Literal.h"
#include "types/ITemplated_Type.h"

#include "types/IBool_Type.h"
#include "types/All_INumeric_Types.h"
#include "types/String_Type.h"

#include "values/IBool_Value.h"
#include "values/All_INumeric_Values.h"
#include "values/String_Value.h"

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

static auto populate_declaration_scope(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node) -> bool;


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

static auto find_type_or_instantiate_templated_type(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node) -> std::shared_ptr<const ts::IType>
{
    Node const* type_node = node.find_first_child_by_type(Node::Type::TYPE);
    if (!type_node)
    {
        std::cerr << "Malformed type node ast\n";
        return nullptr;
    }
    boost::optional<std::string> type_name = get_name_identifier(*type_node);
    if (type_name)
    {
        std::shared_ptr<ts::IType> type = scope.find_specialized_symbol_by_path<ts::IType>(ts::Symbol_Path(*type_name));
        if (!type)
        {
            std::cerr << "Cannot find type " << *type_name << "\n";
            return nullptr;
        }
        return type;
    }
    else
    {
        Node const* template_node = type_node->find_first_child_by_type(Node::Type::TEMPLATE_INSTANTIATION);
        if (!template_node)
        {
            std::cerr << "Malformed type node ast\n";
            return nullptr;
        }

        type_name = get_name_identifier(*template_node);
        if (!type_name)
        {
            std::cerr << "Missing templated type name\n";
            return nullptr;
        }

        std::vector<Node> template_argument_nodes = template_node->get_all_children_of_type(Node::Type::TEMPLATE_ARGUMENT);
        std::vector<std::shared_ptr<const ts::ITemplate_Argument>> template_arguments;
        template_arguments.reserve(template_argument_nodes.size());

        for (Node const& node: template_argument_nodes)
        {
            std::shared_ptr<const ts::IType> template_argument = find_type_or_instantiate_templated_type(ts, scope, node);
            if (!template_argument)
            {
                std::cerr << "Cannot resolve template argument " << template_arguments.size() << "\n";
                return nullptr;
            }
            template_arguments.push_back(template_argument);
        }

        return ts.instantiate_template(*type_name, template_arguments);
    }

    return nullptr;
}


static auto create_namespace(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node) -> bool
{
    TS_ASSERT(node.get_type() == Node::Type::NAMESPACE_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << "Cannot find namespace name identifier";
        return false;
    }

    ts::Namespace* ns = new ts::Namespace(*name);

    //add it to the typesystem so we can search for types
    if (!scope.add_symbol(std::unique_ptr<ts::ISymbol>(ns)))
    {
        return false;
    }


    Node const* body = node.find_first_child_by_type(Node::Type::NAMESPACE_BODY);
    if (body)
    {
        for (Node const& ch: body->get_children())
        {
            if (!populate_declaration_scope(ts, *ns, ch))
            {
                return false;
            }
        }
    }

    return true;
}

static auto create_literal(ts::Type_System& ts, Node const& node) -> std::unique_ptr<ts::ILiteral>
{
    TS_ASSERT(node.get_type() == Node::Type::LITERAL);

    Attribute const* value_attribute = node.find_first_attribute_by_name("value");
    if (!value_attribute)
    {
        std::cerr << "Literal without a value!\n";
        return nullptr;
    }

    std::unique_ptr<ts::IValue> value;
    switch (value_attribute->get_type())
    {
    case Attribute::Type::BOOL:
    {
        std::shared_ptr<const ts::IBool_Type> type = ts.find_specialized_symbol_by_name<const ts::IBool_Type>("bool");
        if (type)
        {
            std::unique_ptr<ts::IBool_Value> v = type->create_specialized_value();
            if (v)
            {
                if (v->set_value(value_attribute->get_as_bool()) != ts::success)
                {
                    std::cerr << "Cannot assign value\n";
                    return nullptr;
                }
                value = std::move(v);
            }
        }
        break;
    }
    case Attribute::Type::DOUBLE:
    {
        std::shared_ptr<const ts::IDouble_Type> type = ts.find_specialized_symbol_by_name<const ts::IDouble_Type>("double");
        if (type)
        {
            std::unique_ptr<ts::IDouble_Value> v = type->create_specialized_value();
            if (v)
            {
                if (v->set_value(value_attribute->get_as_double()) != ts::success)
                {
                    std::cerr << "Cannot assign value\n";
                    return nullptr;
                }
                value = std::move(v);
            }
        }
        break;
    }
    case Attribute::Type::FLOAT:
    {
        std::shared_ptr<const ts::IFloat_Type> type = ts.find_specialized_symbol_by_name<const ts::IFloat_Type>("float");
        if (type)
        {
            std::unique_ptr<ts::IFloat_Value> v = type->create_specialized_value();
            if (v)
            {
                if (v->set_value(value_attribute->get_as_float()) != ts::success)
                {
                    std::cerr << "Cannot assign value\n";
                    return nullptr;
                }
                value = std::move(v);
            }
        }
        break;
    }
    case Attribute::Type::INTEGRAL:
    {
        std::shared_ptr<const ts::IInt64_Type> type = ts.find_specialized_symbol_by_name<const ts::IInt64_Type>("int64_t");
        if (type)
        {
            std::unique_ptr<ts::IInt64_Value> v = type->create_specialized_value();
            if (v)
            {
                if (v->set_value(value_attribute->get_as_integral()) != ts::success)
                {
                    std::cerr << "Cannot assign value\n";
                    return nullptr;
                }
                value = std::move(v);
            }
        }
        break;
    }
    case Attribute::Type::STRING:
    {
        std::shared_ptr<const ts::IString_Type> type = ts.find_specialized_symbol_by_name<const ts::IString_Type>("string");
        if (type)
        {
            std::unique_ptr<ts::IString_Value> v = type->create_specialized_value();
            if (v)
            {
                if (v->set_value(value_attribute->get_as_string()) != ts::success)
                {
                    std::cerr << "Cannot assign value\n";
                    return nullptr;
                }
                value = std::move(v);
            }
        }
        break;
    }
    }

    if (!value)
    {
        std::cerr << "Cannot create literal of type " << value_attribute->to_string() << "\n";
        return nullptr;
    }

    return std::unique_ptr<ts::ILiteral>(new ts::Literal(std::move(value)));
}

static auto create_initializer(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node) -> std::unique_ptr<ts::IInitializer>
{
    if (node.get_type() == Node::Type::INITIALIZER)
    {
        if (node.get_children().size() != 1)
        {
            std::cerr << "Invalid intializer node!\n";
            return nullptr;
        }

        Node const* literal_node = node.find_first_child_by_type(Node::Type::LITERAL);
        if (literal_node)
        {
            std::unique_ptr<ts::ILiteral> literal = create_literal(ts, *literal_node);
            return std::move(literal);
        }
    }
    else if (node.get_type() == Node::Type::INITIALIZER_LIST)
    {
        std::vector<std::unique_ptr<ts::IInitializer>> initializers;
        for (Node const& ch: node.get_children())
        {
            std::unique_ptr<ts::IInitializer> initializer_ch = create_initializer(ts, scope, ch);
            if (!initializer_ch)
            {
                return nullptr;
            }
            initializers.push_back(std::move(initializer_ch));
        }

        return std::unique_ptr<ts::IInitializer>(new ts::Initializer_List(std::move(initializers)));
    }
    else
    {
        std::cerr << "Unknown initializer\n";
        return nullptr;
    }

    return nullptr;
}


static auto create_member_def(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node) -> std::unique_ptr<ts::Member_Def>
{
    TS_ASSERT(node.get_type() == Node::Type::MEMBER_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << "Cannot find member name identifier\n";
        return nullptr;
    }

    std::shared_ptr<const ts::IType> type = find_type_or_instantiate_templated_type(ts, scope, node);
    if (!type)
    {
        std::cerr << "Cannot find member type\n";
        return nullptr;
    }

    std::unique_ptr<ts::IValue> value = type->create_value();

    Node const* initializer_node = node.find_first_child_by_type(Node::Type::INITIALIZER);
    if (!initializer_node)
    {
        initializer_node = node.find_first_child_by_type(Node::Type::INITIALIZER_LIST);
    }
    if (initializer_node)
    {
        std::unique_ptr<ts::IInitializer> initializer = create_initializer(ts, scope, *initializer_node);
        if (!initializer)
        {
            return nullptr;
        }

        if (value->copy_assign(*initializer) != ts::success)
        {
            std::cerr << "Cannot initialize value\n";
            return nullptr;
        }
    }
    else
    {
    }

    std::unique_ptr<ts::Member_Def> def;
    def.reset(new ts::Member_Def(*name, *type, std::move(value)));
    return std::move(def);
}

static auto create_alias(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node) -> bool
{
    TS_ASSERT(node.get_type() == Node::Type::ALIAS_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << "Cannot find alias name identifier\n";
        return false;
    }

    std::shared_ptr<const ts::IType> type = find_type_or_instantiate_templated_type(ts, scope, node);
    if (!type)
    {
        std::cerr << "Cannot find alias type\n";
        return false;
    }

    std::unique_ptr<ts::IType> aliased_type = type->clone(*name);

    return scope.add_symbol(std::move(aliased_type)) != nullptr;
}

static auto create_struct_type(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node) -> bool
{
    TS_ASSERT(node.get_type() == Node::Type::STRUCT_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << "Cannot find struct name identifier\n";
        return false;
    }

    ts::Struct_Type* type = new ts::Struct_Type(*name);

    //add it to the typesystem so we can search for types
    if (!scope.add_symbol(std::unique_ptr<ts::ISymbol>(type)))
    {
        return false;
    }

    Node const* body = node.find_first_child_by_type(Node::Type::STRUCT_BODY);
    if (body)
    {
        for (Node const& ch: body->get_children())
        {
            if (ch.get_type() == Node::Type::STRUCT_DECLARATION)
            {
                if (!create_struct_type(ts, *type, ch))
                {
                    return false;
                }
            }
            else if (ch.get_type() == Node::Type::ALIAS_DECLARATION)
            {
                if (!create_alias(ts, *type, ch))
                {
                    return false;
                }
            }
            else if (ch.get_type() == Node::Type::MEMBER_DECLARATION)
            {
                std::unique_ptr<ts::Member_Def> t = create_member_def(ts, *type, ch);
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
                std::cerr << "Illegal node type in struct\n";
                return false;
            }
        }
    }

    return true;
}

static auto create_symbol(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node) -> bool
{
    if (node.get_type() == Node::Type::NAMESPACE_DECLARATION)
    {
        return create_namespace(ts, scope, node);
    }
    else if (node.get_type() == Node::Type::STRUCT_DECLARATION)
    {
        return create_struct_type(ts, scope, node);
    }
    else if (node.get_type() == Node::Type::ALIAS_DECLARATION)
    {
        return create_alias(ts, scope, node);
    }

    return false;
}

static auto populate_declaration_scope(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node) -> bool
{
    return create_symbol(ts, scope, node);
}


auto Builder::populate_type_system(ts::Type_System& ts) -> bool
{
    if (m_root_node.get_children().empty())
    {
        return true;
    }

    for (Node const& ch: m_root_node.get_children())
    {
        if (!populate_declaration_scope(ts, ts, ch))
        {
            return false;
        }
    }

    return true;
}

}
