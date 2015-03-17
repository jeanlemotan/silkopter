#pragma once

#include "system/System.h"

//NOT THREAD SAFE

namespace q
{
namespace video
{

template<class T>
class Buffer_Pool
{
public:
	typedef std::shared_ptr<T> TPtr;

	Buffer_Pool(size_t max_count, size_t preallocate_size, typename T::Usage usage)
		: m_max_count(max_count)
		, m_preallocate_size(preallocate_size)
		, m_usage(usage)
	{
		m_buffer_last_used_frames.resize(max_count);
		m_buffers.reserve(m_max_count);
		for (auto& b: m_buffers)
		{
			b = T::create();
			QASSERT(b);
			if (b)
			{
				b->allocate(m_preallocate_size, m_usage);
				m_buffers.emplace_back(std::move(b));
			}
		}
	}

	TPtr const& get_buffer()
	{
		size_t fidx = System::inst().get_renderer()->get_frame_idx();
		for (size_t i = 0; i < m_buffers.size(); i++)
		{
			auto& b = m_buffers[i];
			auto& frame_idx = m_buffer_last_used_frames[i];

			if (fidx - frame_idx >= 3 && !b->is_locked())
			{
				frame_idx = fidx;
				return b;
			}
		}
		if (m_buffers.size() >= m_max_count)
		{
			return m_empty_buffer;
		}
		TPtr buffer = T::create();
		QASSERT(buffer);
		if (buffer)
		{
			buffer->allocate(m_preallocate_size, m_usage);
			m_buffers.emplace_back(std::move(buffer));
			m_buffer_last_used_frames.push_back(fidx);
			return m_buffers.back();
		}
		return m_empty_buffer;
	}

	void clear()
	{
		m_buffers.clear();
		m_buffer_last_used_frames.clear();
	}

private:
    size_t m_max_count = 0;
    size_t m_preallocate_size = 0;
    typename T::Usage m_usage;
	std::vector<TPtr> m_buffers;
	std::vector<size_t> m_buffer_last_used_frames;
	TPtr m_empty_buffer;
};

typedef Buffer_Pool<Vertex_Buffer> Vertex_Buffer_Pool;
typedef Buffer_Pool<Index_Buffer> Index_Buffer_Pool;

}
}
