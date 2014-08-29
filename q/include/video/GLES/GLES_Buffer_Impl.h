#pragma once

namespace q
{
namespace video
{

	class GLES_Buffer_Impl : public util::Noncopyable
	{
	public:
		enum class Type
		{
			INDEX,
			VERTEX,
			UNIFORM
		};

		GLES_Buffer_Impl(Type type);
		~GLES_Buffer_Impl();

		bool		allocate(size_t size, Buffer::Usage usage);
		size_t		get_size() const;
		Buffer::Usage get_usage() const;

		bool		is_locked_for_reading() const;
		uint8_t const*	lock_for_reading();
		void		unlock_for_reading();

		bool		is_locked_for_writing() const;
		uint8_t*			lock_for_writing();
		void		unlock_for_writing();

		bool		is_locked_for_consuming() const;

		bool		is_locked() const;

		uint32_t	get_gl_id() const;
		void		set_last_rendered_frame_idx(size_t frame_idx);

	private:
		void		destroy_gl_object(gles::Interface& interf);
		void		create_gl_object(gles::Interface& interf);

		Type		m_type;

		std::vector<uint8_t> m_data;

		uint32_t	m_gl_id;
		size_t		m_size;
		Buffer::Usage m_usage;
		
		uint8_t const*	m_read_address;

		uint8_t*			m_write_address;
#if defined Q_WINDOWS
		std::atomic<int> m_read_count;
		std::atomic<int> m_write_count;
#else
		std::atomic_int m_read_count;
		std::atomic_int m_write_count;
#endif

		size_t		m_last_frame_rendered_idx;

		uint32_t	m_target;
	};

}
}
