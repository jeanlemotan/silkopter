#include "QDataStdAfx.h"
#include "data/Sink.h"
#include "data/Source.h"
#include "data/Sub_Source.h"
#include "data/Memory_Source.h"
#include "data/Data_Pack.h"
#include "data/Zip_Data_Pack.h"
#include "data/File_Source.h"
#include "data/File_System.h"
#include <zlib.h>


using namespace q;
using namespace data;


//static const int16_t ZIP_INFO_IN_DATA_DESCRITOR =	0x0008;

static bool is_compressed(uint16_t flags)
{
	return flags != 0;
}
static bool is_compression_supported(uint16_t flags)
{
	return !is_compressed(flags) || flags == 8;
}

//////////////////////////////////////////////////////////////////////////

Zip_Data_Pack::Zip_Data_Pack(Path const& path)
	: m_path(path)
{
	load();
}

Zip_Data_Pack::Zip_Data_Pack(File_System_ptr file_system, Path const& path)
	: m_file_system(file_system)
	, m_path(path)
{
	load();
}
Zip_Data_Pack::~Zip_Data_Pack()
{

}
void Zip_Data_Pack::load()
{
	m_files.clear();

	Source_uptr source = open_file();
	if (!source)
	{
		return;
	}

	m_files.reserve(1024);

    //auto start = std::chrono::system_clock::now();

	size_t file_count = 0;
	size_t dir_count = 0;

	std::vector<char> buffer;
	buffer.reserve(512);

	std::string str_buffer;
	str_buffer.reserve(512);

	size_t central_dir_offset = find_central_directory_offset(*source);
	if (central_dir_offset == static_cast<size_t>(-1))
	{
        QLOGE("Corrupted zip file '{}'", m_path);
		return;
	}

	source->seek(central_dir_offset);

	Source& s = *source;
	while (s.get_offset() < s.get_size())
	{
		File_Descriptor fd;

		uint32_t signature;
		uint16_t version_made_by;
		uint16_t version_needed_to_extract;
		uint16_t filename_length;
		uint16_t extra_field_length;
		uint16_t file_comment_length;
		uint16_t disk_number_start;
		uint16_t internal_file_attributes;
		uint32_t external_file_attributes;
		uint32_t relative_offset_of_local_header;
		s >> signature;
		s >> version_made_by;
		s >> version_needed_to_extract;
		s >> fd.general_bit_flag;
		s >> fd.compression_method;
		s >> fd.last_mod_file_time;
		s >> fd.last_mod_file_date;
		s >> fd.crc_32;
		s >> fd.compressed_size;
		s >> fd.uncompressed_size;
		s >> filename_length;
		s >> extra_field_length;
		s >> file_comment_length;
		s >> disk_number_start;
		s >> internal_file_attributes;
		s >> external_file_attributes;
		s >> relative_offset_of_local_header;
		if (signature != 0x02014b50)
		{
			break;
		}

		//CV: not valid - compressedSize is not valid to detect directories
		//bool isDirectory = zipData.header.dataDescriptor.compressedSize == 0;
		bool is_directory = false;

		// read filename
		buffer.resize(filename_length + 1);
		s.read((uint8_t*)buffer.data(), filename_length);
		buffer[filename_length] = 0x0;
		char lastCh = filename_length > 0 ? buffer[filename_length - 1] : 0;
		if (lastCh == '/' || lastCh == '\\')
		{
			is_directory = true;
			//cut the last path separator
			buffer[filename_length - 1] = 0;
		}
		str_buffer.resize(0);
		str_buffer.append(&buffer[0]);

		s.seek_relative(extra_field_length);
		s.seek_relative(file_comment_length);

		if (!is_compression_supported(fd.compression_method))
		{
            QLOGE("Zip file '{}' is compressed with an unsupported compression - {}. File ignored.", str_buffer, fd.compression_method);
			continue;
		}

		//PASSERT2(zipData.header.dataDescriptor.compressedSize == zipData.header.dataDescriptor.uncompressedSize, "Zip '{0}' file '{1}' is compressed. Only uncompressed files are supported.", m_path, strbuffer);

		//size_t id = m_entries.size();

		fd.path = Path(str_buffer);
		// plus the size of the local header
		fd.file_offset = relative_offset_of_local_header + 30 + filename_length + extra_field_length;

        //QLOGI("FILE {0}:{1}", m_files.size(), str_buffer);
		m_files.push_back(fd);

		if (is_directory)
		{
			dir_count++;
		}
		else
		{
			file_count++;
		}
	}

// 	auto duration = (System::GetUTime() - start) / 1000000.f;
// 	PINFO("Loaded file system '{0}' ({1} files, {2} directories): {3}s, {4}Kb", m_path, fileCount, dirCount, duration, x / 1024.f);
}

