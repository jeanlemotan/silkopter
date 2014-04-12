#pragma once

namespace q
{
namespace video
{

	class Pass
	{
		friend class Technique;
	public:
		Pass();
		Pass(Pass&&);
		~Pass();
		auto operator=(Pass&&) -> Pass&;
        
#if !defined(_WIN32)
        Pass(Pass const& other) = default;
        auto operator=(Pass const& other) -> Pass& = default;
#endif

		void set_name(String const& name);
		auto get_name() const -> String;

		void set_scope_name(String const& scope_name);
		auto get_scope_name() const -> String;
		auto get_scope_idx() const -> size_t;

		//////////////////////////////////////////////////////////////////////////
		void add_uniform_def(Uniform_Def const& def);

		//uniforms that are controllable by the material
		auto get_material_uniform_def_count() const -> size_t;
		auto get_material_uniform_def(size_t idx) const -> Uniform_Def const&;
		auto find_material_uniform_def_idx_by_name(String const& name) const -> int;

		//returns the index of the material uniform from the uniform idx
		auto get_material_uniform_idx_from_uniform_idx(size_t uniform_idx) const -> int;

		//returns offset(idx - 1) + size(idx - 1)
		auto get_material_uniform_def_data_offset(size_t uniform_idx) const -> size_t;
		auto get_material_uniform_def_data_size(size_t uniform_idx) const -> size_t;

		//all uniforms.
		//There are 3 kinds:
		//	- material uniforms
		//	- auto uniforms (like view matrix) - specified by the user
		//	- auto uniforms added by shader patching (bone indices for example)
		auto get_uniform_def_count() const -> size_t;
		auto get_uniform_def(size_t uniform_idx) const -> Uniform_Def const&;
		auto find_uniform_def_idx_by_name(String const& name) const -> int;

		//////////////////////////////////////////////////////////////////////////
		void add_sampler_def(Sampler_Def const& def);

		//samplers for the material
		auto get_material_sampler_def_count() const -> size_t;
		auto get_material_sampler_def(size_t sampler_idx) const -> Sampler_Def const&;
		auto find_material_sampler_def_idx_by_name(String const& name) const -> int;

		auto get_sampler_def_count() const -> size_t;
		auto get_sampler_def(size_t sampler_idx) const -> Sampler_Def const&;
		auto find_sampler_def_idx_by_name(String const& name) const -> int;

		//////////////////////////////////////////////////////////////////////////

		void add_attribute_def(Attribute_Def const& def);
		auto get_attribute_def_count() const -> size_t;
		auto get_attribute_def(size_t attribute_idx) const -> Attribute_Def const&;
		auto find_attribute_def_idx_by_name(String const& name) const -> int;

		//////////////////////////////////////////////////////////////////////////

		void set_render_state_def(Render_State_Def const& def);
		auto get_render_state_def() const -> Render_State_Def const&;

		//////////////////////////////////////////////////////////////////////////

		void set_uber_shader(Uber_Shader const& p);
		auto get_uber_shader() const -> Uber_Shader const&;

		//////////////////////////////////////////////////////////////////////////

		void set_render_target(Render_Target_ptr const& rt);
		auto get_render_target() const -> Render_Target_ptr const&;

		//////////////////////////////////////////////////////////////////////////

		void compile();

	private:
		String		m_name;

		//selective rendering
		String		m_scope_name;
		size_t		m_scope_index; //the renderer holds a mask of the active scopes

		//uniforms
		std::vector<Uniform_Def>	m_uniforms;
		std::vector<std::pair<size_t, size_t>> m_material_uniform_data;

		std::vector<size_t>	m_material_uniform_idx_to_uniform_idx;
		std::vector<size_t>	m_uniform_idx_to_material_uniform_idx;
		std::map<String, size_t> m_material_uniform_map;

		//samplers
		std::vector<Sampler_Def>	m_samplers;

		std::vector<size_t>	m_material_samplers_idx;
		std::map<String, size_t> m_material_sampler_map;

		// attributes
		std::vector<Attribute_Def>	m_attributes;
		std::map<String, size_t> m_attribute_map;

		//render state
		Render_State_Def	m_render_state;

		//shader
		Uber_Shader		m_uber_shader;

		//render target
		Render_Target_ptr	m_render_target;
	};

	inline auto Pass::get_render_target() const -> Render_Target_ptr const&
	{
		return m_render_target;
	}

}
}