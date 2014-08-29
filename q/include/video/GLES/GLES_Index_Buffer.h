#pragma once

#include "GLES_Buffer_Impl.h"

namespace q
{
namespace video
{

	class GLES_Index_Buffer : public Index_Buffer
	{
	public:
		GLES_Index_Buffer();
		~GLES_Index_Buffer();

		virtual bool		allocate(size_t size, Usage usage);
		virtual size_t		get_size() const;
		virtual Usage		get_usage() const;

		virtual bool		is_locked_for_reading() const;
		virtual uint8_t const*	lock_for_reading();
		virtual void		unlock_for_reading();

		virtual bool		is_locked_for_writing() const;
		virtual uint8_t*			lock_for_writing();
		virtual void		unlock_for_writing();

		virtual bool		is_locked_for_consuming() const;

		GLES_Buffer_Impl&		get_buffer();
		GLES_Buffer_Impl const&	get_buffer() const;

	private:
		GLES_Buffer_Impl		m_buffer;
	};

}
}
