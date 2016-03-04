#include "hud/Multi_HUD_Widget.h"
#include "physics/constants.h"

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

    m_border_text_style.font = m_context.font;
    m_border_text_style.height = 20;
    m_border_text_style.decoration = q::text::Decoration::OUTLINE;
    m_border_text_style.decoration_color = 0x77000000;
    m_border_text_style.decoration_thickness = 1;

    m_camera.set_perspective_vertical_fov(math::anglef(math::radians(60.f)));
    m_camera.set_near_distance(0.05f);
    m_camera.set_far_distance(20000.f);

    //commands defaults
    m_commands.vertical.mode.set(silk::stream::IMulti_Commands::Vertical::Mode::THRUST);
    m_commands.horizontal.mode.set(silk::stream::IMulti_Commands::Horizontal::Mode::ANGLE);
    m_commands.yaw.mode.set(silk::stream::IMulti_Commands::Yaw::Mode::ANGLE);
}

void Multi_HUD_Widget::decode_video(silk::stream::gs::Video::Value const& frame)
{
    if (frame.data.empty())
    {
        return;
    }

    if (m_video.decoder.decode_frame(frame, frame.resolution, m_video.data, Video_Decoder::Format::RGBA))
    {
        m_video.size = frame.resolution;
    }
    else
    {
        m_video.data.clear();
    }
}

void Multi_HUD_Widget::upload_video_frame()
{
    if (m_video.data.empty())
    {
        return;
    }

    if (!m_video.texture)
    {
        m_video.texture = q::video::Texture::create(q::Path("video/texture"));
    }
    if (m_video.texture->get_size() != m_video.size)
    {
        if (!m_video.texture->allocate(q::video::Texture::Format::RGBA_8, m_video.size))
        {
            return;
        }
    }

    m_video.texture->upload_data(0, m_video.data.data());
}
void Multi_HUD_Widget::process_vertical_thrust_rate()
{
    float v = 0;

    if (m_gamepad)
    {
        v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::LEFT).value.y;

        constexpr float expo = 2.f;
        v = math::sgn(v) * std::pow(v, expo); //some expo
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    float kb_value = kb.is_key_pressed(qinput::Key_Code::SHIFT) ? 0.5f : 0.1f;
    v += kb.is_key_pressed('=') ? kb_value : 0;
    v -= kb.is_key_pressed('-') ? kb_value : 0;

    auto config = m_hal.get_multi_config();
    if (config)
    {
        v *= config->motor_thrust * config->motors.size();
        m_commands.vertical.thrust_rate.set(v);
    }
}

void Multi_HUD_Widget::process_vertical_thrust()
{
    float v = 0;

    if (m_gamepad)
    {
        v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::LEFT).value.y;

        constexpr float expo = 2.f;
        v = math::sgn(v) * std::pow(v, expo); //some expo
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    float kb_value = kb.is_key_pressed(qinput::Key_Code::SHIFT) ? 0.8f : 0.6f;
    v += kb.is_key_pressed('=') ? kb_value : 0;
    v -= kb.is_key_pressed('-') ? kb_value : 0;

    auto config = m_hal.get_multi_config();
    if (config)
    {
        v *= config->motor_thrust * config->motors.size();
        m_commands.vertical.thrust.set(v);
    }
}

