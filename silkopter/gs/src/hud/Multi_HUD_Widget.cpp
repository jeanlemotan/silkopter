#include "hud/Multi_HUD_Widget.h"

Multi_HUD_Widget::Multi_HUD_Widget(silk::HAL& hal, silk::Comms& comms, qinput::Input_Mgr& input_mgr, Render_Context& context, QWidget* parent)
    : m_hal(hal)
    , m_comms(comms)
    , m_input_mgr(input_mgr)
    , m_context(context)
{
    setParent(parent);
    setStyleSheet("background-color:black;");

    m_render_widget = new Render_Widget(this);
    m_render_widget->init();

    setLayout(new QVBoxLayout(this));
    layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->setContentsMargins(4, 4, 4, 4);
    layout()->addWidget(m_render_widget);

    //find video stream
    auto video_streams = m_hal.get_streams().get_all_of_type<silk::stream::gs::Video>();
    if (!video_streams.empty())
    {
        m_video.stream = video_streams[0];
        auto vs = m_video.stream.lock();
        m_hal.set_stream_telemetry_active(vs->name, true);
        m_video.connection = vs->samples_available_signal.connect([this](silk::stream::gs::Video::Samples const& samples)
        {
            video_samples_available(samples);
        });
    }
}

void Multi_HUD_Widget::video_samples_available(silk::stream::gs::Video::Samples const& samples)
{
    if (samples.empty())
    {
        return;
    }
    auto const& sample = samples.back();

    if (m_video.decoder.decode_frame(sample, sample.value.resolution, m_video.data))
    {
        if (!m_video.texture)
        {
            m_video.texture = q::video::Texture::create(q::Path("video/texture"));
        }
        if (m_video.texture->get_size() != sample.value.resolution)
        {
            if (!m_video.texture->allocate(q::video::Texture::Format::RGBA_8, sample.value.resolution))
            {
                return;
            }
        }

        m_video.texture->upload_data(0, m_video.data.data());
    }
}


void Multi_HUD_Widget::process_vertical_thrust_rate()
{
    float v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::LEFT).value.y;

    constexpr float expo = 2.f;
    v = math::sgn(v) * std::pow(v, expo); //some expo

    auto config = m_hal.get_multi_config();
    if (config)
    {
        v *= config->motor_thrust * config->motors.size();
        m_input.vertical.thrust_rate.set(v);
    }
}

void Multi_HUD_Widget::process_vertical_thrust_offset()
{
    float v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::LEFT).value.y;

    constexpr float expo = 2.f;
    v = math::sgn(v) * std::pow(v, expo); //some expo

    auto config = m_hal.get_multi_config();
    if (config)
    {
        v *= config->motor_thrust * config->motors.size();
        m_input.vertical.thrust_offset.set(v);
    }
}

void Multi_HUD_Widget::process_vertical_climb_rate()
{
    float v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::LEFT).value.y;

    v *= 5.f;

    m_input.vertical.velocity.set(v);
}

void Multi_HUD_Widget::process_vertical()
{
    switch (m_input.vertical.mode.value)
    {
    case silk::stream::IMulti_Input::Vertical::Mode::THRUST_RATE:
        process_vertical_thrust_rate();
        break;
    case silk::stream::IMulti_Input::Vertical::Mode::THRUST_OFFSET:
        process_vertical_thrust_offset();
        break;
    case silk::stream::IMulti_Input::Vertical::Mode::VELOCITY:
        process_vertical_climb_rate();
        break;
    }

    if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_UP))
    {
        int v = math::clamp(static_cast<int>(m_input.vertical.mode.get()) - 1,
                            static_cast<int>(silk::stream::IMulti_Input::Vertical::Mode::THRUST_RATE),
                            static_cast<int>(silk::stream::IMulti_Input::Vertical::Mode::VELOCITY));

        m_input.vertical.mode.set(static_cast<silk::stream::IMulti_Input::Vertical::Mode>(v));
    }
    else if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_DOWN))
    {
        int v = math::clamp(static_cast<int>(m_input.vertical.mode.get()) + 1,
                            static_cast<int>(silk::stream::IMulti_Input::Vertical::Mode::THRUST_RATE),
                            static_cast<int>(silk::stream::IMulti_Input::Vertical::Mode::VELOCITY));

        m_input.vertical.mode.set(static_cast<silk::stream::IMulti_Input::Vertical::Mode>(v));
    }

}

