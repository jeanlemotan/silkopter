#pragma once


namespace q
{
namespace video
{

	class Uniform
	{
	public:

		enum class Type : uint8_t
		{
			BOOL,
			INT,
			FLOAT, 
			VEC2,
			VEC3,
			VEC4,
			MAT3,
			MAT4
		};

	public:
		Uniform();
		Uniform(Type type, size_t count, uint8_t* data = 0);
		Uniform(bool val);
        Uniform(int32_t val);
		Uniform(float val);
		Uniform(math::vec2f const& val);
		Uniform(math::vec3f const& val);
		Uniform(math::vec4f const& val);
		Uniform(const math::mat3f& val);
		Uniform(math::mat4f const& val);
		Uniform(Uniform const& other);
		Uniform(Uniform&& other);
		~Uniform();

		Uniform&	operator=(Uniform const& other);

		bool		operator==(Uniform const& other) const;
		DEFINE_EQ_OPERATORS(Uniform);

		Uniform&	operator=(bool val);
        Uniform&	operator=(int32_t val);
		Uniform&	operator=(float val);
		Uniform&	operator=(math::vec2f const& val);
		Uniform&	operator=(math::vec3f const& val);
		Uniform&	operator=(math::vec4f const& val);
		Uniform&	operator=(const math::mat3f& val);
		Uniform&	operator=(math::mat4f const& val);

		size_t		get_count() const;

		Type		get_type() const;
		size_t		get_data_size() const;

		bool		get_as_bool(size_t idx = 0) const;
		int32_t		get_as_int32(size_t idx = 0) const;
		float		get_as_float(size_t idx = 0) const;
		math::vec2f get_as_vec2(size_t idx = 0) const;
		math::vec3f	get_as_vec3(size_t idx = 0) const;
		math::vec4f get_as_vec4(size_t idx = 0) const;
		math::mat3f get_as_mat3(size_t idx = 0) const;
		math::mat4f get_as_mat4(size_t idx = 0) const;

		void		set(size_t idx, bool val);
		void		set(size_t idx, int32_t val);
		void		set(size_t idx, float val);
		void		set(size_t idx, math::vec2f const& val);
		void		set(size_t idx, math::vec3f const& val);
		void		set(size_t idx, math::vec4f const& val);
		void		set(size_t idx, math::mat3f const& val);
		void		set(size_t idx, math::mat4f const& val);

		uint8_t const*	get_data() const;

		void		copy_from(bool const* val, size_t count);
        void		copy_from(int32_t const* val, size_t count);
		void		copy_from(float const* val, size_t count);
		void		copy_from(math::vec2f const* val, size_t count);
		void		copy_from(math::vec3f const* val, size_t count);
		void		copy_from(math::vec4f const* val, size_t count);
		void		copy_from(math::mat3f const* val, size_t count);
		void		copy_from(math::mat4f const* val, size_t count);

	private:
		void		destruct();
		void		construct(Type type, size_t count);

        Type		m_type = Type::FLOAT;
        uint16_t	m_count = 0;
        uint8_t*	m_data = nullptr;
		boost::auto_buffer<uint8_t, boost::store_n_bytes<64>> m_owns_data;
	};

	inline Uniform::Uniform()
	{
		float val = 0.f;
		construct(m_type, 1);
		copy_from(&val, 1);
	}
	inline Uniform::Uniform(Type type, size_t count, uint8_t* data)
        : m_data(data)
	{
		construct(type, count);
	}
	inline Uniform::Uniform(bool val)
		: m_type(Type::BOOL)
	{
		construct(m_type, 1);
		copy_from(&val, 1);
	}

    inline Uniform::Uniform(int32_t val)
		: m_type(Type::INT)
	{
		construct(m_type, 1);
		copy_from(&val, 1);
	}

	inline Uniform::Uniform(float val)
		: m_type(Type::FLOAT)
	{
		construct(m_type, 1);
		copy_from(&val, 1);
	}

	inline Uniform::Uniform(math::vec2f const& val)
		: m_type(Type::VEC2)
	{
		construct(m_type, 1);
		copy_from(&val, 1);
	}

	inline Uniform::Uniform(math::vec3f const& val)
		: m_type(Type::VEC3)
	{
		construct(m_type, 1);
		copy_from(&val, 1);
	}

	inline Uniform::Uniform(math::vec4f const& val)
		: m_type(Type::VEC4)
	{
		construct(m_type, 1);
		copy_from(&val, 1);
	}

