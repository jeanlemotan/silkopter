#pragma once

namespace q
{
namespace video
{

	class Buffer : public util::Noncopyable
	{
	public:
		virtual ~Buffer() {}

		enum class Usage : uint8_t 
		{ 
			STATIC,			//rarely updated
			STREAM,			//updated often
		};

		//Reallocated the buffer with the size and usage specified
		//Calling with size 0 deallocated the buffer
		virtual bool		allocate(size_t size, Usage usage_hint) = 0;

		//Returns the current size of the buffer in bytes
		virtual size_t		get_size() const = 0;

		//Returns the usage hint
		virtual Usage		get_usage() const = 0;

		//Returns if the buffer is currently locked for reading.
		//If it's locked, calling lockForReading is guaranteed to return a valid pointer
		virtual bool		is_locked_for_reading() const = 0;

		//Locks the buffer for reading.
		//Fails if:
		//			- the buffer is locked for writing or consuming at the moment
		//			- not supported by the platform.
		// If the buffer was already locked for reading, it's guaranteed to return a valid pointer. IT also increments the lock counter
		virtual uint8_t const*	lock_for_reading() = 0;

		//Unlocks the buffer. If was locked multiple times you need to call unlock the same number of times
		//If not locked, no side effects
		virtual void		unlock_for_reading() = 0;

		//Returns if the buffer is currently locked for writing.
		//If it's already locked, calling lockForWriting is guaranteed to return a pointer
		virtual bool		is_locked_for_writing() const = 0;

		//Locks the buffer for reading.
		//Fails if the buffer is already locked for reading, consuming at the moment
		// If the buffer was already locked for writing, it's guaranteed to return a valid pointer. IT also increments the lock counter
		virtual uint8_t*	lock_for_writing() = 0;

		//Unlocks the buffer for writing. If not locked it has no side effects
		virtual void		unlock_for_writing() = 0;

		//Returns true of the buffer is locked for consuming. This usually means rendering.
		//Both writing and reading locks will fail while the buffer is consumed.
		//Usually a buffer is locked for consuming from the moment it's rendered until 3 frames after
		virtual bool		is_locked_for_consuming() const = 0;

		//Returns true if the buffer is locked for reading, writing or consuming
		bool				is_locked() const;
	};

	inline bool Buffer::is_locked() const
	{
		return is_locked_for_consuming() || is_locked_for_reading() || is_locked_for_writing();
	}



}
}
