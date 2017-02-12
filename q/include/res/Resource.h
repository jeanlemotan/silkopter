#pragma once

namespace q
{
namespace res
{

class Resource
{
public:
    Resource(Path const& path);
    virtual ~Resource() {}

    virtual bool		is_valid() const = 0;
    virtual void		unload() = 0;

    Path const&			get_path() const;

private:
    Path				m_path;
};

}
}
