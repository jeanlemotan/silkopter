#include "QStdAfx.h"

using namespace q;
using namespace scene;

Scene::Scene()
{
}

void Scene::add(Scene_Node_ptr const& node)
{
	if (node)
	{
		m_to_render.emplace_back(node);
	}
}

void Scene::render()
{
	//setup
	auto r = System::inst().get_renderer();
	r->set_camera(m_camera);

	//cull/render
	for(auto const& n: m_to_render)
	{ 
		n->render(*r, *this);
	}

	//clean up
	//mToRender.resize(0);
	m_to_render.clear();
}

void Scene::set_camera(Camera const& camera)
{
	m_camera = camera;
}
