#include "QStdAfx.h"

using namespace q;
using namespace video;

Vertex_Declaration::Vertex_Declaration()
	: m_topology(Topology::TRIANGLE_LIST)
	, m_range_start(0)
	, m_range_count(0)
	, m_vertex_count(0)
	, m_index_count(0)
	, m_vertex_buffers(Vertex_Buffers_Alloc(m_vertex_buffers_arena))
	, m_attributes(Attributes_Alloc(m_attributes_arena))
	, m_semantic_attribute_idx(Semantic_Attribute_Alloc(m_semantic_attribute_idx_arena))
{
	m_semantic_attribute_idx.resize(static_cast<int>(Semantic::USER) + 1, -1);
}

Vertex_Declaration::Vertex_Declaration(Vertex_Declaration const& other)
	: m_topology(other.m_topology)
	, m_range_start(other.m_range_start)
	, m_range_count(other.m_range_count)
	, m_vertex_count(other.m_vertex_count)
	, m_index_count(other.m_index_count)
	, m_index_buffer(other.m_index_buffer)
	, m_vertex_buffers(Vertex_Buffers_Alloc(m_vertex_buffers_arena))
	, m_attributes(Attributes_Alloc(m_attributes_arena))
	, m_semantic_attribute_idx(Semantic_Attribute_Alloc(m_semantic_attribute_idx_arena))
{
	m_vertex_buffers = other.m_vertex_buffers;
	m_attributes = other.m_attributes;
	m_semantic_attribute_idx = other.m_semantic_attribute_idx;
}
Vertex_Declaration::Vertex_Declaration(Vertex_Declaration&& other)
	: m_topology(other.m_topology)
	, m_range_start(other.m_range_start)
	, m_range_count(other.m_range_count)
	, m_vertex_count(other.m_vertex_count)
	, m_index_count(other.m_index_count)
	, m_vertex_buffers(Vertex_Buffers_Alloc(m_vertex_buffers_arena))
	, m_attributes(Attributes_Alloc(m_attributes_arena))
	, m_semantic_attribute_idx(Semantic_Attribute_Alloc(m_semantic_attribute_idx_arena))
{
	std::swap(m_index_buffer, other.m_index_buffer);

	m_vertex_buffers.resize(other.m_vertex_buffers.size());
	std::copy(std::make_move_iterator(other.m_vertex_buffers.begin()), std::make_move_iterator(other.m_vertex_buffers.end()), m_vertex_buffers.begin());
	//std::swap(m_vertex_buffers, other.m_vertex_buffers);
	m_attributes.resize(other.m_attributes.size());
	std::copy(std::make_move_iterator(other.m_attributes.begin()), std::make_move_iterator(other.m_attributes.end()), m_attributes.begin());
	//std::swap(m_attributes, other.m_attributes);
	m_semantic_attribute_idx.resize(other.m_semantic_attribute_idx.size());
	std::copy(std::make_move_iterator(other.m_semantic_attribute_idx.begin()), std::make_move_iterator(other.m_semantic_attribute_idx.end()), m_semantic_attribute_idx.begin());
	//std::swap(m_semantic_attribute_idx, other.m_semantic_attribute_idx);
}
auto Vertex_Declaration::operator=(Vertex_Declaration&& other) -> Vertex_Declaration&
{
	m_topology = other.m_topology;
	m_range_start = other.m_range_start;
	m_range_count = other.m_range_count;
	m_vertex_count = other.m_vertex_count;
	m_index_count = other.m_index_count;
	std::swap(m_index_buffer, other.m_index_buffer);

	m_vertex_buffers.resize(other.m_vertex_buffers.size());
	std::copy(std::make_move_iterator(other.m_vertex_buffers.begin()), std::make_move_iterator(other.m_vertex_buffers.end()), m_vertex_buffers.begin());
	//std::swap(m_vertex_buffers, other.m_vertex_buffers);
	m_attributes.resize(other.m_attributes.size());
	std::copy(std::make_move_iterator(other.m_attributes.begin()), std::make_move_iterator(other.m_attributes.end()), m_attributes.begin());
	//std::swap(m_attributes, other.m_attributes);
	m_semantic_attribute_idx.resize(other.m_semantic_attribute_idx.size());
	std::copy(std::make_move_iterator(other.m_semantic_attribute_idx.begin()), std::make_move_iterator(other.m_semantic_attribute_idx.end()), m_semantic_attribute_idx.begin());
	//std::swap(m_semantic_attribute_idx, other.m_semantic_attribute_idx);
	return *this;
}
auto Vertex_Declaration::operator=(Vertex_Declaration const& other) -> Vertex_Declaration&
{
	m_topology = other.m_topology;
	m_range_start = other.m_range_start;
	m_range_count = other.m_range_count;
	m_vertex_count = other.m_vertex_count;
	m_index_count = other.m_index_count;
	m_index_buffer = other.m_index_buffer;
	m_vertex_buffers = other.m_vertex_buffers;
	m_attributes = other.m_attributes;
	m_semantic_attribute_idx = other.m_semantic_attribute_idx;
	return *this;
}

