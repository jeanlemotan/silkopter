#include "value_editors/Value_Editor_Factory.h"

//////////////////////////////////////////////////////////////////////////

std::shared_ptr<IValue_Editor> Value_Editor_Factory::create_editor(std::shared_ptr<ts::IValue> value) const
{
    std::shared_ptr<const ts::IType> type = value->get_type();

    for (const Creators& creators: m_registered_editors)
	{
        std::shared_ptr<IValue_Editor> editor = creators.mutable_creator(value);
		if (editor)
		{
			return editor;
		}
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////

std::shared_ptr<IValue_Editor> Value_Editor_Factory::create_editor(std::shared_ptr<const ts::IValue> value) const
{
    std::shared_ptr<const ts::IType> type = value->get_type();

    for (const Creators& creators : m_registered_editors)
	{
        std::shared_ptr<IValue_Editor> editor = creators.const_creator(value);
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