void Multi_HUD_Widget::process_vertical_altitude()
{
    float dts = q::Seconds(m_dt).count();
    float v = m_commands.vertical.altitude.get();

    if (m_gamepad)
    {
        float stick = m_gamepad->get_stick_data(qinput::Gamepad::Stick::LEFT).value.y;

        constexpr float expo = 3.f;
        stick = math::sgn(stick) * std::pow(stick, expo); //some expo

        v += stick * dts;
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    float kb_value = (kb.is_key_pressed(qinput::Key_Code::SHIFT) ? 3.f : 1.f) * dts;
    v += kb.is_key_pressed('=') ? kb_value : 0;
    v -= kb.is_key_pressed('-') ? kb_value : 0;

    m_commands.vertical.altitude.set(v);
}

void Multi_HUD_Widget::process_vertical()
{
    switch (m_commands.vertical.mode.get())
    {
    case silk::stream::IMulti_Commands::Vertical::Mode::THRUST_RATE:
        process_vertical_thrust_rate();
        break;
    case silk::stream::IMulti_Commands::Vertical::Mode::THRUST:
        process_vertical_thrust();
        break;
    case silk::stream::IMulti_Commands::Vertical::Mode::ALTITUDE:
        process_vertical_altitude();
        break;
    }

    if (m_gamepad)
    {
        if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_UP))
        {
            int v = math::clamp(static_cast<int>(m_commands.vertical.mode.get()) - 1,
                                static_cast<int>(silk::stream::IMulti_Commands::Vertical::Mode::THRUST_RATE),
                                static_cast<int>(silk::stream::IMulti_Commands::Vertical::Mode::ALTITUDE));

            m_commands.vertical.mode.set(static_cast<silk::stream::IMulti_Commands::Vertical::Mode>(v));
        }
        else if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_DOWN))
        {
            int v = math::clamp(static_cast<int>(m_commands.vertical.mode.get()) + 1,
                                static_cast<int>(silk::stream::IMulti_Commands::Vertical::Mode::THRUST_RATE),
                                static_cast<int>(silk::stream::IMulti_Commands::Vertical::Mode::ALTITUDE));

            m_commands.vertical.mode.set(static_cast<silk::stream::IMulti_Commands::Vertical::Mode>(v));
        }
    }
    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    if (kb.is_key_released('V'))
    {
        m_commands.vertical.mode.set(silk::stream::IMulti_Commands::Vertical::Mode::THRUST_RATE);
    }
    else if (kb.is_key_released('B'))
    {
        m_commands.vertical.mode.set(silk::stream::IMulti_Commands::Vertical::Mode::THRUST);
    }
    else if (kb.is_key_released('N'))
    {
        m_commands.vertical.mode.set(silk::stream::IMulti_Commands::Vertical::Mode::ALTITUDE);
    }

}

void Multi_HUD_Widget::process_horizontal_angle_rate()
{
    math::vec2f v;

    if (m_gamepad)
    {
        v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::RIGHT).value;
        v.set(-v.y, v.x); //vertical stick rotates along X axis, horizontal stick along Y axis

        constexpr float expo = 2.f;
        v = math::sgn(v) * math::vec2f(std::pow(v.x, expo), std::pow(v.y, expo)); //some expo

        v *= math::vec2f(math::anglef::_2pi);
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    float kb_value = kb.is_key_pressed(qinput::Key_Code::SHIFT) ? math::radians(120.f) : math::radians(45.f);
    v.y -= kb.is_key_pressed(qinput::Key_Code::LEFT) ? kb_value : 0;
    v.y += kb.is_key_pressed(qinput::Key_Code::RIGHT) ? kb_value : 0;
    v.x -= kb.is_key_pressed(qinput::Key_Code::UP) ? kb_value : 0;
    v.x += kb.is_key_pressed(qinput::Key_Code::DOWN) ? kb_value : 0;

    m_commands.horizontal.angle_rate.set(v);
}

void Multi_HUD_Widget::process_horizontal_angle()
{
    math::vec2f v;

    if (m_gamepad)
    {
        v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::RIGHT).value;
        v.set(-v.y, v.x); //vertical stick rotates along X axis, horizontal stick along Y axis

        constexpr float expo = 3.f;
        v = math::sgn(v) * math::vec2f(std::pow(v.x, expo), std::pow(v.y, expo)); //some expo

        v *= math::vec2f(math::anglef::pi / 4.f);
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    float kb_value = kb.is_key_pressed(qinput::Key_Code::SHIFT) ? math::radians(40.f) : math::radians(20.f);
    v.y -= kb.is_key_pressed(qinput::Key_Code::LEFT) ? kb_value : 0;
    v.y += kb.is_key_pressed(qinput::Key_Code::RIGHT) ? kb_value : 0;
    v.x -= kb.is_key_pressed(qinput::Key_Code::UP) ? kb_value : 0;
    v.x += kb.is_key_pressed(qinput::Key_Code::DOWN) ? kb_value : 0;

    m_commands.horizontal.angle.set(v);
}

