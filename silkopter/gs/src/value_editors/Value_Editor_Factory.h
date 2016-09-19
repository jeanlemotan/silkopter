#pragma once

#include "IValue_Editor.h"
#include <unordered_map>
#include <memory>

class Value_Editor_Factory : public std::enable_shared_from_this<Value_Editor_Factory>
{
public:
    Value_Editor_Factory();
    void register_standard_editors();

    std::shared_ptr<IValue_Editor>	create_editor(std::shared_ptr<const ts::IValue> value, std::string const& editor, std::string const& suffix) const;
    std::shared_ptr<IValue_Editor>	create_editor(std::shared_ptr<ts::IValue> value, std::string const& editor, std::string const& suffix) const;

    template<class Type, class Value, class Editor, class... FixedConstructorParams> void register_editor(FixedConstructorParams&&... fixedConstructorParams);
    void unregister_all_editors();

private:
    using Mutable_Creator = std::function<std::shared_ptr<IValue_Editor>(std::shared_ptr<ts::IValue>, std::string const& editor_str, std::string const& suffix_str)>;
    using Const_Creator = std::function<std::shared_ptr<IValue_Editor>(std::shared_ptr<const ts::IValue>, std::string const& editor_str, std::string const& suffix_str)>;

    struct Creators
    {
        Mutable_Creator mutable_creator;
        Const_Creator const_creator;
    };

    std::vector<Creators> m_registered_editors;
};

template<class Type, class Value, class Editor, class... FixedConstructorParams>
void Value_Editor_Factory::register_editor(FixedConstructorParams&&... fixedConstructorParams)
{
    Creators creators;
    creators.mutable_creator = [=](std::shared_ptr<ts::IValue> value, std::string const& editor_str, std::string const& suffix_str) -> std::shared_ptr<IValue_Editor>
    {
        std::shared_ptr<Value> v = std::dynamic_pointer_cast<Value>(value);
        if (v)
        {
            return std::make_shared<Editor>(v, editor_str, suffix_str, fixedConstructorParams...);
        }
        return nullptr;
    };
    creators.const_creator = [=](std::shared_ptr<const ts::IValue> value, std::string const& editor_str, std::string const& suffix_str) -> std::shared_ptr<IValue_Editor>
    {
        std::shared_ptr<const Value> v = std::dynamic_pointer_cast<const Value>(value);
        if (v)
        {
            return std::make_shared<Editor>(v, editor_str, suffix_str, fixedConstructorParams...);
        }
        return nullptr;
    };

    m_registered_editors.push_back(creators);
}

