#pragma once

namespace q
{
namespace data
{

class Zip_Data_Pack : public Data_Pack
{
public:
    Zip_Data_Pack(Path const& path);
    Zip_Data_Pack(File_System_ptr file_system, Path const& path);
    virtual ~Zip_Data_Pack();

    size_t  get_count() const;
    Path const& get_path(size_t idx) const;
    Path const& find_path_by_name(std::string const& name) const;
    std::vector<Path> find_all_paths_by_name(std::string const& name) const;
    int find_idx_by_path(Path const& path) const;

    Source_uptr open(Path const& path) const;

protected:
    //std::string				getFilename(size_t idx) const;

    Source_uptr open_file() const;

private:
    void load();

    size_t find_central_directory_offset(Source& source) const;

    struct File_Descriptor
    {
        Path path;

        uint32_t file_offset; // position of compressed data in file
        uint16_t general_bit_flag;
        uint16_t compression_method;
        uint16_t last_mod_file_date;
        uint16_t last_mod_file_time;
        uint32_t crc_32;
        uint32_t compressed_size;
        uint32_t uncompressed_size;
    };

    File_System_ptr m_file_system;
    Path			m_path;

    std::vector<File_Descriptor>	m_files;
    //bool				m_recurse;
};

}
}
