#include "QStdAfx.h"
#include "video/Technique.h"

using namespace q;
using namespace video;
using namespace thread;

Technique::Technique(Path const& path)
	: Resource(path)
{
}
Technique::~Technique()
{
	;
}

void Technique::add_pass(Pass const& pass)
{
	QASSERT(find_pass_idx_by_name(pass.get_name()) == -1);
	size_t idx = m_passes.size();
	m_passes.push_back(pass);
	m_pass_map[pass.get_name()] = idx;
}
size_t Technique::get_pass_count() const
{
	return m_passes.size();
}
Pass const& Technique::get_pass(size_t idx) const
{
	QASSERT(idx < get_pass_count());
	return m_passes[idx];
}
int Technique::find_pass_idx_by_name(String const& name) const
{
	auto it = m_pass_map.find(name);
	return it != m_pass_map.end() ? it->second : -1;
}

void Technique::compile()
{
	for (size_t i = 0; i < get_pass_count(); i++)
	{
		m_passes[i].compile();
	}
}

//////////////////////////////////////////////////////////////////////////

void Technique::unload()
{
	//mPasses.resize(0);
	m_passes.clear();
	m_pass_map.clear();
}

//////////////////////////////////////////////////////////////////////////

bool Technique::is_valid() const
{
	return !m_passes.empty();
}
