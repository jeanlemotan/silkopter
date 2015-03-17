#include "QStdAfx.h"
#include "res/Resource.h"

using namespace q;
using namespace res;

Resource::Resource(Path const& path)
	: m_path(path)
{
}

Path const& Resource::get_path() const
{
	return m_path;
}
