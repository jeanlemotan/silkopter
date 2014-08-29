#pragma once

namespace q
{
namespace data
{
	class File_Sink;
	DECLARE_CLASS_PTR(File_Sink);

class File_Sink : public Sink
{
public:
	File_Sink(Path const& path);
	virtual ~File_Sink();

	bool		is_open() const;

	size_t		get_size() const;

	size_t		write(uint8_t const* buffer, size_t size);
	size_t		get_offset() const;
	size_t		seek(size_t offset);
	size_t		seek_relative(int offset);

private:
	FILE*		m_file;
	size_t		m_offset;
	size_t		m_size;
};


}
}