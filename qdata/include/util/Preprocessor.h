#pragma once

namespace q
{
namespace util
{

class Preprocessor : util::Noncopyable
{
public:
    enum class Flag : uint8_t
    {
        SPACES_FOR_COMMENTS = 1 << 0,	//replaces line comments with spaces
        IGNORE_COMMENTS		= 1 << 1,	//ignores comments
        IGNORE_INCLUDES		= 1 << 2,	//don't follow includes
    };
    typedef Flag_Set<Flag, uint8_t> Flags;

public:
    Preprocessor(data::File_System& fs, Path const& root, Flags flags = Flags(0));

    std::string preprocess(std::string const& source);

private:
    std::string	resolve_includes(Path const& root, std::string const& source);
    std::string resolve_comments(std::string const& source);

    data::File_System&m_file_system;
    Flags			m_flags;
    Path			m_root;
};

}
}