void Multi_HUD_Widget::process_horizontal_position()
{
    float dts = q::Seconds(m_dt).count();

    //compute the front/right in enu space
    math::vec2f front_vector = math::normalized(math::vec2f(math::rotate(m_uav.local_to_enu_quat, physics::constants::uav_front_vector)));
    math::vec2f right_vector = math::normalized(math::vec2f(math::rotate(m_uav.local_to_enu_quat, physics::constants::uav_right_vector)));

    math::vec2f v = m_commands.horizontal.position.get();
    if (m_gamepad)
    {
        math::vec2f stick = m_gamepad->get_stick_data(qinput::Gamepad::Stick::RIGHT).value;

        constexpr float expo = 3.f;
        stick = math::sgn(stick) * math::vec2f(std::pow(stick.x, expo), std::pow(stick.y, expo)); //some expo

        v += right_vector * (stick.x * dts);
        v += front_vector * (stick.y * dts);
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    float kb_value = kb.is_key_pressed(qinput::Key_Code::SHIFT) ? 3.f : 0.5f;
    v -= right_vector * (kb.is_key_pressed(qinput::Key_Code::LEFT) ? kb_value : 0);
    v += right_vector * (kb.is_key_pressed(qinput::Key_Code::RIGHT) ? kb_value : 0);
    v += front_vector * (kb.is_key_pressed(qinput::Key_Code::UP) ? kb_value : 0);
    v -= front_vector * (kb.is_key_pressed(qinput::Key_Code::DOWN) ? kb_value : 0);

    m_commands.horizontal.position.set(v);
}

void Multi_HUD_Widget::process_horizontal()
{
    switch (m_commands.horizontal.mode.get())
    {
    case silk::stream::IMulti_Commands::Horizontal::Mode::ANGLE_RATE:
        process_horizontal_angle_rate();
        break;
    case silk::stream::IMulti_Commands::Horizontal::Mode::ANGLE:
        process_horizontal_angle();
        break;
    case silk::stream::IMulti_Commands::Horizontal::Mode::POSITION:
        process_horizontal_position();
        break;
    }

    if (m_gamepad)
    {
        if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_LEFT))
        {
            int v = math::clamp(static_cast<int>(m_commands.horizontal.mode.get()) - 1,
                                static_cast<int>(silk::stream::IMulti_Commands::Horizontal::Mode::ANGLE_RATE),
                                static_cast<int>(silk::stream::IMulti_Commands::Horizontal::Mode::POSITION));

            m_commands.horizontal.mode.set(static_cast<silk::stream::IMulti_Commands::Horizontal::Mode>(v));
        }
        else if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_RIGHT))
        {
            int v = math::clamp(static_cast<int>(m_commands.horizontal.mode.get()) + 1,
                                static_cast<int>(silk::stream::IMulti_Commands::Horizontal::Mode::ANGLE_RATE),
                                static_cast<int>(silk::stream::IMulti_Commands::Horizontal::Mode::POSITION));

            m_commands.horizontal.mode.set(static_cast<silk::stream::IMulti_Commands::Horizontal::Mode>(v));
        }
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    if (kb.is_key_released('H'))
    {
        m_commands.horizontal.mode.set(silk::stream::IMulti_Commands::Horizontal::Mode::ANGLE_RATE);
    }
    else if (kb.is_key_released('J'))
    {
        m_commands.horizontal.mode.set(silk::stream::IMulti_Commands::Horizontal::Mode::ANGLE);
    }
    else if (kb.is_key_released('K'))
    {
        m_commands.horizontal.mode.set(silk::stream::IMulti_Commands::Horizontal::Mode::POSITION);
    }
}

