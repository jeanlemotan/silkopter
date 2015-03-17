#include "QStdAfx.h"
#include "video/Atlaser.h"

using namespace q;
using namespace video;

Atlaser::Atlaser()
{
}

Atlaser::Atlaser(math::vec2u32 const& size)
	: m_size(size)
{
	reset(size);
}

void Atlaser::reset()
{
	QASSERT(m_size.x > 2 && m_size.y > 2);
	reset(m_size);
}

void Atlaser::reset(math::vec2u32 const& size)
{
	m_size = size;
	QASSERT(m_size.x > 2 && m_size.y > 2);

	m_nodes.clear();

	// We want a one pixel border around the whole atlas to avoid any artefact when
	// sampling texture
	m_nodes.emplace_back(1, 1, m_size.x - 2);
}

int Atlaser::fit(uint32_t idx, math::vec2u32 const& size)
{
	QASSERT(m_size.x > 2 && m_size.y > 2);
	auto node = m_nodes[idx];
	int x = node.x;
	int width_left = size.x;
	uint32_t i = idx;
	if (x + size.x > m_size.x - 1)
	{
		return -1;
	}

	int y = node.y;
	while (width_left > 0)
	{
		node = m_nodes[i];
		if (node.y > y)
		{
			y = node.y;
		}
		if (y + size.y > m_size.y - 1)
		{
			return -1;
		}
		width_left -= node.z;
		++i;
	}
	return y;
}


void Atlaser::merge()
{
	QASSERT(m_size.x > 2 && m_size.y > 2);
	for (uint32_t i = 0; i < m_nodes.size() - 1; ++i)
	{
		auto& node = m_nodes[i];
		auto& next = m_nodes[i + 1];
		if (node.y == next.y)
		{
			node.z += next.z;
			m_nodes.erase(m_nodes.begin() + i + 1);
			--i;
		}
	}
}


boost::optional<math::vec2u32> Atlaser::get_region(math::vec2u32 const& size)
{
	QASSERT(m_size.x > 2 && m_size.y > 2);

	int best_height = std::numeric_limits<int>::max();
	int best_index  = -1;
	int best_width = std::numeric_limits<int>::max();
	math::vec2s32 pos;
	for (uint32_t i = 0; i < m_nodes.size(); ++i)
	{
		int y = fit(i, size);
		if (y >= 0)
		{
			auto node = m_nodes[i];
			if (y + (int)size.y < best_height || (y + (int)size.y == best_height && node.z < best_width))
			{
				best_height = y + size.y;
				best_index = i;
				best_width = node.z;
				pos.x = node.x;
				pos.y = y;
			}
		}
	}

	if (best_index == -1)
	{
		return boost::optional<math::vec2u32>();
	}

	math::vec3s32 node(pos.x, pos.y + size.y, size.x);
	m_nodes.insert(m_nodes.begin() + best_index, node);

	for (uint32_t i = best_index + 1; i < m_nodes.size(); ++i)
	{
		auto& node = m_nodes[i];
		auto& prev = m_nodes[i - 1];
		if (node.x < prev.x + prev.z)
		{
			int shrink = prev.x + prev.z - node.x;
			node.x += shrink;
			node.z -= shrink;
			if (node.z <= 0)
			{
				m_nodes.erase(m_nodes.begin() + i);
				--i;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	merge();

	return math::vec2u32(pos);
}

math::vec2u32 const& Atlaser::get_size() const
{
	return m_size;
}
