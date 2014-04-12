#pragma once

namespace q
{
namespace data
{

	class Folder_Data_Pack : public Data_Pack
	{
	public:
		Folder_Data_Pack(Path const& root, bool recurse = true);
        virtual ~Folder_Data_Pack() {}

		size_t				get_count() const;
		Path const&			get_path(size_t idx) const;
		Path const&			find_path_by_name(String const& name) const;
		std::vector<Path>	find_all_paths_by_name(String const& name) const;
		int					find_idx_by_path(Path const& path) const;

		Source_uptr			open(Path const& path) const;

	protected:
		//String				getFilename(size_t idx) const;

	private:
		Path				m_root;
		util::fs::Folder_Walker	m_file_walker;
		std::vector<util::fs::Folder_Walker::File_Info> m_files;
		bool				m_recurse;
	};

}
}