void Multi_HUD_Widget::process_horizontal_angle_rate()
{
    math::vec2f v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::RIGHT).value;

    v.set(-v.y, v.x); //vertical stick rotates along X axis, horizontal stick along Y axis

    constexpr float expo = 2.f;
    v = math::sgn(v) * math::vec2f(std::pow(v.x, expo), std::pow(v.y, expo)); //some expo

    v *= math::vec2f(math::anglef::_2pi);

    m_input.horizontal.angle_rate.set(v);
}

void Multi_HUD_Widget::process_horizontal_angle()
{
    math::vec2f v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::RIGHT).value;

    v.set(-v.y, v.x); //vertical stick rotates along X axis, horizontal stick along Y axis

    constexpr float expo = 2.f;
    v = math::sgn(v) * math::vec2f(std::pow(v.x, expo), std::pow(v.y, expo)); //some expo

    v *= math::vec2f(math::anglef::pi / 4.f);

    m_input.horizontal.angle.set(v);
}

void Multi_HUD_Widget::process_horizontal_velocity()
{
    math::vec2f v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::RIGHT).value;
    v *= 5.f;
    m_input.horizontal.velocity.set(v);
}

void Multi_HUD_Widget::process_horizontal()
{
    switch (m_input.horizontal.mode.value)
    {
    case silk::stream::IMulti_Input::Horizontal::Mode::ANGLE_RATE:
        process_horizontal_angle_rate();
        break;
    case silk::stream::IMulti_Input::Horizontal::Mode::ANGLE:
        process_horizontal_angle();
        break;
    case silk::stream::IMulti_Input::Horizontal::Mode::VELOCITY:
        process_horizontal_velocity();
        break;
    }

    if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_LEFT))
    {
        int v = math::clamp(static_cast<int>(m_input.horizontal.mode.get()) - 1,
                            static_cast<int>(silk::stream::IMulti_Input::Horizontal::Mode::ANGLE_RATE),
                            static_cast<int>(silk::stream::IMulti_Input::Horizontal::Mode::VELOCITY));

        m_input.horizontal.mode.set(static_cast<silk::stream::IMulti_Input::Horizontal::Mode>(v));
    }
    else if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_RIGHT))
    {
        int v = math::clamp(static_cast<int>(m_input.horizontal.mode.get()) + 1,
                            static_cast<int>(silk::stream::IMulti_Input::Horizontal::Mode::ANGLE_RATE),
                            static_cast<int>(silk::stream::IMulti_Input::Horizontal::Mode::VELOCITY));

        m_input.horizontal.mode.set(static_cast<silk::stream::IMulti_Input::Horizontal::Mode>(v));
    }
}

void Multi_HUD_Widget::process_yaw_angle_rate()
{
    float v = 0;
    v += m_gamepad->get_axis_data(qinput::Gamepad::Axis::LEFT_TRIGGER).value; //left rotates counter-clockwise (so positive angle)
    v -= m_gamepad->get_axis_data(qinput::Gamepad::Axis::RIGHT_TRIGGER).value;//right rotates clockwise (so negative angle)

    constexpr float expo = 2.f;
    v = math::sgn(v) * std::pow(v, expo); //some expo

    v *= math::anglef::pi2;

    m_input.yaw.angle_rate.set(v);
}

void Multi_HUD_Widget::process_yaw()
{
    switch (m_input.yaw.mode.value)
    {
    case silk::stream::IMulti_Input::Yaw::Mode::ANGLE_RATE:
        process_yaw_angle_rate();
        break;
    }
}

