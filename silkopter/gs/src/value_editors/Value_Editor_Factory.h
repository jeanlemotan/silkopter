#pragma once

#include "IValue_Editor.h"
#include <unordered_map>
#include <memory>

class Value_Editor_Factory : std::enable_shared_from_this<Value_Editor_Factory>
{
public:
    std::shared_ptr<IValue_Editor>	create_editor(std::shared_ptr<const ts::IValue> value) const;
    std::shared_ptr<IValue_Editor>	create_editor(std::shared_ptr<ts::IValue> value) const;

    template<class Type, class Editor, class... FixedConstructorParams> void register_editor(FixedConstructorParams&&... fixedConstructorParams);
    void unregister_all_editors();

private:
    using Mutable_Creator = std::function<std::shared_ptr<IValue_Editor>(std::shared_ptr<ts::IValue>)>;
    using Const_Creator = std::function<std::shared_ptr<IValue_Editor>(std::shared_ptr<const ts::IValue>)>;

    struct Creators
    {
        Mutable_Creator mutable_creator;
        Const_Creator const_creator;
    };

    std::vector<Creators> m_registered_editors;
};

template<class Type, class Editor, class... FixedConstructorParams>
void Value_Editor_Factory::register_editor(FixedConstructorParams&&... fixedConstructorParams)
{
    Creators creators;
    creators.mutable_creator = [=](std::shared_ptr<ts::IValue> value) -> std::shared_ptr<IValue_Editor>
    {
        typedef typename Type::type_traits Traits;
        typedef typename Traits::value_interface Value_Interface;
        std::shared_ptr<Value_Interface> v = std::dynamic_pointer_cast<Value_Interface>(value);
        if (v)
        {
            return std::make_shared<Editor>(v, fixedConstructorParams...);
        }
        return nullptr;
    };
    creators.const_creator = [=](std::shared_ptr<const ts::IValue> value) -> std::shared_ptr<IValue_Editor>
    {
        typedef typename Type::type_traits Traits;
        typedef typename Traits::value_interface Value_Interface;
        std::shared_ptr<const Value_Interface> v = std::dynamic_pointer_cast<const Value_Interface>(value);
        if (v)
        {
            return std::make_shared<Editor>(v, fixedConstructorParams...);
        }
        return nullptr;
    };

    m_registered_editors.push_back(creators);
}

// 	template<class Type>
// 	void Value_Editor_Factory::UnregisterEditorFor()
// 	{
// 		m_registeredEditors.erase(jtl::jtypeid<Type>());
// 	}
// 	template<class Type> bool Value_Editor_Factory::ContainsEditorFor() const
// 	{
// 		return m_registeredEditors.find(jtl::jtypeid<Type>()) != m_registeredEditors.end();
// 	}

