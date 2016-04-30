#include "def_lang/impl/Literal.h"
#include "def_lang/IValue.h"
#include "def_lang/IDeclaration_Scope.h"
#include "def_lang/impl/Initializer_List.h"

#include "def_lang/All_INumeric_Types.h"
#include "def_lang/All_INumeric_Values.h"
#include "def_lang/IBool_Type.h"
#include "def_lang/IBool_Value.h"
#include "def_lang/IString_Type.h"
#include "def_lang/IString_Value.h"

namespace ts
{

Literal::Literal(IDeclaration_Scope const& scope, bool value)
{
    std::shared_ptr<const ts::IBool_Type> type = scope.find_specialized_symbol_by_path<const ts::IBool_Type>("::bool");
    TS_ASSERT(type);
    if (!type)
    {
        return;
    }

    std::shared_ptr<ts::IBool_Value> v = type->create_specialized_value();
    auto result = v->construct(ts::Initializer_List({}));
    if (result != success)
    {
        TS_ASSERT(false);
        return;
    }
    result = v->set_value(value);
    if (result != success)
    {
        TS_ASSERT(false);
        return;
    }

    m_value = v;
}

Literal::Literal(IDeclaration_Scope const& scope, int64_t value)
{
    std::shared_ptr<const ts::IInt_Type> type = scope.find_specialized_symbol_by_path<const ts::IInt_Type>("::int");
    TS_ASSERT(type);
    if (!type)
    {
        return;
    }

    std::shared_ptr<ts::IInt_Value> v = type->create_specialized_value();
    auto result = v->construct(ts::Initializer_List({}));
    if (result != success)
    {
        TS_ASSERT(false);
        return;
    }
    result = v->set_value(value);
    if (result != success)
    {
        TS_ASSERT(false);
        return;
    }

    m_value = v;
}

Literal::Literal(IDeclaration_Scope const& scope, float value)
{
    std::shared_ptr<const ts::IFloat_Type> type = scope.find_specialized_symbol_by_path<const ts::IFloat_Type>("::float");
    TS_ASSERT(type);
    if (!type)
    {
        return;
    }

    std::shared_ptr<ts::IFloat_Value> v = type->create_specialized_value();
    auto result = v->construct(ts::Initializer_List({}));
    if (result != success)
    {
        TS_ASSERT(false);
        return;
    }
    result = v->set_value(value);
    if (result != success)
    {
        TS_ASSERT(false);
        return;
    }

    m_value = v;
}

Literal::Literal(IDeclaration_Scope const& scope, double value)
{
    std::shared_ptr<const ts::IDouble_Type> type = scope.find_specialized_symbol_by_path<const ts::IDouble_Type>("::double");
    TS_ASSERT(type);
    if (!type)
    {
        return;
    }

    std::shared_ptr<ts::IDouble_Value> v = type->create_specialized_value();
    auto result = v->construct(ts::Initializer_List({}));
    if (result != success)
    {
        TS_ASSERT(false);
        return;
    }
    result = v->set_value(value);
    if (result != success)
    {
        TS_ASSERT(false);
        return;
    }

    m_value = v;
}

Literal::Literal(IDeclaration_Scope const& scope, std::string const& value)
{
    std::shared_ptr<const ts::IString_Type> type = scope.find_specialized_symbol_by_path<const ts::IString_Type>("::string");
    TS_ASSERT(type);
    if (!type)
    {
        return;
    }

    std::shared_ptr<ts::IString_Value> v = type->create_specialized_value();
    auto result = v->construct(ts::Initializer_List({}));
    if (result != success)
    {
        TS_ASSERT(false);
        return;
    }
    result = v->set_value(value);
    if (result != success)
    {
        TS_ASSERT(false);
        return;
    }

    m_value = v;
}

Literal::Literal(std::shared_ptr<const IValue> value)
    : m_value(std::move(value))
{
}

Literal::~Literal()
{

}

std::string Literal::get_template_instantiation_string() const
{
    if (ITemplate_Argument const* ta = dynamic_cast<ITemplate_Argument const*>(m_value.get()))
    {
        return ta->get_template_instantiation_string();
    }
    return std::string();
}

std::shared_ptr<const IValue> Literal::get_value() const
{
    return m_value;
}

}
