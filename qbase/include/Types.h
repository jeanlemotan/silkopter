#pragma once

#define DEFINE_COPYING(ClassName)										\
ClassName(const ClassName&) { QASSERT(0); }						\
ClassName& operator=(const ClassName&) { QASSERT(0); return *this; }

#define DEFINE_CMP_OPERATORS(ClassName)												\
bool operator>(ClassName const& other) const { return !(*this <= other); }			\
bool operator>=(ClassName const& other) const { return !(*this < other); }			\
bool operator<=(ClassName const& other) const { return (*this < other) || (*this == other); }			

#define DEFINE_EQ_OPERATORS(ClassName)												\
bool operator!=(ClassName const& other) const  { return !(*this == other); }

#define DEFINE_OPERATORS(ClassName)													\
DEFINE_CMP_OPERATORS(ClassName)\
DEFINE_EQ_OPERATORS(ClassName)

#define DECLARE_CLASS_PTR(Class)			\
typedef std::shared_ptr<Class> Class##_ptr;		\
typedef std::shared_ptr<const Class> Class##_cptr;	\
typedef std::weak_ptr<Class> Class##_wptr;			\
typedef std::weak_ptr<const Class> Class##_cwptr;	\
typedef std::unique_ptr<Class> Class##_uptr;	

namespace q
{

//helper type to wrap a handle in a std::unique_ptr
//it basically give NullableType semantics to a integer - in particular assignment and comparison to nullptr
struct Handle
{
	Handle() : value(0) {}
	Handle(std::nullptr_t) : value(0) {}
	explicit Handle(uint32_t v) : value(v) {} // make it explicit if you need it
	// special members can be generated

	Handle& operator=(std::nullptr_t) { value = 0; return *this; }

	// contextual conversion to bool
	//explicit operator bool() const{ return value != 0; }

	uint32_t value;
};

inline bool operator==(Handle lhs, Handle rhs){ return lhs.value == rhs.value; }
inline bool operator!=(Handle lhs, Handle rhs){ return lhs.value != rhs.value; }
// comparision against 'nullptr' is handled by the above operators
// since 'nullptr' can be implicitly converted to 'Handle'


//similar to std::make_shared
template<typename T>
std::unique_ptr<T> make_unique()
{
	return std::unique_ptr<T>(new T());
}


typedef std::chrono::high_resolution_clock Clock;
typedef Clock::duration Duration;
typedef std::chrono::duration<float> Secondsf;

}