void Vertex_Declaration::set_vertex_count(size_t count)
{
	if (m_vertex_count == count)
	{
		return;
	}

	m_vertex_count = count;
	for (size_t i = 0; i < m_attributes.size(); i++)
	{
		Attribute_Data& data = m_attributes[i];
		Vertex_Buffer_ptr const& buffer = get_vertex_buffer(data.attribute.buffer_idx);
		QASSERT(buffer);

		size_t size = m_vertex_count * data.attribute.stride;
		if (buffer->get_size() < size)
		{
			buffer->allocate(size, buffer->get_usage());
		}
	}

}
auto Vertex_Declaration::get_vertex_count() const -> size_t
{
	return m_vertex_count;
}

void Vertex_Declaration::set_index_count(size_t count)
{
	if (m_index_count == count)
	{
		return;
	}
	QASSERT(m_index_buffer);

	m_index_count = count;
	size_t size = m_index_count * sizeof(uint16_t);
	if (m_index_buffer->get_size() < size)
	{
		m_index_buffer->allocate(size, m_index_buffer->get_usage());
	}
}
auto Vertex_Declaration::get_index_count() const -> size_t
{
	return m_index_buffer ? m_index_count : 0;
}

void Vertex_Declaration::set_index_buffer(Index_Buffer_ptr buffer)
{
	if (buffer)
	{
		size_t index_count = buffer->get_size() / sizeof(uint16_t);
		if (index_count < m_index_count)
		{
			QLOG_INFO("Q", "New buffer is too small. resizing.");
			bool res = buffer->allocate(m_index_count * sizeof(uint16_t), buffer->get_usage());
			if (!res)
			{
				QLOG_INFO("Q", "New buffer is too small and failed to resize. Ignoring");
				return;
			}
		}
	}
	else
	{
		m_index_count = 0;
	}
	m_index_buffer = buffer;
}
auto Vertex_Declaration::get_index_buffer() const -> Index_Buffer_ptr const&
{
	return m_index_buffer;
}

auto Vertex_Declaration::add_attribute(Vertex_Buffer_ptr buffer, Semantic semantic, Type type, uint8_t count, uint16_t offset, uint16_t stride) -> int
{
	int idx = find_attribute_idx_by_semantic(semantic);
	if (idx >= 0)
	{
		QLOG_ERR("Q", "Attribute semantic {} already exists. Ignoring.", (int)semantic);
		return -1;
	}
	return add_attribute(buffer, String::null, semantic, type, count, offset, stride);
}

auto Vertex_Declaration::add_attribute(Vertex_Buffer_ptr buffer, String const& name, Type type, uint8_t count, uint16_t offset, uint16_t stride) -> int
{
	int idx = find_attribute_idx_by_name(name);
	if (idx >= 0)
	{
		QLOG_ERR("Q", "Attribute name '{}' already exists. Ignoring.", name);
		return -1;
	}
	return add_attribute(buffer, name, Semantic::USER, type, count, offset, stride);
}