void Multi_HUD_Widget::process_mode_idle()
{
    if (m_gamepad->is_button_held(qinput::Gamepad::Button::HOME) &&
        m_gamepad->is_button_released(qinput::Gamepad::Button::PS_X))
    {
        m_input.mode.set(silk::stream::IMulti_Input::Mode::ARMED);
        QLOGI("Trying to ARM");
    }

    m_ecef_home = m_state.ecef_position;
}

void Multi_HUD_Widget::process_mode_armed()
{
    if (m_gamepad->is_button_held(qinput::Gamepad::Button::HOME) &&
        m_gamepad->is_button_released(qinput::Gamepad::Button::PS_X))
    {
        m_input.mode.set(silk::stream::IMulti_Input::Mode::IDLE);
        QLOGI("Trying to DISARM");
    }

    process_vertical();
    process_horizontal();
    process_yaw();
}

void Multi_HUD_Widget::process_mode()
{
    switch (m_input.mode.value)
    {
    case silk::stream::IMulti_Input::Mode::IDLE:
        process_mode_idle();
        break;
    case silk::stream::IMulti_Input::Mode::ARMED:
        process_mode_armed();
        break;
    }
}


void Multi_HUD_Widget::acquire_gamepad()
{
    if (!m_gamepad)
    {
        auto gamepads = m_input_mgr.get_all_gamepads();
        if (!gamepads.empty())
        {
            m_gamepad = gamepads.front();
        }
    }
}

void Multi_HUD_Widget::process_gamepad()
{
    acquire_gamepad();
    if (!m_gamepad)
    {
        return;
    }

    process_mode();

    m_comms.send_multi_input_value(m_input);
}

