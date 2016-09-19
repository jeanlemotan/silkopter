#include "value_editors/Value_Editor_Factory.h"

#include "value_editors/Bool_Value_Editor.h"
#include "value_editors/Double_Value_Editor.h"
#include "value_editors/Enum_Value_Editor.h"
#include "value_editors/Float_Value_Editor.h"
#include "value_editors/Int_Value_Editor.h"
#include "value_editors/Optional_Value_Editor.h"
#include "value_editors/String_Value_Editor.h"
#include "value_editors/Variant_Value_Editor.h"
#include "value_editors/Vec2f_Value_Editor.h"
#include "value_editors/Vec2i_Value_Editor.h"
#include "value_editors/Vec3f_Value_Editor.h"
#include "value_editors/Vec3i_Value_Editor.h"
#include "value_editors/Vec4f_Value_Editor.h"
#include "value_editors/Vec4i_Value_Editor.h"


//////////////////////////////////////////////////////////////////////////

Value_Editor_Factory::Value_Editor_Factory()
{
}

//////////////////////////////////////////////////////////////////////////

void Value_Editor_Factory::register_standard_editors()
{
    register_editor<ts::IBool_Type,     ts::IBool_Value,     Bool_Value_Editor>();
    register_editor<ts::IDouble_Type,   ts::IDouble_Value,   Double_Value_Editor>();
    register_editor<ts::IEnum_Type,     ts::IEnum_Value,     Enum_Value_Editor>();
    register_editor<ts::IFloat_Type,    ts::IFloat_Value,    Float_Value_Editor>();
    register_editor<ts::IInt_Type,      ts::IInt_Value,      Int_Value_Editor>();
    register_editor<ts::IOptional_Type, ts::IOptional_Value, Optional_Value_Editor>(shared_from_this());
    register_editor<ts::IString_Type,   ts::IString_Value,   String_Value_Editor>();
    register_editor<ts::IVariant_Type,  ts::IVariant_Value,  Variant_Value_Editor>(shared_from_this());
    register_editor<ts::IVec2f_Type,    ts::IVec2f_Value,    Vec2f_Value_Editor>();
    register_editor<ts::IVec2i_Type,    ts::IVec2i_Value,    Vec2i_Value_Editor>();
    register_editor<ts::IVec3f_Type,    ts::IVec3f_Value,    Vec3f_Value_Editor>();
    register_editor<ts::IVec3i_Type,    ts::IVec3i_Value,    Vec3i_Value_Editor>();
    register_editor<ts::IVec4f_Type,    ts::IVec4f_Value,    Vec4f_Value_Editor>();
    register_editor<ts::IVec4i_Type,    ts::IVec4i_Value,    Vec4i_Value_Editor>();
}

//////////////////////////////////////////////////////////////////////////

std::shared_ptr<IValue_Editor> Value_Editor_Factory::create_editor(std::shared_ptr<ts::IValue> value, std::string const& suffix_str) const
{
    std::shared_ptr<const ts::IType> type = value->get_type();

    for (const Creators& creators: m_registered_editors)
	{
        std::shared_ptr<IValue_Editor> editor = creators.mutable_creator(value, suffix_str);
		if (editor)
		{
			return editor;
		}
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////

std::shared_ptr<IValue_Editor> Value_Editor_Factory::create_editor(std::shared_ptr<const ts::IValue> value, std::string const& suffix_str) const
{
    std::shared_ptr<const ts::IType> type = value->get_type();

    for (const Creators& creators : m_registered_editors)
	{
        std::shared_ptr<IValue_Editor> editor = creators.const_creator(value, suffix_str);
		if (editor)
		{
			return editor;
		}
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////

void Value_Editor_Factory::unregister_all_editors()
{
    m_registered_editors.clear();
}

//////////////////////////////////////////////////////////////////////////
