#include "Simulator.h"

#include <chrono>
#include "simulator/Axis.h"

Simulator::Simulator(QWidget* parent)
    : QMainWindow(parent)
    , m_camera_controller(m_context.camera)
{
    m_ui.setupUi(this);
    show();
}

void Simulator::init(silk::Comms& comms, std::string const& node_name)
{
    m_comms = &comms;
    m_node_name = node_name;

    m_stream_path = node_name + "/simulator_state";
    auto result = m_comms->set_stream_telemetry_enabled(m_stream_path, true);
    if (result != ts::success)
    {
        QLOGE("Failed to enable simulator state stream '{}' telemetry: {}", m_stream_path, result.error().what());
    }

    m_message_connection = m_comms->sig_node_message_received.connect(std::bind(&Simulator::message_received, this, std::placeholders::_1, std::placeholders::_2));
    m_telemetry_connection = m_comms->sig_telemetry_samples_available.connect(std::bind(&Simulator::telemetry_received, this, std::placeholders::_1));

    init_graphics();
    init_world();

    //m_view->show();


    QObject::connect(m_ui.actionSimulation, &QAction::toggled, [this](bool yes)
    {
        auto message = std::make_shared<silk::messages::Multirotor_Simulator_Set_Simulation_Enabled_Message>();
        message->set_enabled(yes);
        auto result = m_comms->send_node_message(m_node_name, message);
        QASSERT(result == ts::success);
    });
    QObject::connect(m_ui.actionReset, &QAction::triggered, [this](bool yes)
    {
        auto message = std::make_shared<silk::messages::Multirotor_Simulator_Reset_Message>();
        auto result = m_comms->send_node_message(m_node_name, message);
        QASSERT(result == ts::success);
    });
    QObject::connect(m_ui.actionStopMotion, &QAction::triggered, [this](bool yes)
    {
        auto message = std::make_shared<silk::messages::Multirotor_Simulator_Stop_Motion_Message>();
        auto result = m_comms->send_node_message(m_node_name, message);
        QASSERT(result == ts::success);
    });
    QObject::connect(m_ui.actionGravity, &QAction::toggled, [this](bool yes)
    {
        auto message = std::make_shared<silk::messages::Multirotor_Simulator_Set_Gravity_Enabled_Message>();
        message->set_enabled(yes);
        auto result = m_comms->send_node_message(m_node_name, message);
        QASSERT(result == ts::success);
    });
    QObject::connect(m_ui.actionTerrain, &QAction::toggled, [this](bool yes)
    {
        auto message = std::make_shared<silk::messages::Multirotor_Simulator_Set_Ground_Enabled_Message>();
        message->set_enabled(yes);
        auto result = m_comms->send_node_message(m_node_name, message);
        QASSERT(result == ts::success);
    });
    QObject::connect(m_ui.actionDrag, &QAction::toggled, [this](bool yes)
    {
        auto message = std::make_shared<silk::messages::Multirotor_Simulator_Set_Drag_Enabled_Message>();
        message->set_enabled(yes);
        auto result = m_comms->send_node_message(m_node_name, message);
        QASSERT(result == ts::success);
    });
}

void Simulator::init_graphics()
{
    m_context.camera.set_perspective_vertical_fov(math::anglef(math::radians(60.f)));
    m_context.camera.set_near_distance(0.05f);
    m_context.camera.set_far_distance(20000.f);
    m_context.scene.set_camera(m_context.camera);
    m_context.painter.set_camera(m_context.camera);

    //////////////////////////////////////////////////////////////////////////
    // load resources
    auto technique = q::System::inst().get_factory().load<q::video::Technique>(q::Path("techniques/primitive_3d.technique"));
    QASSERT(technique);
    m_context.materials.primitive.set_technique(technique);
    m_context.materials.primitive.get_render_state(0).set_depth_test(true);
    m_context.materials.primitive.get_render_state(0).set_depth_write(true);
    m_context.materials.primitive.get_render_state(0).set_culling(false);

    technique = q::System::inst().get_factory().load<q::video::Technique>(q::Path("techniques/primitive_2d.technique"));
    QASSERT(technique);
    m_context.materials.primitive_2d.set_technique(technique);
    m_context.materials.primitive_2d.get_render_state(0).set_depth_test(true);
    m_context.materials.primitive_2d.get_render_state(0).set_depth_write(true);
    m_context.materials.primitive_2d.get_render_state(0).set_culling(false);

    technique = q::System::inst().get_factory().load<q::video::Technique>(q::Path("techniques/textured_2d.technique"));
    QASSERT(technique);
    m_context.materials.textured_2d.set_technique(technique);
    m_context.materials.textured_2d.get_render_state(0).set_depth_test(true);
    m_context.materials.textured_2d.get_render_state(0).set_depth_write(true);
    m_context.materials.textured_2d.get_render_state(0).set_culling(false);

    technique = q::System::inst().get_factory().load<q::video::Technique>(q::Path("techniques/font.technique"));
    QASSERT(technique);
    m_context.materials.font.set_technique(technique);
    m_context.materials.font.get_render_state(0).set_depth_test(false);
    m_context.materials.font.get_render_state(0).set_depth_write(false);
    m_context.materials.font.get_render_state(0).set_culling(false);
    m_context.materials.font.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);

