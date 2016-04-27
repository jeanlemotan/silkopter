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

namespace ts
{

Type_System::Type_System()
    : Declaration_Scope_EP(std::string())
{

}

void Type_System::populate_builtin_types()
{
    auto result = add_symbol(std::shared_ptr<IType>(new Bool_Type("bool")));
    TS_ASSERT(result == success);
    result = add_symbol(std::shared_ptr<IType>(new String_Type("string")));
    TS_ASSERT(result == success);

    result = add_symbol(std::shared_ptr<IType>(new Int_Type("int")));
    TS_ASSERT(result == success);

    result = add_symbol(std::shared_ptr<IType>(new Float_Type("float")));
    TS_ASSERT(result == success);
    result = add_symbol(std::shared_ptr<IType>(new Double_Type("double")));
    TS_ASSERT(result == success);

    {
        std::shared_ptr<Namespace> math_namespace = std::shared_ptr<Namespace>(new Namespace("math"));

        result = math_namespace->add_symbol(std::shared_ptr<IType>(new Vec2f_Type("vec2f")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::shared_ptr<IType>(new Vec2d_Type("vec2d")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::shared_ptr<IType>(new Vec2i_Type("vec2i")));
        TS_ASSERT(result == success);

        result = math_namespace->add_symbol(std::shared_ptr<IType>(new Vec3f_Type("vec3f")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::shared_ptr<IType>(new Vec3d_Type("vec3d")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::shared_ptr<IType>(new Vec3i_Type("vec3i")));
        TS_ASSERT(result == success);

        result = math_namespace->add_symbol(std::shared_ptr<IType>(new Vec4f_Type("vec4f")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::shared_ptr<IType>(new Vec4d_Type("vec4d")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::shared_ptr<IType>(new Vec4i_Type("vec4i")));
        TS_ASSERT(result == success);

        result = add_symbol(std::move(math_namespace));
        TS_ASSERT(result == success);
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

    std::shared_ptr<const ITemplated_Type> instance = find_specialized_symbol_by_name<ITemplated_Type>(instanced_name);
    if (instance)
    {
        return instance;
    }

    if (name == "vector")
    {
        std::shared_ptr<ts::Vector_Type> vector = std::shared_ptr<ts::Vector_Type>(new ts::Vector_Type(instanced_name));
        auto init_result = vector->init(arguments);
        if (init_result != success)
        {
            return init_result.error();
        }
        auto add_result = add_symbol(std::move(vector));
        if (add_result != success)
        {
            return add_result.error();
        }
        return std::dynamic_pointer_cast<const ITemplated_Type>(add_result.payload());
    }
    else if (name == "poly")
    {
        std::shared_ptr<ts::Poly_Type> poly = std::shared_ptr<ts::Poly_Type>(new ts::Poly_Type(instanced_name));
        auto init_result = poly->init(arguments);
        if (init_result != success)
        {
            return init_result.error();
        }
        auto add_result = add_symbol(std::move(poly));
        if (add_result != success)
        {
            return add_result.error();
        }
        return std::dynamic_pointer_cast<const ITemplated_Type>(add_result.payload());
    }

    return Error("Unknown template type '" + name + "'");
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

