#include "QDataStdAfx.h"
#include "util/Folder_Walker.h"
#include "data/Sink.h"
#include "data/Source.h"
#include "data/Data_Pack.h"
#include "data/Folder_Data_Pack.h"
#include "data/File_Source.h"

using namespace q;
using namespace data;

Folder_Data_Pack::Folder_Data_Pack(Path const& root, bool recurse)
	: m_root(root)
	, m_recurse(recurse)
{
	m_file_walker.refresh(m_root, m_recurse);
	m_files = m_file_walker.get_files();
}

size_t Folder_Data_Pack::get_count() const
{
	return m_files.size();
}
Path const& Folder_Data_Pack::get_path(size_t idx) const
{
	QASSERT(idx < get_count());
	auto const& f = m_files[idx];
	return f.path;
}
Path const& Folder_Data_Pack::find_path_by_name(String const& name) const
{
	auto it = std::find_if(m_files.begin(), m_files.end(), [&] (util::fs::Folder_Walker::File_Info const& fi) 
	{ 
		return fi.path.get_filename() == name; 
	});

	if (it != m_files.end())
	{
		return it->path;
	}
	return Path::null;
}
std::vector<Path> Folder_Data_Pack::find_all_paths_by_name(String const& name) const
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
int Folder_Data_Pack::find_idx_by_path(Path const& path) const
{
	auto it = std::find_if(m_files.begin(), m_files.end(), [&](const util::fs::Folder_Walker::File_Info& fi) 
	{
		return fi.path == path;
	});

	return it != m_files.end() ? std::distance(m_files.begin(), it) : -1;
}
Source_uptr Folder_Data_Pack::open(Path const& path) const
{
	int idx = find_idx_by_path(path);
	if (idx < 0)
	{
		return Source_uptr();
	}
	auto file = File_Source_uptr(new File_Source(m_files[idx].full_path));
	return file->is_open() ? std::move(file) : Source_uptr();
}