size_t Zip_Data_Pack::get_count() const
{
	return m_files.size();
}
Path const& Zip_Data_Pack::get_path(size_t idx) const
{
	QASSERT(idx < get_count());
	auto const& f = m_files[idx];
	return f.path;
}
Path const& Zip_Data_Pack::find_path_by_name(String const& name) const
{
	int xxx = 0;
	auto it = std::find_if(m_files.begin(), m_files.end(), [&] (File_Descriptor const& fi)
	{
		xxx++;
		return fi.path.get_filename() == name;
	});

	if (it != m_files.end())
	{
		return it->path;
	}
	return Path::null;
}
std::vector<Path> Zip_Data_Pack::find_all_paths_by_name(String const& name) const
{
	std::vector<Path> paths;
	paths.reserve(5);
	for (auto const& f: m_files)
	{
		if (name == f.path.get_filename())
		{
			paths.emplace_back(f.path);
		}
	}
	return paths;
}
int Zip_Data_Pack::find_idx_by_path(Path const& path) const
{
	for (size_t i = 0; i < m_files.size(); i++)
	{
		if (path == m_files[i].path)
		{
			return i;
		}
	}
	return -1;
}
// String ZipDataPack::getFilename(size_t idx) const
// {
// 	auto const& f = mFileWalker.getFiles()[idx];
// 	std::string str;
// 	str.reserve(mRoot.getAsString().size() + 1 + f.path.getAsString().size() + 1);
// 	str += mRoot.getAsString().c_str();
// 	str += '/';
// 	str += f.path.getAsString().c_str();
// 	return str.c_str();
// }
Source_uptr Zip_Data_Pack::open(Path const& path) const
{
	int idx = find_idx_by_path(path);
	if (idx < 0)
	{
		return Source_uptr();
	}

	auto const& fd = m_files[idx];

	uint32_t compressed_size = fd.compressed_size;
	uint32_t uncompressed_size = fd.uncompressed_size;

	if (!is_compressed(fd.compression_method))
	{
		if (compressed_size > 0)
		{
			auto source = open_file();
			QASSERT(source);
			if (!source)
			{
				return Source_uptr();
			}

			Sub_Source_uptr s(new Sub_Source(std::move(source), fd.file_offset, compressed_size));
			return std::move(s);
		}
	}
	else if (is_compression_supported(fd.compression_method))
	{
		//Compressed.
		if (compressed_size == 0 || uncompressed_size == 0)
		{
			//empty file
			return Source_uptr();
		}

		auto source = open_file();
		QASSERT(source);
		if (!source)
		{
			return Source_uptr();
		}

		std::vector<uint8_t> decompressed(uncompressed_size);
		if (decompressed.size() > 32000)
		{
            QLOGW("Decompressing '{}' file from '{}' fs: {}K -> {}K.", fd.path, m_path, compressed_size / 1024, uncompressed_size / 1024);
		}

		std::vector<uint8_t> compressed(compressed_size);
		source->seek(fd.file_offset);
		source->read(compressed.data(), compressed_size);

		// Setup the inflate stream.
		z_stream zstream;
		zstream.next_in = (Bytef*)compressed.data();
		zstream.avail_in = (uInt)compressed_size;
		zstream.next_out = (Bytef*)decompressed.data();
		zstream.avail_out = (uInt)uncompressed_size;
		zstream.zalloc = (alloc_func)0;
		zstream.zfree = (free_func)0;

		// Perform inflation. wbits < 0 indicates no zlib header inside the data.
		int err = inflateInit2(&zstream, -MAX_WBITS);
		if (err == Z_OK)
		{
			err = inflate(&zstream, Z_FINISH);
			inflateEnd(&zstream);
			if (err == Z_STREAM_END)
			{
				err = Z_OK;
			}
		}

		if (err != Z_OK)
		{
            QLOGE("Error {} while decompressing file '{}' from pack '{}'", err, fd.path, m_path);
			return Source_uptr();
		}
		return Source_uptr(new Memory_Source(std::move(decompressed)));
	}
	else
	{
        QLOGE("Error while decompressing file '{}' from pack '{}': Compression method {} not supported.", fd.path, m_path, fd.compression_method);
		return Source_uptr();
	}

	return Source_uptr();

}

Source_uptr Zip_Data_Pack::open_file() const
{
	Source_uptr source;
	if (m_file_system)
	{
		source = m_file_system->open(m_path);
	}
	else
	{
		auto file = File_Source_uptr(new File_Source(m_path));
		if (file)
		{
			source = std::move(file);
		}
	}
	return std::move(source);
}

size_t Zip_Data_Pack::find_central_directory_offset(Source& source) const
{
	source.seek(source.get_size() - 22);

	uint32_t signature;
	source >> signature;
	if (signature != 0x06054b50)
	{
		return static_cast<size_t>(-1);
	}

	source.seek_relative(2); //number of this disk
	source.seek_relative(2); //number of this disk with the start of the central directory
	source.seek_relative(2); //total number of entries in the central dir on this disk
	source.seek_relative(2); //total number of entries in the central dir
	source.seek_relative(4); //size of the central directory
	uint32_t offset;
	source >> offset;

	if (offset >= source.get_offset())
	{
		return static_cast<size_t>(-1);
	}

	return offset;
}
