#include "def_lang/ast/Builder.h"
#include "Lexer.h"
#include "yy_parser.hpp"

#include <sstream>

#include <boost/optional.hpp>

#include "def_lang/IValue.h"
#include "def_lang/IInitializer.h"
#include "def_lang/IExpression.h"
#include "def_lang/ILiteral.h"
#include "def_lang/IAttribute_Container.h"
#include "def_lang/ITemplated_Type.h"
#include "def_lang/IBool_Type.h"
#include "def_lang/IBool_Value.h"
#include "def_lang/All_INumeric_Types.h"
#include "def_lang/All_INumeric_Values.h"

#include "def_lang/impl/Namespace.h"
#include "def_lang/impl/Struct_Type.h"
#include "def_lang/impl/Member_Def.h"
#include "def_lang/impl/Initializer_List.h"
#include "def_lang/impl/Literal_Initializer.h"
#include "def_lang/impl/Enum_Item_Initializer.h"
#include "def_lang/impl/Literal_Expression.h"
#include "def_lang/impl/Literal.h"
#include "def_lang/impl/Max_Attribute.h"
#include "def_lang/impl/Min_Attribute.h"
#include "def_lang/impl/Decimals_Attribute.h"
#include "def_lang/impl/Default_Attribute.h"
#include "def_lang/impl/UI_Name_Attribute.h"
#include "def_lang/impl/Native_Type_Attribute.h"
#include "def_lang/impl/String_Type.h"
#include "def_lang/impl/String_Value.h"
#include "def_lang/impl/Enum_Type.h"
#include "def_lang/impl/Enum_Item.h"


