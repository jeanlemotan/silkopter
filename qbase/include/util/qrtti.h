#pragma once

#include "util/murmurhash.h"

namespace q
{
namespace rtti
{
    typedef uint32_t class_id;
    typedef std::string class_name;

	template<class T, class U> bool is_of_type(U const& temp);
    template<class U> bool is_of_type(U const& temp, class_name const& className);
	template<class T, class U> std::shared_ptr<T> cast_to(std::shared_ptr<U> const& temp);
	template<class T, class U> T* cast_to(U* temp);
	template<class T, class U> T const* cast_to(U const* temp);
    template<class T> class_name const& get_class_name(T const& temp);
    template<class T> class_name const& get_class_name();
    template<class T> class_id get_class_id(T const& temp);
    template<class T> class_id get_class_id();

	//details
	namespace detail
	{
		template<typename T>
		struct Type_Id_Checker{};
	} //namespace detail

#define DEFINE_RTTI_CLASS_COMMON_PART(CLASS) \
	friend class q::util::Class_Factory;\
    static q::rtti::class_id rtti_get_class_id() { static q::rtti::class_id id = q::util::compute_murmur_hash32(#CLASS, 0xF123); return id; } \
    typedef q::rtti::detail::Type_Id_Checker<CLASS> type_tag;\
    static q::rtti::class_name const& rtti_get_class_name() { static q::rtti::class_name name(#CLASS); return name; }\
    virtual q::rtti::class_name const& rtti_get_instance_class_name() const { return rtti_get_class_name(); }\
    virtual q::rtti::class_id rtti_get_instance_class_id() const { return rtti_get_class_id(); }\
    template<class U> friend bool q::rtti::is_of_type(U const& temp, q::rtti::class_name const& className);\
	template<class T, class U> friend bool q::rtti::is_of_type(U const& temp);	\
	template<class T, class U> std::shared_ptr<T> cast_to(std::shared_ptr<U> const& temp); \
	template<class T, class U> friend T* q::rtti::cast_to(U* temp);			\
	template<class T, class U> friend T const* q::rtti::cast_to(U const* temp); \
    template<class T> friend q::rtti::class_name const& q::rtti::get_class_name(T const& temp); \
    template<class T> friend q::rtti::class_name const& q::rtti::get_class_name(); \
    template<class T> friend q::rtti::class_id q::rtti::get_class_id(T const& temp); \
    template<class T> friend q::rtti::class_id q::rtti::get_class_id();

#define DEFINE_RTTI_CLASS_COMMON_TEMPLATE_PART(CLASS, CLASS_NAME) \
	friend class q::util::Class_Factory;\
    static q::rtti::class_id rtti_get_class_id() { static q::rtti::class_id id = q::util::compute_murmur_hash32(CLASS_NAME, 0xF123); return id; } \
    typedef q::rtti::detail::Type_Id_Checker<CLASS> type_tag;\
    static q::rtti::class_name const& rtti_get_class_name() { static q::rtti::class_name name(CLASS_NAME); return name; }\
    virtual q::rtti::class_name const& rtti_get_instance_class_name() const { return rtti_get_class_name(); }\
    virtual q::rtti::class_id rtti_get_instance_class_id() const { return rtti_get_class_id(); }\
    template<class U> friend bool q::rtti::is_of_type(U const& temp, q::rtti::class_name const& className);\
	template<class T, class U> friend bool q::rtti::is_of_type(U const& temp);	\
	template<class T, class U> std::shared_ptr<T> cast_to(std::shared_ptr<U> const& temp); \
	template<class T, class U> friend T* q::rtti::cast_to(U* temp);			\
	template<class T, class U> friend T const* q::rtti::cast_to(U const* temp); \
    template<class T> friend q::rtti::class_name const& q::rtti::get_class_name(T const& temp); \
    template<class T> friend q::rtti::class_name const& q::rtti::get_class_name(); \
    template<class T> friend q::rtti::class_id q::rtti::get_class_id(T const& temp); \
    template<class T> friend q::rtti::class_id q::rtti::get_class_id();

#define DEFINE_RTTI_CLASS2_PART(CLASS, PARENT_1, PARENT_2)	\
    public:                                 \
        typedef PARENT_1 Parent_1;          \
        typedef PARENT_2 Parent_2;          \
    protected:							\
        virtual bool rtti_is_of_type(q::rtti::class_id typeId) const { return (typeId == rtti_get_class_id()) || PARENT_1::rtti_is_of_type(typeId) || PARENT_2::rtti_is_of_type(typeId); } \
        virtual bool rtti_is_of_type(q::rtti::class_name const& typeId) const { return (typeId == rtti_get_class_name()) || PARENT_1::rtti_is_of_type(typeId) || PARENT_2::rtti_is_of_type(typeId); } \


#define DEFINE_RTTI_CLASS_PART(CLASS, PARENT)	\
    public:                                 \
        typedef PARENT Parent;          \
    protected:							\
        virtual bool rtti_is_of_type(q::rtti::class_id typeId) const { return (typeId == rtti_get_class_id()) || PARENT::rtti_is_of_type(typeId); } \
        virtual bool rtti_is_of_type(q::rtti::class_name const& typeId) const { return (typeId == rtti_get_class_name()) || PARENT::rtti_is_of_type(typeId); } \

#define DEFINE_RTTI_BASE_CLASS_PART(CLASS)	\
	protected:							\
        virtual bool rtti_is_of_type(q::rtti::class_id typeId) const { return (typeId == rtti_get_class_id()); } \
        virtual bool rtti_is_of_type(q::rtti::class_name const& typeId) const { return (typeId == rtti_get_class_name()); } \


	//////////////////////////////////////////////////////////////////////////
	// To be used in normal classes
#define DEFINE_RTTI_CLASS2(CLASS, PARENT_1, PARENT_2)					\
        DEFINE_RTTI_CLASS2_PART(CLASS, PARENT_1, PARENT_2)			\
        DEFINE_RTTI_CLASS_COMMON_PART(CLASS)

#define DEFINE_RTTI_CLASS(CLASS, PARENT)							\
        DEFINE_RTTI_CLASS_PART(CLASS, PARENT)						\
        DEFINE_RTTI_CLASS_COMMON_PART(CLASS)

#define DEFINE_RTTI_BASE_CLASS(CLASS)								\
        DEFINE_RTTI_BASE_CLASS_PART(CLASS)							\
        DEFINE_RTTI_CLASS_COMMON_PART(CLASS)

	//////////////////////////////////////////////////////////////////////////
	// To be used in templates with a class name
#define DEFINE_RTTI_CLASS2_TEMPLATE(CLASS_NAME, CLASS, PARENT_1, PARENT_2)	\
        DEFINE_RTTI_CLASS2_PART(CLASS, PARENT_1, PARENT_2)				\
        DEFINE_RTTI_CLASS_COMMON_TEMPLATE_PART(CLASS, CLASS_NAME)

#define DEFINE_RTTI_CLASS_TEMPLATE(CLASS_NAME, CLASS, PARENT)			\
        DEFINE_RTTI_CLASS_PART(CLASS, PARENT)							\
        DEFINE_RTTI_CLASS_COMMON_TEMPLATE_PART(CLASS, CLASS_NAME)

#define DEFINE_RTTI_BASE_CLASS_TEMPLATE(CLASS_NAME, CLASS)				\
        DEFINE_RTTI_BASE_CLASS_PART(CLASS)								\
        DEFINE_RTTI_CLASS_COMMON_TEMPLATE_PART(CLASS, CLASS_NAME)


	template<class T, class U>
	bool is_of_type(U const& temp)
	{
		//if you get this error, your class is not RTTI ready. Use the macros above
		detail::Type_Id_Checker<T> static_type_checking_helper = typename T::type_tag();
        (void)static_type_checking_helper;
		return temp.rtti_is_of_type(T::rtti_get_class_id());
	}
	template<class U>
    bool is_of_type(U const& temp, class_name const& className)
	{
		return temp.rtti_is_of_type(className);
	}
	template<class T, class U>
	std::shared_ptr<T> cast_to(std::shared_ptr<U> const& temp)
	{
		return (temp && is_of_type<T>(*temp)) ? std::static_pointer_cast<T>(temp) : std::shared_ptr<T>();
	}
	template<class T, class U>
	T* cast_to(U* temp)
	{
		return (temp && is_of_type<T>(*temp)) ? static_cast<T*>(temp) : 0;
	}
	template<class T, class U>
	T const* cast_to(U const* temp)
	{
		return (temp && is_of_type<T>(*temp)) ? static_cast<T const*>(temp) : 0;
	}
	template<class T>
    class_name const& get_class_name(T const& temp)
	{
		return temp.rtti_get_instance_class_name();
	}
	template<class T>
    class_name const& get_class_name()
	{
		return T::rtti_get_class_name();
	}
	template<class T>
    class_id get_class_id(T const& temp)
	{
		return temp.rtti_get_instance_class_id();
	}
	template<class T>
    class_id get_class_id()
	{
		return T::rtti_get_class_id();
	}
	template<class T, class U>
	bool are_of_same_type(T const& a, U const& b)
	{
		return get_class_id(a) == get_class_id(b);
	}


} //namespace rtti
} //namespace q