//    m_context.font = q::System::inst().get_factory().load<q::text::Font>(q::Path("fonts/DroidSans.ttf"));
    m_context.font = q::System::inst().get_factory().load<q::text::Font>(q::Path("fonts/ConsolaMono.ttf"));
    QASSERT(m_context.font);

    m_ui.render_widget->init();
}

void Simulator::init_world()
{
}

void Simulator::telemetry_received(silk::Comms::ITelemetry_Stream const& _stream)
{
    if (_stream.stream_path == m_stream_path)
    {
        auto const* stream = dynamic_cast<silk::Comms::Telemetry_Stream<silk::stream::IMultirotor_Simulator_State> const*>(&_stream);
        if (stream && !stream->samples.empty())
        {
            m_sim_state = stream->samples.back().value;
        }
    }
}

void Simulator::message_received(std::string const& node_name, silk::messages::INode_Message const& _message)
{
    if (node_name != m_node_name)
    {
        return;
    }
}

/////////////////////////////////////////////////////////////////////////

void Simulator::mousePressEvent(QMouseEvent* event)
{
    QWidget::mousePressEvent(event);
    m_camera_controller.mouse_press_event(event);
}

//////////////////////////////////////////////////////////////////////////

void Simulator::mouseReleaseEvent(QMouseEvent* event)
{
    QWidget::mouseReleaseEvent(event);
    m_camera_controller.mouse_release_event(event);
}

//////////////////////////////////////////////////////////////////////////

void Simulator::mouseMoveEvent(QMouseEvent* event)
{
    QWidget::mouseMoveEvent(event);
    m_camera_controller.mouse_move_event(event);
}

//////////////////////////////////////////////////////////////////////////

void Simulator::wheelEvent(QWheelEvent* event)
{
    QWidget::wheelEvent(event);
    m_camera_controller.wheel_event(event);
}

void Simulator::render_ground()
{
    math::vec3s32 offset(m_sim_state.enu_position);
    offset.z = 0;

    math::trans3df trans;
    trans.set_translation(math::vec3f(offset));
    m_context.painter.push_post_clip_transform(trans);

    auto mat = m_context.materials.primitive;
    mat.get_render_state(0).set_depth_test(true);
    mat.get_render_state(0).set_depth_write(false);
    mat.get_render_state(0).set_culling(false);
    mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);
    m_context.painter.set_material(mat);

    const float k_size = 10000;
    const int k_line_count = 300;
    const int k_half_line_size = k_line_count / 2;

    //m_context.painter.fill_rectangle(q::draw::Vertex(math::vec3f(-k_size, -k_size, 0), 0x20FFFFFF), q::draw::Vertex(math::vec3f(k_size, k_size, 0), 0x20FFFFFF));

    float max_alpha = 20.f;
    float color_inc = 2.f / float(k_line_count);
    for (int i = 0; i < k_line_count; i++)
    {
        float x = float(i) - k_half_line_size;
        uint32_t color = 0x00FFFFFF;
        uint32_t color2 = color | static_cast<int>((1.f - math::abs(x) * color_inc) * max_alpha) << 24;

        m_context.painter.draw_line(q::draw::Vertex(math::vec3f(-k_half_line_size, x, 0), color), q::draw::Vertex(math::vec3f(0, x, 0), color2));
        m_context.painter.draw_line(q::draw::Vertex(math::vec3f(0, x, 0), color2), q::draw::Vertex(math::vec3f(k_half_line_size, x, 0), color));

        m_context.painter.draw_line(q::draw::Vertex(math::vec3f(x, -k_half_line_size, 0), color), q::draw::Vertex(math::vec3f(x, 0, 0), color2));
        m_context.painter.draw_line(q::draw::Vertex(math::vec3f(x, 0, 0), color2), q::draw::Vertex(math::vec3f(x, k_half_line_size, 0), color));
    }

    m_context.painter.pop_post_clip_transform();
}