namespace ts
{
namespace ast
{

Builder::Builder()
    : m_root_node(Node::Type::ROOT, Source_Location())
    , m_parse_result(success)
{
    m_lexer.reset(new Lexer(*this));
    m_parser.reset(new yy::parser(*this));
}

Builder::~Builder()
{
}

Source_Location Builder::get_location() const
{
    yy::location const& loc = m_lexer->get_location();
    return Source_Location(get_filename(), loc.begin.line, loc.begin.column);
}

void Builder::report_error(Error const& error)
{
    m_parse_result = error;
}

Result<void> Builder::parse_file(std::string const& filename)
{
    m_parse_result = success;

    auto result = start_file(filename);
    if (!result)
    {
        return Error("Cannot parse file '" + filename + "'");
    }

    m_parser->parse();

    return std::move(m_parse_result);
}

Result<void> Builder::parse_string(std::string const& def)
{
    m_parse_result = success;

    std::shared_ptr<std::stringstream> ss(new std::stringstream(def));
    if (!start_stream("<string>", std::move(ss)))
    {
        return Error("Cannot parse string");
    }

    return std::move(m_parse_result);
}

bool Builder::start_file(std::string const& filename)
{
    std::shared_ptr<std::ifstream> fs(new std::ifstream(filename));
    if (!fs->is_open())
    {
        std::cerr << "Cannot open file '" + filename + "'";
        return false;
    }

    return start_stream(filename, std::move(fs));
}

bool Builder::start_stream(std::string const& filename, std::shared_ptr<std::istream>&& stream)
{
    yy_buffer_state* buffer = m_lexer->yy_create_buffer(*stream, 8192);
    if (!buffer)
    {
        std::cerr << "Cannot create lexer buffer";
        return false;
    }

    Import import;
    import.stream = std::move(stream);
    import.filename = filename;
    m_imports.push_back(std::move(import));

    m_lexer->yypush_buffer_state(buffer);

    return true;
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

static Result<void> populate_declaration_scope(Type_System& ts, IDeclaration_Scope& scope, Node const& node);
static Result<void> create_enum_type(Type_System& ts, IDeclaration_Scope& scope, Node const& node);

static Result<std::string> get_name_identifier(Node const& node)
{
    Node const* identifier = node.find_first_child_by_type(Node::Type::IDENTIFIER);
    if (!identifier)
    {
        return Error("Cannot find 'identifier'' node");
    }
    Attribute const* identifier_value = identifier->find_first_attribute_by_name("value");
    if (!identifier_value)
    {
        return Error("Cannot find 'value'' attribute for identifier node");
    }
    if (identifier_value->get_type() != Attribute::Type::STRING)
    {
        return Error("Wrong type for 'value' attribute of 'identifier' node. Expected string.");
    }
    return identifier_value->get_as_string();
}

static Result<std::shared_ptr<const IType>> find_type_or_instantiate_templated_type(Type_System& ts, IDeclaration_Scope& scope, Node const& node)
{
    Node const* type_node = node.find_first_child_by_type(Node::Type::TYPE);
    if (!type_node)
    {
        return Error(node.get_source_location().to_string() + "Malformed type node ast");
    }
    auto name_result = get_name_identifier(*type_node);
    if (name_result == success)
    {
        std::string type_name = name_result.payload();
        std::shared_ptr<const IType> type = scope.find_specialized_symbol_by_path<const IType>(Symbol_Path(type_name));
        if (!type)
        {
            return Error(type_node->get_source_location().to_string() + "Cannot find type " + type_name);
        }
        return type;
    }
    else
    {
        Node const* template_node = type_node->find_first_child_by_type(Node::Type::TEMPLATE_INSTANTIATION);
        if (!template_node)
        {
            return Error(type_node->get_source_location().to_string() + "Malformed type node ast");
        }

        name_result = get_name_identifier(*template_node);
        if (name_result != success)
        {
            return Error(template_node->get_source_location().to_string() + "Missing templated type name: " + name_result.error().what());
        }
        std::string type_name = name_result.payload();

        std::vector<Node> template_argument_nodes = template_node->get_all_children_of_type(Node::Type::TEMPLATE_ARGUMENT);
        std::vector<std::shared_ptr<const ITemplate_Argument>> template_arguments;
        template_arguments.reserve(template_argument_nodes.size());

        for (Node const& node: template_argument_nodes)
        {
            auto instantiate_result = find_type_or_instantiate_templated_type(ts, scope, node);
            if (instantiate_result != success)
            {
                return Error(node.get_source_location().to_string() +
                                 "Cannot resolve template argument " +
                                 std::to_string(template_arguments.size()) +
                                 ": " +
                                 instantiate_result.error().what());
            }

            std::shared_ptr<const IType> template_argument = instantiate_result.payload();
            template_arguments.push_back(template_argument);
        }

        auto instantiate_result = ts.instantiate_template(type_name, template_arguments);
        if (instantiate_result != success)
        {
            return Error(type_node->get_source_location().to_string() + "Cannot instantiate template: " + instantiate_result.error().what());
        }
        return instantiate_result.payload();
    }
}


static Result<void> create_namespace(Type_System& ts, IDeclaration_Scope& scope, Node const& node)
{
    TS_ASSERT(node.get_type() == Node::Type::NAMESPACE_DECLARATION);

    auto name_result = get_name_identifier(node);
    if (name_result != success)
    {
        return Error(node.get_source_location().to_string() + "Cannot find namespace name identifier: " + name_result.error().what());
    }

    std::string name = name_result.payload();

    Namespace* ns = new Namespace(name);

    //add it to the typesystem so we can search for types
    auto result = scope.add_symbol(std::shared_ptr<ISymbol>(ns));
    if (result != success)
    {
        return Error(node.get_source_location().to_string() + result.error().what());
    }

    Node const* body = node.find_first_child_by_type(Node::Type::NAMESPACE_BODY);
    if (body)
    {
        for (Node const& ch: body->get_children())
        {
            auto result = populate_declaration_scope(ts, *ns, ch);
            if (result != success)
            {
                return result;
            }
        }
    }

    return success;
}

static Result<std::shared_ptr<ILiteral>> create_literal(Type_System& ts, Node const& node)
{
    TS_ASSERT(node.get_type() == Node::Type::LITERAL);

    Attribute const* value_attribute = node.find_first_attribute_by_name("value");
    if (!value_attribute)
    {
        return Error(node.get_source_location().to_string() + "Literal without a value!");
    }

    std::shared_ptr<IValue> value;
    switch (value_attribute->get_type())
    {
    case Attribute::Type::BOOL: return std::make_shared<Literal>(ts, value_attribute->get_as_bool());
    case Attribute::Type::DOUBLE: return std::make_shared<Literal>(ts, value_attribute->get_as_double());
    case Attribute::Type::FLOAT: return std::make_shared<Literal>(ts, value_attribute->get_as_float());
    case Attribute::Type::INTEGRAL: return std::make_shared<Literal>(ts, value_attribute->get_as_integral());
    case Attribute::Type::STRING: return std::make_shared<Literal>(ts, value_attribute->get_as_string());
    }

    return Error(node.get_source_location().to_string() + "Cannot create literal of type " + value_attribute->to_string());
}

static Result<std::shared_ptr<const IInitializer>> create_literal_initializer(Type_System& ts, Node const& node)
{
    if (node.get_type() == Node::Type::INITIALIZER)
    {
        if (node.get_children().size() != 1)
        {
            return Error(node.get_source_location().to_string() + "Invalid intializer node!");
        }

        Node const* literal_node = node.find_first_child_by_type(Node::Type::LITERAL);
        if (literal_node)
        {
            auto result = create_literal(ts, *literal_node);
            if (result != success)
            {
                return result.error();
            }
            std::shared_ptr<ILiteral> literal = result.extract_payload();
            return std::make_shared<Literal_Initializer>(literal);
        }
    }

    return Error(node.get_source_location().to_string() + "Unknown initializer");
}

static Result<std::shared_ptr<const IInitializer>> create_identifier_initializer(Type_System& ts, IDeclaration_Scope const& scope,  Node const& node)
{
    if (node.get_type() == Node::Type::INITIALIZER)
    {
        if (node.get_children().size() != 1)
        {
            return Error(node.get_source_location().to_string() + "Invalid intializer node!");
        }

        Node const* identifier_node = node.find_first_child_by_type(Node::Type::IDENTIFIER);
        if (identifier_node)
        {
            Attribute const* value_attribute = identifier_node->find_first_attribute_by_name("value");
            if (!value_attribute)
            {
                return Error(identifier_node->get_source_location().to_string() + "Initializer without a value!");
            }

            if (value_attribute->get_type() != Attribute::Type::STRING)
            {
                return Error(identifier_node->get_source_location().to_string() + "Expected string identifier!");
            }

            Symbol_Path symbol_path(value_attribute->get_as_string());
            if (symbol_path.get_count() == 0)
            {
                return Error(identifier_node->get_source_location().to_string() + "Bad identifier!");
            }

            std::shared_ptr<const IEnum_Item> item = scope.find_specialized_symbol_by_path<const IEnum_Item>(symbol_path);
            if (!item)
            {
                return Error(identifier_node->get_source_location().to_string() + "Cannot find enum item '" + symbol_path.to_string() + "'");
            }
            return std::make_shared<Enum_Item_Initializer>(item);
        }
    }

    return Error(node.get_source_location().to_string() + "Unknown initializer");
}

static Result<std::shared_ptr<const IInitializer>> create_initializer(Type_System& ts, IDeclaration_Scope const& scope, Node const& node)
{
    auto result = create_literal_initializer(ts, node);
    if (result == success)
    {
        return result;
    }
    return create_identifier_initializer(ts, scope, node);
}

static Result<std::shared_ptr<const IInitializer_List>> create_initializer_list(Type_System& ts, IDeclaration_Scope const& scope, Node const& node)
{
    if (node.get_type() == Node::Type::INITIALIZER)
    {
        auto result = create_initializer(ts, scope, node);
        if (result != success)
        {
            return result.error();
        }
        return std::make_shared<Initializer_List>(std::vector<std::shared_ptr<const IInitializer>> { result.payload() });
    }
    else if (node.get_type() == Node::Type::INITIALIZER_LIST)
    {
        std::vector<std::shared_ptr<const IInitializer>> initializers;
        for (Node const& ch: node.get_children())
        {
            if (ch.get_type() == Node::Type::INITIALIZER)
            {
                auto result = create_initializer(ts, scope, ch);
                if (result != success)
                {
                    return result.error();
                }
                initializers.push_back(result.payload());
            }
            else if (ch.get_type() == Node::Type::INITIALIZER_LIST)
            {
                auto result = create_initializer_list(ts, scope, ch);
                if (result != success)
                {
                    return result.error();
                }
                initializers.push_back(result.payload());
            }
            else
            {
                return Error(node.get_source_location().to_string() + "Unknown initializer");
            }
        }

        return std::make_shared<Initializer_List>(std::move(initializers));
    }

    return Error(node.get_source_location().to_string() + "Unknown initializer");
}


static Result<void> create_type_attributes(Type_System& ts, IType& type, Node const& node)
{
    std::vector<Node> attribute_nodes = node.get_all_children_of_type(Node::Type::ATTRIBUTE);
    for (Node const& attribute_node: attribute_nodes)
    {
        auto name_result = get_name_identifier(attribute_node);
        if (name_result != success)
        {
            return name_result.error();
        }

        std::string attribute_name = name_result.payload();

        std::shared_ptr<const IInitializer_List> initializer_list;
        Node const* initializer_node = attribute_node.find_first_child_by_type(Node::Type::INITIALIZER);
        if (!initializer_node)
        {
            initializer_node = attribute_node.find_first_child_by_type(Node::Type::INITIALIZER_LIST);
        }
        if (initializer_node)
        {
            auto result = create_initializer_list(ts, ts, *initializer_node);
            if (result != success)
            {
                return Error(attribute_node.get_source_location().to_string() + "Cannot create initializer: " + result.error().what());
            }
            initializer_list = result.extract_payload();
        }
        if (!initializer_list)
        {
            return Error(attribute_node.get_source_location().to_string() + "Missing initializer for attribute");
        }

        std::shared_ptr<IAttribute> attribute;

        if (attribute_name == "min")
        {
            std::shared_ptr<IValue> value = type.create_value();
            auto result = value->construct(*initializer_list);
            if (result != success)
            {
                return Error(attribute_node.get_source_location().to_string() + "Cannot initialize attribute: " + result.error().what());
            }

            attribute = std::make_shared<Min_Attribute>(std::move(value));
        }
        else if (attribute_name == "max")
        {
            std::shared_ptr<IValue> value = type.create_value();
            auto result = value->construct(*initializer_list);
            if (result != success)
            {
                return Error(attribute_node.get_source_location().to_string() + "Cannot initialize attribute: " + result.error().what());
            }

            attribute = std::make_shared<Max_Attribute>(std::move(value));
        }
        else if (attribute_name == "decimals")
        {
            std::shared_ptr<IInt_Value> value = ts.find_specialized_symbol_by_name<IInt_Type>("int")->create_specialized_value();
            auto result = value->construct(*initializer_list);
            if (result != success)
            {
                return Error(attribute_node.get_source_location().to_string() + "Cannot initialize attribute: " + result.error().what());
            }

            attribute = std::make_shared<Decimals_Attribute>(value->get_value());
        }
        else if (attribute_name == "ui_name")
        {
            std::shared_ptr<IString_Value> value = ts.find_specialized_symbol_by_name<IString_Type>("string")->create_specialized_value();
            auto result = value->construct(*initializer_list);
            if (result != success)
            {
                return Error(attribute_node.get_source_location().to_string() + "Cannot initialize attribute: " + result.error().what());
            }

            attribute = std::make_shared<UI_Name_Attribute>(value->get_value());
        }
        else if (attribute_name == "native_type")
        {
            std::shared_ptr<IString_Value> value = ts.find_specialized_symbol_by_name<IString_Type>("string")->create_specialized_value();
            auto result = value->construct(*initializer_list);
            if (result != success)
            {
                return Error(attribute_node.get_source_location().to_string() + "Cannot initialize attribute: " + result.error().what());
            }

            attribute = std::make_shared<Native_Type_Attribute>(value->get_value());
        }
        else
        {
            return Error(attribute_node.get_source_location().to_string() + "Unknown attribute " + attribute_name);
        }

        auto add_result = type.add_attribute(std::move(attribute));
        if (add_result != success)
        {
            return Error(attribute_node.get_source_location().to_string() + "Bad attribute: " + add_result.error().what());
        }
    }

    return success;
}

static Result<void> create_member_def_attributes(Type_System& ts, IMember_Def& member_def, Node const& node)
{
    std::vector<Node> attribute_nodes = node.get_all_children_of_type(Node::Type::ATTRIBUTE);
    for (Node const& attribute_node: attribute_nodes)
    {
        auto name_result = get_name_identifier(attribute_node);
        if (name_result != success)
        {
            return name_result.error();
        }

        std::string attribute_name = name_result.payload();

        std::shared_ptr<const IInitializer_List> initializer_list;
        Node const* initializer_node = attribute_node.find_first_child_by_type(Node::Type::INITIALIZER);
        if (!initializer_node)
        {
            initializer_node = attribute_node.find_first_child_by_type(Node::Type::INITIALIZER_LIST);
        }
        if (initializer_node)
        {
            auto result = create_initializer_list(ts, ts, *initializer_node);
            if (result != success)
            {
                return Error(attribute_node.get_source_location().to_string() + "Cannot create initializer: " + result.error().what());
            }
            initializer_list = result.extract_payload();
        }
        if (!initializer_list)
        {
            return Error(attribute_node.get_source_location().to_string() + "Missing initializer for attribute");
        }

        std::shared_ptr<IAttribute> attribute;

        if (attribute_name == "ui_name")
        {
            std::shared_ptr<IString_Value> value = ts.find_specialized_symbol_by_name<IString_Type>("string")->create_specialized_value();
            auto result = value->construct(*initializer_list);
            if (result != success)
            {
                return Error(attribute_node.get_source_location().to_string() + "Cannot initialize attribute: " + result.error().what());
            }

            attribute = std::make_shared<UI_Name_Attribute>(value->get_value());
        }
        else
        {
            return Error(attribute_node.get_source_location().to_string() + "Unknown attribute " + attribute_name);
        }

        auto add_result = member_def.add_attribute(std::move(attribute));
        if (add_result != success)
        {
            return Error(attribute_node.get_source_location().to_string() + "Bad attribute: " + add_result.error().what());
        }
    }

    return success;
}

static Result<void> create_enum_item_attributes(Type_System& ts, IEnum_Item& enum_item, Node const& node)
{
    std::vector<Node> attribute_nodes = node.get_all_children_of_type(Node::Type::ATTRIBUTE);
    for (Node const& attribute_node: attribute_nodes)
    {
        auto name_result = get_name_identifier(attribute_node);
        if (name_result != success)
        {
            return name_result.error();
        }

        std::string attribute_name = name_result.payload();

        std::shared_ptr<const IInitializer_List> initializer_list;
        Node const* initializer_node = attribute_node.find_first_child_by_type(Node::Type::INITIALIZER);
        if (!initializer_node)
        {
            initializer_node = attribute_node.find_first_child_by_type(Node::Type::INITIALIZER_LIST);
        }
        if (initializer_node)
        {
            auto result = create_initializer_list(ts, ts, *initializer_node);
            if (result != success)
            {
                return Error(attribute_node.get_source_location().to_string() + "Cannot create initializer: " + result.error().what());
            }
            initializer_list = result.extract_payload();
        }

        std::shared_ptr<IAttribute> attribute;

        if (attribute_name == "default")
        {
            if (initializer_list)
            {
                return Error(attribute_node.get_source_location().to_string() + "Initializer for default attribute not supported");
            }

            attribute = std::make_shared<Default_Attribute>();
        }
        else if (attribute_name == "ui_name")
        {
            if (!initializer_list)
            {
                return Error(attribute_node.get_source_location().to_string() + "Missing initializer for attribute");
            }

            std::shared_ptr<IString_Value> value = ts.find_specialized_symbol_by_name<IString_Type>("string")->create_specialized_value();
            auto result = value->construct(*initializer_list);
            if (result != success)
            {
                return Error(attribute_node.get_source_location().to_string() + "Cannot initialize attribute: " + result.error().what());
            }

            attribute = std::make_shared<UI_Name_Attribute>(value->get_value());
        }
        else
        {
            return Error(attribute_node.get_source_location().to_string() + "Unknown attribute " + attribute_name);
        }

        auto add_result = enum_item.add_attribute(std::move(attribute));
        if (add_result != success)
        {
            return Error(attribute_node.get_source_location().to_string() + "Bad attribute: " + add_result.error().what());
        }
    }

    return success;
}


static Result<std::shared_ptr<IMember_Def>> create_member_def(Type_System& ts, IDeclaration_Scope& scope, Node const& node)
{
    TS_ASSERT(node.get_type() == Node::Type::MEMBER_DECLARATION);

    auto name_result = get_name_identifier(node);
    if (name_result != success)
    {
        return name_result.error();
    }
    std::string name = name_result.payload();

    auto type_result = find_type_or_instantiate_templated_type(ts, scope, node);
    if (type_result != success)
    {
        return Error(node.get_source_location().to_string() + "Cannot find member type: " + type_result.error().what());
    }

    std::shared_ptr<const IType> type = type_result.extract_payload();

    std::shared_ptr<IValue> value = type->create_value();

    Node const* initializer_node = node.find_first_child_by_type(Node::Type::INITIALIZER);
    if (!initializer_node)
    {
        initializer_node = node.find_first_child_by_type(Node::Type::INITIALIZER_LIST);
    }
    if (initializer_node)
    {
        auto initializer_result = create_initializer_list(ts, scope, *initializer_node);
        if (initializer_result != success)
        {
            return Error(initializer_node->get_source_location().to_string() + "Cannot create member initializer: " + initializer_result.error().what());
        }

        std::shared_ptr<const IInitializer_List> initializer_list = initializer_result.payload();
        auto result = value->construct(*initializer_list);
        if (result != success)
        {
            return Error(initializer_node->get_source_location().to_string() + "Cannot initialize value: " + result.error().what());
        }
    }
    else
    {
        auto result = value->construct();
        if (result != success)
        {
            return Error(node.get_source_location().to_string() + "Cannot initialize value: " + result.error().what());
        }
    }

    std::shared_ptr<IMember_Def> def = std::make_shared<Member_Def>(name, type, std::move(value));

    auto create_result = create_member_def_attributes(ts, *def, node);
    if (create_result != success)
    {
        return create_result.error();
    }

    return std::move(def);
}

static Result<void> create_alias(Type_System& ts, IDeclaration_Scope& scope, Node const& node)
{
    TS_ASSERT(node.get_type() == Node::Type::ALIAS_DECLARATION);

    auto name_result = get_name_identifier(node);
    if (name_result != success)
    {
        return name_result.error();
    }
    std::string name = name_result.payload();

    auto type_result = find_type_or_instantiate_templated_type(ts, scope, node);
    if (type_result != success)
    {
        return Error(node.get_source_location().to_string() + "Cannot find member type: " + type_result.error().what());
    }

    std::shared_ptr<const IType> type = type_result.extract_payload();

    std::shared_ptr<IType> aliased_type = type->clone(name);

    auto create_result = create_type_attributes(ts, *aliased_type, node);
    if (create_result != success)
    {
        return Error(node.get_source_location().to_string() + "Cannot create alias attributes: " + create_result.error().what());
    }

    auto add_result = scope.add_symbol(std::move(aliased_type));
    if (add_result != success)
    {
        return Error(node.get_source_location().to_string() + "Cannot add alias: " + add_result.error().what());
    }

    return success;
}

static Result<void> create_struct_type(Type_System& ts, IDeclaration_Scope& scope, Node const& node)
{
    TS_ASSERT(node.get_type() == Node::Type::STRUCT_DECLARATION);

    auto name_result = get_name_identifier(node);
    if (name_result != success)
    {
        return name_result.error();
    }
    std::string name = name_result.payload();

    std::shared_ptr<const IStruct_Type> parent;

    Node const* inheritance_node = node.find_first_child_by_type(Node::Type::INHERITANCE);
    if (inheritance_node)
    {
        auto type_result = find_type_or_instantiate_templated_type(ts, scope, *inheritance_node);
        if (type_result != success)
        {
            return Error(node.get_source_location().to_string() + "Cannot find parent type: " + type_result.error().what());
        }

        parent = std::dynamic_pointer_cast<const IStruct_Type>(type_result.payload());
        if (!parent)
        {
            return Error(node.get_source_location().to_string() + "Wrong parent type. Expecting struct, got " + type_result.payload()->get_symbol_path().to_string());
        }
    }

    Struct_Type* type = new Struct_Type(name, parent);

    //add it to the typesystem so we can search for types
    auto result = scope.add_symbol(std::shared_ptr<ISymbol>(type));
    if (result != success)
    {
        return Error(node.get_source_location().to_string() + result.error().what());
    }

    Node const* body = node.find_first_child_by_type(Node::Type::STRUCT_BODY);
    if (body)
    {
        for (Node const& ch: body->get_children())
        {
            if (ch.get_type() == Node::Type::STRUCT_DECLARATION)
            {
                auto result = create_struct_type(ts, *type, ch);
                if (result != success)
                {
                    return result.error();
                }
            }
            else if (ch.get_type() == Node::Type::ENUM_DECLARATION)
            {
                auto result = create_enum_type(ts, *type, ch);
                if (result != success)
                {
                    return result.error();
                }
            }
            else if (ch.get_type() == Node::Type::ALIAS_DECLARATION)
            {
                auto result = create_alias(ts, *type, ch);
                if (result != success)
                {
                    return result.error();
                }
            }
            else if (ch.get_type() == Node::Type::MEMBER_DECLARATION)
            {
                auto result = create_member_def(ts, *type, ch);
                if (result != success)
                {
                    return result.error();
                }

                std::shared_ptr<IMember_Def> t = result.extract_payload();
                auto add_result = type->add_member_def(std::move(t));
                if (add_result != success)
                {
                    return Error(body->get_source_location().to_string() + "Cannot add member: " + add_result.error().what());
                }
            }
            else
            {
                return Error(body->get_source_location().to_string() + "Illegal node type in struct");
            }
        }
    }

    return success;
}

static Result<void> create_enum_item(Type_System& ts, IEnum_Type& type, Node const& node, int64_t& io_integral_value)
{
    TS_ASSERT(node.get_type() == Node::Type::IDENTIFIER);

    Attribute const* identifier_attribute = node.find_first_attribute_by_name("value");
    if (!identifier_attribute)
    {
        return Error("Cannot find 'value'' attribute for identifier node");
    }
    if (identifier_attribute->get_type() != Attribute::Type::STRING)
    {
        return Error("Wrong type for 'value' attribute of 'identifier' node. Expected string.");
    }
    std::string name = identifier_attribute->get_as_string();

    Attribute const* integral_attribute = node.find_first_attribute_by_name("integral_value");
    if (integral_attribute)
    {
        if (integral_attribute->get_type() != Attribute::Type::INTEGRAL)
        {
            return Error("The value of an enum item can only be an integral.");
        }
        io_integral_value = integral_attribute->get_as_integral();
    }

    std::shared_ptr<Enum_Item> item = std::make_shared<Enum_Item>(name, io_integral_value);
    auto create_result = create_enum_item_attributes(ts, *item, node);
    if (create_result != success)
    {
        return create_result.error();
    }
    auto result = type.add_symbol(item);
    if (result != success)
    {
        return Error(node.get_source_location().to_string() + result.error().what());
    }

    return success;
}

static Result<void> create_enum_type(Type_System& ts, IDeclaration_Scope& scope, Node const& node)
{
    TS_ASSERT(node.get_type() == Node::Type::ENUM_DECLARATION);

    auto name_result = get_name_identifier(node);
    if (name_result != success)
    {
        return name_result.error();
    }
    std::string name = name_result.payload();

    Enum_Type* type = new Enum_Type(name);

    //add it to the typesystem so we can search for types
    auto result = scope.add_symbol(std::shared_ptr<ISymbol>(type));
    if (result != success)
    {
        return Error(node.get_source_location().to_string() + result.error().what());
    }

    int64_t integral_value = 0;

    Node const* body = node.find_first_child_by_type(Node::Type::ENUM_BODY);
    if (body)
    {
        for (Node const& ch: body->get_children())
        {
            if (ch.get_type() == Node::Type::IDENTIFIER)
            {
                auto result = create_enum_item(ts, *type, ch, integral_value);
                if (result != success)
                {
                    return result.error();
                }
            }
            else
            {
                return Error(body->get_source_location().to_string() + "Illegal node type in enum");
            }

            integral_value++;
        }
    }

    return success;
}

static Result<void> create_symbol(Type_System& ts, IDeclaration_Scope& scope, Node const& node)
{
    if (node.get_type() == Node::Type::NAMESPACE_DECLARATION)
    {
        return create_namespace(ts, scope, node);
    }
    else if (node.get_type() == Node::Type::STRUCT_DECLARATION)
    {
        return create_struct_type(ts, scope, node);
    }
    else if (node.get_type() == Node::Type::ENUM_DECLARATION)
    {
        return create_enum_type(ts, scope, node);
    }
    else if (node.get_type() == Node::Type::ALIAS_DECLARATION)
    {
        return create_alias(ts, scope, node);
    }

    return Error("Invalid declaration");
}

static Result<void> populate_declaration_scope(Type_System& ts, IDeclaration_Scope& scope, Node const& node)
{
    return create_symbol(ts, scope, node);
}


Result<void> Builder::compile(Type_System& ts)
{
    if (m_root_node.get_children().empty())
    {
        return success;
    }

    for (Node const& ch: m_root_node.get_children())
    {
        auto result = populate_declaration_scope(ts, ts, ch);
        if (result != success)
        {
            return result;
        }
    }

    return success;
}

}
}

