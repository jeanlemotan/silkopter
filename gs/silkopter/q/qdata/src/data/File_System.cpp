#include "QDataStdAfx.h"
#include "data/Sink.h"
#include "data/Source.h"
#include "data/Data_Pack.h"
#include "data/File_System.h"
#include "data/File_Source.h"

using namespace q;
using namespace data;

void File_System::mount(String const& mp, data::Data_Pack_uptr pack)
{
	std::lock_guard<std::mutex> sm(m_mutex);
	m_mount_points[mp] = std::move(pack);
}
void File_System::unmount(String const& mp)
{
	std::lock_guard<std::mutex> sm(m_mutex);
	m_mount_points.erase(mp);
}

Path File_System::find_path_by_name(String const& name)	const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	for (auto const& it: m_mount_points)
	{
		auto const& r = it.second->find_path_by_name(name);
		if (!r.empty())
		{
			return Path(it.first) + r;
		}
	}
	return Path();
}

std::vector<Path> File_System::find_all_paths_by_name(String const& name) const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	std::vector<Path> results;
	//results.reserve(10);
	for (auto const& dp: m_mount_points)
	{
		auto paths = dp.second->find_all_paths_by_name(name);
		if (!paths.empty())
		{
			for (auto const& p: paths)
			{
				results.emplace_back(dp.first);
				results.back() += p;
			}
		}
	}
	return results;
}

Source_uptr File_System::open(Path const& path) const
{
	std::lock_guard<std::mutex> sm(m_mutex);

	auto const& mp = path[0];
	auto it = m_mount_points.find(mp);
	if (it != m_mount_points.end())
	{
		auto new_path = path.get_sub_path(1, 0);
		auto src = it->second->open(new_path);
		if (src)
		{
			return src;
		}
	}

	//fall back to the real filesystem if failed
	if (util::fs::is_file(path))
	{
		auto file = File_Source_uptr(new File_Source(path));
		return file->is_open() ? std::move(file) : Source_uptr();
	}
	return Source_uptr();
}