auto Vertex_Declaration::add_attribute(Vertex_Buffer_ptr buffer, String const& name, Semantic semantic, Type type, uint8_t count, uint16_t offset, uint16_t stride) -> int
{
	if (!buffer)
	{
		QLOG_ERR("Q", "Attribute name '{}' has null buffer. Ignoring.", name);
		return -1;
	}

	if (count == 0)
	{
		QLOG_ERR("Q", "Attribute name '{}' has no elements. Ignoring.", name);
		return -1;
	}

	if ((stride & 3) != 0 || (offset & 3) != 0)
	{
		QLOG_ERR("Q", "Attribute name '{}' needs stride or offset multiple of 4. Ignoring.", name);
		return -1;
	}
	for (auto const& data: m_attributes)
	{
		if (get_vertex_buffer(data.attribute.buffer_idx) == buffer && data.attribute.stride != stride)
		{
			QLOG_ERR("Q", "Attribute name '{}' reuses a buffer with a different stride. Ignoring.", name);
			return -1;
		}
	}

	size_t vertex_count = buffer->get_size() / stride;
	if (m_vertex_count > 0 && m_vertex_count != vertex_count)
	{
		QLOG_ERR("Q", "Attribute name '{}' needs to have {} vertices. It has {} instead. Ignoring.", name, m_vertex_count, vertex_count);
		return -1;
	}

	size_t data_size = 0;
	switch (type)
	{
	case Type::FLOAT: data_size = 4; break;
	case Type::S8:
	case Type::U8:
	case Type::FP_S8:
	case Type::FP_U8: data_size = 1; break;
	case Type::S16: 
	case Type::U16: 
	case Type::FP_S16: 
	case Type::FP_U16: data_size = 2; break;
	default: QASSERT(0); break;
	}

	Attribute_Data data;
	data.data_size = data_size;
	data.attribute.stride = stride;
	data.attribute.count = count;
	data.attribute.offset = offset;
	data.attribute.name = name;
	data.attribute.type = type;

	int buffer_idx = -1;
	for (size_t i = 0; i < m_vertex_buffers.size(); i++)
	{
		if (m_vertex_buffers[i] == buffer)
		{
			buffer_idx = i;
			break;
		}
	}
	if (buffer_idx < 0)
	{
		if (m_vertex_buffers.size() >= 255)
		{
			QLOG_ERR("Q", "Attribute name '{}' reached the max number of buffers - 256.", name);
			return -1;
		}

		buffer_idx = m_vertex_buffers.size();
		m_vertex_buffers.push_back(buffer);
	}
	m_vertex_count = vertex_count;

	data.attribute.buffer_idx = static_cast<uint8_t>(buffer_idx);

	int idx = m_attributes.size();
	m_attributes.push_back(data);
	m_semantic_attribute_idx[static_cast<int>(semantic)] = idx;

	return idx;
}
auto Vertex_Declaration::find_attribute_idx_by_name(String const& name) const -> int
{
	auto it = std::find_if(m_attributes.begin(), m_attributes.end(), [&](Attribute_Data const& a) { return a.attribute.name == name; });
	if (it != m_attributes.end())
	{
		return std::distance(m_attributes.begin(), it);
	}
	return -1;
}

auto Vertex_Declaration::get_attribute_count() const -> size_t
{
	return m_attributes.size();
}
auto Vertex_Declaration::get_attribute(size_t idx) const -> Attribute const&
{
	return m_attributes[idx].attribute;
}

auto Vertex_Declaration::lock_indices_for_reading() -> q::util::Const_Ptr_Iterator<uint16_t>
{
	return q::util::Const_Ptr_Iterator<uint16_t>(m_index_buffer ? m_index_buffer->lock_for_reading() : 0, sizeof(uint16_t), get_index_count());
}
auto Vertex_Declaration::lock_indices_for_writing() -> q::util::Ptr_Iterator<uint16_t>
{
	return q::util::Ptr_Iterator<uint16_t>(m_index_buffer ? m_index_buffer->lock_for_writing() : 0, sizeof(uint16_t), get_index_count());
}
void Vertex_Declaration::unlock_indices()
{
	if (m_index_buffer)
	{
		m_index_buffer->unlock_for_reading();
		m_index_buffer->unlock_for_writing();
	}
}

