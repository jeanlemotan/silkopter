#pragma once

namespace math
{

	//////////////////////////////////////////////////////////////////////////

	template<typename T> MATH_FORCE_INLINE T abs(T v)
	{
		return v < 0 ? -v : v;
	}
	template<> MATH_FORCE_INLINE float abs(float v)
	{
		auto* __restrict v2 = reinterpret_cast<uint32_t*>(&v);
		uint32_t a = (*v2) & 0x7fffffff;
		auto* __restrict a2 = reinterpret_cast<float*>(&a);
		float f = *a2;
		return f;
	}
	template<> MATH_FORCE_INLINE int64_t abs(int64_t a)
	{
		return a < 0 ? -a : a;
	}
	template<typename T> MATH_FORCE_INLINE angle<T> abs(angle<T> const& v)
	{
		return angle<T>(abs(v.radians));
	}
	template<typename T> MATH_FORCE_INLINE vec2<T> abs(vec2<T> const& v)
	{
		return vec2<T>(abs(v.x), abs(v.y));
	}
	template<typename T> MATH_FORCE_INLINE vec2<T> sgn(vec2<T> const& v)
	{
		return vec2<T>(sgn(v.x), sgn(v.y));
	}
	template<typename T> MATH_FORCE_INLINE vec3<T> abs(vec3<T> const& v)
	{
		return vec3<T>(abs(v.x), abs(v.y), abs(v.z));
	}
	template<typename T> MATH_FORCE_INLINE vec3<T> sgn(vec3<T> const& v)
	{
		return vec3<T>(sgn(v.x), sgn(v.y), sgn(v.z));
	}
	template<typename T> MATH_FORCE_INLINE vec4<T> abs(vec4<T> const& v)
	{
		return vec4<T>(abs(v.x), abs(v.y), abs(v.z), abs(v.w));
	}

	//////////////////////////////////////////////////////////////////////////

	template<typename T> MATH_FORCE_INLINE T sgn(T const& v)
	{
		return v < T(0) ? T(-1) : v == T(0) ? T(0) : T(1);
	}
	template<typename T> MATH_FORCE_INLINE T sgn(angle<T> const& v)
	{
		return sgn(v.radians);
	}
	template<typename T> MATH_FORCE_INLINE vec4<T> sgn(vec4<T> const& v)
	{
		return vec4<T>(sgn(v.x), sgn(v.y), sgn(v.z), sgn(v.w));
	}

	//////////////////////////////////////////////////////////////////////////

	template<typename T> MATH_FORCE_INLINE T sqrt(T const& v);
	template<> MATH_FORCE_INLINE float sqrt(float const& v)
	{
		return ::sqrtf(v);
	}
	template<typename T> MATH_FORCE_INLINE vec2<T> sqrt(vec2<T> const& v)
	{
		return vec2<T>(sqrt(v.x), sqrt(v.y));
	}
	template<typename T> MATH_FORCE_INLINE vec3<T> sqrt(vec3<T> const& v)
	{
		return vec3<T>(sqrt(v.x), sqrt(v.y), sqrt(v.z));
	}
	template<typename T> MATH_FORCE_INLINE vec4<T> sqrt(vec4<T> const& v)
	{
		return vec4<T>(sqrt(v.x), sqrt(v.y), sqrt(v.z), sqrt(v.w));
	}
	//////////////////////////////////////////////////////////////////////////


	template<typename T> MATH_FORCE_INLINE T inv_sqrt(T const& v)
	{
		assert(!is_zero(v));
		return 1 / sqrt(v);
	}
	template<typename T> MATH_FORCE_INLINE vec2<T> inv_sqrt(vec2<T> const& v)
	{
		return vec2<T>(inv_sqrt(v.x), inv_sqrt(v.y));
	}
	template<typename T> MATH_FORCE_INLINE vec3<T> inv_sqrt(vec3<T> const& v)
	{
		return vec3<T>(inv_sqrt(v.x), inv_sqrt(v.y), inv_sqrt(v.z));
	}
	template<typename T> MATH_FORCE_INLINE vec4<T> inv_sqrt(vec4<T> const& v)
	{
		return vec4<T>(inv_sqrt(v.x), inv_sqrt(v.y), inv_sqrt(v.z), inv_sqrt(v.w));
	}

	//////////////////////////////////////////////////////////////////////////

