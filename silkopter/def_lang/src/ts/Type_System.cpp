#include "Type_System.h"

#include "Namespace.h"
#include "types/All_Numeric_Types.h"
#include "values/All_Numeric_Values.h"
#include "types/Bool_Type.h"
#include "types/String_Type.h"
#include "ITemplate_Argument.h"
#include "types/Vector_Type.h"

namespace ts
{

void Type_System::populate_builtin_types()
{
    auto result = add_symbol(std::unique_ptr<IType>(new Bool_Type("bool")));
    TS_ASSERT(result == success);
    result = add_symbol(std::unique_ptr<IType>(new String_Type("string")));
    TS_ASSERT(result == success);

    result = add_symbol(std::unique_ptr<IType>(new Int8_Type("int8_t")));
    TS_ASSERT(result == success);
    result = add_symbol(std::unique_ptr<IType>(new UInt8_Type("uint8_t")));
    TS_ASSERT(result == success);
    result = add_symbol(std::unique_ptr<IType>(new Int16_Type("int16_t")));
    TS_ASSERT(result == success);
    result = add_symbol(std::unique_ptr<IType>(new UInt16_Type("uint16_t")));
    TS_ASSERT(result == success);
    result = add_symbol(std::unique_ptr<IType>(new Int32_Type("int32_t")));
    TS_ASSERT(result == success);
    result = add_symbol(std::unique_ptr<IType>(new UInt32_Type("uint32_t")));
    TS_ASSERT(result == success);
    result = add_symbol(std::unique_ptr<IType>(new Int64_Type("int64_t")));
    TS_ASSERT(result == success);

    result = add_symbol(std::unique_ptr<IType>(new Float_Type("float")));
    TS_ASSERT(result == success);
    result = add_symbol(std::unique_ptr<IType>(new Double_Type("double")));
    TS_ASSERT(result == success);

    result = add_symbol(std::unique_ptr<IType>(new Float_Type("ufloat")));
    TS_ASSERT(result == success);
    result = add_symbol(std::unique_ptr<IType>(new Double_Type("udouble")));
    TS_ASSERT(result == success);

    {
        std::unique_ptr<Namespace> math_namespace = std::unique_ptr<Namespace>(new Namespace("math"));

        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2f_Type("vec2f")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2d_Type("vec2d")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2u8_Type("vec2u8")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2s8_Type("vec2s8")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2u16_Type("vec2u16")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2s16_Type("vec2s16")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2u32_Type("vec2u32")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2s32_Type("vec2s32")));
        TS_ASSERT(result == success);

        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3f_Type("vec3f")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3d_Type("vec3d")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3u8_Type("vec3u8")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3s8_Type("vec3s8")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3u16_Type("vec3u16")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3s16_Type("vec3s16")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3u32_Type("vec3u32")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3s32_Type("vec3s32")));
        TS_ASSERT(result == success);

        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4f_Type("vec4f")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4d_Type("vec4d")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4u8_Type("vec4u8")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4s8_Type("vec4s8")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4u16_Type("vec4u16")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4s16_Type("vec4s16")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4u32_Type("vec4u32")));
        TS_ASSERT(result == success);
        result = math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4s32_Type("vec4s32")));
        TS_ASSERT(result == success);

        result = add_symbol(std::move(math_namespace));
        TS_ASSERT(result == success);
    }
}


auto Type_System::instantiate_template(std::string const& name, std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments) -> Result<std::shared_ptr<const ITemplated_Type>>
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
        std::unique_ptr<ts::Vector_Type> vector = std::unique_ptr<ts::Vector_Type>(new ts::Vector_Type(instanced_name));
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

    return nullptr;
}





}

