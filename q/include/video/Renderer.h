#pragma once

#include "Render_Target.h"
#include "Capabilities.h"
#include "video/Uniform_Def.h"
#include "video/Sampler_Def.h"
#include "video/Shader.h"

namespace q
{
namespace scene
{
class Camera;
}

namespace video
{
class Uniform_Def;
class Sampler_Def;
class Attribute_Def;

class Renderer : public util::Noncopyable
{
    friend class Vertex_Buffer;
    friend class Index_Buffer;
    friend class Uniform_Buffer;

    friend class Render_Target;
    friend class Texture;
    friend class Shader;
    friend class Uniform_Def;
    friend class Sampler_Def;
    friend class Attribute_Def;
public:
    virtual ~Renderer() {}

    typedef std::function<void()> End_Frame_Callback;
    typedef std::function<void()> Flush_Callback;

    virtual void			init(math::vec2u32 const& size,
                                 Render_Target::Color_Format color_format,
                                 Render_Target::Depth_Format depth_format,
                                 Render_Target::Stencil_Format stencil_format) = 0;

    virtual math::vec2u32		get_viewport_size() const = 0;
    virtual void			set_viewport_size(math::vec2u32 const& size) = 0;
    virtual Capabilities	get_capabilities() const = 0;

    enum class Hdr_Format
    {
        NONE,
        HALF, //half-float render target
        FULL  //float render target
    };

    virtual void			set_hdr_format(Hdr_Format format) = 0;
    virtual void			set_hdr_resolver(Post_Fx const& hdr_resolver) = 0;

    struct Hdr_Params
    {
        float start_range = 0;
        float end_range = 0;
    };
    virtual void			set_hdr_parameters(Hdr_Params const& params) = 0;

    virtual void			add_end_frame_callback(End_Frame_Callback const* callback) = 0;
    virtual void			remove_end_frame_callback(End_Frame_Callback const* callback) = 0;
    virtual void			add_flush_callback(Flush_Callback const* callback) = 0;
    virtual void			remove_flush_callback(Flush_Callback const* callback) = 0;

    virtual void			set_user_uniform_getter(std::string const& name, Uniform_Def::Getter const& getter) = 0;
    virtual void			set_user_sampler_getter(std::string const& name, Sampler_Def::Getter const& getter) = 0;

    virtual void			add_render_target(std::string const& name, Render_Target_ptr rt) = 0;
    virtual Render_Target_ptr	find_render_target_by_name(std::string const& name) = 0;

    virtual Dynamic_Image_uptr		read_color_pixels(math::vec2u32 const& position, math::vec2u32 const& size) = 0;
    virtual Dynamic_Image_uptr		read_depth_pixels(math::vec2u32 const& position, math::vec2u32 const& size) = 0;

    virtual void			begin_frame() = 0;
    virtual void			end_frame() = 0;

    virtual	void			set_camera(scene::Camera const& camera) = 0;

    virtual void			set_render_target(Render_Target_ptr const& rt) = 0;
    virtual Render_Target_ptr const& get_render_target() const = 0;

    enum class Hdr_Stage
    {
        BEFORE_RESOLVE,
        AFTER_RESOLVE
    };

    virtual void			begin_post_fx(Post_Fx const& pfx, Hdr_Stage stage) = 0;
    virtual void			end_post_fx() = 0;

    virtual void			render(Render_Job const& job) = 0;
    virtual void			flush() = 0;
    virtual void			wait_for_gpu() = 0;

    virtual size_t			get_frame_idx() const = 0;

    virtual bool			link_uniform(Uniform_Def& def) const = 0;
    virtual bool			link_sampler(Sampler_Def& def, Path const& link) const = 0;

protected:
    virtual Vertex_Buffer_ptr	create_vertex_buffer() = 0;
    virtual Index_Buffer_ptr	create_index_buffer() = 0;
    virtual Uniform_Buffer_ptr	create_uniform_buffer() = 0;
    virtual Render_Target_ptr	create_render_target() = 0;
    virtual Texture_ptr		create_texture(Path const& path) = 0;
    virtual Shader_ptr		create_shader(Shader::Type type, Shader_Source const& vertex, Shader_Source const& fragment) = 0;
};

}
}
