#include "Builder.h"
#include "Lexer.h"
#include "yy_parser.hpp"


#include <boost/optional.hpp>

#include "IValue.h"
#include "IInitializer.h"
#include "IExpression.h"
#include "ILiteral.h"
#include "IAttribute_Container.h"
#include "ITemplated_Type.h"
#include "IBool_Type.h"
#include "IBool_Value.h"
#include "All_INumeric_Types.h"
#include "All_INumeric_Values.h"

#include "impl/Namespace.h"
#include "impl/Struct_Type.h"
#include "impl/Member_Def.h"
#include "impl/Initializer_List.h"
#include "impl/Literal_Expression.h"
#include "impl/Literal.h"
#include "impl/Max_Attribute.h"
#include "impl/Min_Attribute.h"
#include "impl/Decimals_Attribute.h"
#include "impl/UI_Name_Attribute.h"
#include "impl/String_Type.h"
#include "impl/String_Value.h"


namespace ast
{

Builder::Builder()
    : m_root_node(Node::Type::ROOT, ts::Source_Location())
{
    m_lexer.reset(new Lexer(*this));
    m_parser.reset(new yy::parser(*this));
}

Builder::~Builder()
{
}

ts::Source_Location Builder::get_location() const
{
    yy::location const& loc = m_lexer->get_location();
    return ts::Source_Location(get_filename(), loc.begin.line, loc.begin.column);
}

ts::Result<void> Builder::parse(std::string const& filename)
{
    auto result = start_file(filename);
    if (result != ts::success)
    {
        return result;
    }

    m_parser->parse();

    return ts::success;
}

ts::Result<void> Builder::start_file(std::string const& filename)
{
    std::shared_ptr<std::ifstream> fs(new std::ifstream(filename));
    if (!fs->is_open())
    {
        return ts::Error("Cannot open file '" + filename + "'");
    }

    yy_buffer_state* buffer = m_lexer->yy_create_buffer(*fs, 32);
    if (!buffer)
    {
        return ts::Error("Cannot create lexer buffer");
    }

    Import import;
    import.stream = std::move(fs);
    import.filename = filename;
    m_imports.push_back(std::move(import));

    m_lexer->yypush_buffer_state(buffer);

    return ts::success;
}

bool Builder::end_file()
{
    if (m_imports.empty())
    {
        return false;
    }
    m_lexer->yypop_buffer_state();
    m_imports.pop_back();

    if (m_imports.empty())
    {
        return false;
    }
    return true;
}

Node& Builder::get_ast_root_node()
{
    return m_root_node;
}

Node const& Builder::get_ast_root_node() const
{
    return m_root_node;
}

std::string Builder::get_filename() const
{
    return m_imports.empty() ? std::string() : m_imports.back().filename;
}

Lexer& Builder::get_lexer()
{
    return *m_lexer;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////

static ts::Result<void> populate_declaration_scope(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node);


static ts::Result<std::string> get_name_identifier(Node const& node)
{
    Node const* identifier = node.find_first_child_by_type(Node::Type::IDENTIFIER);
    if (!identifier)
    {
        return ts::Error("Cannot find 'identifier'' node");
    }
    Attribute const* identifier_value = identifier->find_first_attribute_by_name("value");
    if (!identifier_value)
    {
        return ts::Error("Cannot find 'value'' attribute for identifier node");
    }
    if (identifier_value->get_type() != Attribute::Type::STRING)
    {
        return ts::Error("Wrong type for 'value' attribute of 'identifier' node. Expected string.");
    }
    return identifier_value->get_as_string();
}

static ts::Result<std::shared_ptr<const ts::IType>> find_type_or_instantiate_templated_type(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node)
{
    Node const* type_node = node.find_first_child_by_type(Node::Type::TYPE);
    if (!type_node)
    {
        return ts::Error(node.get_source_location().to_string() + "Malformed type node ast");
    }
    auto name_result = get_name_identifier(*type_node);
    if (name_result == ts::success)
    {
        std::string type_name = name_result.payload();
        std::shared_ptr<ts::IType> type = scope.find_specialized_symbol_by_path<ts::IType>(ts::Symbol_Path(type_name));
        if (!type)
        {
            return ts::Error(type_node->get_source_location().to_string() + "Cannot find type " + type_name);
        }
        return type;
    }
    else
    {
        Node const* template_node = type_node->find_first_child_by_type(Node::Type::TEMPLATE_INSTANTIATION);
        if (!template_node)
        {
            return ts::Error(type_node->get_source_location().to_string() + "Malformed type node ast");
        }

        name_result = get_name_identifier(*template_node);
        if (name_result != ts::success)
        {
            return ts::Error(template_node->get_source_location().to_string() + "Missing templated type name: " + name_result.error().what());
        }
        std::string type_name = name_result.payload();

        std::vector<Node> template_argument_nodes = template_node->get_all_children_of_type(Node::Type::TEMPLATE_ARGUMENT);
        std::vector<std::shared_ptr<const ts::ITemplate_Argument>> template_arguments;
        template_arguments.reserve(template_argument_nodes.size());

        for (Node const& node: template_argument_nodes)
        {
            auto instantiate_result = find_type_or_instantiate_templated_type(ts, scope, node);
            if (instantiate_result != ts::success)
            {
                return ts::Error(node.get_source_location().to_string() +
                                 "Cannot resolve template argument " +
                                 std::to_string(template_arguments.size()) +
                                 ": " +
                                 instantiate_result.error().what());
            }

            std::shared_ptr<const ts::IType> template_argument = instantiate_result.payload();
            template_arguments.push_back(template_argument);
        }

        auto instantiate_result = ts.instantiate_template(type_name, template_arguments);
        if (instantiate_result != ts::success)
        {
            return ts::Error(type_node->get_source_location().to_string() + "Cannot instantiate template: " + instantiate_result.error().what());
        }
        return instantiate_result.payload();
    }
}


static ts::Result<void> create_namespace(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node)
{
    TS_ASSERT(node.get_type() == Node::Type::NAMESPACE_DECLARATION);

    auto name_result = get_name_identifier(node);
    if (name_result != ts::success)
    {
        return ts::Error(node.get_source_location().to_string() + "Cannot find namespace name identifier: " + name_result.error().what());
    }

    std::string name = name_result.payload();

    ts::Namespace* ns = new ts::Namespace(name);

    //add it to the typesystem so we can search for types
    auto result = scope.add_symbol(std::shared_ptr<ts::ISymbol>(ns));
    if (result != ts::success)
    {
        return ts::Error(node.get_source_location().to_string() + result.error().what());
    }

    Node const* body = node.find_first_child_by_type(Node::Type::NAMESPACE_BODY);
    if (body)
    {
        for (Node const& ch: body->get_children())
        {
            auto result = populate_declaration_scope(ts, *ns, ch);
            if (result != ts::success)
            {
                return result;
            }
        }
    }

    return ts::success;
}

static ts::Result<std::shared_ptr<ts::ILiteral>> create_literal(ts::Type_System& ts, Node const& node)
{
    TS_ASSERT(node.get_type() == Node::Type::LITERAL);

    Attribute const* value_attribute = node.find_first_attribute_by_name("value");
    if (!value_attribute)
    {
        return ts::Error(node.get_source_location().to_string() + "Literal without a value!");
    }

    std::shared_ptr<ts::IValue> value;
    switch (value_attribute->get_type())
    {
    case Attribute::Type::BOOL:
    {
        std::shared_ptr<const ts::IBool_Type> type = ts.find_specialized_symbol_by_name<const ts::IBool_Type>("bool");
        if (type)
        {
            std::shared_ptr<ts::IBool_Value> v = type->create_specialized_value();
            if (v)
            {
                auto result = v->set_value(value_attribute->get_as_bool());
                if (result != ts::success)
                {
                    return ts::Error(node.get_source_location().to_string() + "Cannot assign value:" + result.error().what());
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
            std::shared_ptr<ts::IDouble_Value> v = type->create_specialized_value();
            if (v)
            {
                auto result = v->set_value(value_attribute->get_as_double());
                if (result != ts::success)
                {
                    return ts::Error(node.get_source_location().to_string() + "Cannot assign value:" + result.error().what());
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
            std::shared_ptr<ts::IFloat_Value> v = type->create_specialized_value();
            if (v)
            {
                auto result = v->set_value(value_attribute->get_as_float());
                if (result != ts::success)
                {
                    return ts::Error(node.get_source_location().to_string() + "Cannot assign value:" + result.error().what());
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
            std::shared_ptr<ts::IInt64_Value> v = type->create_specialized_value();
            if (v)
            {
                auto result = v->set_value(value_attribute->get_as_integral());
                if (result != ts::success)
                {
                    return ts::Error(node.get_source_location().to_string() + "Cannot assign value:" + result.error().what());
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
            std::shared_ptr<ts::IString_Value> v = type->create_specialized_value();
            if (v)
            {
                auto result = v->set_value(value_attribute->get_as_string());
                if (result != ts::success)
                {
                    return ts::Error(node.get_source_location().to_string() + "Cannot assign value:" + result.error().what());
                }
                value = std::move(v);
            }
        }
        break;
    }
    }

    if (!value)
    {
        return ts::Error(node.get_source_location().to_string() + "Cannot create literal of type " + value_attribute->to_string());
    }

    return std::make_shared<ts::Literal>(std::move(value));
}

static ts::Result<std::shared_ptr<ts::IInitializer>> create_initializer(ts::Type_System& ts, Node const& node)
{
    if (node.get_type() == Node::Type::INITIALIZER)
    {
        if (node.get_children().size() != 1)
        {
            return ts::Error(node.get_source_location().to_string() + "Invalid intializer node!");
        }

        Node const* literal_node = node.find_first_child_by_type(Node::Type::LITERAL);
        if (literal_node)
        {
            auto result = create_literal(ts, *literal_node);
            if (result != ts::success)
            {
                return result.error();
            }
            std::shared_ptr<ts::ILiteral> literal = result.extract_payload();
            return std::move(literal);
        }
    }
    else if (node.get_type() == Node::Type::INITIALIZER_LIST)
    {
        std::vector<std::shared_ptr<ts::IInitializer>> initializers;
        for (Node const& ch: node.get_children())
        {
            auto result = create_initializer(ts, ch);
            if (result != ts::success)
            {
                return result.error();
            }
            std::shared_ptr<ts::IInitializer> initializer_ch = result.extract_payload();
            initializers.push_back(std::move(initializer_ch));
        }

        return std::make_shared<ts::Initializer_List>(std::move(initializers));
    }
    else
    {
        return ts::Error(node.get_source_location().to_string() + "Unknown initializer");
    }

    return nullptr;
}

static ts::Result<void> create_attributes(ts::Type_System& ts, ts::IType const& type, ts::IAttribute_Container& container, Node const& node)
{
    std::vector<Node> attribute_nodes = node.get_all_children_of_type(Node::Type::ATTRIBUTE);
    for (Node const& attribute_node: attribute_nodes)
    {
        auto name_result = get_name_identifier(attribute_node);
        if (name_result != ts::success)
        {
            return name_result.error();
        }

        std::string attribute_name = name_result.payload();

        std::shared_ptr<ts::IInitializer> initializer;
        Node const* initializer_node = attribute_node.find_first_child_by_type(Node::Type::INITIALIZER);
        if (!initializer_node)
        {
            initializer_node = attribute_node.find_first_child_by_type(Node::Type::INITIALIZER_LIST);
        }
        if (initializer_node)
        {
            auto result = create_initializer(ts, *initializer_node);
            if (result != ts::success)
            {
                return ts::Error(attribute_node.get_source_location().to_string() + "Cannot create initializer: " + result.error().what());
            }
            initializer = result.extract_payload();
        }

        std::shared_ptr<ts::IAttribute> attribute;

        if (attribute_name == "min")
        {
            if (!initializer)
            {
                return ts::Error(attribute_node.get_source_location().to_string() + "Missing initializer for attribute");
            }

            std::shared_ptr<ts::IValue> value = type.create_value();
            auto result = value->copy_assign(*initializer);
            if (result != ts::success)
            {
                return ts::Error(attribute_node.get_source_location().to_string() + "Cannot initialize attribute: " + result.error().what());
            }

            attribute = std::make_shared<ts::Min_Attribute>(std::move(value));
        }
        else if (attribute_name == "max")
        {
            if (!initializer)
            {
                return ts::Error(attribute_node.get_source_location().to_string() + "Missing initializer for attribute");
            }

            std::shared_ptr<ts::IValue> value = type.create_value();
            auto result = value->copy_assign(*initializer);
            if (result != ts::success)
            {
                return ts::Error(attribute_node.get_source_location().to_string() + "Cannot initialize attribute: " + result.error().what());
            }

            attribute = std::make_shared<ts::Max_Attribute>(std::move(value));
        }
        else if (attribute_name == "decimals")
        {
            if (!initializer)
            {
                return ts::Error(attribute_node.get_source_location().to_string() + "Missing initializer for attribute");
            }

            std::shared_ptr<ts::IInt64_Value> value = ts.find_specialized_symbol_by_name<ts::IInt64_Type>("int64_t")->create_specialized_value();
            auto result = value->copy_assign(*initializer);
            if (result != ts::success)
            {
                return ts::Error(attribute_node.get_source_location().to_string() + "Cannot initialize attribute: " + result.error().what());
            }

            attribute = std::make_shared<ts::Decimals_Attribute>(value->get_value());
        }
        else if (attribute_name == "ui_name")
        {
            std::shared_ptr<ts::IString_Value> value = ts.find_specialized_symbol_by_name<ts::IString_Type>("string")->create_specialized_value();
            auto result = value->copy_assign(*initializer);
            if (result != ts::success)
            {
                return ts::Error(attribute_node.get_source_location().to_string() + "Cannot initialize attribute: " + result.error().what());
            }

            attribute = std::make_shared<ts::UI_Name_Attribute>(value->get_value());
        }
        else
        {
            return ts::Error(attribute_node.get_source_location().to_string() + "Unknown attribute " + attribute_name);
        }

        auto add_result = container.add_attribute(std::move(attribute));
        if (add_result != ts::success)
        {
            return ts::Error(attribute_node.get_source_location().to_string() + "Bad attribute: " + add_result.error().what());
        }
    }

    return ts::success;
}


static ts::Result<std::shared_ptr<ts::IMember_Def>> create_member_def(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node)
{
    TS_ASSERT(node.get_type() == Node::Type::MEMBER_DECLARATION);

    auto name_result = get_name_identifier(node);
    if (name_result != ts::success)
    {
        return name_result.error();
    }
    std::string name = name_result.payload();

    auto type_result = find_type_or_instantiate_templated_type(ts, scope, node);
    if (type_result != ts::success)
    {
        return ts::Error(node.get_source_location().to_string() + "Cannot find member type: " + type_result.error().what());
    }

    std::shared_ptr<const ts::IType> type = type_result.extract_payload();

    std::shared_ptr<ts::IValue> value = type->create_value();

    Node const* initializer_node = node.find_first_child_by_type(Node::Type::INITIALIZER);
    if (!initializer_node)
    {
        initializer_node = node.find_first_child_by_type(Node::Type::INITIALIZER_LIST);
    }
    if (initializer_node)
    {
        auto initializer_result = create_initializer(ts, *initializer_node);
        if (initializer_result != ts::success)
        {
            return ts::Error(initializer_node->get_source_location().to_string() + "Cannot create member initializer: " + initializer_result.error().what());
        }

        std::shared_ptr<ts::IInitializer> initializer = initializer_result.extract_payload();
        auto result = value->copy_assign(*initializer);
        if (result != ts::success)
        {
            return ts::Error(initializer_node->get_source_location().to_string() + "Cannot initialize value: " + result.error().what());
        }
    }
    else
    {
    }

    std::shared_ptr<ts::IMember_Def> def = std::make_shared<ts::Member_Def>(name, type, std::move(value));

    auto create_result = create_attributes(ts, *type, *def, node);
    if (create_result != ts::success)
    {
        return create_result.error();
    }

    return std::move(def);
}

static ts::Result<void> create_alias(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node)
{
    TS_ASSERT(node.get_type() == Node::Type::ALIAS_DECLARATION);

    auto name_result = get_name_identifier(node);
    if (name_result != ts::success)
    {
        return name_result.error();
    }
    std::string name = name_result.payload();

    auto type_result = find_type_or_instantiate_templated_type(ts, scope, node);
    if (type_result != ts::success)
    {
        return ts::Error(node.get_source_location().to_string() + "Cannot find member type: " + type_result.error().what());
    }

    std::shared_ptr<const ts::IType> type = type_result.extract_payload();

    std::shared_ptr<ts::IType> aliased_type = type->clone(name);

    auto create_result = create_attributes(ts, *aliased_type, *aliased_type, node);
    if (create_result != ts::success)
    {
        return ts::Error(node.get_source_location().to_string() + "Cannot create alias attributes: " + create_result.error().what());
    }

    auto add_result = scope.add_symbol(std::move(aliased_type));
    if (add_result != ts::success)
    {
        return ts::Error(node.get_source_location().to_string() + "Cannot add alias: " + add_result.error().what());
    }

    return ts::success;
}

static ts::Result<void> create_struct_type(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node)
{
    TS_ASSERT(node.get_type() == Node::Type::STRUCT_DECLARATION);

    auto name_result = get_name_identifier(node);
    if (name_result != ts::success)
    {
        return name_result.error();
    }
    std::string name = name_result.payload();

    ts::Struct_Type* type = new ts::Struct_Type(name);

    //add it to the typesystem so we can search for types
    auto result = scope.add_symbol(std::shared_ptr<ts::ISymbol>(type));
    if (result != ts::success)
    {
        return ts::Error(node.get_source_location().to_string() + result.error().what());
    }

    Node const* body = node.find_first_child_by_type(Node::Type::STRUCT_BODY);
    if (body)
    {
        for (Node const& ch: body->get_children())
        {
            if (ch.get_type() == Node::Type::STRUCT_DECLARATION)
            {
                auto result = create_struct_type(ts, *type, ch);
                if (result != ts::success)
                {
                    return result.error();
                }
            }
            else if (ch.get_type() == Node::Type::ALIAS_DECLARATION)
            {
                auto result = create_alias(ts, *type, ch);
                if (result != ts::success)
                {
                    return result.error();
                }
            }
            else if (ch.get_type() == Node::Type::MEMBER_DECLARATION)
            {
                auto result = create_member_def(ts, *type, ch);
                if (result != ts::success)
                {
                    return result.error();
                }

                std::shared_ptr<ts::IMember_Def> t = result.extract_payload();
                auto add_result = type->add_member_def(std::move(t));
                if (add_result != ts::success)
                {
                    return ts::Error(body->get_source_location().to_string() + "Cannot add member: " + add_result.error().what());
                }
            }
            else
            {
                return ts::Error(body->get_source_location().to_string() + "Illegal node type in struct");
            }
        }
    }

    return ts::success;
}

static ts::Result<void> create_symbol(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node)
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

static ts::Result<void> populate_declaration_scope(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node)
{
    return create_symbol(ts, scope, node);
}


ts::Result<void> Builder::compile(ts::Type_System& ts)
{
    if (m_root_node.get_children().empty())
    {
        return ts::success;
    }

    for (Node const& ch: m_root_node.get_children())
    {
        auto result = populate_declaration_scope(ts, ts, ch);
        if (result != ts::success)
        {
            return result;
        }
    }

    return ts::success;
}

}
