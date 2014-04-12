#pragma once

namespace q
{
namespace rtti
{

	template<class T, class U> bool is_of_type(U const& temp);
	template<class U> bool is_of_type(U const& temp, std::string const& className);
	template<class T, class U> std::shared_ptr<T> cast_to(std::shared_ptr<U> const& temp);
	template<class T, class U> T* cast_to(U* temp);
	template<class T, class U> T const* cast_to(U const* temp);
	template<class T> std::string get_class_name(T const& temp);
	template<class T> std::string get_class_name();
	template<class T> size_t get_class_id(T const& temp);
	template<class T> size_t get_class_id();

	//details
	namespace detail
	{
		template<typename T>
		struct Type_Id_Checker{};
	} //namespace detail

#define DEFINE_RTTI_CLASS_COMMON_PART(Class) \
	friend class q::util::Class_Factory;\
	static size_t rtti_get_class_id() { static size_t id; return (size_t)&id; } \
	typedef q::rtti::detail::Type_Id_Checker<Class> type_tag;\
	static std::string rtti_get_class_name() { return std::string(#Class); }\
	virtual std::string rtti_get_instance_class_name() const { return rtti_get_class_name(); }\
	virtual size_t rtti_get_instance_class_id() const { return rtti_get_class_id(); }\
	template<class U> friend bool q::rtti::is_of_type(U const& temp, std::string const& className);\
	template<class T, class U> friend bool q::rtti::is_of_type(U const& temp);	\
	template<class T, class U> std::shared_ptr<T> cast_to(std::shared_ptr<U> const& temp); \
	template<class T, class U> friend T* q::rtti::cast_to(U* temp);			\
	template<class T, class U> friend T const* q::rtti::cast_to(U const* temp); \
	template<class T> friend std::string q::rtti::get_class_name(T const& temp); \
	template<class T> friend std::string q::rtti::get_class_name(); \
	template<class T> friend size_t q::rtti::get_class_id(T const& temp); \
	template<class T> friend size_t q::rtti::get_class_id();

#define DEFINE_RTTI_CLASS_COMMON_TEMPLATE_PART(Class, className) \
	friend class q::util::Class_Factory;\
	static size_t rtti_get_class_id() { static size_t id; return (size_t)&id; } \
	typedef q::rtti::detail::Type_Id_Checker<Class> type_tag;\
	static std::string rtti_get_class_name() { return std::string(className); }\
	virtual std::string rtti_get_instance_class_name() const { return rtti_get_class_name(); }\
	virtual size_t rtti_get_instance_class_id() const { return rtti_get_class_id(); }\
	template<class U> friend bool q::rtti::is_of_type(U const& temp, std::string const& className);\
	template<class T, class U> friend bool q::rtti::is_of_type(U const& temp);	\
	template<class T, class U> std::shared_ptr<T> cast_to(std::shared_ptr<U> const& temp); \
	template<class T, class U> friend T* q::rtti::cast_to(U* temp);			\
	template<class T, class U> friend T const* q::rtti::cast_to(U const* temp); \
	template<class T> friend std::string q::rtti::get_class_name(T const& temp); \
	template<class T> friend std::string q::rtti::get_class_name(); \
	template<class T> friend size_t q::rtti::get_class_id(T const& temp); \
	template<class T> friend size_t q::rtti::get_class_id();

#define DEFINE_RTTI_CLASS2_PART(Class, Parent1, Parent2)	\
	protected:							\
		virtual bool rtti_is_of_type(size_t typeId) const { return (typeId == rtti_get_class_id()) || Parent1::rtti_is_of_type(typeId) || Parent2::rtti_is_of_type(typeId); } \
		virtual bool rtti_is_of_type(std::string const& typeId) const { return (typeId == rtti_get_class_name()) || Parent1::rtti_is_of_type(typeId) || Parent2::rtti_is_of_type(typeId); } \


#define DEFINE_RTTI_CLASS_PART(Class, Parent)	\
	protected:							\
		virtual bool rtti_is_of_type(size_t typeId) const { return (typeId == rtti_get_class_id()) || Parent::rtti_is_of_type(typeId); } \
		virtual bool rtti_is_of_type(std::string const& typeId) const { return (typeId == rtti_get_class_name()) || Parent::rtti_is_of_type(typeId); } \

#define DEFINE_RTTI_BASE_CLASS_PART(Class)	\
	protected:							\
		virtual bool rtti_is_of_type(size_t typeId) const { return (typeId == rtti_get_class_id()); } \
		virtual bool rtti_is_of_type(std::string const& typeId) const { return (typeId == rtti_get_class_name()); } \


	//////////////////////////////////////////////////////////////////////////
	// To be used in normal classes
#define DEFINE_RTTI_CLASS2(Class, Parent1, Parent2)					\
		DEFINE_RTTI_CLASS2_PART(Class, Parent1, Parent2)			\
		DEFINE_RTTI_CLASS_COMMON_PART(Class)

#define DEFINE_RTTI_CLASS(Class, Parent)							\
		DEFINE_RTTI_CLASS_PART(Class, Parent)						\
		DEFINE_RTTI_CLASS_COMMON_PART(Class)

#define DEFINE_RTTI_BASE_CLASS(Class)								\
		DEFINE_RTTI_BASE_CLASS_PART(Class)							\
		DEFINE_RTTI_CLASS_COMMON_PART(Class)

	//////////////////////////////////////////////////////////////////////////
	// To be used in templates with a class name
#define DEFINE_RTTI_CLASS2_TEMPLATE(className, Class, Parent1, Parent2)	\
		DEFINE_RTTI_CLASS2_PART(Class, Parent1, Parent2)				\
		DEFINE_RTTI_CLASS_COMMON_TEMPLATE_PART(Class, className)

#define DEFINE_RTTI_CLASS_TEMPLATE(className, Class, Parent)			\
		DEFINE_RTTI_CLASS_PART(Class, Parent)							\
		DEFINE_RTTI_CLASS_COMMON_TEMPLATE_PART(Class, className)

#define DEFINE_RTTI_BASE_CLASS_TEMPLATE(className, Class)				\
		DEFINE_RTTI_BASE_CLASS_PART(Class)								\
		DEFINE_RTTI_CLASS_COMMON_TEMPLATE_PART(Class, className)


	template<class T, class U>
	bool is_of_type(U const& temp)
	{
		//if you get this error, your class is not RTTI ready. Use the macros above
		detail::Type_Id_Checker<T> static_type_checking_helper = typename T::type_tag();
        (void)static_type_checking_helper;
		return temp.rtti_is_of_type(T::rtti_get_class_id());
	}
	template<class U>
	bool is_of_type(U const& temp, std::string const& className)
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
	std::string get_class_name(T const& temp)
	{
		return temp.rtti_get_instance_class_name();
	}
	template<class T>
	std::string get_class_name()
	{
		return T::rtti_get_class_name();
	}
	template<class T>
	size_t get_class_id(T const& temp)
	{
		return temp.rtti_get_instance_class_id();
	}
	template<class T>
	size_t get_class_id()
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

