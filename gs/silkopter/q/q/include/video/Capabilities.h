#pragma once

namespace q
{
namespace video
{

	class Capabilities
	{
	public:
		Capabilities() 
			: max_anisotropy(0)
			, max_aa_samples(0)
			, float_texture_support(false)
			, half_float_texture_support(false)
			, depth_texture_support(false)
			, pvrtc_compression_support(false)
			, etc1_compression_support(false)
			, dxt_compression_support(false)
			, half_float_attribute_support(false)
			, vertex_textures(0)
			, hardware_instancing(false)
			, multiple_render_targets(1)
			, occlusion_query_support(false)
		{
		}

		//caps start here

		math::vec2u32 max_texture_size;
		math::vec2u32 max_render_target_size;

		size_t max_anisotropy;
		size_t max_aa_samples;

		//texture formats
		bool float_texture_support;
		bool half_float_texture_support;
		bool depth_texture_support;

		//compressed textures
		bool pvrtc_compression_support;
		bool etc1_compression_support;
		bool dxt_compression_support;

		//vertex attribute support
		bool half_float_attribute_support;

		size_t vertex_textures;
		bool hardware_instancing;

		size_t multiple_render_targets;

		bool occlusion_query_support;
	};

}
}