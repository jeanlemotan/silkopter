#pragma once

namespace q
{
namespace util
{
namespace fs
{

	class Folder_Walker
	{
	public:
		struct File_Info
		{
			Path	full_path;
			Path	path; //relative to root
		};

		Folder_Walker();
		Folder_Walker(Path const& root, bool recurse = false);
		void				refresh(Path const& root, bool recurse);
		void				refresh();
		std::vector<File_Info> const&	get_files() const;
		Path const&			get_root() const;

	private:
		Path					m_root;
		std::vector<File_Info>	m_all_collected_files;
        bool					m_recurse = false;
	};

} //fs
} //util
} //q
