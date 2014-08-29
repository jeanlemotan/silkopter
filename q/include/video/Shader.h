#pragma once

namespace q
{
namespace video
{

	class Shader : util::Noncopyable
	{
	public:
        virtual ~Shader() {}
        
		enum class Type : uint8_t
		{
			NORMAL,
			SKINNING_1NPV, //1 node per vertex
			SKINNING_2NPV, //2 node per vertex
			SKINNING_3NPV, //3 node per vertex
			SKINNING_4NPV, //4 node per vertex
		};

		static Shader_ptr	create(Type type, Shader_Source const& vertex, Shader_Source const& fragment);

		virtual Type		get_type() const = 0;
		virtual Shader_Source const& get_vertex_shader_source() const = 0;
		virtual Shader_Source const& get_fragment_shader_source() const = 0;
		virtual void		compile(std::vector<Uniform_Def>& uniforms,
									std::vector<Sampler_Def>& samplers,
									std::vector<Attribute_Def>& attributes) = 0;
	protected:
	};

}
}