	template<typename T> MATH_FORCE_INLINE T pow(T const& a, T const& b);
	template<> MATH_FORCE_INLINE float pow(float const& a, float const& b)
	{
		return powf(a, b);
	}
	template<typename T> MATH_FORCE_INLINE vec2<T> pow(vec2<T> const& a, vec2<T> const& b)
	{
		return vec2<T>(pow(a.x, b.x), pow(a.y, b.y));
	}
	template<typename T> MATH_FORCE_INLINE vec3<T> pow(vec3<T> const& a, vec3<T> const& b)
	{
		return vec3<T>(pow(a.x, b.x), pow(a.y, b.y), pow(a.z, b.z));
	}
	template<typename T> MATH_FORCE_INLINE vec4<T> pow(vec4<T> const& a, vec4<T> const& b)
	{
		return vec4<T>(pow(a.x, b.x), pow(a.y, b.y), pow(a.z, b.z), pow(a.w, b.w));
	}

	//////////////////////////////////////////////////////////////////////////

	template<> MATH_FORCE_INLINE float positive_zero(float const& v)
	{
		return v == -0.f ? 0.f : v;
	}
	template<typename T> MATH_FORCE_INLINE T positive_zero(T const& v)
	{
		return v;
	}
	template<typename T> MATH_FORCE_INLINE angle<T> positive_zero(angle<T> const& v)
	{
		return angle<T>(positive_zero(v.x));
	}
	template<typename T> MATH_FORCE_INLINE vec2<T> positive_zero(vec2<T> const& v)
	{
		return vec2<T>(positive_zero(v.x), positive_zero(v.y));
	}
	template<typename T> MATH_FORCE_INLINE vec3<T> positive_zero(vec3<T> const& v)
	{
		return vec3<T>(positive_zero(v.x), positive_zero(v.y), positive_zero(v.z));
	}
	template<typename T> MATH_FORCE_INLINE vec4<T> positive_zero(vec4<T> const& v)
	{
		return vec4<T>(positive_zero(v.x), positive_zero(v.y), positive_zero(v.z), positive_zero(v.w));
	}
	template<typename T> MATH_FORCE_INLINE quat<T> positive_zero(quat<T> const& v)
	{
		return quat<T>(positive_zero(v.x), positive_zero(v.y), positive_zero(v.z), positive_zero(v.w));
	}
	template<typename T> MATH_FORCE_INLINE T inverse(T const& v)
	{
		assert(v != 0);
		return 1 / v;
	}
	template<typename T> MATH_FORCE_INLINE mat2<T> inverse(mat2<T> const& v)
	{
		mat2<T> tmp(v);
		bool res = tmp.invert();
		assert(res);
		return tmp;
	}
	template<typename T> MATH_FORCE_INLINE mat3<T> inverse(mat3<T> const& v)
	{
		mat3<T> tmp(v);
		bool res = tmp.invert();
		assert(res);
		return tmp;
	}
	template<typename T> MATH_FORCE_INLINE trans2d<T> inverse(const trans2d<T>& v)
	{
		trans2d<T> tmp(v);
		bool res = tmp.invert();
		assert(res);
		return tmp;
	}
	template<typename T> MATH_FORCE_INLINE mat4<T> inverse(const mat4<T>& v)
	{
		mat4<T> tmp(v);
		bool res = tmp.invert();
		assert(res);
		return tmp;
	}
	template<typename T> MATH_FORCE_INLINE trans3d<T> inverse(trans3d<T> const& v)
	{
		trans3d<T> tmp(v);
		bool res = tmp.invert();
		assert(res);
		return tmp;
	}
	template<typename T> MATH_FORCE_INLINE quat<T> inverse(quat<T> const& v)
	{
		quat<T> tmp(v);
		tmp.invert();
		return tmp;
	}
	template<typename T> MATH_FORCE_INLINE mat2<T> transposed(mat2<T> const& v)
	{
		mat2<T> tmp(v);
		tmp.transpose();
		return tmp;
	}
	template<typename T> MATH_FORCE_INLINE mat3<T> transposed(mat3<T> const& v)
	{
		mat3<T> tmp(v);
		tmp.transpose();
		return tmp;
	}
	template<typename T> MATH_FORCE_INLINE mat4<T> transposed(mat4<T> const& v)
	{
		mat4<T> tmp(v);
		tmp.transpose();
		return tmp;
	}
	template<typename T> MATH_FORCE_INLINE angle<T> normalized(angle<T> const& v)
	{
		angle<T> x(v);
		x.normalize();
		return x;
	}
	template<typename T> MATH_FORCE_INLINE vec2<T> normalized(vec2<T> const& v)
	{
		T len = length(v);
		assert(len > 0);
		return v / len;
	}
	template<typename T> MATH_FORCE_INLINE vec3<T> normalized(vec3<T> const& v)
	{
		T len = length(v);
		assert(len > 0);
		return v / len;
	}
	template<typename T> MATH_FORCE_INLINE vec4<T> normalized(vec4<T> const& v)
	{
		T len = length(v);
		assert(len > 0);
		return v / len;
	}

