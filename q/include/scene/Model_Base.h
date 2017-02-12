#pragma once

#include "Ptr_Fw_Declaration.h"
#include "scene/Skeleton.h"
#include "video/Vertex_Declaration.h"
#include "video/Material.h"

namespace q
{
namespace scene
{
class Model_Base : public res::Resource, public std::enable_shared_from_this<Model_Base>
{
public:
    Model_Base(Path const& path);
    virtual ~Model_Base();

    Model_ptr			create_instance();

    virtual void		unload();
    virtual bool		is_valid() const;

    void				set_skeleton(Skeleton const&	skeleton);
    Skeleton const&		get_skeleton() const;

    struct Mesh_Data
    {
        Mesh_Data() : node_idx(0), material_idx(0), skin_npv(0) {}
        std::string name;
        size_t node_idx;
        size_t material_idx;
        size_t skin_npv;
        std::vector<uint32_t> indices;
        std::vector<math::trans3df> bind_pose_transforms;
        video::Vertex_Declaration vertex_declaration;
        math::aabb3f aabb;
    };

    size_t				add_mesh(std::string const& name, size_t node_idx, size_t material_idx, video::Vertex_Declaration const& declaration, math::aabb3f const& aabb);
    void				set_mesh_skin_data(size_t mesh_idx, size_t skin_npv, std::vector<uint32_t> const& indices, std::vector<math::trans3df> const& bindPoseTransforms);
    size_t				get_mesh_count() const;
    Mesh_Data const&	get_mesh(size_t idx) const;

    struct Material_Data
    {
        std::string name;
        video::Material material;
    };

    size_t				add_material(std::string const& name, video::Material const& material);
    size_t				get_material_count() const;
    Material_Data const&get_material(size_t idx);
    int					find_material_idx_by_name(std::string const& name);

    struct Texture_Channel_Data
    {
        std::string name;
        std::string texture_name;
        video::Sampler::Wrap wrap_u;
        video::Sampler::Wrap wrap_v;
    };

    size_t				add_texture_channel(std::string const& name, std::string const& textureName, video::Sampler::Wrap wrapu, video::Sampler::Wrap wrapv);
    size_t				get_texture_channel_count() const;
    Texture_Channel_Data const& get_texture_channel(size_t idx) const;

private:

    Skeleton			m_skeleton;
    std::vector<Mesh_Data> m_meshes;
    std::vector<Material_Data> m_materials;
    std::vector<Texture_Channel_Data> m_texture_channels;

    std::vector<Model_wptr> m_instances;
};

}
}

