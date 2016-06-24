#pragma once

#include "Render_Widget.h"
#include "Camera_Controller3D.h"

namespace silk
{
class Comms;
}

class Attitude : public QWidget
{
public:
	Attitude(QWidget* parent = 0);
	~Attitude();

	void init();

    void render(silk::Comms& comms);

private:
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);

	void paintEvent(QPaintEvent*);

    void render_ground(q::draw::Painter& painter, q::scene::Camera const& camera, q::scene::Scene& scene);
    void render_uav(q::draw::Painter& painter, q::scene::Camera const& camera, q::scene::Scene& scene);

	Render_Widget* m_widget;
	q::scene::Camera m_camera;
    q::scene::Scene m_scene;
	Camera_Controller3D m_camera_controller;

	q::draw::Painter m_painter;
	q::video::Material m_primitive_material;

    math::vec3f m_uav_position;
	math::quatf m_attitude_quat;
	math::mat3f m_local_to_world_mat;
	math::quatf m_local_to_world_quat;
	math::mat3f m_world_to_local_mat;
	math::quatf m_world_to_local_quat;

	std::vector<math::vec3f> m_plot_values;
};
