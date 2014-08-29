#pragma once

namespace q
{
namespace data
{

	class File_System : util::Noncopyable
	{
	public:
		void				mount(String const& mp, data::Data_Pack_uptr pack);
		void				unmount(String const& mp);

		Path				find_path_by_name(String const& name) const;
		std::vector<Path>	find_all_paths_by_name(String const& name) const;

		Source_uptr			open(Path const& path) const;

	private:
		typedef std::map<String, data::Data_Pack_uptr>	MountPointMap;

		MountPointMap			m_mount_points;
		mutable std::mutex		m_mutex;
	};

}
}