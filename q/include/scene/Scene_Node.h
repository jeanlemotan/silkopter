#pragma once

#include "scene/Camera.h"
#include "video/Renderer.h"
#include "scene/Scene.h"
#include "video/Render_Job.h"

namespace q
{
namespace scene
{

struct Cull_Result
{
    Cull_Result() : status(Cull_Status::OUTSIDE), camera_distance(0), projected_radius(0) {}
    Cull_Status status;
    float camera_distance;
    float projected_radius;
};

struct Cull_Params
{
    Camera camera;
};

class Scene_Node : public util::Noncopyable
{
public:
    virtual ~Scene_Node() {};

    virtual void	cull(Cull_Result& result, Cull_Params const& params) const = 0;
    virtual void	render(video::Renderer& renderer, Scene& scene) = 0;
};

//////////////////////////////////////////////////////////////////////////

class Render_Job_Scene_Node : public Scene_Node
{
public:
    Render_Job_Scene_Node(video::Render_Job const& job);
    void	cull(Cull_Result& result, Cull_Params const& params) const;
    void	render(video::Renderer& renderer, Scene& scene);
private:
    video::Render_Job m_job;
};

//////////////////////////////////////////////////////////////////////////

inline Render_Job_Scene_Node::Render_Job_Scene_Node(video::Render_Job const& job)
    : m_job(job)
{
}
inline void Render_Job_Scene_Node::cull(Cull_Result& result, Cull_Params const& params) const
{
    result.status = params.camera.get_frustum().cull_bounds(m_job.get_bounds());
}
inline void Render_Job_Scene_Node::render(video::Renderer& renderer, Scene& /*scene*/)
{
    renderer.render(m_job);
}

}
}