void Vertex_Declaration::unlock(size_t idx)
{
	Attribute_Data const& data = m_attributes[idx];
	if (data.read_address)
	{
		get_vertex_buffer(data.attribute.buffer_idx)->unlock_for_reading();
		data.read_address = 0;
	}
	if (data.write_address)
	{
		get_vertex_buffer(data.attribute.buffer_idx)->unlock_for_writing();
		data.write_address = 0;
	}
}

void Vertex_Declaration::unlock_all()
{
	for (size_t i = 0; i < m_attributes.size(); i++)
	{
		unlock(i);
	}
	unlock_indices();
}

void Vertex_Declaration::replace_vertex_buffer(size_t idx, Vertex_Buffer_ptr new_buffer)
{
	for (auto const& data: m_attributes)
	{
		if (data.attribute.buffer_idx == idx)
		{
			size_t vertex_count = new_buffer->get_size() / data.attribute.stride;
			if (vertex_count < get_vertex_count())
			{
				QLOG_INFO("Q", "New buffer is too small. resizing.");
				bool res = new_buffer->allocate(get_vertex_count() * data.attribute.stride, new_buffer->get_usage());
				if (!res)
				{
					QLOG_INFO("Q", "New buffer is too small and failed to resize. Ignoring");
					continue;
				}
			}
			break;
		}
	}

	m_vertex_buffers[idx] = new_buffer;
}

auto Vertex_Declaration::get_vertex_buffer_count() const -> size_t
{
	return m_vertex_buffers.size();
}
auto Vertex_Declaration::get_vertex_buffer(size_t idx) const -> Vertex_Buffer_ptr const&
{
	return m_vertex_buffers[idx];
}

auto Vertex_Declaration::_lock_for_reading(size_t idx) -> uint8_t const*
{
	Attribute_Data const& data = m_attributes[idx];
	if (!data.read_address)
	{
		uint8_t const* address = get_vertex_buffer(data.attribute.buffer_idx)->lock_for_reading();
		if (address)
		{
			data.read_address = address + data.attribute.offset;
		}
	}
	return data.read_address;
}

auto Vertex_Declaration::_lock_for_writing(size_t idx) -> uint8_t*
{
	Attribute_Data const& data = m_attributes[idx];
	if (!data.write_address)
	{
		uint8_t* address = get_vertex_buffer(data.attribute.buffer_idx)->lock_for_writing();
		if (address)
		{
			data.write_address = address + data.attribute.offset;
		}
	}
	return data.write_address;
}

void Vertex_Declaration::set_topology(Topology t)
{
	m_topology = t;
}
auto Vertex_Declaration::get_topology() const -> Topology
{
	return m_topology;
}

void Vertex_Declaration::set_range(size_t start, size_t count)
{
	m_range_start = start;
	m_range_count = count;
}

auto Vertex_Declaration::get_range_start() const -> size_t
{
	return m_range_start;
}

auto Vertex_Declaration::get_range_count() const -> size_t
{
	return m_range_count;
}

auto Vertex_Declaration::get_primitive_count() const -> size_t
{
	size_t idx_count = get_index_count() > 0 ? get_index_count() : get_vertex_count();
	switch (m_topology)
	{
	case Topology::LINE_LOOP:		return idx_count;
	case Topology::LINE_STRIP:		return idx_count - 1;
	case Topology::LINE_LIST:		return idx_count >> 1;
	case Topology::TRIANGLE_FAN:	return idx_count - 2;
	case Topology::TRIANGLE_STRIP:	return idx_count - 2;
	case Topology::TRIANGLE_LIST:	return idx_count / 3;
	}
	return 0;
}
