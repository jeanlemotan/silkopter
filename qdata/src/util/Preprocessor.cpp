#include "QDataStdAfx.h"
#include "data/Sink.h"
#include "data/Source.h"
#include "data/Data_Pack.h"
#include "data/File_System.h"
#include "util/Preprocessor.h"

using namespace q;
using namespace util;

Preprocessor::Preprocessor(data::File_System& fs, Path const& root, Flags flags)
	: m_file_system(fs)
	, m_flags(flags)
    , m_root(root)
{

}

std::string Preprocessor::resolve_comments(std::string const& src)
{
	std::string::size_type off = src.find('/');
	if (off == std::string::npos || off == src.size() - 1)
	{
		return src;
	}

	std::string::size_type last_off = 0;
	std::string::size_type block_start = 0;
	std::string result;
	int block_level = 0;
	while (true)
	{
		if (block_level == 0 && last_off != off)
		{
			result += src.substr(last_off, off - last_off);
		}
		last_off = off;
		if (off == std::string::npos)
		{
			break;
		}

		if (src[off + 1] == '/' && block_level == 0) //line comment
		{
			auto end = src.find_first_of("\n\r", off);
			if (m_flags[Flag::SPACES_FOR_COMMENTS])
			{
				result.append(end - off, ' ');
			}
			off = end;
			last_off = off;
		}
		else if (block_level > 0 && off > 0 && src[off - 1] == '*') //end block comment
		{
			block_level--;
			if (block_level == 0)
			{
				auto last = block_start;
				auto pos = src.find_first_of("\r\n", block_start);
				while (true)
				{
					bool found_new_line = true;
					if (pos > off || pos == std::string::npos) //no moar new lines? process the last one only
					{
						pos = off + 1;
						found_new_line = false;
					}

					if (m_flags[Flag::SPACES_FOR_COMMENTS])
					{
						result.append(pos - last, ' ');
					}
					if (found_new_line)
					{
						result.append("\r\n");
					}

					if (pos >= off)
					{
						break;
					}
					last = src.find_first_not_of("\r\n", pos);
					pos = src.find_first_of("\r\n", last);
				}
				last_off = off + 1;//to skip the / as well
			}
		}
		else if (src[off + 1] == '*') //start block comment
		{
			if (block_level == 0)
			{
				block_start = off;
			}
			block_level++;
		}

		off = src.find('/', off + 1); //find next possible comment start
	}

	return result;
}

std::string Preprocessor::resolve_includes(Path const& path, std::string const& source)
{
	if (source.empty())
	{
		return source;
	}

	std::string src = m_flags[Flag::IGNORE_COMMENTS] ? source : resolve_comments(source);

	std::string res = src;
    auto off = res.find("#include");
	while (off != res.npos)
	{
		int start_off = off + 8;
        auto end_off = res.find("\n", start_off);
		if (end_off == res.npos)
		{
			end_off = res.length();
		}

		std::string filename = res.substr(start_off, end_off - start_off);
		int qsoff = filename.find_first_of('"');
		int qeoff = filename.find_last_of('"');
		if (qsoff != qeoff)
		{
			filename = filename.substr(qsoff + 1, qeoff - qsoff - 1);
		}

		Path new_path(path.get_sub_path(0, -1) + String(filename));
		auto link = m_file_system.open(new_path);
		if (!link)
		{
            QLOGE("Cannot open file '{}' while preprocessing", new_path);
			return src;
		}

		auto contents = data::read_whole_source_as_string<std::string>(*link);

		contents = resolve_includes(new_path, contents);
		auto new_res = res.substr(0, off);
		new_res += contents;
        if (end_off < res.length())
		{
			new_res += res.substr(end_off + 1, res.length() - (end_off + 1));
		}
		res = new_res;

		off = res.find("#include", end_off);
	}

	return res;
}

String Preprocessor::preprocess(String const& source)
{
	if (source.empty())
	{
		return source;
	}

	std::string src(source.c_str());

	//first comments - then includes, to void resolving the comented includes
	if (!m_flags[Flag::IGNORE_COMMENTS])
	{
		src = resolve_comments(src);
	}

	if (!m_flags[Flag::IGNORE_INCLUDES])
	{
		src = resolve_includes(m_root, src);
	}

	return String(src.c_str());
}