	template<typename T> MATH_FORCE_INLINE quat<T> normalized(quat<T> const& v)
	{
		quat<T> tmp(v);
		tmp.normalize();
		return tmp;
	}
	template<typename T> MATH_FORCE_INLINE T length_sq(vec2<T> const& v)
	{
		return dot(v, v);
	}
	template<typename T> MATH_FORCE_INLINE T length_sq(vec3<T> const& v)
	{
		return dot(v, v);
	}
	template<typename T> MATH_FORCE_INLINE T length_sq(vec4<T> const& v)
	{
		return dot(v, v);
	}
	template<typename T> MATH_FORCE_INLINE T length_sq(quat<T> const& v)
	{
		return dot(v, v);
	}
	template<typename T> MATH_FORCE_INLINE T length(vec2<T> const& v)
	{
		return sqrt(length_sq(v));
	}
	template<typename T> MATH_FORCE_INLINE T length(vec3<T> const& v)
	{
		return sqrt(length_sq(v));
	}
	template<typename T> MATH_FORCE_INLINE T length(vec4<T> const& v)
	{
		return sqrt(length_sq(v));
	}

	template<typename T> MATH_FORCE_INLINE T length(quat<T> const& v)
	{
		return sqrt(length_sq(v));
	}
	template <typename T> MATH_FORCE_INLINE T dot(vec2<T> const& v1, vec2<T> const& v2)
	{
		return v1.x*v2.x + v1.y*v2.y;
	}
	template <typename T> MATH_FORCE_INLINE T dot(vec3<T> const& v1, vec3<T> const& v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	}
	template <typename T> MATH_FORCE_INLINE T dot(vec4<T> const& v1, vec4<T> const& v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
	}

	template<typename T> MATH_FORCE_INLINE T cross(vec2<T> const& v1, vec2<T> const& v2)
	{
		return (v1.x*v2.y - v1.y*v2.x);
	}
	template<typename T> MATH_FORCE_INLINE vec3<T> cross(vec3<T> const& v1, vec3<T> const& v2)
	{
		return vec3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}
	template<typename T> vec4<T> cross(vec4<T> const& x1, vec4<T> const& x2, vec4<T> const& x3)
	{
		// 4d Cross product (GPG6, section 2.2)
#define DOT(AX,AY,AZ,BX,BY,BZ) AX*BX+AY*BY+AZ*BZ
#define CROSS(AX,AY,AZ,BX,BY,BZ,CX,CY,CZ) CX = AY*BZ - AZ*BY; CY = AZ*BX - AX*BZ; CZ = AX*BY - AY*BX;
		vec4<T> c1(vec4<T>::uninitialized);
		vec4<T> c2(vec4<T>::uninitialized);
		vec4<T> c3(vec4<T>::uninitialized);
		vec4<T> c4(vec4<T>::uninitialized);
		CROSS(x2.y,x2.z,x2.w,x3.y,x3.z,x3.w,c1.x,c1.y,c1.z);
		CROSS(x2.x,x2.z,x2.w,x3.x,x3.z,x3.w,c2.x,c2.y,c2.z);
		CROSS(x2.x,x2.y,x2.w,x3.x,x3.y,x3.w,c3.x,c3.y,c3.z);
		CROSS(x2.x,x2.y,x2.z,x3.x,x3.y,x3.z,c4.x,c4.y,c4.z);

		return vec4<T>(
			DOT(x1.y,x1.z,x1.w,c1.x,c1.y,c1.z),
			-DOT(x1.x,x1.z,x1.w,c2.x,c2.y,c2.z),
			DOT(x1.x,x1.y,x1.w,c3.x,c3.y,c3.z),
			-DOT(x1.x,x1.y,x1.z,c4.x,c4.y,c4.z)
			);

#undef DOT
#undef CROSS
	}
	template<typename T> MATH_FORCE_INLINE T square(T const& v)
	{
		return v*v;
	}
	template<typename T> MATH_FORCE_INLINE angle<T> distance(angle<T> const& v1, angle<T> const& v2)
	{
		auto start = v1.radians;
		auto end = v2.radians;
		auto difference = abs(end - start);
		if (difference > angle<T>::pi.radians)
		{
			// We need to add on to one of the values.
			if (end > start)
			{
				// We'll add it on to start...
				start += angle<T>::_2pi.radians;
			}
			else
			{
				// Add it on to end.
				end += angle<T>::_2pi.radians;
			}
		}
		return angle<T>(start - end);
	}
	template<typename T> MATH_FORCE_INLINE T distance(vec2<T> const& v1, vec2<T> const& v2)
	{
		return sqrt(distance_sq(v1, v2));
	}
	template<typename T> MATH_FORCE_INLINE T distance(vec3<T> const& v1, vec3<T> const& v2)
	{
		return sqrt(distance_sq(v1, v2));
	}
	template<typename T> MATH_FORCE_INLINE T distance(vec4<T> const& v1, vec4<T> const& v2)
	{
		return sqrt(distance_sq(v1, v2));
	}

