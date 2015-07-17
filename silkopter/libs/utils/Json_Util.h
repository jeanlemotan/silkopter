#pragma once 

#include "QBase.h"
#include "rapidjson/document.h"

namespace jsonutil
{
//	using namespace rapidjson;
//	typedef rapidjson::Type Type;

    template <class T, class STRING>
    T* find_value(T& value, STRING const& name)
    {
        if (name.empty())
        {
            return &value;
        }
        if (value.IsArray() && name[0] == '[' && name.size() >= 3 && name.size() < 32)
        {
            char buffer[32] = {0};
            memcpy(buffer, name.c_str() + 1, name.size() - 2); //remove the []
            int idx = atoi(buffer);
            if (idx >= 0 && idx < (int)value.Size())
            {
                return &value[idx];
            }
            return nullptr;
        }
        if (value.IsObject())
        {
            auto m = value.FindMember(name.c_str());
            if (m != value.MemberEnd())
            {
                return &m->value;
            }
        }
        return nullptr;
    }

    template <class T>
    inline T* find_value(T& value, q::Path const& path)
    {
        if (path.is_empty())
        {
            return &value;
        }
        auto v = find_value(value, path[0]);
        if (v != nullptr)
        {
            auto p = path.get_sub_path(1, 0);
            return p.is_empty() ? v : find_value(*v, p);
        }
        return nullptr;
    }

    template <class STRING>
    rapidjson::Value* find_value(rapidjson::Document& document, STRING const& name)
    {
        return find_value(static_cast<rapidjson::Value&>(document), name);
    }
    template <class STRING>
    rapidjson::Value const* find_value(rapidjson::Document const& document, STRING const& name)
    {
        return find_value(static_cast<rapidjson::Value const&>(document), name);
    }
    inline rapidjson::Value* find_value(rapidjson::Document& document, q::Path const& path)
    {
        return find_value(static_cast<rapidjson::Value&>(document), path);
    }
    inline rapidjson::Value const* find_value(rapidjson::Document const& document, q::Path const& path)
    {
        return find_value(static_cast<rapidjson::Value const&>(document), path);
    }

    template <class T, class STRING>
    auto remove_value(T& value, STRING const& name) -> bool
    {
        if (name.empty())
        {
            return false;
        }
        if (value.IsArray() && name[0] == '[' && name.size() >= 3 && name.size() < 32)
        {
            char buffer[32] = {0};
            memcpy(buffer, name.c_str() + 1, name.size() - 2); //remove the []
            int idx = atoi(buffer);
            if (idx >= 0 && idx < (int)value.Size())
            {
                value.Erase(value.Begin() + idx);
                return true;
            }
            return false;
        }
        if (value.IsObject())
        {
            return value.RemoveMember(name.c_str());
        }
        return false;
    }

    template <class T>
    inline auto remove_value(T& value, q::Path const& path) -> bool
    {
        if (path.is_empty())
        {
            return false;
        }
        if (path.size() == 1)
        {
            return remove_value(value, path[0]);
        }
        auto v = find_value(value, path[0]);
        if (v != nullptr)
        {
            auto p = path.get_sub_path(1, 0);
            return remove_value(*v, p);
        }
        return false;
    }

    template <class STRING>
    inline rapidjson::Value* get_or_add_value(rapidjson::Value& json, STRING const& name, rapidjson::Type type, typename rapidjson::Value::AllocatorType& allocator)
    {
        auto v = find_value(json, name);
        if (v == nullptr)
        {
            if (json.IsNull())
            {
                json.SetObject();
            }
            if (!json.IsObject())
            {
                return nullptr;
            }
            rapidjson::Value n(name.c_str(), name.size(), allocator);
            rapidjson::Value value(type);
            json.AddMember(std::move(n), std::move(value), allocator);
            return find_value(json, name);
        }
        else if (type == v->GetType())
        {
            return v;
        }
        return nullptr;
    }

    inline rapidjson::Value* get_or_add_value(rapidjson::Value& json, q::Path const& path, rapidjson::Type type, typename rapidjson::Value::AllocatorType& allocator)
    {
        //first try a find
        auto v = find_value(json, path);
        if (v && type == v->GetType())
        {
            return v;
        }

        //now walk the path and create missing elements
        auto p = path;
        rapidjson::Value* parent = &json;
        while (!p.is_empty())
        {
            if (p.size() == 1)
            {
                //last element?
                return get_or_add_value(*parent, p[0], type, allocator);
            }
            else
            {
                parent = get_or_add_value(*parent, p[0], rapidjson::kObjectType, allocator);
                if (!parent)
                {
                    return nullptr;
                }
                p.pop_front();
            }
        }
        return nullptr;
    }