	inline Uniform::Uniform(const math::mat3f& val)
		: m_type(Type::MAT3)
	{
		construct(m_type, 1);
		copy_from(&val, 1);
	}

	inline Uniform::Uniform(math::mat4f const& val)
		: m_type(Type::MAT4)
	{
		construct(m_type, 1);
		copy_from(&val, 1);
	}

	inline Uniform::Uniform(Uniform const& other)
		: m_type(Type::BOOL)
	{
		construct(other.m_type, other.m_count);
		size_t size = get_data_size();
		if (m_data)
		{
			memcpy(m_data, other.m_data, size);
		}
	}

	inline Uniform::Uniform(Uniform&& other)
		: m_type(other.m_type)
		, m_count(other.m_count)
		, m_owns_data(std::move(other.m_owns_data))
	{
		m_data = m_owns_data.empty() ? other.m_data : m_owns_data.data();
	}

	inline Uniform::~Uniform()
	{
	}

	inline Uniform&	Uniform::operator=(Uniform const& other)
	{
		if (m_type != other.m_type || m_count != other.m_count)
		{
			construct(other.m_type, other.m_count);
		}
		size_t size = get_data_size();
		QASSERT(size == other.get_data_size());
		memcpy(m_data, other.m_data, size);

		return *this;
	}

	inline bool Uniform::operator==(Uniform const& other) const
	{
		if (m_type != other.m_type || m_count != other.m_count)
		{
			return false;
		}

		size_t size = get_data_size();
		QASSERT(size == other.get_data_size());
		return memcmp(m_data, other.m_data, size) == 0;
	}
	inline Uniform& Uniform::operator=(bool val)
	{
		set(0, val);
		return *this;
	}

    inline Uniform& Uniform::operator=(int32_t val)
	{
		set(0, val);
		return *this;
	}

	inline Uniform& Uniform::operator=(float val)
	{
		set(0, val);
		return *this;
	}

	inline Uniform& Uniform::operator=(math::vec2f const& val)
	{
		set(0, val);
		return *this;
	}

	inline Uniform& Uniform::operator=(math::vec3f const& val)
	{
		set(0, val);
		return *this;
	}

	inline Uniform& Uniform::operator=(math::vec4f const& val)
	{
		set(0, val);
		return *this;
	}

	inline Uniform& Uniform::operator=(const math::mat3f& val)
	{
		set(0, val);
		return *this;
	}

	inline Uniform& Uniform::operator=(math::mat4f const& val)
	{
		set(0, val);
		return *this;
	}

	inline void Uniform::construct(Type type, size_t count)
	{
		QASSERT(count > 0 && count < 65536);
		if (type == m_type && count <= m_count)
		{
			m_count = static_cast<uint16_t>(count);
			return;
		}

		m_type = type;
		m_count = static_cast<uint16_t>(count);

		size_t size = get_data_size();
		if (!m_data || !m_owns_data.empty())
		{
			//own data
			m_owns_data.uninitialized_resize(size);
			m_data = m_owns_data.data();
		}
		QASSERT(m_data);
	}


	inline Uniform::Type Uniform::get_type() const
	{
		return m_type;
	}

	inline size_t Uniform::get_count() const
	{
		return m_count;
	}
	inline size_t Uniform::get_data_size() const
	{
		const size_t size[] = { 1, 4, 4, 8, 12, 16, 36, 64 };
        QASSERT(static_cast<size_t>(m_type) < sizeof(size) / sizeof(size_t));
        return size[static_cast<size_t>(m_type)] * m_count;
	}

	inline bool Uniform::get_as_bool(size_t idx) const
	{
		QASSERT(get_type() == Type::BOOL);
		QASSERT(idx < get_count());
		return ((bool*)m_data)[idx];
	}
    inline int32_t Uniform::get_as_int32(size_t idx) const
	{
		QASSERT(get_type() == Type::INT);
		QASSERT(idx < get_count());
        return ((int32_t*)m_data)[idx];
	}
	inline float Uniform::get_as_float(size_t idx) const
	{
		QASSERT(get_type() == Type::FLOAT);
		QASSERT(idx < get_count());
		return ((float*)m_data)[idx];
	}
	inline math::vec2f Uniform::get_as_vec2(size_t idx) const
	{
		QASSERT(get_type() == Type::VEC2);
		QASSERT(idx < get_count());
		return ((math::vec2f*)m_data)[idx];
	}
	inline math::vec3f Uniform::get_as_vec3(size_t idx) const
	{
		QASSERT(get_type() == Type::VEC3);
		QASSERT(idx < get_count());
		return ((math::vec3f*)m_data)[idx];
	}
	inline math::vec4f Uniform::get_as_vec4(size_t idx) const
	{
		QASSERT(sizeof(math::vec4f) == 16);
		QASSERT(get_type() == Type::VEC4);
		QASSERT(idx < get_count());
		return ((math::vec4f*)m_data)[idx];
	}
	inline math::mat3f Uniform::get_as_mat3(size_t idx) const
	{
		QASSERT(get_type() == Type::MAT3);
		QASSERT(idx < get_count());
		return ((math::mat3f*)m_data)[idx];
	}
	inline math::mat4f Uniform::get_as_mat4(size_t idx) const
	{
		QASSERT(get_type() == Type::MAT4);
		QASSERT(idx < get_count());
		return ((math::mat4f*)m_data)[idx];
	}

