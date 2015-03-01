#pragma once 

#include "QBase.h"
#include "rapidjson/document.h"

namespace jsonutil
{
//	using namespace rapidjson;
//	typedef rapidjson::Type Type;

    template <class STRING>
    rapidjson::Value* find_value(rapidjson::Value& value, STRING const& name)
    {
        if (name.empty())
        {
            return &value;
        }
        if (!value.IsObject())
        {
            return nullptr;
        }
        for (rapidjson::Value::MemberIterator it = value.MemberBegin(); it != value.MemberEnd(); it++)
        {
            if (name == it->name.GetString())
            {
                return &it->value;
            }
        }
        return nullptr;
    }

    inline rapidjson::Value* find_value(rapidjson::Value& value, q::Path const& path)
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
    inline rapidjson::Value* get_or_add_value(rapidjson::Value& json, STRING const& name, rapidjson::Type type, typename rapidjson::Value::AllocatorType& allocator)
    {
        auto v = find_value(json, name);
        if (v == nullptr)
        {
            rapidjson::Value n(name.c_str(), name.size(), allocator);
            rapidjson::Value value(type);
            json.AddMember(n, value, allocator);
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
        if (!json.IsObject())
        {
            return nullptr;
        }

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


//    template <class VALUE = rapidjson::Value>
//    const typename VALUE::ConstMemberIterator find_member(VALUE const& value, std::string const& name)
//	{
//        QASSERT(!name.empty());
//        QASSERT(value.IsObject());

//		//size_t length = name.length();

//        for (typename VALUE::ConstMemberIterator it = value.MemberBegin(); it != value.MemberEnd(); it++)
//		{
//			if (it->name.GetString() == name)
//			{
//                return it;
//			}
//		}
//        return typename VALUE::ConstMemberIterator();
//	}

//    template<class VALUE, class MEMBER> struct Abstract_Member_Finder
//	{
//        typedef VALUE Value_t;
//        typedef MEMBER Member_t;

//        Abstract_Member_Finder(VALUE& json, std::string const& name)
//		{
//            m_member = json.IsObject() ? find_member(json, name) : Member_t();
//		}

//        Abstract_Member_Finder(VALUE& json, q::Path const& path)
//		{
//            q::Path p = path;
//			VALUE& j = json;
//			do  {
//                m_member = j.IsObject() ? find_member(j, p[0]) : nullptr;
//                p = p.get_sub_path(1);
//            } while(!p.is_empty() && m_member != nullptr);
//		}

//        bool is_valid() const
//		{
//            return m_member != Member_t();
//		}

//        VALUE& get_value() const
//		{
//            QASSERT_MSG(is_valid(), "Cannot get_value from a item that was not found !!!");
//			return m_member->value;
//		}

//        bool is_type(rapidjson::Type type) const
//		{
//            return is_valid() && m_member->value.GetType() == type;
//		}

//        Abstract_Member_Finder<VALUE,MEMBER> find_child(std::string const& name)
//		{
//            Abstract_Member_Finder<VALUE,MEMBER> finder(m_member->value, name.c_str());
//            QASSERT_MSG(m_member->value.GetType() == rapidjson::kObjectType, "Child must be an object");
//            QASSERT_MSG(finder.is_valid(), "Unknown child");
//			return finder;
//		}

//        MEMBER m_member;
//	};

//    typedef Abstract_Member_Finder<rapidjson::Value, rapidjson::Value::MemberIterator> Member_Finder;
//    typedef Abstract_Member_Finder<const rapidjson::Value, rapidjson::Value::ConstMemberIterator> Const_Member_Finder;



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
                rapidjson::Value nval(m->name.GetString(), allocator);
                rapidjson::Value mval;
                clone_value(mval, m->value, allocator);
                dst.AddMember(nval, mval, allocator);
            }
            break;
        case kArrayType:
            dst.SetArray();
            for (size_t i = 0; i < dst.Size(); i++)
            {
                rapidjson::Value mval;
                clone_value(mval, json[(uint32_t)(i)], allocator);
                dst.PushBack(mval, allocator);
            }
            break;
        case kStringType:
            dst.SetString(json.GetString(), allocator);
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

//    inline void set_float_value(rapidjson::Value& json, std::string const& name, float v, rapidjson::Document::AllocatorType& allocator)
//    {
//        rapidjson::Value& vj = get_or_add_member(json, name, kNumberType, allocator);
//        vj.SetDouble(v);
//    }
//    inline void set_int_value(rapidjson::Value& json, std::string const& name, int v, rapidjson::Document::AllocatorType& allocator)
//    {
//        rapidjson::Value& vj = get_or_add_member(json, name, kNumberType, allocator);
//        vj.SetInt(v);
//    }
//    inline void set_uint_value(rapidjson::Value& json, std::string const& name, unsigned int v, rapidjson::Document::AllocatorType& allocator)
//    {
//        rapidjson::Value& vj = get_or_add_member(json, name, kNumberType, allocator);
//        vj.SetUint(v);
//    }
//    inline void set_bool_value(rapidjson::Value& json, std::string const& name, bool v, rapidjson::Document::AllocatorType& allocator)
//    {
//        rapidjson::Value& vj = get_or_add_member(json, name, kNumberType, allocator);
//        vj.SetBool(v);
//    }

//    template <class T> inline void set_vec2_value(rapidjson::Value& json, std::string const& name, math::vec2<T> const& v, rapidjson::Document::AllocatorType& allocator)
//    {
//        rapidjson::Value& vj = get_or_add_member(json, name, kObjectType, allocator);
//        rapidjson::Value& x = get_or_add_member(vj, "x", kNumberType, allocator);
//        x.SetDouble(v.x);
//        rapidjson::Value& y = get_or_add_member(vj, "y", kNumberType, allocator);
//        y.SetDouble(v.y);
//    }

//    template <class T> inline void set_vec3_value(rapidjson::Value& json, std::string const& name, math::vec3<T> const& v, rapidjson::Document::AllocatorType& allocator)
//    {
//        rapidjson::Value& vj = get_or_add_member(json, name, kObjectType, allocator);
//        rapidjson::Value& x = get_or_add_member(vj, "x", kNumberType, allocator);
//        x.SetDouble(v.x);
//        rapidjson::Value& y = get_or_add_member(vj, "y", kNumberType, allocator);
//        y.SetDouble(v.y);
//        rapidjson::Value& z = get_or_add_member(vj, "z", kNumberType, allocator);
//        z.SetDouble(v.z);
//    }
//    template <class T> inline void set_pid_params(rapidjson::Value& json, std::string const& name, T const& v, rapidjson::Document::AllocatorType& allocator)
//    {
//        rapidjson::Value& vj = get_or_add_member(json, name, kObjectType, allocator);
//        rapidjson::Value& p = get_or_add_member(vj, "p", kNumberType, allocator);
//        p.SetDouble(v.kp);
//        rapidjson::Value& i = get_or_add_member(vj, "i", kNumberType, allocator);
//        i.SetDouble(v.ki);
//        rapidjson::Value& d = get_or_add_member(vj, "d", kNumberType, allocator);
//        d.SetDouble(v.kd);
//        rapidjson::Value& max = get_or_add_member(vj, "max", kNumberType, allocator);
//        max.SetDouble(v.max);
//    }

//    inline boost::optional<float> get_float_value(rapidjson::Value const& json, std::string const& name)
//    {
//        Const_Member_Finder finder(json, name);
//        if (!finder.is_type(rapidjson::kNumberType))
//        {
//            return boost::none;
//        }
//        return boost::optional<float>(finder.get_value().GetDouble());
//    }
//    inline boost::optional<int> get_int_value(rapidjson::Value const& json, std::string const& name)
//    {
//        Const_Member_Finder finder(json, name);
//        if (!finder.is_type(rapidjson::kNumberType) ||
//                !finder.get_value().IsInt())
//        {
//            return boost::none;
//        }
//        return boost::optional<int>(finder.get_value().GetInt());
//    }
//    inline boost::optional<unsigned int> get_uint_value(rapidjson::Value const& json, std::string const& name)
//    {
//        Const_Member_Finder finder(json, name);
//        if (!finder.is_type(rapidjson::kNumberType) ||
//                !finder.get_value().IsUint())
//        {
//            return boost::none;
//        }
//        return boost::optional<unsigned int>(finder.get_value().GetUint());
//    }
//    inline boost::optional<bool> get_bool_value(rapidjson::Value const& json, std::string const& name)
//    {
//        Const_Member_Finder finder(json, name);
//        if (!finder.is_type(rapidjson::kTrueType) && !finder.is_type(rapidjson::kFalseType))
//        {
//            return boost::none;
//        }
//        return boost::optional<bool>(finder.get_value().GetBool());
//    }

//    template <class T> inline boost::optional<math::vec3<T>> get_vec3_value(rapidjson::Value const& json, std::string const& name)
//    {
//        Const_Member_Finder finder(json, name);
//        if (!finder.is_type(rapidjson::kObjectType))
//        {
//            return boost::none;
//        }
//        auto& vj = finder.get_value();
//        auto x = Const_Member_Finder(vj, "x");
//        auto y = Const_Member_Finder(vj, "y");
//        auto z = Const_Member_Finder(vj, "z");
//        if (!x.is_type(rapidjson::kNumberType) || !y.is_type(rapidjson::kNumberType) || !z.is_type(rapidjson::kNumberType))
//        {
//            return boost::none;
//        }
//        math::vec3<T> vec(x.get_value().GetDouble(), y.get_value().GetDouble(), z.get_value().GetDouble());
//        return boost::optional<math::vec3<T>>(vec);
//    }
//    template <class T> inline boost::optional<math::vec2<T>> get_vec2_value(rapidjson::Value const& json, std::string const& name)
//    {
//        Const_Member_Finder finder(json, name);
//        if (!finder.is_type(rapidjson::kObjectType))
//        {
//            return boost::none;
//        }
//        auto& vj = finder.get_value();
//        auto x = Const_Member_Finder(vj, "x");
//        auto y = Const_Member_Finder(vj, "y");
//        if (!x.is_type(rapidjson::kNumberType) || !y.is_type(rapidjson::kNumberType))
//        {
//            return boost::none;
//        }
//        math::vec2<T> vec(x.get_value().GetDouble(), y.get_value().GetDouble());
//        return boost::optional<math::vec2<T>>(vec);
//    }
//    template <class T> inline boost::optional<T> get_pid_params(rapidjson::Value const& json, std::string const& name)
//    {
//        Const_Member_Finder finder(json, name);
//        if (!finder.is_type(rapidjson::kObjectType))
//        {
//            return boost::none;
//        }
//        auto& vj = finder.get_value();
//        auto p = get_float_value(vj, "p");
//        auto i = get_float_value(vj, "i");
//        auto d = get_float_value(vj, "d");
//        auto max = get_float_value(vj, "max");
//        if (!p || !i || !d || !max)
//        {
//            return boost::none;
//        }
//        return T(*p, *i, *d, *max);
//    }


}