	template<typename T> MATH_FORCE_INLINE T distance(line3<T> const& l1, line3<T> const& l2)
	{
		return sqrt(distance_sq(l1, l2));
	}
	template<typename T> MATH_FORCE_INLINE T distance_sq(vec2<T> const& v1, vec2<T> const& v2)
	{
		auto v = v2 - v1;
		return dot(v, v);
	}
	template<typename T> MATH_FORCE_INLINE T distance_sq(vec3<T> const& v1, vec3<T> const& v2)
	{
		auto v = v2 - v1;
		return dot(v, v);
	}
	template<typename T> MATH_FORCE_INLINE T distance_sq(vec4<T> const& v1, vec4<T> const& v2)
	{
		auto v = v2 - v1;
		return dot(v, v);
	}
	template<typename T> T distance_sq(line3<T> const& l1, line3<T> const& l2)
	{
		vec3<T> u(l1.end - l1.start);
		vec3<T> v(l2.end - l2.start);
		vec3<T> w(l1.start - l2.start);
		T    a = dot(u, u);         // always >= 0
		T    b = dot(u, v);
		T    c = dot(v, v);         // always >= 0
		T    d = dot(u, w);
		T    e = dot(v, w);
		T    D = a*c - b*b;        // always >= 0
		T    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
		T    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

		// compute the line parameters of the two closest points
		if (D < std::numeric_limits<T>::epsilon()) 
		{ // the lines are almost parallel
			sN = T(0);         // force using point P0 on segment S1
			sD = T(1);         // to prevent possible division by 0.0 later
			tN = e;
			tD = c;
		}
		else 
		{                 // get the closest points on the infinite lines
			sN = (b*e - c*d);
			tN = (a*e - b*d);
			if (sN < T(0)) 
			{        // sc < 0 => the s=0 edge is visible
				sN = T(0);
				tN = e;
				tD = c;
			}
			else if (sN > sD) 
			{  // sc > 1  => the s=1 edge is visible
				sN = sD;
				tN = e + b;
				tD = c;
			}
		}

		if (tN < T(0)) 
		{            // tc < 0 => the t=0 edge is visible
			tN = T(0);
			// recompute sc for this edge
			if (-d < T(0))
			{
				sN = T(0);
			}
			else if (-d > a)
			{
				sN = sD;
			}
			else 
			{
				sN = -d;
				sD = a;
			}
		}
		else if (tN > tD) 
		{      // tc > 1  => the t=1 edge is visible
			tN = tD;
			// recompute sc for this edge
			if ((-d + b) < T(0))
			{
				sN = 0;

			}
			else if ((-d + b) > a)
			{
				sN = sD;
			}
			else 
			{
				sN = (-d +  b);
				sD = a;
			}
		}
		// finally do the division to get sc and tc
		sc = (abs(sN) < std::numeric_limits<T>::epsilon() ? T(0) : sN / sD);
		tc = (abs(tN) < std::numeric_limits<T>::epsilon() ? T(0) : tN / tD);

		// get the difference of the two closest points
		vec3<T> dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)
		return length_sq(dP);   // return the closest distance
	}

	namespace batch
	{
		template<typename T> void dot(T* dst, size_t dstStride, vec3<T> const* src1, size_t src1Stride, vec3<T> const* src2, size_t src2Stride, size_t count)
		{
			auto* __restrict d = dst;
			auto const* __restrict s1 = src1;
			auto const* __restrict s2 = src2;

			assert(src1 && src2 && dst && count);
			for (size_t i = 0; i < count; ++i)
			{
				*d = dot(*s1, *s2);
				s1 = (vec3<T> const* __restrict)(((char const* __restrict)s1) + src1Stride);
				s2 = (vec3<T> const* __restrict)(((char const* __restrict)s2) + src2Stride);
				d = (T* __restrict)(((char* __restrict)d) + dstStride);
			}
		}
		template<typename T> void dot(T* dst, size_t dstStride, vec4<T> const* src1, size_t src1Stride, vec4<T> const* src2, size_t src2Stride, size_t count)
		{
			auto* __restrict d = dst;
			auto const* __restrict s1 = src1;
			auto const* __restrict s2 = src2;

			assert(src1 && src2 && dst && count);
			for (size_t i = 0; i < count; ++i)
			{
				*d = dot(*s1, *s2);
				s1 = (vec4<T> const* __restrict)(((char const* __restrict)s1) + src1Stride);
				s2 = (vec4<T> const* __restrict)(((char const* __restrict)s2) + src2Stride);
				d = (T* __restrict)(((char* __restrict)d) + dstStride);
			}
		}
	}

}