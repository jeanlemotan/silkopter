namespace math
{
	template<typename T> inline trans3d<T>& multiply(trans3d<T>& result, trans3d<T> const& a, trans3d<T> const& b)
	{
		T const* m1 = a.mat.data();
		T const* m2 = b.mat.data();
		T * m3 = result.mat.data();
        QASSERT(m3 != m1 && m3 != m2);

		m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2];// + m1[12]*0;
		m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2];// + m1[13]*0;
		m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2];// + m1[14]*0;
		m3[3] = 0.f;//0*m2[0] + 0*m2[1] + 0*m2[2] + 1*0;

		m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6];// + m1[12]*0;
		m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6];// + m1[13]*0;
		m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6];// + m1[14]*0;
		m3[7] = 0.f;//0*m2[4] + 0*m2[5] + 0*m2[6] + 1*0;

		m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10];// + m1[12]*0;
		m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10];// + m1[13]*0;
		m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10];// + m1[14]*0;
		m3[11] = 0.f;//0*m2[8] + 0*m2[9] + 0*m2[10] + 1*0;

		m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12];//*1;
		m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13];//*1;
		m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14];//*1;
		m3[15] = 1.f;//0*m2[12] + 0*m2[13] + 0*m2[14] + 1*1;

		return result;
	}

	//////////////////////////////////////////////////////////////////////////

	template<typename T> inline trans2d<T>& multiply(trans2d<T>& result, trans2d<T> const& a, trans2d<T> const& b)
	{
		T const* m1 = a.mat.data();
		T const* m2 = b.mat.data();
		T * m3 = result.mat.data();
        QASSERT(&result != &a && &result != &b);

		m3[0] = m1[0]*m2[0] + m1[3]*m2[1] + m1[6]*m2[2];
		m3[1] = m1[1]*m2[0] + m1[4]*m2[1] + m1[7]*m2[2];
		m3[2] = 0;//m1[2]*m2[0] + m1[5]*m2[1] + m1[8]*m2[2];

		m3[3] = m1[0]*m2[3] + m1[3]*m2[4] + m1[6]*m2[5];
		m3[4] = m1[1]*m2[3] + m1[4]*m2[4] + m1[7]*m2[5];
		m3[5] = 0;//m1[2]*m2[3] + m1[5]*m2[4] + m1[8]*m2[5];

		m3[6] = m1[0]*m2[6] + m1[3]*m2[7] + m1[6]*m2[8];
		m3[7] = m1[1]*m2[6] + m1[4]*m2[7] + m1[7]*m2[8];
		m3[8] = 1;//m1[2]*m2[6] + m1[5]*m2[7] + m1[8]*m2[8];

		return result;
	}


	//////////////////////////////////////////////////////////////////////////

	template<typename T> inline mat4<T>& multiply(mat4<T>& result, mat4<T> const& a, mat4<T> const& b)
	{
		T const* m1 = a.data();
		T const* m2 = b.data();
		T * m3 = result.data();
        QASSERT(&result != &a && &result != &b);

		m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

		m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

		m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

		m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

		return result;
	}

	template<typename T> inline mat3<T>& multiply(mat3<T>& result, mat3<T> const& a, mat3<T> const& b)
	{
		T const* m1 = a.data();
		T const* m2 = b.data();
		T * m3 = result.data();
        QASSERT(&result != &a && &result != &b);

		m3[0] = m1[0]*m2[0] + m1[3]*m2[1] + m1[6]*m2[2];
		m3[1] = m1[1]*m2[0] + m1[4]*m2[1] + m1[7]*m2[2];
		m3[2] = m1[2]*m2[0] + m1[5]*m2[1] + m1[8]*m2[2];

		m3[3] = m1[0]*m2[3] + m1[3]*m2[4] + m1[6]*m2[5];
		m3[4] = m1[1]*m2[3] + m1[4]*m2[4] + m1[7]*m2[5];
		m3[5] = m1[2]*m2[3] + m1[5]*m2[4] + m1[8]*m2[5];

		m3[6] = m1[0]*m2[6] + m1[3]*m2[7] + m1[6]*m2[8];
		m3[7] = m1[1]*m2[6] + m1[4]*m2[7] + m1[7]*m2[8];
		m3[8] = m1[2]*m2[6] + m1[5]*m2[7] + m1[8]*m2[8];

		return result;
	}

	//////////////////////////////////////////////////////////////////////////

	template<typename T> inline mat2<T>& multiply(mat2<T>& result, mat2<T> const& a, mat2<T> const& b)
	{
		T const* m1 = a.data();
		T const* m2 = b.data();
		T * m3 = result.data();
        QASSERT(&result != &a && &result != &b);

		m3[0] = m1[0]*m2[0] + m1[2]*m2[1];
		m3[1] = m1[1]*m2[0] + m1[3]*m2[1];

		m3[2] = m1[0]*m2[2] + m1[2]*m2[3];
		m3[3] = m1[1]*m2[2] + m1[3]*m2[3];

		return result;
	}


	namespace cwise
	{
		template<typename T> inline mat4<T>& multiply(mat4<T>& result, mat4<T> const& a, mat4<T> const& b)
		{
			T const* m1 = a.data();
			T const* m2 = b.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat4<T>::element_count; i++)
			{
				m3[i] = m1[i] * m2[i];
			}
			return result;
		}

		template<typename T> inline mat4<T>& add(mat4<T>& result, mat4<T> const& a, mat4<T> const& b)
		{
			T const* m1 = a.data();
			T const* m2 = b.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat4<T>::element_count; i++)
			{
				m3[i] = m1[i] + m2[i];
			}
			return result;
		}
		template<typename T> inline mat4<T>& substract(mat4<T>& result, mat4<T> const& a, mat4<T> const& b)
		{
			T const* m1 = a.data();
			T const* m2 = b.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat4<T>::element_count; i++)
			{
				m3[i] = m1[i] - m2[i];
			}
			return result;
		}
		template<typename T> inline mat4<T>& multiply(mat4<T>& result, mat4<T> const& a, T b)
		{
			T const* m1 = a.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat4<T>::element_count; i++)
			{
				m3[i] = m1[i] * b;
			}
			return result;
		}
		template<typename T> inline mat4<T>& add(mat4<T>& result, mat4<T> const& a, T b)
		{
			T const* m1 = a.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat4<T>::element_count; i++)
			{
				m3[i] = m1[i] + b;
			}
			return result;
		}
		template<typename T> inline mat4<T>& substract(mat4<T>& result, mat4<T> const& a, T b)
		{
			T const* m1 = a.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat4<T>::element_count; i++)
			{
				m3[i] = m1[i] - b;
			}
			return result;
		}

		//////////////////////////////////////////////////////////////////////////

		template<typename T> inline mat3<T>& multiply(mat3<T>& result, mat3<T> const& a, mat3<T> const& b)
		{
			T const* m1 = a.data();
			T const* m2 = b.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat3<T>::element_count; i++)
			{
				m3[i] = m1[i] * m2[i];
			}
			return result;
		}
		template<typename T> inline mat3<T>& add(mat3<T>& result, mat3<T> const& a, mat3<T> const& b)
		{
			T const* m1 = a.data();
			T const* m2 = b.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat3<T>::element_count; i++)
			{
				m3[i] = m1[i] + m2[i];
			}
			return result;
		}
		template<typename T> inline mat3<T>& substract(mat3<T>& result, mat3<T> const& a, mat3<T> const& b)
		{
			T const* m1 = a.data();
			T const* m2 = b.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat3<T>::element_count; i++)
			{
				m3[i] = m1[i] - m2[i];
			}
			return result;
		}
		template<typename T> inline mat3<T>& multiply(mat3<T>& result, mat3<T> const& a, T b)
		{
			T  const* m1 = a.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat3<T>::element_count; i++)
			{
				m3[i] = m1[i] * b;
			}
			return result;
		}
		template<typename T> inline mat3<T>& add(mat3<T>& result, mat3<T> const& a, T b)
		{
			T const* m1 = a.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat3<T>::element_count; i++)
			{
				m3[i] = m1[i] + b;
			}
			return result;
		}
		template<typename T> inline mat3<T>& substract(mat3<T>& result, mat3<T> const& a, T b)
		{
			T const* m1 = a.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat3<T>::element_count; i++)
			{
				m3[i] = m1[i] - b;
			}
			return result;
		}

		//////////////////////////////////////////////////////////////////////////

		template<typename T> inline mat2<T>& multiply(mat2<T>& result, mat2<T> const& a, mat2<T> const& b)
		{
			T const* m1 = a.data();
			T const* m2 = b.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat2<T>::element_count; i++)
			{
				m3[i] = m1[i] * m2[i];
			}
			return result;
		}
		template<typename T> inline mat2<T>& add(mat2<T>& result, mat2<T> const& a, mat2<T> const& b)
		{
			T const* m1 = a.data();
			T const* m2 = b.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat2<T>::element_count; i++)
			{
				m3[i] = m1[i] + m2[i];
			}
			return result;
		}
		template<typename T> inline mat2<T>& substract(mat2<T>& result, mat2<T> const& a, mat2<T> const& b)
		{
			T const* m1 = a.data();
			T const* m2 = b.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat2<T>::element_count; i++)
			{
				m3[i] = m1[i] - m2[i];
			}
			return result;
		}
		template<typename T> inline mat2<T>& multiply(mat2<T>& result, mat2<T> const& a, T b)
		{
			T const* m1 = a.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat2<T>::element_count; i++)
			{
				m3[i] = m1[i] * b;
			}
			return result;
		}
		template<typename T> inline mat2<T>& add(mat2<T>& result, mat2<T> const& a, T b)
		{
			T const* m1 = a.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat2<T>::element_count; i++)
			{
				m3[i] = m1[i] + b;
			}
			return result;
		}
		template<typename T> inline mat2<T>& substract(mat2<T>& result, mat2<T> const& a, T b)
		{
			T const* m1 = a.data();
			T * m3 = result.data();
			for (uint8_t i = 0; i < mat2<T>::element_count; i++)
			{
				m3[i] = m1[i] - b;
			}
			return result;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// mat2 vec2 transforms
	//////////////////////////////////////////////////////////////////////////

	template<typename T> inline vec2<T> transform(mat2<T> const& mat, vec2<T> const& v)
	{
		T const* m = mat.data();
		return vec2<T>(
			v.x*m[0] + v.y*m[2],
			v.x*m[1] + v.y*m[3]
		);
	}

	//////////////////////////////////////////////////////////////////////////
	// mat3 vec2 transforms
	//////////////////////////////////////////////////////////////////////////

	template<typename T> inline vec2<T> transform(mat3<T> const& mat, vec2<T> const& v)
	{
		T const* m = mat.data();
		return vec2<T>(
			v.x*m[0] + v.y*m[3] + m[6],
			v.x*m[1] + v.y*m[4] + m[7]
		);
	}
	template<typename T> inline vec2<T> transform(trans2d<T> const& t, vec2<T> const& v)
	{
		T const* m = t.mat.data();
		return vec2<T>(
			v.x*m[0] + v.y*m[3] + m[6],
			v.x*m[1] + v.y*m[4] + m[7]
		);
	}

	//////////////////////////////////////////////////////////////////////////
	// mat4 vec4 transforms
	//////////////////////////////////////////////////////////////////////////

	template<typename T> inline vec4<T> transform(mat4<T> const& mat, vec4<T> const& vect)
	{
		T const* m = mat.data();
		return vec4<T>(
			vect.x*m[0] + vect.y*m[4] + vect.z*m[ 8] + vect.w*m[12],
			vect.x*m[1] + vect.y*m[5] + vect.z*m[ 9] + vect.w*m[13],
			vect.x*m[2] + vect.y*m[6] + vect.z*m[10] + vect.w*m[14],
			vect.x*m[3] + vect.y*m[7] + vect.z*m[11] + vect.w*m[15]
		);
	}

	template<typename T> inline aabb3<T> transform(mat4<T> const& m, aabb3<T> const& v)
	{
		// compute projected radius interval
		auto hs = v.get_extent() * 0.5f;
		vec3<T> tmp;
		tmp.x = abs(m.m[0])*hs.x + abs(m.m[1])*hs.y + abs(m.m[ 2])*hs.z;
		tmp.y = abs(m.m[4])*hs.x + abs(m.m[5])*hs.y + abs(m.m[ 6])*hs.z;
		tmp.z = abs(m.m[8])*hs.x + abs(m.m[9])*hs.y + abs(m.m[10])*hs.z;

		auto center = transform(m, v.get_center());
		return aabb3<T>(center - tmp, center + tmp);
	}
	template<typename T> inline aabb3<T> transform(trans3d<T> const& m, aabb3<T> const& v)
	{
		return transform(m.mat, v);
	}
	template<typename T> inline aabb2<T> transform(trans2d<T> const& m, aabb2<T> const& v)
	{
		return transform(m.mat, v);
	}

	template<typename T> inline aabb2<T> transform(mat3<T> const& m, aabb2<T> const& v)
	{
		// compute projected radius interval
		auto hs = v.get_extent() * 0.5f;
		vec2<T> tmp;
		tmp.x = abs(m.m[0])*hs.x + abs(m.m[1])*hs.y;
		tmp.y = abs(m.m[3])*hs.x + abs(m.m[4])*hs.y;

		auto center = transform(m, v.get_center());
		return aabb2<T>(center - tmp, center + tmp);
	}

	template<typename T> inline vec4<T> project(mat4<T> const& m, vec4<T> const& v)
	{
		vec4<T> tmp = transform(m, v);
		if (!is_zero(tmp.w))
		{
			T invW = T(1) / tmp.w;
			return vec4<T>(vec3<T>(tmp) * invW, tmp.w);
		}
		else
		{
			return tmp;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// mat3 vec3 transforms
	//////////////////////////////////////////////////////////////////////////

	template<typename T> inline vec3<T> transform(mat3<T> const& mat, vec3<T> const& v)
	{
		T const* m = mat.data();
		return vec3<T>(
			v.x*m[0] + v.y*m[3] + v.z*m[6],
			v.x*m[1] + v.y*m[4] + v.z*m[7],
			v.x*m[2] + v.y*m[5] + v.z*m[8]
		);
	}

	template<typename T> inline vec2<T> rotate(mat3<T> const& mat, vec2<T> const& v)
	{
		T const* m = mat.data();
		return vec2<T>(v.x*m[0] + v.y*m[3], v.x*m[1] + v.y*m[4]);
	}
	template<typename T> inline vec2<T> rotate(trans2d<T> const& t, vec2<T> const& v)
	{
		T const* m = t.mat.data();
		return vec2<T>(v.x*m[0] + v.y*m[3], v.x*m[1] + v.y*m[4]);
	}

	//////////////////////////////////////////////////////////////////////////
	// mat4 vec3 transforms
	//////////////////////////////////////////////////////////////////////////

	template<typename T> inline vec3<T> transform(mat4<T> const& mat, vec3<T> const& vect)
	{
		T const* m = mat.data();
		return vec3<T>(
			vect.x*m[0] + vect.y*m[4] + vect.z*m[ 8] + m[12],
			vect.x*m[1] + vect.y*m[5] + vect.z*m[ 9] + m[13],
			vect.x*m[2] + vect.y*m[6] + vect.z*m[10] + m[14]
		);
	}
	template<typename T> inline vec3<T> rotate(mat4<T> const& mat, vec3<T> const& vect)
	{
		T const* m = mat.data();
		return vec3<T>(
			vect.x*m[0] + vect.y*m[4] + vect.z*m[ 8],
			vect.x*m[1] + vect.y*m[5] + vect.z*m[ 9],
			vect.x*m[2] + vect.y*m[6] + vect.z*m[10]
		);
	}
	template<typename T> inline vec3<T> transform(trans3d<T> const& m, vec3<T> const& vect)
	{
		return transform(m.mat, vect);
	}
	template<typename T> inline vec3<T> rotate(trans3d<T> const& m, vec3<T> const& vect)
	{
		return rotate(m.mat, vect);
	}
	template<typename T> inline vec3<T> rotate(quat<T> const& q, vec3<T> const& vect)
	{
		//Eigen implementation
		vec3<T> uv, uuv;
		vec3<T> qvec(q.x, q.y, q.z);
		uv = cross(qvec, vect);
		uv += uv;
		uuv = cross(qvec, uv);

		return vect + q.w * uv + uuv;
	}
	template<typename T> inline vec3<T> project(mat4<T> const& m, vec3<T> const& v)
	{
		vec4<T> tmp = transform(m, vec4<T>(v, T(1)));
		if (!is_zero(tmp.w))
		{
			T invW = T(1) / tmp.w;
			return vec3<T>(tmp)*invW;
		}
		else
		{
			return vec3<T>(tmp);
		}
	}

	namespace batch
	{
		template<typename T> void transform(trans3d<T> const& t, vec3<T>* dst, size_t dstStride, vec3<T> const* src, size_t srcStride, size_t count)
		{
			transform(t.mat, dst, dstStride, src, srcStride, count);
		}
		template<typename T> void transform(mat4<T> const& m, vec3<T>* dst, size_t dstStride, vec3<T> const* src, size_t srcStride, size_t count)
		{
            QASSERT(src && dst && count);
			for (size_t i = 0; i < count; ++i)
			{
				*dst = transform(m, *src);
				src = (vec3<T> const*)(((char const*)src) + srcStride);
				dst = (vec3<T>*)(((char*)dst) + dstStride);
			}
		}
		template<typename T> void transform(mat4<T> const& m, vec4<T>* dst, size_t dstStride, vec4<T> const* src, size_t srcStride, size_t count)
		{
            QASSERT(src && dst && count);
			for (size_t i = 0; i < count; ++i)
			{
				*dst = transform(m, *src);
				src = (vec4<T> const*)(((char const*)src) + srcStride);
				dst = (vec4<T>*)(((char*)dst) + dstStride);
			}
		}

		//////////////////////////////////////////////////////////////////////////

		template<typename T> void multiply(mat4<T> const& m, mat4<T>* dst, size_t dstStride, mat4<T> const* src, size_t srcStride, size_t count)
		{
            QASSERT(src && dst && count);
			for (size_t i = 0; i < count; ++i)
			{
				multiply(*dst, m, *src);
				src = (mat4<T> const*)(((char const*)src) + srcStride);
				dst = (mat4<T>*)(((char*)dst) + dstStride);
			}
		}
		template<typename T> void multiply(trans3d<T> const& m, trans3d<T>* dst, size_t dstStride, trans3d<T> const* src, size_t srcStride, size_t count)
		{
            QASSERT(src && dst && count);
			for (size_t i = 0; i < count; ++i)
			{
				multiply(*dst, m, *src);
				src = (trans3d<T> const*)(((char const*)src) + srcStride);
				dst = (trans3d<T>*)(((char*)dst) + dstStride);
			}
		}

	}

}
