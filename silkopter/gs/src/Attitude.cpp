#include "stdafx.h"
#include "Attitude.h"
#include "Axis.h"
#include "Comms.h"

Attitude::Attitude(QWidget* parent /* = 0 */)
	: QWidget(parent)
	, m_camera_controller(m_camera)
{
	setMinimumHeight(200);
	setMouseTracking(true);

    m_widget = new Render_Widget(Render_Widget::Flags(), this);
	setLayout(new QVBoxLayout());
	layout()->setMargin(0);
	layout()->setSpacing(0);
	layout()->addWidget(m_widget);

	m_camera_controller.set_depth_getter([&](const math::vec2u32& pos) -> float { return m_widget->get_depth_at(pos); });

    m_camera.set_perspective_vertical_fov(math::radians(60.f).radians);
	m_camera.set_near_distance(0.05f);
	m_camera.set_far_distance(20000.f);

//	{
//		QSettings settings;
//		math::vec3f position;
//		math::from_string(position, std::string(settings.value("attitude/camera/position", "0,0,0").toString().toLatin1().data()));
//		m_camera.set_position(position);
//		math::quatf rotation;
//		math::from_string(rotation, std::string(settings.value("attitude/camera/rotation", "0,0,0,1").toString().toLatin1().data()));
//		m_camera.set_rotation(rotation);
//	}


// 	//X - front
// 	//Y - right
// 	//-Z - up
// 	math::quatf rot1, rot2;
// 	rot1.set_from_euler_xyz(math::vec3f(0, math::anglef::pi.radians, 0));
// 	rot2.set_from_euler_xyz(math::vec3f(0, 0, -math::anglef::pi2.radians));
// 	m_camera.set_coordinate_system(rot1 * rot2);

	//Y - front
	//X - right
	//Z - up
}

Attitude::~Attitude()
{
	QSettings settings;
	settings.setValue("attitude/camera/position", math::to_string(m_camera.get_position()).c_str());
	settings.setValue("attitude/camera/rotation", math::to_string(m_camera.get_rotation()).c_str());
}

void Attitude::init()
{
	auto technique = q::System::inst().get_factory().load<q::video::Technique>(q::Path("techniques/primitive_3d.technique"));
	QASSERT(technique);
	m_primitive_material.set_technique(technique);
	m_primitive_material.get_render_state(0).set_depth_test(true);
	m_primitive_material.get_render_state(0).set_depth_write(true);
	m_primitive_material.get_render_state(0).set_culling(false);

	m_widget->init();
}

void Attitude::render_ground(q::draw::Painter& painter, q::scene::Camera const& camera, q::scene::Scene& scene)
{
    math::vec3s32 offset(m_uav_position);
    offset.z = 0;

    math::trans3df trans;
    trans.set_translation(math::vec3f(offset));
    painter.push_post_clip_transform(trans);

    auto mat = m_primitive_material;
    mat.get_render_state(0).set_depth_test(true);
    mat.get_render_state(0).set_depth_write(false);
    mat.get_render_state(0).set_culling(false);
    mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);
    painter.set_material(mat);

    const float k_size = 10000;
    const int k_line_count = 60;
    const int k_fade_size = 10;

    painter.fill_rectangle(q::draw::Vertex(math::vec3f(-k_size, -k_size, 0), 0x20FFFFFF), q::draw::Vertex(math::vec3f(k_size, k_size, 0), 0x20FFFFFF));
    for (int i = 0; i < k_line_count; i++)
    {
        float x = float(i) - k_line_count/2;
        uint32_t color = 0x20FFFFFF;
        painter.draw_line(q::draw::Vertex(math::vec3f(-k_line_count/2, x, 0), color), q::draw::Vertex(math::vec3f(k_line_count/2, x, 0), color));
        painter.draw_line(q::draw::Vertex(math::vec3f(x, -k_line_count/2, 0), color), q::draw::Vertex(math::vec3f(x, k_line_count/2, 0), color));
    }

    painter.pop_post_clip_transform();

    {
        math::vec3f offset(m_uav_position);
        offset.z = 0;
        offset.x += 2.f;
        trans.set_translation(offset);
        painter.push_post_clip_transform(trans);
        render_axes(painter, 1.f);
        painter.pop_post_clip_transform();
    }
}

void Attitude::render_uav(q::draw::Painter& painter, q::scene::Camera const& camera, q::scene::Scene& scene)
{
    math::trans3df trans;
    trans.set_rotation(m_local_to_world_mat);
    trans.set_translation(m_uav_position);
    painter.push_post_clip_transform(trans);

    painter.set_material(m_primitive_material);
    render_axes(painter, 1.f);

    //render motors
    {
//		auto const& mm = m_uav.get_motor_mixer();

        //float pitch = math::dot(m_local_to_world_mat.get_axis_y(), math::vec3f(0, 0, 1));
        //float roll = math::dot(m_local_to_world_mat.get_axis_x(), math::vec3f(0, 0, 1));
        //m_uav.get_motor_mixer().set_data(0.5f, 0, -pitch, -roll);

        const float uav_radius = 0.3f;
        const float motor_radius = 0.2f;
        const math::vec2f positions[4] =
        {
            math::vec2f(0, 1),
            math::vec2f(-1, 0),
            math::vec2f(0, -1),
            math::vec2f(1, 0),
        };
        //for (uint8_t i = 0; i < mm.get_motor_count(); i++)
        for (uint8_t i = 0; i < 4; i++)
        {
            //auto const& info = mm.get_motor_info(i);
            painter.draw_circle(q::draw::Vertex(positions[i]*uav_radius, 0xFFFFFFFF), motor_radius);
            //float ratio = m_motors[i].get_rpm() / m_motors[i].get_max_rpm();
            //painter.fill_circle(q::draw::Vertex(info.position*uav_radius, 0xFF00FF00), motor_radius * ratio);
        }
    }

    painter.pop_post_clip_transform();

//    //RENDER IMU DATA
//    {
//        math::trans3df trans;
//        trans.set_rotation(m_uav.get_attitude().get_local_to_world_mat());
//        trans.set_translation(m_uav_position);
//        painter.push_post_clip_transform(trans);

//        painter.set_material(m_primitive_material);
//        render_axes(painter, 0.5f);

//        painter.pop_post_clip_transform();
//    }
}

