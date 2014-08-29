#pragma once

namespace q
{
namespace video
{

class Vertex_Declaration
{
	friend class Renderer;
	friend class Render_Job;

public:

	enum class Type : uint8_t
	{
		FLOAT,	//full floating point values
		FP_S8,	//fixed point int8_t. Values end up in the shader as -1..1
		FP_U8,	//fixed point uint8_t. Values end up in the shader as 0..1
		FP_S16,	//fixed point int16_t. Values end up in the shader as -1..1
		FP_U16,	//fixed point uint16_t. Values end up in the shader as 0..1
		S8,		//int8_t. Values end up in the shader as -127..128
		U8,		//uint8_t. Values end up in the shader as 0..255
		S16,	//int16_t. Values end up in the shader as -32767..32768
		U16		//uint16_t. Values end up in the shader as 0..65535
	};

	enum class Semantic : uint8_t
	{
		POSITIONS,
		NORMALS,
		TANGENTS, //for the first UV
		COLORS,
		TEX_COORDS0,
		TEX_COORDS1,
		TEX_COORDS2,
		TEX_COORDS3,
		NODE_INDICES,
		NODE_WEIGHTS,
		USER
	};

	struct Attribute
	{
        String		name; //for user semantics this contains the name of the attribute. Otherwise it's empty
        Semantic	semantic = Semantic::USER;
        Type		type = Type::FLOAT; //component type
        uint8_t		count = 0; //how many components per vector
        uint8_t		buffer_idx = 0; //the index of the vertex buffer
        uint16_t	offset = 0; //in bytes
        uint16_t	stride = 0; //in bytes
	};

	enum class Topology : uint8_t
	{
		LINE_LIST,
		LINE_LOOP,
		LINE_STRIP,
		TRIANGLE_LIST,
		TRIANGLE_STRIP,
		TRIANGLE_FAN
	};

public:
	Vertex_Declaration();
	Vertex_Declaration(Vertex_Declaration const& other);
 	Vertex_Declaration(Vertex_Declaration&& other);
	Vertex_Declaration& operator=(Vertex_Declaration const& other);
 	Vertex_Declaration& operator=(Vertex_Declaration&& other);

	void set_topology(Topology t);
	auto get_topology() const -> Topology;

	void set_range(size_t start, size_t count);
	auto get_range_start() const -> size_t;
	auto get_range_count() const -> size_t;

	auto get_primitive_count() const -> size_t;

	void set_vertex_count(size_t count);
	auto get_vertex_count() const -> size_t;
		 
	void set_index_count(size_t count);
	auto get_index_count() const -> size_t;
		 
	void set_index_buffer(Index_Buffer_ptr buffer);
	auto get_index_buffer() const -> Index_Buffer_ptr const&;
		 
	auto add_attribute(Vertex_Buffer_ptr buffer, Semantic semantic, Type type, uint8_t count, uint16_t offset, uint16_t stride) -> int;
	auto add_attribute(Vertex_Buffer_ptr buffer, String const& name, Type type, uint8_t count, uint16_t offset, uint16_t stride) -> int;
		 
	auto find_attribute_idx_by_name(String const& name) const -> int;
	auto find_attribute_idx_by_semantic(Semantic semantic) const -> int;
		 
	auto get_attribute_count() const -> size_t;
	auto get_attribute(size_t idx) const -> Attribute const&;
		 
	auto get_vertex_buffer_count() const -> size_t;
	auto get_vertex_buffer(size_t idx) const -> Vertex_Buffer_ptr const&;
	void replace_vertex_buffer(size_t idx, Vertex_Buffer_ptr new_buffer);
		 
	auto lock_indices_for_reading() -> util::Const_Ptr_Iterator<uint16_t>;
	auto lock_indices_for_writing() -> util::Ptr_Iterator<uint16_t>;
	void unlock_indices();

	template<class T>	
	auto lock_for_reading(size_t idx) -> util::Const_Ptr_Iterator<T>;
	template<class T>	
	auto lock_for_writing(size_t idx) -> util::Ptr_Iterator<T>;

	void unlock(size_t idx);
	void unlock_all();

private:
	auto add_attribute(Vertex_Buffer_ptr buffer, String const& name, Semantic semantic, Type type, uint8_t count, uint16_t offset, uint16_t stride) -> int;

	auto _lock_for_reading(size_t idx) -> uint8_t const*;
	auto _lock_for_writing(size_t idx) -> uint8_t*;

    Topology m_topology = Topology::TRIANGLE_LIST;
    size_t	m_range_start = 0;
    size_t	m_range_count = 0;

    size_t m_vertex_count = 0;
    size_t m_index_count = 0;

	Index_Buffer_ptr	m_index_buffer;

	typedef mem::Embedded_Allocator<Vertex_Buffer_ptr, sizeof(Vertex_Buffer_ptr) * 8> Vertex_Buffers_Alloc;
	Vertex_Buffers_Alloc::arena_type m_vertex_buffers_arena;
	std::vector<Vertex_Buffer_ptr, Vertex_Buffers_Alloc> m_vertex_buffers;

	struct Attribute_Data
	{
		Attribute	attribute;
        size_t		data_size = 0;
        mutable uint8_t*        write_address = nullptr;
        mutable uint8_t const*	read_address = nullptr;
	};

	typedef mem::Embedded_Allocator<Attribute_Data, sizeof(Attribute_Data) * 8> Attributes_Alloc;
	typedef mem::Embedded_Allocator<int, sizeof(int) * ((int)Semantic::USER + 5)> Semantic_Attribute_Alloc;

	Attributes_Alloc::arena_type m_attributes_arena;
	Semantic_Attribute_Alloc::arena_type m_semantic_attribute_idx_arena;

	std::vector<Attribute_Data, Attributes_Alloc> m_attributes;
	std::vector<int, Semantic_Attribute_Alloc> m_semantic_attribute_idx;
};

inline auto Vertex_Declaration::find_attribute_idx_by_semantic(Semantic semantic) const -> int 
{
	return m_semantic_attribute_idx[static_cast<int>(semantic)];
}

template<class T>
auto Vertex_Declaration::lock_for_reading(size_t idx) -> util::Const_Ptr_Iterator<T>
{
	Attribute_Data const& data = m_attributes[idx];
	QASSERT(data.data_size*data.attribute.count == sizeof(T));
	uint8_t const* ptr = _lock_for_reading(idx);
	return util::Const_Ptr_Iterator<T>(ptr, data.attribute.stride, get_vertex_count());
}
template<class T>
auto Vertex_Declaration::lock_for_writing(size_t idx) -> util::Ptr_Iterator<T>
{
	Attribute_Data const& data = m_attributes[idx];
	QASSERT(data.data_size*data.attribute.count == sizeof(T));
	uint8_t* ptr = _lock_for_writing(idx);
	return util::Ptr_Iterator<T>(ptr, data.attribute.stride, get_vertex_count());
}


}
}