    template <class STRING>
    inline auto add_value(rapidjson::Value& json, STRING const& name, rapidjson::Value&& value, typename rapidjson::Value::AllocatorType& allocator) -> bool
    {
        auto v = find_value(json, name);
        if (v != nullptr)
        {
            return false;
        }

        if (json.IsNull())
        {
            json.SetObject();
        }
        if (!json.IsObject())
        {
            return false;
        }

        rapidjson::Value n(name.c_str(), name.size(), allocator);
        json.AddMember(std::move(n), std::move(value), allocator);
        return true;
    }

    inline auto add_value(rapidjson::Value& json, q::Path const& path, rapidjson::Value&& value, typename rapidjson::Value::AllocatorType& allocator) -> bool
    {
        //now walk the path and create missing elements
        auto p = path;
        rapidjson::Value* parent = &json;
        while (!p.is_empty())
        {
            if (p.size() == 1)
            {
                //last element?
                return add_value(*parent, p[0], std::move(value), allocator);
            }
            else
            {
                parent = get_or_add_value(*parent, p[0], rapidjson::kObjectType, allocator);
                if (!parent)
                {
                    return false;
                }
                p.pop_front();
            }
        }
        return false;
    }

    rapidjson::Value clone_value(rapidjson::Value const& json, rapidjson::Document::AllocatorType& allocator);

    inline void clone_value(rapidjson::Value& dst, rapidjson::Value const& json, rapidjson::Document::AllocatorType& allocator)
    {
        using namespace rapidjson;
        switch (json.GetType())
        {
        case kNullType:		dst.SetNull(); break;
        case kFalseType:	dst.SetBool(false); break;
        case kTrueType:		dst.SetBool(true); break;
        case kObjectType:
            dst.SetObject();
            for (auto m = json.MemberBegin(); m != json.MemberEnd(); ++m)
            {
                rapidjson::Value nval(m->name.GetString(), m->name.GetStringLength(), allocator);
                dst.AddMember(std::move(nval), clone_value(m->value, allocator), allocator);
            }
            break;
        case kArrayType:
            dst.SetArray();
            for (size_t i = 0; i < json.Size(); i++)
            {
                dst.PushBack(clone_value(json[(uint32_t)(i)], allocator), allocator);
            }
            break;
        case kStringType:
            dst.SetString(json.GetString(), json.GetStringLength(), allocator);
            break;
        case kNumberType:
            if (json.IsInt())			dst.SetInt(json.GetInt());
            else if (json.IsUint())		dst.SetUint(json.GetUint());
            else if (json.IsInt64())	dst.SetInt64(json.GetInt64());
            else if (json.IsUint64())	dst.SetUint64(json.GetUint64());
            else						dst.SetDouble(json.GetDouble());
            break;
        default:
            QASSERT(0);
            break;
        }
    }

    inline rapidjson::Document clone_value(rapidjson::Value const& json)
    {
        rapidjson::Document dst;
        clone_value(dst, json, dst.GetAllocator());
        return std::move(dst);
    }
    inline rapidjson::Value clone_value(rapidjson::Value const& json, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value dst;
        clone_value(dst, json, allocator);
        return std::move(dst);
    }

//    inline bool remove_all_members(rapidjson::Value& json, std::string const& name)
//	{
//        QASSERT(json.IsObject());
//		bool found = false;
//		while (json.RemoveMember(name.c_str())) found = true;
//		return found;
//	}

//    inline bool validate_path(rapidjson::Value const& json, q::Path const& path)
//	{
//        Const_Member_Finder finder(json, path[0].c_str());
//        if (!finder.is_valid())
//		{
//			return false;
//		}
//        else if (path.get_count() > 1)
//		{
//            q::Path const& p = path.get_sub_path(1);
//            return validate_path(finder.get_value(), p);
//		}
//		return true;
//	}

//	template<class T>
//    inline void update_value(Value& value, T const& data, jsonutil::Document::AllocatorType&)
//	{
//		Value newValue(data);
//		value = newValue;
//	}

//	template<>
//    inline void update_value(Value& value, std::string const& data, jsonutil::Document::AllocatorType& allocator)
//	{
//        Value newValue(data.c_str(), allocator);
//		value = newValue;
//	}
//    template<>
//    inline void update_value(Value& value, char const* const& data, jsonutil::Document::AllocatorType& allocator)
//    {
//        Value newValue(data, allocator);
//        value = newValue;
//    }


}
