
#ifdef STD_STRING_CONVERSIONS

#include <sstream>

namespace math
{

inline std::string to_string(uint8_t v)
{
	std::stringstream str;
	str << v;
	return str.str();
}
inline std::string to_string(uint16_t v)
{
	std::stringstream str;
	str << v;
	return str.str();
}
inline std::string to_string(uint32_t v)
{
	std::stringstream str;
	str << v;
	return str.str();
}
inline std::string to_string(int8_t v)
{
	std::stringstream str;
	str << v;
	return str.str();
}
inline std::string to_string(int16_t v)
{
	std::stringstream str;
	str << v;
	return str.str();
}
inline std::string to_string(int32_t v)
{
	std::stringstream str;
	str << v;
	return str.str();
}
inline std::string to_string(float v)
{
	std::stringstream str;
	str << v;
	return str.str();
}
inline std::string to_string(double v)
{
	std::stringstream str;
	str << v;
	return str.str();
}
template<class T>
std::string to_string(vec2<T> const& v)
{
	std::stringstream str;
	str << v.x << ',' << v.y;
	return str.str();
}
template<class T>
std::string to_string(vec3<T> const& v)
{
	std::stringstream str;
	str << v.x << ',' << v.y << ',' << v.z;
	return str.str();
}
template<class T>
std::string to_string(vec4<T> const& v)
{
	std::stringstream str;
	str << v.x << ',' << v.y << ',' << v.z << ',' << v.w;
	return str.str();
}
template<class T>
std::string to_string(mat2<T> const& v)
{
	std::stringstream str;
	str << v[0] << ',' << v[1] << ',' 
		<< v[2] << ',' << v[3];
	return str.str();
}
template<class T>
std::string to_string(mat3<T> const& v)
{
	std::stringstream str;
	str << v[0] << ',' << v[1] << ',' << v[2] << ',' 
		<< v[3] << ',' << v[4] << ',' << v[5] << ',' 
		<< v[6] << ',' << v[7] << ',' << v[8];
	return str.str();
}
template<class T>
std::string to_string(mat4<T> const& v)
{
	std::stringstream str;
	str << v[0] << ',' << v[1] << ',' << v[2] << ',' << v[3] << ','
		<< v[4] << ',' << v[5] << ',' << v[6] << ',' << v[7] << ','
		<< v[8] << ',' << v[9] << ',' << v[10] << ',' << v[11] << ','
		<< v[12] << ',' << v[13] << ',' << v[14] << ',' << v[15];
	return str.str();
}
template<class T>
std::string to_string(quat<T> const& v)
{
	std::stringstream str;
	str << v.x << ',' << v.y << ',' << v.z << ',' << v.w;
	return str.str();
}


inline bool from_string(uint8_t& v, std::string const& s)
{
	std::stringstream str(s);
	return (str >> v).fail();
}
inline bool from_string(uint16_t& v, std::string const& s)
{
	std::stringstream str(s);
	return (str >> v).fail();
}
inline bool from_string(uint32_t& v, std::string const& s)
{
	std::stringstream str(s);
	return (str >> v).fail();
}
inline bool from_string(int8_t& v, std::string const& s)
{
	std::stringstream str(s);
	return (str >> v).fail();
}
inline bool from_string(int16_t& v, std::string const& s)
{
	std::stringstream str(s);
	return (str >> v).fail();
}
inline bool from_string(int32_t& v, std::string const& s)
{
	std::stringstream str(s);
	return (str >> v).fail();
}
inline bool from_string(float& v, std::string const& s)
{
	std::stringstream str(s);
	return (str >> v).fail();
}
inline bool from_string(double& v, std::string const& s)
{
	std::stringstream str(s);
	return (str >> v).fail();
}

template <class T>
bool from_string(vec2<T>& v, std::string const& s)
{
	std::stringstream str(s);
	char comma;
	vec2<T> m;
	if ((str >> m.x >> comma >> m.y).fail())
	{
		return false;
	}
	v = m;
	return true;
}

template <class T>
bool from_string(vec3<T>& v, std::string const& s)
{
	std::stringstream str(s);
	char comma;
	vec3<T> m;
	if ((str >> m.x >> comma >> m.y >> comma >> m.z).fail())
	{
		return false;
	}
	v = m;
	return true;
}

template <class T>
bool from_string(vec4<T>& v, std::string const& s)
{
	std::stringstream str(s);
	char comma;
	vec4<T> m;
	if ((str >> m.x >> comma >> m.y >> comma >> m.z >> comma >> m.w).fail())
	{
		return false;
	}
	v = m;
	return true;
}

template <class T>
bool from_string(mat2<T>& v, std::string const& s)
{
	std::stringstream str(s);
	char comma;
	mat2<T> m;
	for (uint32_t i = 0; i < 4; i++)
	{
		if ((str >> m[i] >> comma).fail())
		{
			return false;
		}
	}
	v = m;
	return true;
}

template <class T>
bool from_string(mat3<T>& v, std::string const& s)
{
	std::stringstream str(s);
	char comma;
	mat3<T> m;
	for (uint32_t i = 0; i < 9; i++)
	{
		if ((str >> m[i] >> comma).fail())
		{
			return false;
		}
	}
	v = m;
	return true;
}

template <class T>
bool from_string(mat4<T>& v, std::string const& s)
{
	std::stringstream str(s);
	char comma;
	mat4<T> m;
	for (uint32_t i = 0; i < 16; i++)
	{
		if ((str >> m[i] >> comma).fail())
		{
			return false;
		}
	}
	v = m;
	return true;
}

template <class T>
bool from_string(quat<T>& v, std::string const& s)
{
	std::stringstream str(s);
	char comma;
	quat<T> m;
	if ((str >> m.x >> comma >> m.y >> comma >> m.z >> comma >> m.w).fail())
	{
		return false;
	}
	v = m;
	return true;
}

}

#endif //STD_STRING_CONVERSIONS


namespace util
{
namespace formatting
{

	template<class Dst_Adapter, class Placeholder, class T>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, math::vec2<T> const& p)
	{
		format_string(dst, ph, p.x);
		dst.append(',');
		format_string(dst, ph, p.y);
	}

	template<class Dst_Adapter, class Placeholder, class T>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, math::vec3<T> const& p)
	{
		format_string(dst, ph, p.x);
		dst.append(',');
		format_string(dst, ph, p.y);
		dst.append(',');
		format_string(dst, ph, p.z);
	}

	template<class Dst_Adapter, class Placeholder, class T>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, math::vec4<T> const& p)
	{
		format_string(dst, ph, p.x);
		dst.append(',');
		format_string(dst, ph, p.y);
		dst.append(',');
		format_string(dst, ph, p.z);
		dst.append(',');
		format_string(dst, ph, p.w);
	}

	template<class Dst_Adapter, class Placeholder, class T>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, math::quat<T> const& p)
	{
		format_string(dst, ph, p.x);
		dst.append(',');
		format_string(dst, ph, p.y);
		dst.append(',');
		format_string(dst, ph, p.z);
		dst.append(',');
		format_string(dst, ph, p.w);
	}

}
}