	inline void Uniform::set(size_t idx, bool val)
	{
		QASSERT(get_type() == Type::BOOL && idx < get_count());
		((bool*)m_data)[idx] = val;
	}
    inline void Uniform::set(size_t idx, int32_t val)
	{
		QASSERT(get_type() == Type::INT && idx < get_count());
        ((int32_t*)m_data)[idx] = val;
	}
	inline void Uniform::set(size_t idx, float val)
	{
		QASSERT(get_type() == Type::FLOAT && idx < get_count());
		((float*)m_data)[idx] = val;
	}
	inline void Uniform::set(size_t idx, math::vec2f const& val)
	{
		QASSERT(get_type() == Type::VEC2 && idx < get_count());
		((math::vec2f*)m_data)[idx] = val;
	}
	inline void Uniform::set(size_t idx, math::vec3f const& val)
	{
		QASSERT(get_type() == Type::VEC3 && idx < get_count());
		((math::vec3f*)m_data)[idx] = val;
	}
	inline void Uniform::set(size_t idx, math::vec4f const& val)
	{
		QASSERT(sizeof(math::vec4f) == 16);
		QASSERT(get_type() == Type::VEC4 && idx < get_count());
		((math::vec4f*)m_data)[idx] = val;
	}
	inline void Uniform::set(size_t idx, math::mat3f const& val)
	{
		QASSERT(get_type() == Type::MAT3 && idx < get_count());
		((math::mat3f*)m_data)[idx] = val;
	}
	inline void Uniform::set(size_t idx, math::mat4f const& val)
	{
		QASSERT(get_type() == Type::MAT4 && idx < get_count());
		((math::mat4f*)m_data)[idx] = val;
	}


	inline uint8_t const* Uniform::get_data() const
	{
		return (uint8_t const*)m_data;
	}

	inline void Uniform::copy_from(const bool* val, size_t count)
	{
		QASSERT(get_type() == Type::BOOL && count <= get_count());
		memcpy(m_data, val, count * sizeof(bool));
	}

    inline void Uniform::copy_from(const int32_t* val, size_t count)
	{
		QASSERT(get_type() == Type::INT && count <= get_count());
        memcpy(m_data, val, count * sizeof(int32_t));
	}

	inline void Uniform::copy_from(float const* val, size_t count)
	{
		QASSERT(get_type() == Type::FLOAT && count <= get_count());
		memcpy(m_data, val, count * sizeof(float));
	}

	inline void Uniform::copy_from(const math::vec2f* val, size_t count)
	{
		QASSERT(get_type() == Type::VEC2 && count <= get_count());
		memcpy(m_data, val, count * sizeof(math::vec2f));
	}

	inline void Uniform::copy_from(const math::vec3f* val, size_t count)
	{
		QASSERT(get_type() == Type::VEC3 && count <= get_count());
		for (size_t i = 0; i < count; i++)
		{
			*(math::vec3f*)m_data = val[i];
			m_data += sizeof(math::vec3f);
		}
	}

	inline void Uniform::copy_from(const math::vec4f* val, size_t count)
	{
		QASSERT(sizeof(math::vec4f) == 16);
		QASSERT(get_type() == Type::VEC4 && count <= get_count());
		memcpy(m_data, val, count * sizeof(math::vec4f));
	}

	inline void Uniform::copy_from(const math::mat3f* val, size_t count)
	{
		QASSERT(get_type() == Type::MAT3 && count <= get_count());
		memcpy(m_data, val, count * sizeof(math::mat3f));
	}

	inline void Uniform::copy_from(const math::mat4f* val, size_t count)
	{
		QASSERT(get_type() == Type::MAT4 && count <= get_count());
		memcpy(m_data, val, count * sizeof(math::mat4f));
	}


}
}
