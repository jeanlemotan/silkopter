#include "def_lang/Type_System.h"

#include "def_lang/impl/Namespace.h"
#include "def_lang/impl/All_Numeric_Types.h"
#include "def_lang/impl/All_Numeric_Values.h"
#include "def_lang/impl/Bool_Type.h"
#include "def_lang/impl/Bool_Value.h"
#include "def_lang/impl/String_Type.h"
#include "def_lang/impl/String_Value.h"
#include "def_lang/ITemplate_Argument.h"
#include "def_lang/impl/Vector_Type.h"
#include "def_lang/impl/Poly_Type.h"
#include "def_lang/impl/Variant_Type.h"
#include "def_lang/impl/Optional_Type.h"
#include "def_lang/impl/Literal_Attribute.h"

namespace ts
{

Type_System::Type_System()
    : m_root_scope(new Namespace(std::string()))
{

}

void Type_System::set_native_type(IType& type, std::string const& native_type)
{
    std::shared_ptr<IString_Value> value = create_specialized_value<IString_Value>("string");
    TS_ASSERT(value);
    auto result = value->construct();
    TS_ASSERT(result == success);
    result = value->set_value(native_type);
    TS_ASSERT(result == success);

    std::shared_ptr<ILiteral_Attribute> att = std::make_shared<Literal_Attribute>("native_type", value);
    result = type.add_attribute(att);
    TS_ASSERT(result == success);
}

void Type_System::populate_builtin_types()
{
    std::shared_ptr<IType> type = std::make_shared<String_Type>("string");
    auto sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);
    set_native_type(*type, "std::string");

    type = std::make_shared<Bool_Type>("bool");
    set_native_type(*type, "bool");
    sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Int_Type>("int");
    set_native_type(*type, "int64_t");
    sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Float_Type>("float");
    set_native_type(*type, "float");
    sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Double_Type>("double");
    set_native_type(*type, "double");
    sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Vec2f_Type>("vec2f");
    set_native_type(*type, "vec2f");
    sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Vec2d_Type>("vec2d");
    set_native_type(*type, "vec2d");
    sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Vec2i_Type>("vec2i");
    set_native_type(*type, "vec2i");
    sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Vec3f_Type>("vec3f");
    set_native_type(*type, "vec3f");
    sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Vec3d_Type>("vec3d");
    set_native_type(*type, "vec3d");
    sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Vec3i_Type>("vec3i");
    set_native_type(*type, "vec3i");
    sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Vec4f_Type>("vec4f");
    set_native_type(*type, "vec4f");
    sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Vec4d_Type>("vec4d");
    set_native_type(*type, "vec4d");
    sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Vec4i_Type>("vec4i");
    set_native_type(*type, "vec4i");
    sym_result = m_root_scope->add_symbol(type);
    TS_ASSERT(sym_result == success);
}


Result<std::shared_ptr<const ITemplated_Type>> Type_System::instantiate_template(std::string const& name, std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments)
{
    if (name.empty())
    {
        return Error("cannot instantiate an unnamed template");
    }
    std::string instanced_name = name;
    for (std::shared_ptr<const ITemplate_Argument> const& arg: arguments)
    {
        if (arg->get_template_instantiation_string().empty())
        {
            return Error("invalid template argument");
        }
        instanced_name += "#";
        instanced_name += arg->get_template_instantiation_string();
    }
    std::transform(instanced_name.begin(), instanced_name.end(), instanced_name.begin(), [](char c) { return c == ':' ? '_' : c; });

    std::shared_ptr<const ITemplated_Type> instance = m_root_scope->find_specialized_symbol_by_name<ITemplated_Type>(instanced_name);
    if (instance)
    {
        return instance;
    }

    std::shared_ptr<ts::ITemplated_Type> type;

    if (name == "vector")
    {
        type = std::make_shared<ts::Vector_Type>(instanced_name);
    }
    else if (name == "poly")
    {
        type = std::make_shared<ts::Poly_Type>(instanced_name);
    }
    else if (name == "variant")
    {
        type = std::make_shared<ts::Variant_Type>(instanced_name);
    }
    else if (name == "optional")
    {
        type = std::make_shared<ts::Optional_Type>(instanced_name);
    }
    else
    {
        return Error("Unknown template type '" + name + "'");
    }

    auto init_result = type->init(arguments);
    if (init_result != success)
    {
        return init_result.error();
    }
    auto add_result = m_root_scope->add_symbol(std::move(type));
    if (add_result != success)
    {
        return add_result.error();
    }
    return std::dynamic_pointer_cast<const ITemplated_Type>(add_result.payload());
}

std::shared_ptr<IValue> Type_System::create_value(Symbol_Path const& type_path) const
{
    std::shared_ptr<const ts::IType> type = m_root_scope->find_specialized_symbol_by_path<const ts::IType>(type_path);
    if (!type)
    {
        return nullptr;
    }
    return type->create_value();
}

std::shared_ptr<IDeclaration_Scope> Type_System::get_root_scope()
{
    return m_root_scope;
}
std::shared_ptr<const IDeclaration_Scope> Type_System::get_root_scope() const
{
    return m_root_scope;
}




}

