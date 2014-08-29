#pragma once

namespace q
{
namespace data
{

	class File_Source;
	DECLARE_CLASS_PTR(File_Source);

class File_Source : public Source
{
public:
	File_Source(Path const& path);
	virtual ~File_Source();

	size_t		get_size() const;

	bool		is_open() const;

	size_t		read(uint8_t* buffer, size_t size);
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