void Multi_HUD_Widget::process_yaw_angle_rate()
{
    float v = 0;
    if (m_gamepad)
    {
        v += m_gamepad->get_axis_data(qinput::Gamepad::Axis::LEFT_TRIGGER).value; //left rotates counter-clockwise (so positive angle)
        v -= m_gamepad->get_axis_data(qinput::Gamepad::Axis::RIGHT_TRIGGER).value;//right rotates clockwise (so negative angle)

        constexpr float expo = 2.f;
        v = math::sgn(v) * std::pow(v, expo); //some expo

        v *= math::anglef::pi2;
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    float kb_value = kb.is_key_pressed(qinput::Key_Code::SHIFT) ? math::radians(70.f) : math::radians(40.f);
    v += kb.is_key_pressed(',') ? kb_value : 0;
    v -= kb.is_key_pressed('.') ? kb_value : 0;

    m_commands.yaw.angle_rate.set(v);
}

void Multi_HUD_Widget::process_yaw_angle()
{
    float dts = q::Seconds(m_dt).count();

    float v = m_commands.yaw.angle.get();
    if (m_gamepad)
    {
        float gv = 0;
        gv += m_gamepad->get_axis_data(qinput::Gamepad::Axis::LEFT_TRIGGER).value; //left rotates counter-clockwise (so positive angle)
        gv -= m_gamepad->get_axis_data(qinput::Gamepad::Axis::RIGHT_TRIGGER).value;//right rotates clockwise (so negative angle)

        constexpr float expo = 4.f;
        gv = math::sgn(gv) * std::pow(gv, expo) * dts; //some expo

        v += gv * math::anglef::pi2;
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    float kb_value = kb.is_key_pressed(qinput::Key_Code::SHIFT) ? math::radians(7.f) : math::radians(4.f);
    v += kb.is_key_pressed(',') ? kb_value : 0;
    v -= kb.is_key_pressed('.') ? kb_value : 0;

    m_commands.yaw.angle.set(v);
}

void Multi_HUD_Widget::process_yaw()
{
    switch (m_commands.yaw.mode.get())
    {
    case silk::stream::IMulti_Commands::Yaw::Mode::ANGLE_RATE:
        process_yaw_angle_rate();
        break;
    case silk::stream::IMulti_Commands::Yaw::Mode::ANGLE:
        process_yaw_angle();
        break;
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    if (kb.is_key_released('Y'))
    {
        m_commands.yaw.mode.set(silk::stream::IMulti_Commands::Yaw::Mode::ANGLE_RATE);
    }
    else if (kb.is_key_released('U'))
    {
        m_commands.yaw.mode.set(silk::stream::IMulti_Commands::Yaw::Mode::ANGLE);
    }
}

void Multi_HUD_Widget::process_toggles()
{
    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    if (kb.is_key_released(qinput::Key_Code::HOME))
    {
        m_commands.toggles.return_home.set(!m_commands.toggles.return_home.get());
        QLOGI("Return Home {}", m_commands.toggles.return_home.get() ? "ON" : "OFF");
    }
}

void Multi_HUD_Widget::process_mode_idle()
{
    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    if ((m_gamepad && m_gamepad->is_button_held(qinput::Gamepad::Button::HOME) && m_gamepad->is_button_released(qinput::Gamepad::Button::PS_X)) ||
        (kb.is_key_pressed(qinput::Key_Code::SHIFT) && kb.is_key_released('A')))
    {
        m_commands.mode.set(silk::stream::IMulti_Commands::Mode::ARMED);
        QLOGI("Trying to ARM");
    }
}

void Multi_HUD_Widget::process_mode_armed()
{
    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    if ((m_gamepad && m_gamepad->is_button_held(qinput::Gamepad::Button::HOME) && m_gamepad->is_button_released(qinput::Gamepad::Button::PS_X)) ||
        (kb.is_key_pressed(qinput::Key_Code::SHIFT) && kb.is_key_released('A')))
    {
        m_commands.mode.set(silk::stream::IMulti_Commands::Mode::IDLE);
        QLOGI("Trying to DISARM");
    }

    process_vertical();
    process_horizontal();
    process_yaw();
    process_toggles();
}

void Multi_HUD_Widget::process_mode()
{
    switch (m_commands.mode.get())
    {
    case silk::stream::IMulti_Commands::Mode::IDLE:
        process_mode_idle();
        break;
    case silk::stream::IMulti_Commands::Mode::ARMED:
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

void Multi_HUD_Widget::process_commands()
{
    acquire_gamepad();
    process_mode();

    m_comms.send_multi_commands_value(m_commands);
}

struct Merge_Commands
{
    template<class T>
    bool operator()(const char* name, T const& remote_prev, T const& remote_crt, T& local)
    {
        if (remote_prev.version != remote_crt.version)
        {
            if (local.get() == remote_crt.get())
            {
                QLOGI("Confirmed command {}: value {}@{}", name, remote_crt.get(), static_cast<uint32_t>(remote_crt.version));
            }
            else
            {
                QLOGI("Overriden command {}: remote {}@{}, local {}@{}", name, remote_crt.get(), static_cast<uint32_t>(remote_crt.version), local.get(), static_cast<uint32_t>(local.version));
            }
            local.set_unversioned(remote_crt.get());
        }
        return true;
    }
};

void Multi_HUD_Widget::sync_commands()
{
    Merge_Commands func;
    silk::stream::IMulti_Commands::apply(func, m_prev_state.commands, m_state.commands, m_commands);
    m_prev_state.commands = m_state.commands;
}


void Multi_HUD_Widget::process()
{
    QLOG_TOPIC("command");

    auto now = q::Clock::now();
    m_dt = now - m_last_tp;
    m_last_tp = now;

    {
        auto const& samples = m_comms.get_video_samples();
        for (auto const& s: samples)
        {
            decode_video(s.value);
        }
    }

    {
        auto const& samples = m_comms.get_multi_state_samples();
        for (auto const& s: samples)
        {
            m_state = s.value;
        }
    }

    util::coordinates::LLA lla_position = util::coordinates::ecef_to_lla(m_state.home_position.value);
    util::coordinates::enu_to_ecef_transform_and_inv(lla_position, m_uav.enu_to_ecef_transform, m_uav.ecef_to_enu_transform);

    m_uav.enu_position = math::vec3f(math::transform(m_uav.ecef_to_enu_transform, m_state.position.value));
    m_uav.enu_velocity = math::vec3f(math::rotate(m_uav.ecef_to_enu_transform, math::vec3d(m_state.velocity.value)));
    m_uav.local_to_enu_quat = m_state.frame.value;


    upload_video_frame();

    process_commands();

    sync_commands();

    m_prev_state = m_state;

    render();
}

void Multi_HUD_Widget::render()
{
    m_render_widget->begin_rendering();

    m_camera.set_viewport_and_aspect_ratio(q::video::Viewport(math::vec2u32::zero, math::vec2u32(m_render_widget->width(), m_render_widget->height())));

    auto color = math::color::u32_to_rgba<float>(0x2c3e50);
    q::System::inst().get_renderer()->get_render_target()->set_color_clear_value(color);
    q::System::inst().get_renderer()->get_render_target()->clear_all();

    m_context.painter.push_post_clip_transform(math::trans3df());
    m_context.painter.set_post_clip_transform(math::trans3df());

    render_video();


//    {
//        util::coordinates::LLA lla;
//        lla.latitude = 0.59341195;
//        lla.longitude = -2.0478571;
//        lla.altitude = 251.702;

//        auto N = util::coordinates::normal_distance(lla.latitude);
//        auto ecef = util::coordinates::lla_to_ecef(lla);
//        auto trans = util::coordinates::enu_to_ecef_transform(lla);
//        auto axis_x = trans.get_axis_x();
//        auto axis_y = trans.get_axis_y();
//        auto axis_z = trans.get_axis_z();

//        math::vec3d g(0, 0, -1);
//        auto G = math::rotate(trans, g);
//    }


    //compute the camera rotation from the uav rotation except for the yaw
    m_camera.set_position(m_uav.enu_position);
    m_camera.set_rotation(m_state.frame.value);

    m_context.painter.set_camera(m_camera);

//    math::trans3df uav_local_to_enu_transform;
//    uav_local_to_enu_transform.set_translation(m_uav.enu_position);
//    uav_local_to_enu_transform.set_rotation(m_state.frame.value);


    render_ground();
    render_hud();

    m_context.painter.flush();

    m_render_widget->end_rendering();
    m_render_widget->update();

    m_context.painter.pop_post_clip_transform();
}

void Multi_HUD_Widget::render_video()
{
    if (!m_video.texture)
    {
        return;
    }

    m_context.painter.push_post_clip_transform(math::trans3df());
    m_context.painter.set_post_clip_transform(math::trans3df());


    auto mat = m_context.materials.textured_2d;
    mat.get_render_state(0).set_depth_test(true);
    mat.get_render_state(0).set_depth_write(false);
    mat.get_render_state(0).set_culling(false);
    mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::SOLID);
    auto sampler = mat.get_sampler(0, 0);
    sampler.set_texture(m_video.texture);
    mat.set_sampler(0, 0, sampler);
    m_context.painter.set_material(mat);

    float tw = 1.f / m_video.size.x;
    float th = 1.f / m_video.size.y;

    //float margin = 500;
    float widget_ar = float(width()) / height();
    float video_ar = float(m_video.size.x) / m_video.size.y;
    if (widget_ar < 1.f)
    {
        //width smaller than height
        //top and bottom bars
        //show full width

        float middle_height = width() / video_ar;
        float bar_height = (height() - middle_height) / 2.f;

        //top
        m_context.painter.fill_rectangle(q::draw::Vertex(math::vec2f(0, 0), math::vec2f(0, 0)),
                                         q::draw::Vertex(math::vec2f(width(), bar_height), math::vec2f(1, th)));

        //middle
        m_context.painter.fill_rectangle(q::draw::Vertex(math::vec2f(0, bar_height), math::vec2f(0, 0)),
                                         q::draw::Vertex(math::vec2f(width(), bar_height + middle_height), math::vec2f(1, 1)));

        //bottom
        m_context.painter.fill_rectangle(q::draw::Vertex(math::vec2f(0, bar_height + middle_height), math::vec2f(0, 1 - th)),
                                         q::draw::Vertex(math::vec2f(width(), bar_height + middle_height + bar_height), math::vec2f(1, 1 - th)));
    }
    else
    {
        //height smaller than width
        //left and right bars
        //show full height

        float middle_width = height() * video_ar;
        float bar_width = (width() - middle_width) / 2.f;

        //left
        m_context.painter.fill_rectangle(q::draw::Vertex(math::vec2f(0, 0), math::vec2f(0, 0)),
                                         q::draw::Vertex(math::vec2f(bar_width, height()), math::vec2f(tw, 1)));

        //middle
        m_context.painter.fill_rectangle(q::draw::Vertex(math::vec2f(bar_width, 0), math::vec2f(0, 0)),
                                         q::draw::Vertex(math::vec2f(bar_width + middle_width, height()), math::vec2f(1, 1)));

        //right
        m_context.painter.fill_rectangle(q::draw::Vertex(math::vec2f(bar_width + middle_width, 0), math::vec2f(1 - tw, 0)),
                                         q::draw::Vertex(math::vec2f(bar_width + middle_width + bar_width, height()), math::vec2f(1 - tw, 1)));
    }

    m_context.painter.pop_post_clip_transform();
}

void Multi_HUD_Widget::render_ground()
{
    //m_state.frame

    math::vec3s32 offset(m_uav.enu_position);
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
    render_modes();
    render_horizon();
    render_altitude();
}

void Multi_HUD_Widget::render_modes()
{
    m_context.painter.push_post_clip_transform(math::trans3df());
    m_context.painter.set_post_clip_transform(math::trans3df());

    m_context.painter.set_material(m_context.materials.font);
    m_border_text_style.height = 20;
    m_context.texter.set_style(m_border_text_style);

    std::string str;
    switch (m_commands.mode.get())
    {
    case silk::stream::IMulti_Commands::Mode::IDLE:
        str = q::util::format2<std::string>("M:Idle");
        break;
    case silk::stream::IMulti_Commands::Mode::ARMED:
        str = q::util::format2<std::string>("M:Armed");
        break;
    }
    m_context.texter.draw_string(m_context.painter, str, math::vec2f(0, 20));

    float v_speed = m_uav.enu_velocity.z;
    switch (m_commands.vertical.mode.get())
    {
    case silk::stream::IMulti_Commands::Vertical::Mode::THRUST_RATE:
        str = q::util::format2<std::string>("V:Rate:{.1}N/s | ALT:{.1}m | SPD:{.2}m/s", m_commands.vertical.thrust_rate.get(), m_uav.enu_position.z, v_speed);
        break;
    case silk::stream::IMulti_Commands::Vertical::Mode::THRUST:
        str = q::util::format2<std::string>("V:Thrust:{.1}N | ALT:{.1}m | SPD:{.2}m/s", m_commands.vertical.thrust.get(), m_uav.enu_position.z, v_speed);
        break;
    case silk::stream::IMulti_Commands::Vertical::Mode::ALTITUDE:
        str = q::util::format2<std::string>("V:Altitude:{.1}m | ALT:{.1}m | SPD:{.2}m/s", m_commands.vertical.altitude.get(), m_uav.enu_position.z, v_speed);
        break;
    }
    m_context.texter.draw_string(m_context.painter, str, math::vec2f(0, 50));

    float h_speed = math::length(math::vec2f(m_uav.enu_velocity.x, m_uav.enu_velocity.y));
    switch (m_commands.horizontal.mode.get())
    {
    case silk::stream::IMulti_Commands::Horizontal::Mode::ANGLE_RATE:
        str = q::util::format2<std::string>("H:Rate:{.1}°/s | SPD:{.2}m/s", m_commands.horizontal.angle_rate.get(), h_speed);
        break;
    case silk::stream::IMulti_Commands::Horizontal::Mode::ANGLE:
        str = q::util::format2<std::string>("H:Angle:{.1}° | SPD:{.2}m/s", math::degrees(m_commands.horizontal.angle.get()), h_speed);
        break;
    case silk::stream::IMulti_Commands::Horizontal::Mode::POSITION:
        str = q::util::format2<std::string>("H:Position:{.1}m | SPD:{.2}m/s", m_commands.horizontal.position.get(), h_speed);
        break;
    }
    m_context.texter.draw_string(m_context.painter, str, math::vec2f(0, 80));

    switch (m_commands.yaw.mode.get())
    {
    case silk::stream::IMulti_Commands::Yaw::Mode::ANGLE_RATE:
        str = q::util::format2<std::string>("Y:Rate:{.1}°/s", m_commands.yaw.angle_rate.get());
        break;
    case silk::stream::IMulti_Commands::Yaw::Mode::ANGLE:
        str = q::util::format2<std::string>("Y:Angle:{.1}°", math::degrees(m_commands.yaw.angle.get()));
        break;
    }
    m_context.texter.draw_string(m_context.painter, str, math::vec2f(0, 110));

    m_context.painter.pop_post_clip_transform();
}

void Multi_HUD_Widget::render_horizon()
{
    float yaw, pitch, roll;
    m_state.frame.value.get_as_euler_zxy(pitch, roll, yaw);


    auto mat = m_context.materials.primitive;
    mat.get_render_state(0).set_depth_test(true);
    mat.get_render_state(0).set_depth_write(false);
    mat.get_render_state(0).set_culling(false);
    mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);
    m_context.painter.set_material(mat);

    const float k_length = 0.3f;
    const float k_big_length = 0.5f;
    const float k_huge_length = 1.3f;
    const float k_distance_from_camera = 5.f;
    const float k_step = math::radians(5.f);
    const size_t k_line_count = math::radians(360.f) / k_step;
    //const int k_half_line_size = k_line_count / 2;
//    float max_alpha = 20.f;
//    float color_inc = 2.f / float(k_line_count);
    float angle = 0.f;
    for (size_t i = 0; i < k_line_count; i++, angle += k_step)
    {
        float length = i == 0 ? k_huge_length : (i % 4 == 0 ? k_big_length : k_length);
        math::quatf rot;
        rot.set_from_euler_zyx(angle, 0, yaw);
        math::vec3f p1 = m_uav.enu_position + math::rotate(rot, math::vec3f(-length, k_distance_from_camera, 0));
        math::vec3f p2 = m_uav.enu_position + math::rotate(rot, math::vec3f( length, k_distance_from_camera, 0));

        uint32_t color = 0x88FFFFFF;

        m_context.painter.draw_line(q::draw::Vertex(p1, color), q::draw::Vertex(p2, color));
    }
    //draw zero length
    {
        uint32_t color = 0x88FFFFFF;

        math::quatf rot;
        rot.set_from_euler_zyx(0, 0, yaw);
        math::vec3f p1 = m_uav.enu_position + math::rotate(rot, math::vec3f(-k_huge_length - 0.2f, k_distance_from_camera, 0));
        math::vec3f p2 = m_uav.enu_position + math::rotate(rot, math::vec3f(-k_huge_length, k_distance_from_camera, 0));
        m_context.painter.draw_line(q::draw::Vertex(p1, color), q::draw::Vertex(p2, color));

        p1 = m_uav.enu_position + math::rotate(rot, math::vec3f(k_huge_length + 0.2f, k_distance_from_camera, 0));
        p2 = m_uav.enu_position + math::rotate(rot, math::vec3f(k_huge_length, k_distance_from_camera, 0));
        m_context.painter.draw_line(q::draw::Vertex(p1, color), q::draw::Vertex(p2, color));

        math::vec2f out;
        m_camera.project(out, p1);

        m_context.painter.set_material(m_context.materials.font);
        m_border_text_style.height = 14;
        m_context.texter.set_style(m_border_text_style);
        m_context.texter.draw_string(m_context.painter, q::util::format2<std::string>("{.1}°", math::degrees(pitch)).c_str(), out);
    }
}

void Multi_HUD_Widget::render_altitude()
{
    //auto enu_position = math::transform(m_ecef_to_enu_trans, m_state.ecef_position.value);


//    auto mat = m_context.materials.font;
//    mat.get_render_state(0).set_depth_test(true);
//    mat.get_render_state(0).set_depth_write(false);
//    mat.get_render_state(0).set_culling(false);
//    mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);
//    m_context.painter.set_material(mat);

//    m_context.painter.set_material(m_context.materials.font);
//    m_border_text_style.height = 20;
//    m_context.texter.set_style(m_border_text_style);
//    m_context.texter.draw_string(m_context.painter, q::util::format2<std::string>("ALT: {.1}m", m_uav.enu_position.z).c_str(), math::vec2f(0, 150));
}

