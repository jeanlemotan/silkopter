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

namespace ts
{

Type_System::Type_System()
    : Declaration_Scope_EP(std::string())
{

}

Result<void> Type_System::validate_symbol(std::shared_ptr<const ISymbol> symbol)
{
    return ts::success;
}

void Type_System::populate_builtin_types()
{
    std::shared_ptr<IType> type = std::make_shared<Bool_Type>("bool");
    auto att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("bool"));
    TS_ASSERT(att_result == success);
    auto sym_result = add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<String_Type>("string");
    att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("std::string"));
    TS_ASSERT(att_result == success);
    sym_result = add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Int_Type>("int");
    att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("int64_t"));
    TS_ASSERT(att_result == success);
    sym_result = add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Float_Type>("float");
    att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("float"));
    TS_ASSERT(att_result == success);
    sym_result = add_symbol(type);
    TS_ASSERT(sym_result == success);

    type = std::make_shared<Double_Type>("double");
    att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("double"));
    TS_ASSERT(att_result == success);
    sym_result = add_symbol(type);
    TS_ASSERT(sym_result == success);

    {
        std::shared_ptr<Namespace> math_namespace = std::shared_ptr<Namespace>(new Namespace("math"));

        type = std::make_shared<Vec2f_Type>("vec2f");
        att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("math::vec2<float>"));
        TS_ASSERT(att_result == success);
        sym_result = math_namespace->add_symbol(type);
        TS_ASSERT(sym_result == success);

        type = std::make_shared<Vec2d_Type>("vec2d");
        att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("math::vec2<double>"));
        TS_ASSERT(att_result == success);
        sym_result = math_namespace->add_symbol(type);
        TS_ASSERT(sym_result == success);

        type = std::make_shared<Vec2i_Type>("vec2i");
        att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("math::vec2<int64_t>"));
        TS_ASSERT(att_result == success);
        sym_result = math_namespace->add_symbol(type);
        TS_ASSERT(sym_result == success);

        type = std::make_shared<Vec3f_Type>("vec3f");
        att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("math::vec3<float>"));
        TS_ASSERT(att_result == success);
        sym_result = math_namespace->add_symbol(type);
        TS_ASSERT(sym_result == success);

        type = std::make_shared<Vec3d_Type>("vec3d");
        att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("math::vec3<double>"));
        TS_ASSERT(att_result == success);
        sym_result = math_namespace->add_symbol(type);
        TS_ASSERT(sym_result == success);

        type = std::make_shared<Vec3i_Type>("vec3i");
        att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("math::vec3<int64_t>"));
        TS_ASSERT(att_result == success);
        sym_result = math_namespace->add_symbol(type);
        TS_ASSERT(sym_result == success);

        type = std::make_shared<Vec4f_Type>("vec4f");
        att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("math::vec4<float>"));
        TS_ASSERT(att_result == success);
        sym_result = math_namespace->add_symbol(type);
        TS_ASSERT(sym_result == success);

        type = std::make_shared<Vec4d_Type>("vec4d");
        att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("math::vec4<double>"));
        TS_ASSERT(att_result == success);
        sym_result = math_namespace->add_symbol(type);
        TS_ASSERT(sym_result == success);

        type = std::make_shared<Vec4i_Type>("vec4i");
        att_result = type->add_attribute(std::make_shared<Native_Type_Attribute>("math::vec4<int64_t>"));
        TS_ASSERT(att_result == success);
        sym_result = math_namespace->add_symbol(type);
        TS_ASSERT(sym_result == success);

        sym_result = add_symbol(std::move(math_namespace));
        TS_ASSERT(sym_result == success);
    }
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

    std::shared_ptr<const ITemplated_Type> instance = find_specialized_symbol_by_name<ITemplated_Type>(instanced_name);
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
    auto add_result = add_symbol(std::move(type));
    if (add_result != success)
    {
        return add_result.error();
    }
    return std::dynamic_pointer_cast<const ITemplated_Type>(add_result.payload());
}

std::shared_ptr<IValue> Type_System::create_value(Symbol_Path const& type_path) const
{
    std::shared_ptr<const ts::IType> type = find_specialized_symbol_by_path<const ts::IType>(type_path);
    if (!type)
    {
        return nullptr;
    }
    return type->create_value();
}





}