#define SYNC(x) \
{\
    auto& prev_remote = m_prev_state.last_input.x;\
    auto& remote = m_state.last_input.x;\
    auto& local = m_input.x;\
    if (prev_remote.version != local.version && remote.version >= local.version)\
    {\
        if (local.value == remote.value)\
        {\
            QLOGI("Confirmed input {}: value {}@{}", #x, remote.get(), static_cast<uint32_t>(remote.version));\
        }\
        else\
        {\
            QLOGI("Overriden input {}: value {}@{}, req {}@{}", #x, remote.get(), static_cast<uint32_t>(remote.version), local.get(), static_cast<uint32_t>(local.version));\
        }\
        local.value = remote.value;\
        local.version = remote.version;\
    }\
}

void Multi_HUD_Widget::sync_input()
{
    SYNC(toggles.land);
    SYNC(toggles.take_off);
    SYNC(toggles.return_home);

    SYNC(vertical.mode);
    SYNC(vertical.thrust_rate);
    SYNC(vertical.thrust_offset);
    SYNC(vertical.velocity);

    SYNC(horizontal.mode);
    SYNC(horizontal.angle_rate);
    SYNC(horizontal.angle);
    SYNC(horizontal.velocity);

    SYNC(yaw.mode);
    SYNC(yaw.angle_rate);

    SYNC(mode);
    SYNC(reference_frame);

    SYNC(assists.stay_in_range);
    SYNC(assists.stay_in_battery_range);
    SYNC(assists.stay_in_perimeter);
    SYNC(assists.avoid_altitude_drop);
    SYNC(assists.avoid_the_user);
    SYNC(assists.avoid_proximity);
}
#undef SYNC


void Multi_HUD_Widget::process()
{
    QLOG_TOPIC("input");

    auto const& samples = m_comms.get_multi_state_samples();
    if (!samples.empty())
    {
        m_state = samples.back().value;
    }

    process_gamepad();

    sync_input();

    m_prev_state = m_state;

    render();
}

void Multi_HUD_Widget::render()
{
    m_render_widget->begin_rendering();

    m_context.camera.set_viewport_and_aspect_ratio(q::video::Viewport(math::vec2u32::zero, math::vec2u32(m_render_widget->width(), m_render_widget->height())));

    auto color = math::color::u32_to_rgba<float>(0x2c3e50);
    q::System::inst().get_renderer()->get_render_target()->set_color_clear_value(color);
    q::System::inst().get_renderer()->get_render_target()->clear_all();


    render_video();


    util::coordinates::LLA lla;
    lla.latitude = 0.59341195;
    lla.longitude = -2.0478571;
    lla.altitude = 251.702;

    auto N = util::coordinates::normal_distance(lla.latitude);
    auto ecef = util::coordinates::lla_to_ecef(lla);
    auto trans = util::coordinates::enu_to_ecef_transform(lla);
    auto axis_x = trans.get_axis_x();
    auto axis_y = trans.get_axis_y();
    auto axis_z = trans.get_axis_z();

    math::vec3d g(0, 0, -1);
    auto G = math::rotate(trans, g);

//    {
//        auto delta = m_uav.state.enu_position - m_camera_position_target;
//        delta *= 0.9f;
//        m_camera_position_target += delta;
//        m_context.camera.set_position(m_context.camera.get_position() + delta);
//        m_camera_controller.set_focus_point(m_camera_position_target);
//    }

    m_context.painter.set_camera(m_context.camera);

    render_ground();
    render_hud();

    m_context.painter.flush();

    m_render_widget->end_rendering();
    m_render_widget->update();
}

void Multi_HUD_Widget::render_video()
{
    if (!m_video.texture)
    {
        return;
    }

    auto mat = m_context.materials.textured_2d;
    mat.get_render_state(0).set_depth_test(true);
    mat.get_render_state(0).set_depth_write(false);
    mat.get_render_state(0).set_culling(false);
    mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::SOLID);
    auto sampler = mat.get_sampler(0, 0);
    sampler.set_texture(m_video.texture);
    mat.set_sampler(0, 0, sampler);
    m_context.painter.set_material(mat);

//    float ar = float(width()) / height();
//    math::vec2f uv0, uv1;
//    uv1.y =

    m_context.painter.fill_rectangle(q::draw::Vertex(math::vec2f(0, 0), math::vec2f(0, 0)), q::draw::Vertex(math::vec2f(width(), height()), math::vec2f(1, 1)));
}

void Multi_HUD_Widget::render_ground()
{
    if (!m_ecef_home)
    {
        return;
    }
    math::vec3d home = *m_ecef_home;

    //m_state.frame

    math::vec3s32 offset(home);
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

void Multi_HUD_Widget::render_hud()
{
    m_context.painter.set_material(m_context.materials.font);
    q::text::Texter texter;
    q::text::Style style;
    style.font = m_context.font;
    style.height = 12;
    texter.set_style(style);
    switch (m_input.mode.value)
    {
    case silk::stream::IMulti_Input::Mode::IDLE:
        texter.draw_string(m_context.painter, "Idle", math::vec2f(0, 20));
        break;
    case silk::stream::IMulti_Input::Mode::ARMED:
        texter.draw_string(m_context.painter, "Armed", math::vec2f(0, 20));
        break;
    }
    switch (m_input.vertical.mode.value)
    {
    case silk::stream::IMulti_Input::Vertical::Mode::THRUST_RATE:
        texter.draw_string(m_context.painter, "Thrust Rate", math::vec2f(0, 60));
        break;
    case silk::stream::IMulti_Input::Vertical::Mode::THRUST_OFFSET:
        texter.draw_string(m_context.painter, "Thrust Offset", math::vec2f(0, 60));
        break;
    case silk::stream::IMulti_Input::Vertical::Mode::VELOCITY:
        texter.draw_string(m_context.painter, "Climb Rate", math::vec2f(0, 60));
        break;
    }
    switch (m_input.horizontal.mode.value)
    {
    case silk::stream::IMulti_Input::Horizontal::Mode::ANGLE_RATE:
        texter.draw_string(m_context.painter, "Angle Rate", math::vec2f(0, 100));
        break;
    case silk::stream::IMulti_Input::Horizontal::Mode::ANGLE:
        texter.draw_string(m_context.painter, "Angle", math::vec2f(0, 100));
        break;
    case silk::stream::IMulti_Input::Horizontal::Mode::VELOCITY:
        texter.draw_string(m_context.painter, "Velocity", math::vec2f(0, 100));
        break;
    }
}