void Simulator::render_uav(math::trans3df const& trans)
{
    m_context.painter.push_post_clip_transform(trans);

    auto mat = m_context.materials.primitive;
    mat.get_render_state(0).set_depth_test(true);
    mat.get_render_state(0).set_depth_write(false);
    mat.get_render_state(0).set_culling(false);
    mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);

    m_context.painter.set_material(mat);

    //render motors
    {
        //float pitch = math::dot(m_local_to_world_mat.get_axis_y(), math::vec3f(0, 0, 1));
        //float roll = math::dot(m_local_to_world_mat.get_axis_x(), math::vec3f(0, 0, 1));
        //m_uav.get_motor_mixer().set_data(0.5f, 0, -pitch, -roll);

        //UAV radius
        m_context.painter.draw_circle(q::draw::Vertex(math::vec3f::zero, 0xFFFFFFFF), m_sim_state.radius, 32); //motor

        const float propeller_radius = 0.12f;
        const float motor_radius = 0.02f;
        for (size_t i = 0; i < m_sim_state.motors.size(); i++)
        {
            auto const& m = m_sim_state.motors[i];

            m_context.painter.draw_line(q::draw::Vertex(math::vec3f::zero, 0xFFFFFFFF), q::draw::Vertex(m.position, 0xFFFFFFFF));
            m_context.painter.fill_circle(q::draw::Vertex(m.position, 0xFFFFFFFF), motor_radius, 16); //motor

            m_context.painter.fill_circle(q::draw::Vertex(m.position, 0x40FFFFFF), propeller_radius, 32); //motor + prop
            if (!math::is_zero(m.max_thrust, math::epsilon<float>()))
            {
                float ratio = m.thrust / m.max_thrust;
                m_context.painter.fill_circle(q::draw::Vertex(m.position, 0xAA00FF00), math::lerp(0.f, propeller_radius, ratio), 32);
            }
        }
    }

    //render axis
    if (m_ui.actionShowAxis->isChecked())
    {
        render_axis(m_context.painter, 1.f, 0.2f);
    }

    m_context.painter.pop_post_clip_transform();

    //render altitude meter
    if (m_ui.actionShowAltitude->isChecked())
    {
        math::vec3f p0 = m_sim_state.enu_position;
        math::vec3f p1 = p0;
        p0.z = 0;
        math::vec3f p2 = p1 + math::vec3f(0, 0, math::sgn(p0.z)) * 2.f;
        m_context.painter.draw_line(q::draw::Vertex(p0, 0x50FFFFFF), q::draw::Vertex(p1, 0x50FFFFFF));
        m_context.painter.draw_line(q::draw::Vertex(p1, 0x50FFFFFF), q::draw::Vertex(p2, 0x00FFFFFF));
        m_context.painter.fill_circle(q::draw::Vertex(p0, 0x50FFFFFF), 0.05, 16);
        m_context.painter.fill_sphere(q::draw::Vertex(p1, 0x50FFFFFF), 0.05, 4);
    }
}

void Simulator::render_brain_state()
{
    auto mat = m_context.materials.primitive;
    mat.get_render_state(0).set_depth_test(true);
    mat.get_render_state(0).set_depth_write(false);
    mat.get_render_state(0).set_culling(false);
    mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);

    m_context.painter.set_material(mat);

    silk::stream::IMultirotor_State::Value& multirotor_state = m_sim_state.multirotor_state;
    if (!multirotor_state.home_ecef_position.is_initialized())
    {
        return;
    }

    util::coordinates::LLA home_lla_position = util::coordinates::ecef_to_lla(*multirotor_state.home_ecef_position);
    math::trans3dd enu_to_ecef_trans = util::coordinates::enu_to_ecef_transform(home_lla_position);
    math::trans3dd ecef_to_enu_trans = math::inverse(enu_to_ecef_trans);

    //auto lla_position = util::coordinates::ecef_to_lla(m_uav.brain_state.value.ecef_position.value);
    //QLOGI("LAT: {}, LON: {}, ALT: {}", lla_position.latitude, lla_position.longitude, lla_position.altitude);

    //render what the brain _thinks_ the orientation is
    {
        math::trans3df trans;
        trans.set_rotation(multirotor_state.local_frame);
        trans.set_translation(m_sim_state.enu_position);
        m_context.painter.push_post_clip_transform(trans);

        //render axis
        if (m_ui.actionShowAxis->isChecked())
        {
            render_axis(m_context.painter, 1.f, 0.15f);
        }

        m_context.painter.pop_post_clip_transform();
    }

    //render where the brain _thinks_ it is
    {
        auto enu_position = math::vec3f(math::transform(ecef_to_enu_trans, multirotor_state.ecef_position));

        math::trans3df trans;
        trans.set_rotation(multirotor_state.local_frame);
        trans.set_translation(enu_position);
        m_context.painter.push_post_clip_transform(trans);

        //render axis
        if (m_ui.actionShowAxis->isChecked())
        {
            render_axis(m_context.painter, 1.f, 0.15f);
        }

        m_context.painter.pop_post_clip_transform();

        m_context.painter.fill_sphere(q::draw::Vertex(enu_position, 0x50FFFFFF), 0.05, 4);
    }
}

