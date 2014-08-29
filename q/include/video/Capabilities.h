#pragma once

namespace q
{
namespace video
{

	class Capabilities
	{
	public:
		//caps start here

		math::vec2u32 max_texture_size;
		math::vec2u32 max_render_target_size;

        size_t max_anisotropy = 0;
        size_t max_aa_samples = 0;

		//texture formats
        bool float_texture_support = false;
        bool half_float_texture_support = false;
        bool depth_texture_support = false;

		//compressed textures
        bool pvrtc_compression_support = false;
        bool etc1_compression_support = false;
        bool dxt_compression_support = false;

		//vertex attribute support
        bool half_float_attribute_support = false;

        size_t vertex_textures = 0;
        bool hardware_instancing = false;

        size_t multiple_render_targets = 0;

        bool occlusion_query_support = false;
	};

}
}
