#pragma once

namespace q
{
namespace data
{

	class Sink : util::Noncopyable
	{
		DEFINE_RTTI_BASE_CLASS(q::data::Sink);
	public:
        virtual ~Sink() {}
        
		//Serial interface
		virtual size_t		write(uint8_t const* buffer, size_t size) = 0;

		virtual size_t		get_size() const = 0;

		//returns the source offset. It's always <= size
		virtual size_t		get_offset() const = 0;

		//changes the position to the specified offset. Returns the new offset
		//NOTE - if you seek beyond the sink size, it will change the size
		virtual size_t		seek(size_t offset) = 0;
		virtual size_t		seek_relative(int offset) = 0;
	};

	//////////////////////////////////////////////////////////////////////////


	template<class T> Sink& operator<<(Sink& sink, const T& val)
	{
		sink.write((uint8_t const*)&val, sizeof(val));
		return sink;
	}

	inline Sink& operator<<(Sink& sink, char const* val)
	{
		size_t size = strlen(val);
		QASSERT(size < 65536);
		sink << (uint16_t)size;
		sink.write((uint8_t const*)val, size);
		return sink;
	}

	inline Sink& operator<<(Sink& sink, String const& val)
	{
		size_t size = val.size();
		QASSERT(size < 65536);
		sink << (uint16_t)size;
		sink.write((uint8_t const*)val.c_str(), size);
		return sink;
	}

	inline Sink& operator<<(Sink& sink, std::string const& val)
	{
		size_t size = val.size();
		QASSERT(size < 65536);
		sink << (uint16_t)size;
		sink.write((uint8_t const*)val.c_str(), size);
		return sink;
	}

}
}