void Simulator::render_world_axis()
{
    auto mat = m_context.materials.primitive;
    mat.get_render_state(0).set_depth_test(true);
    mat.get_render_state(0).set_depth_write(false);
    mat.get_render_state(0).set_culling(false);
    mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);

    m_context.painter.set_material(mat);

    math::trans3df trans;
    m_context.painter.push_post_clip_transform(trans);

    q::scene::Camera camera;
    camera.set_parallel_zoom(4.f);
    camera.set_viewport(q::video::Viewport(math::vec2u32(0, 0), math::vec2u32(150, 150)));
    camera.set_near_distance(-100.0f);
    camera.set_far_distance( 100.0f);

    //ENU
    {
        math::quatf rot(m_context.camera.get_rotation());

        camera.set_transform(math::rotate(rot, math::vec3f(0, 5, 0)), rot);
        m_context.painter.set_camera(camera);
        render_axis(m_context.painter, 1.f, 1.5f);
    }

    //ECEF
//    {
//        auto home_lla_position = util::coordinates::ecef_to_lla(m_uav.brain_state.value.home_position.value);
//        auto enu_to_ecef_rotation = util::coordinates::enu_to_ecef_rotation(home_lla_position);
//        math::quatd rotd;
//        rotd.set_from_mat3(enu_to_ecef_rotation);
//        math::quatf rot(rotd);
//        rot = rot * m_context.camera.get_rotation();

//        camera.set_viewport(q::video::Viewport(math::vec2u32(0, 150), math::vec2u32(150, 150)));
//        camera.set_transform(math::rotate(rot, math::vec3f(0, 5, 0)), rot);
//        m_context.painter.set_camera(camera);
//        render_axis(m_context.painter, 1.f, 1.5f);
//    }

    m_context.painter.pop_post_clip_transform();
    m_context.painter.set_camera(m_context.camera);
}

void Simulator::process()
{
    m_ui.render_widget->begin_rendering();

//    math::vec3f const& pos = m_state.enu_position;
//    m_uav_transform->setTranslation(QVector3D(pos.x, pos.y, pos.z));

//    {
//        QVector3D offset = QVector3D(pos.x, pos.y, pos.z) - m_camera_entity->viewCenter();
//        m_camera_entity->setViewCenter(m_camera_entity->viewCenter() + offset);
//        m_camera_entity->setPosition(m_camera_entity->position() + offset);
//    }

//    math::quatf const& rot = m_state.local_to_enu_rotation;
//    m_uav_transform->setRotation(QQuaternion(rot.w, rot.x, rot.y, rot.z));

    m_context.camera.set_viewport_and_aspect_ratio(q::video::Viewport(math::vec2u32::zero, math::vec2u32(m_ui.render_widget->width(), m_ui.render_widget->height())));

    auto color = math::color::u32_to_rgba<float>(0x2c3e50);
    q::System::inst().get_renderer()->get_render_target()->set_color_clear_value(color);
    q::System::inst().get_renderer()->get_render_target()->clear_all();

    m_camera_controller.process();

    {
        auto delta = m_sim_state.enu_position - m_camera_position_target;
        //delta *= 0.9f;
        m_camera_position_target += delta;
        m_context.camera.set_position(m_context.camera.get_position() + delta);
        m_camera_controller.set_focus_point(m_camera_position_target);
    }

    m_context.painter.set_camera(m_context.camera);

    render_ground();

    {
        math::trans3df trans;
        trans.set_rotation(m_sim_state.local_to_enu_rotation);
        trans.set_translation(m_sim_state.enu_position);
        render_uav(trans);
    }
    {
        render_brain_state();
    }

    render_world_axis();

    m_context.painter.flush();

    m_ui.render_widget->end_rendering();
    m_ui.render_widget->update();
}
