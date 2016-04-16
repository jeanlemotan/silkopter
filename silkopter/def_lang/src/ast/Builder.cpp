#include "Builder.h"
#include "Lexer.h"

#include <boost/optional.hpp>

#include "IValue.h"
#include "IInitializer.h"
#include "IExpression.h"
#include "ILiteral.h"
#include "IAttribute_Container.h"
#include "ITemplated_Type.h"
#include "IBool_Type.h"
#include "IBool_Value.h"

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
#include "impl/All_INumeric_Types.h"
#include "impl/String_Type.h"
#include "impl/All_INumeric_Values.h"
#include "impl/String_Value.h"


std::ostream& operator<<(std::ostream& os, ts::Source_Location const& location)
{
    os << location.get_file_path() << ":" << location.get_line() << ":" << location.get_column() << ": ";
    return os;
}


namespace ast
{

Builder::Builder()
    : m_root_node(Node::Type::ROOT, ts::Source_Location())
{
    m_lexer.reset(new Lexer(*this));
}

Builder::~Builder()
{
}

auto Builder::get_location() const -> ts::Source_Location
{
    yy::location const& loc = m_lexer->get_location();
    return ts::Source_Location(get_filename(), loc.begin.line, loc.begin.column);
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

    Import import;
    import.stream = std::move(fs);
    import.filename = filename;
    m_imports.push_back(std::move(import));

    m_lexer->yypush_buffer_state(buffer);

    return true;
}

auto Builder::end_file() -> bool
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
    return m_imports.empty() ? std::string() : m_imports.back().filename;
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
        std::cerr << node.get_source_location() << "Malformed type node ast\n";
        return nullptr;
    }
    boost::optional<std::string> type_name = get_name_identifier(*type_node);
    if (type_name)
    {
        std::shared_ptr<ts::IType> type = scope.find_specialized_symbol_by_path<ts::IType>(ts::Symbol_Path(*type_name));
        if (!type)
        {
            std::cerr << type_node->get_source_location() << "Cannot find type " << *type_name << "\n";
            return nullptr;
        }
        return type;
    }
    else
    {
        Node const* template_node = type_node->find_first_child_by_type(Node::Type::TEMPLATE_INSTANTIATION);
        if (!template_node)
        {
            std::cerr << type_node->get_source_location() << "Malformed type node ast\n";
            return nullptr;
        }

        type_name = get_name_identifier(*template_node);
        if (!type_name)
        {
            std::cerr << template_node->get_source_location() << "Missing templated type name\n";
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
                std::cerr << node.get_source_location() << "Cannot resolve template argument " << template_arguments.size() << "\n";
                return nullptr;
            }
            template_arguments.push_back(template_argument);
        }

        auto result = ts.instantiate_template(*type_name, template_arguments);
        if (result != ts::success)
        {
            std::cerr << type_node->get_source_location() << "Cannot instantiate template: " << result.error().what() << "\n";
            return nullptr;
        }
        return result.payload();
    }

    return nullptr;
}