void Attitude::render(silk::Comms& comms)
{
	m_camera.set_viewport_and_aspect_ratio(q::video::Viewport(math::vec2u32::zero, math::vec2u32(m_widget->width(), m_widget->height())));

	//m_context.scene.SetCamera(m_context.camera);

	m_widget->begin_rendering();

	q::System::inst().get_renderer()->get_render_target()->set_color_clear_value(math::vec4f(math::vec3f(m_widget->width() / 3024.f, 0, 0), 1));
	q::System::inst().get_renderer()->get_render_target()->clear_all();

    auto camera_offset = m_camera.get_position() - m_uav_position;

    math::vec3f position = comms.get_uav_position_data().value;
    //position.z = 0;

    m_uav_position = position;

    m_camera.set_position(position + camera_offset);
    m_camera_controller.set_focus_point(position);
    m_painter.set_camera(m_camera);

    {
        m_attitude_quat = comms.get_uav_rotation_data().value;
        m_local_to_world_quat = m_attitude_quat;
        m_world_to_local_quat = math::inverse<float, math::fast>(m_local_to_world_quat);
        m_local_to_world_quat.get_as_mat3_and_inv<math::fast>(m_local_to_world_mat, m_world_to_local_mat);
    }

    render_ground(m_painter, m_camera, m_scene);
    render_uav(m_painter, m_camera, m_scene);

    {
//		math::trans3df trans;
//        trans.set_translation(position);
//		trans.set_rotation(m_attitude_quat);
//		render_axes(trans, m_primitive_material, 2.f, m_painter);

//        {
//            math::vec3f compass = math::normalized<float, math::safe>(comms.get_sensor_compass_data().value);
//            //m_plot_values.push_back(protocol.data_board_compass.value);
//            //compass = protocol.data_board_compass.value;//math::rotate(m_local_to_world_quat, protocol.data_board_compass.value);
//            render_arrow(math::vec3f::zero, compass, m_primitive_material, 0xFFFFFF00, m_painter);
//		}

//        {
//            auto acceleration = math::normalized<float, math::safe>(comms.get_sensor_accelerometer_data().value);
//            render_arrow(math::vec3f::zero, acceleration, m_primitive_material, 0xFFFFFFFF, m_painter);

//            acceleration = math::transform(m_local_to_world_mat, acceleration);
//            render_arrow(math::vec3f::zero, acceleration, m_primitive_material, 0xFF00FFFF, m_painter);
//		}

//		if (m_plot_values.size() > 8000)
//		{
//			m_plot_values.erase(m_plot_values.begin(), m_plot_values.begin() + 1);
//		}
						
//		for (size_t i = 0; i + 1 < m_plot_values.size(); i++)
//		{
//			float intensity = float(i) * 255 / m_plot_values.size();
//			auto color = math::color::lerp_color(0xFF000000, 0xFFFFFF00, intensity);
//			m_painter.draw_line(q::draw::Vertex(m_plot_values[i], color), q::draw::Vertex(m_plot_values[i+1], color));
//		}

		m_painter.flush();
	}

	m_widget->end_rendering();
	m_widget->update();
}

//////////////////////////////////////////////////////////////////////////

void Attitude::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
	m_camera_controller.mouse_press_event(event);
}

//////////////////////////////////////////////////////////////////////////

void Attitude::mouseReleaseEvent(QMouseEvent* event)
{
	QWidget::mouseReleaseEvent(event);
	m_camera_controller.mouse_release_event(event);
}

//////////////////////////////////////////////////////////////////////////

void Attitude::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mouseMoveEvent(event);
	m_camera_controller.mouse_move_event(event);
}

//////////////////////////////////////////////////////////////////////////

void Attitude::wheelEvent(QWheelEvent* event)
{
	QWidget::wheelEvent(event);
	m_camera_controller.wheel_event(event);
}

void Attitude::paintEvent(QPaintEvent* ev)
{
	QWidget::paintEvent(ev);
// 	return;
// 
// 	QPainter painter(this);
// 
// 	const float k_radius = 40.f;
// 
// 	auto coss = math::cos(m_euler);
// 	auto sinn = math::sin(m_euler);
// 
// 	float x_center = width() / 2.f;
// 	float y_center = k_radius + k_radius*0.2f;
// 
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	float x1 = x_center + k_radius * coss.x;
// 	float y1 = y_center + k_radius * sinn.x;
// 	float x2 = x_center - k_radius * coss.x;
// 	float y2 = y_center - k_radius * sinn.x;
// 	painter.setPen(QPen(Qt::red));
// 	painter.drawLine(QPointF(x1, height() - y1), QPointF(x2, height() - y2));
// 
// 	y_center += k_radius*2 + k_radius*0.2f;
// 	x1 = x_center + k_radius * coss.y;
// 	y1 = y_center + k_radius * sinn.y;
// 	x2 = x_center - k_radius * coss.y;
// 	y2 = y_center - k_radius * sinn.y;
// 	painter.setPen(QPen(Qt::green));
// 	painter.drawLine(QPointF(x1, height() - y1), QPointF(x2, height() - y2));
}



