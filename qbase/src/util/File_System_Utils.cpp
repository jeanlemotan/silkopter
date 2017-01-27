#include "QBaseStdAfx.h"
#include "QBase.h"

#include <sys/stat.h> // used for fstat in IsFolder
#include <stdio.h>
#include <stdlib.h>

#if defined Q_WINDOWS
#	define NOMINMAX
#	define NOWINRES
#	define NOMCX
#	include <windows.h>
#	include <process.h>
#	include <sys/utime.h>//_utime
#	include <direct.h>
#elif defined(Q_POSIX_API)
#	include <utime.h> //utime
#	include <unistd.h>
#	include <sys/types.h>
#	include <dirent.h>
#	include <errno.h>
#endif

//////////////////////////////////////////////////////////////////////////

namespace q
{
namespace util
{
namespace fs
{

bool exists(const Path& path)
{
	if (path.is_empty())
	{
		return false;
	}

    struct stat file_status;
    int ret = stat(path.get_as_string().c_str(), &file_status);
	if (ret != -1)
	{
		return true;
	}

	return false;
}


bool is_file(const Path& path)
{
	if (path.is_empty())
	{
		return false;
	}

    struct stat file_status;
    int ret = stat(path.get_as_string().c_str(), &file_status);
	if (ret != -1)
	{
        return ((file_status.st_mode & S_IFDIR) == 0) ? true : false;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

bool is_folder(Path const& path)
{
	if (path.is_empty())
	{
		return false;
	}

    struct stat file_status;
    int ret = stat(path.get_as_string().c_str(), &file_status);
	if (ret != -1)
	{
        return ((file_status.st_mode & S_IFDIR) != 0) ? true : false;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

size_t get_file_size(Path const& path)
{
	if (path.is_empty())
	{
		return false;
	}

	size_t size = 0;
	struct stat buf;
	if (stat(path.get_as_string().c_str(), &buf) == 0)
	{
		size = buf.st_size;
	}
	return size;
}

//////////////////////////////////////////////////////////////////////////

time_t get_file_time(Path const& path)
{
	if (path.is_empty())
	{
        return time_t(0);
	}

#if defined(Q_WINDOWS)

	struct __stat64 s;
	int err = _stat64(path.get_as_string().c_str(), &s);
	if (err == 0)
	{
        return s.st_mtime;
	}

#elif defined(Q_POSIX_API)

	struct stat s;
	int err = stat(path.get_as_string().c_str(), &s);
	if (err == 0)
	{
        return s.st_mtime;
	}

#endif
    return time_t(0);
}

//////////////////////////////////////////////////////////////////////////

bool set_file_time(Path const& path, time_t t)
{
	if (path.is_empty())
	{
		return false;
	}

#if defined(Q_WINDOWS)

	struct _utimbuf b;
    b.actime = t;
    b.modtime = t;
	int err = _utime(path.get_as_string().c_str(), &b);
	return err == 0;

#elif defined(Q_POSIX_API)

	struct utimbuf b;
    b.actime = t;
    b.modtime = t;
	int err = utime(path.get_as_string().c_str(), &b);
	return err == 0;

#endif
}

//////////////////////////////////////////////////////////////////////////

bool delete_file(Path const& path)
{
	if (!exists(path))
	{
		//nothing to delete
		return true;
	}
	if (!is_file(path))
	{
		//not a file, fail
		return false;
	}

	int res = remove(path.get_as_string().c_str());
	return res == 0;
}

//////////////////////////////////////////////////////////////////////////

Path get_current_folder()
{
	char buf[1024];

#ifdef Q_POSIX_API

	char* x = getcwd(buf, sizeof(buf));

#else

	char* x = _getcwd(buf, sizeof(buf));

#endif

	if (x != nullptr)
	{
		return Path(buf);
	}
	return Path();
}

//////////////////////////////////////////////////////////////////////////

bool set_current_folder(Path const& path)
{
	if (path.empty())
	{
		return false;
	}

#ifdef Q_POSIX_API

	int res = chdir(path.get_as_string().c_str());

#else

	int res = _chdir(path.get_as_string().c_str());

#endif

	return res == 0;
}

//////////////////////////////////////////////////////////////////////////

Path get_absolute_folder(Path const& path)
{
	auto cwd = get_current_folder();
	if (path.empty())
	{
		return cwd;
	}

#if defined Q_WINDOWS

    if (path[0].find(':') == std::string::npos)
	{
		return cwd + path;
	}

#elif defined(Q_POSIX_API)

	if (path[0] != "/")
	{
		return cwd + path;
	}

#endif

	return path;
}

//////////////////////////////////////////////////////////////////////////

bool create_folder(Path const& path)
{
	if (path.is_empty() || is_file(path))
	{
		return false;
	}
	if (is_folder(path))
	{
		return true;
	}

	size_t count = 0;//number of created directories
	for (size_t i = 0; i < path.get_count(); i++)
	{
        Path sub_path = path.get_sub_path(0, i + 1);
        if (is_folder(sub_path))
		{
			continue;
		}

#if defined Q_WINDOWS

        BOOL ret = ::CreateDirectoryA(sub_path.get_as_string().c_str(), nullptr);
		if (ret)
		{
			count++;
		}

#elif defined Q_POSIX_API

		mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
        int ret = mkdir(sub_path.get_as_string().c_str(), mode);
		if (ret == 0)
		{
			count++;
		}

#endif
	}

	return count > 0;
}

bool delete_folder(Path const& /*path*/)
{
// 	String tmp = GetNormalizedPath(path + "/");
//
// 	// delete all files in the directory
// 	StringArray sa = enumerateFiles(tmp, "*");
// 	size_t ndeletes = 0;
// 	std::cout << "Deleting directory " << tmp.c_str() << "(" << sa.size() << "): ";
// 	for (size_t i = 0; i < sa.size(); i++)
// 	{
// 		bool ret = DeleteFile(tmp + sa[i]);
// 		if (ret == false)
// 		{
// 			continue;
// 		}
// 		ndeletes++;
// 	}
// 	std::cout << ndeletes << std::endl;
//
// 	// delete empty directory
// #ifdef JET_WINDOWS
// 	BOOL ret = RemoveDirectory(tmp.c_str());
// 	return ret == TRUE;
// #endif
//
// #ifdef JET_IOS_DEVICE
// 	return rmdir(tmp.c_str()) == 0;
// #endif
//
	QASSERT(0);
	return false;
}


}
}
}