static auto create_namespace(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node) -> bool
{
    TS_ASSERT(node.get_type() == Node::Type::NAMESPACE_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << node.get_source_location() << "Cannot find namespace name identifier";
        return false;
    }

    ts::Namespace* ns = new ts::Namespace(*name);

    //add it to the typesystem so we can search for types
    auto result = scope.add_symbol(std::unique_ptr<ts::ISymbol>(ns));
    if (result != ts::success)
    {
        std::cerr << node.get_source_location() << result.error().what();
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
        std::cerr << node.get_source_location() << "Literal without a value!\n";
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
                auto result = v->set_value(value_attribute->get_as_bool());
                if (result != ts::success)
                {
                    std::cerr << node.get_source_location() << "Cannot assign value:" + result.error().what() + "\n";
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
                auto result = v->set_value(value_attribute->get_as_double());
                if (result != ts::success)
                {
                    std::cerr << node.get_source_location() << "Cannot assign value:" + result.error().what() + "\n";
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
                auto result = v->set_value(value_attribute->get_as_float());
                if (result != ts::success)
                {
                    std::cerr << node.get_source_location() << "Cannot assign value:" + result.error().what() + "\n";
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
                auto result = v->set_value(value_attribute->get_as_integral());
                if (result != ts::success)
                {
                    std::cerr << node.get_source_location() << "Cannot assign value:" + result.error().what() + "\n";
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
                auto result = v->set_value(value_attribute->get_as_string());
                if (result != ts::success)
                {
                    std::cerr << node.get_source_location() << "Cannot assign value:" + result.error().what() + "\n";
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
        std::cerr << node.get_source_location() << "Cannot create literal of type " << value_attribute->to_string() << "\n";
        return nullptr;
    }

    return std::unique_ptr<ts::ILiteral>(new ts::Literal(std::move(value)));
}

static auto create_initializer(ts::Type_System& ts, Node const& node) -> std::unique_ptr<ts::IInitializer>
{
    if (node.get_type() == Node::Type::INITIALIZER)
    {
        if (node.get_children().size() != 1)
        {
            std::cerr << node.get_source_location() << "Invalid intializer node!\n";
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
            std::unique_ptr<ts::IInitializer> initializer_ch = create_initializer(ts, ch);
            if (!initializer_ch)
            {
                std::cerr << ch.get_source_location() << "Unknown initializer\n";
                return nullptr;
            }
            initializers.push_back(std::move(initializer_ch));
        }

        return std::unique_ptr<ts::IInitializer>(new ts::Initializer_List(std::move(initializers)));
    }
    else
    {
        std::cerr << node.get_source_location() << "Unknown initializer\n";
        return nullptr;
    }

    return nullptr;
}

static auto create_attributes(ts::Type_System& ts, ts::IType const& type, ts::IAttribute_Container& container, Node const& node) -> bool
{
    std::vector<Node> attribute_nodes = node.get_all_children_of_type(Node::Type::ATTRIBUTE);
    for (Node const& attribute_node: attribute_nodes)
    {
        boost::optional<std::string> name = get_name_identifier(attribute_node);
        if (!name)
        {
            std::cerr << attribute_node.get_source_location() << "Missing attribute identifier\n";
            return false;
        }

        std::unique_ptr<ts::IInitializer> initializer;
        Node const* initializer_node = attribute_node.find_first_child_by_type(Node::Type::INITIALIZER);
        if (!initializer_node)
        {
            initializer_node = attribute_node.find_first_child_by_type(Node::Type::INITIALIZER_LIST);
        }
        if (initializer_node)
        {
            initializer = create_initializer(ts, *initializer_node);
            if (!initializer)
            {
                return false;
            }
        }

        std::unique_ptr<ts::IAttribute> attribute;

        if (*name == "min")
        {
            if (!initializer)
            {
                std::cerr << attribute_node.get_source_location() << "Missing initializer for attribute\n";
                return false;
            }

            std::unique_ptr<ts::IValue> value = type.create_value();
            auto result = value->copy_assign(*initializer);
            if (result != ts::success)
            {
                std::cerr << attribute_node.get_source_location() << "Cannot initialize attribute: " + result.error().what() + "\n";
                return false;
            }

            attribute = std::unique_ptr<ts::Min_Attribute>(new ts::Min_Attribute(std::move(value)));
        }
        else if (*name == "max")
        {
            if (!initializer)
            {
                std::cerr << attribute_node.get_source_location() << "Missing initializer for attribute\n";
                return false;
            }

            std::unique_ptr<ts::IValue> value = type.create_value();
            auto result = value->copy_assign(*initializer);
            if (result != ts::success)
            {
                std::cerr << attribute_node.get_source_location() << "Cannot initialize attribute: " + result.error().what() + "\n";
                return false;
            }

            attribute = std::unique_ptr<ts::Max_Attribute>(new ts::Max_Attribute(std::move(value)));
        }
        else if (*name == "decimals")
        {
            if (!initializer)
            {
                std::cerr << attribute_node.get_source_location() << "Missing initializer for attribute\n";
                return false;
            }

            std::unique_ptr<ts::IInt64_Value> value = ts.find_specialized_symbol_by_name<ts::IInt64_Type>("int64_t")->create_specialized_value();
            auto result = value->copy_assign(*initializer);
            if (result != ts::success)
            {
                std::cerr << attribute_node.get_source_location() << "Cannot initialize attribute: " + result.error().what() + "\n";
                return false;
            }

            attribute = std::unique_ptr<ts::Decimals_Attribute>(new ts::Decimals_Attribute(value->get_value()));
        }
        else if (*name == "ui_name")
        {
            std::unique_ptr<ts::IString_Value> value = ts.find_specialized_symbol_by_name<ts::IString_Type>("string")->create_specialized_value();
            auto result = value->copy_assign(*initializer);
            if (result != ts::success)
            {
                std::cerr << attribute_node.get_source_location() << "Cannot initialize attribute: " + result.error().what() + "\n";
                return false;
            }

            attribute = std::unique_ptr<ts::UI_Name_Attribute>(new ts::UI_Name_Attribute(value->get_value()));
        }
        else
        {
            std::cerr << attribute_node.get_source_location() << "Unknown attribute " << *name << "\n";
            return false;
        }

        auto add_result = container.add_attribute(std::move(attribute));
        if (add_result != ts::success)
        {
            std::cerr << attribute_node.get_source_location() << "Bad attribute: " + add_result.error().what() + "\n";
            return false;
        }
    }

    return true;
}


static auto create_member_def(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node) -> std::unique_ptr<ts::IMember_Def>
{
    TS_ASSERT(node.get_type() == Node::Type::MEMBER_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << node.get_source_location() << "Cannot find member name identifier\n";
        return nullptr;
    }

    std::shared_ptr<const ts::IType> type = find_type_or_instantiate_templated_type(ts, scope, node);
    if (!type)
    {
        std::cerr << node.get_source_location() << "Cannot find member type\n";
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
        std::unique_ptr<ts::IInitializer> initializer = create_initializer(ts, *initializer_node);
        if (!initializer)
        {
            return nullptr;
        }

        auto result = value->copy_assign(*initializer);
        if (result != ts::success)
        {
            std::cerr << initializer_node->get_source_location() << "Cannot initialize value: " + result.error().what() + "\n";
            return nullptr;
        }
    }
    else
    {
    }

    std::unique_ptr<ts::IMember_Def> def;
    def.reset(new ts::Member_Def(*name, *type, std::move(value)));

    if (!create_attributes(ts, *type, *def, node))
    {
        return false;
    }

    return std::move(def);
}

static auto create_alias(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node) -> bool
{
    TS_ASSERT(node.get_type() == Node::Type::ALIAS_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << node.get_source_location() << "Cannot find alias name identifier\n";
        return false;
    }

    std::shared_ptr<const ts::IType> type = find_type_or_instantiate_templated_type(ts, scope, node);
    if (!type)
    {
        std::cerr << node.get_source_location() << "Cannot find alias type\n";
        return false;
    }

    std::unique_ptr<ts::IType> aliased_type = type->clone(*name);

    if (!create_attributes(ts, *aliased_type, *aliased_type, node))
    {
        return false;
    }

    return scope.add_symbol(std::move(aliased_type)) == ts::success;
}

static auto create_struct_type(ts::Type_System& ts, ts::IDeclaration_Scope& scope, Node const& node) -> bool
{
    TS_ASSERT(node.get_type() == Node::Type::STRUCT_DECLARATION);

    boost::optional<std::string> name = get_name_identifier(node);
    if (!name)
    {
        std::cerr << node.get_source_location() << "Cannot find struct name identifier\n";
        return false;
    }

    ts::Struct_Type* type = new ts::Struct_Type(*name);

    //add it to the typesystem so we can search for types
    auto result = scope.add_symbol(std::unique_ptr<ts::ISymbol>(type));
    if (result != ts::success)
    {
        std::cerr << node.get_source_location() << result.error().what();
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
                std::unique_ptr<ts::IMember_Def> t = create_member_def(ts, *type, ch);
                if (!t)
                {
                    return false;
                }

                auto result = type->add_member_def(std::move(t));
                if (result != ts::success)
                {
                    std::cerr << body->get_source_location() << "Illegal node type in struct\n";
                    return false;
                }
            }
            else
            {
                std::cerr << body->get_source_location() << "Illegal node type in struct\n";
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
