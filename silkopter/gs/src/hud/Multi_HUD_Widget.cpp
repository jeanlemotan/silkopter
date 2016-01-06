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

    m_border_text_style.font = m_context.font;
    m_border_text_style.height = 20;
    m_border_text_style.decoration = q::text::Decoration::OUTLINE;
    m_border_text_style.decoration_color = 0x77000000;
    m_border_text_style.decoration_thickness = 1;

    m_camera.set_perspective_vertical_fov(math::anglef(math::radians(60.f)));
    m_camera.set_near_distance(0.05f);
    m_camera.set_far_distance(20000.f);

    //input defaults
    m_input.vertical.mode.set(silk::stream::IMulti_Input::Vertical::Mode::THRUST_OFFSET);
    m_input.horizontal.mode.set(silk::stream::IMulti_Input::Horizontal::Mode::ANGLE);
    m_input.yaw.mode.set(silk::stream::IMulti_Input::Yaw::Mode::ANGLE);
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
        m_input.vertical.thrust_rate.set(v);
    }
}

void Multi_HUD_Widget::process_vertical_thrust_offset()
{
    float v = 0;

    if (m_gamepad)
    {
        v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::LEFT).value.y;

        constexpr float expo = 2.f;
        v = math::sgn(v) * std::pow(v, expo); //some expo
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    float kb_value = kb.is_key_pressed(qinput::Key_Code::SHIFT) ? 0.5f : 0.2f;
    v += kb.is_key_pressed('=') ? kb_value : 0;
    v -= kb.is_key_pressed('-') ? kb_value : 0;

    auto config = m_hal.get_multi_config();
    if (config)
    {
        v *= config->motor_thrust * config->motors.size();
        m_input.vertical.thrust_offset.set(v);
    }
}

void Multi_HUD_Widget::process_vertical_climb_rate()
{
    float v = 0;

    if (m_gamepad)
    {
        v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::LEFT).value.y;
        v *= 5.f;
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    float kb_value = kb.is_key_pressed(qinput::Key_Code::SHIFT) ? 3.f : 1.f;
    v += kb.is_key_pressed('=') ? kb_value : 0;
    v -= kb.is_key_pressed('-') ? kb_value : 0;

    m_input.vertical.speed.set(v);
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
    case silk::stream::IMulti_Input::Vertical::Mode::SPEED:
        process_vertical_climb_rate();
        break;
    }

    if (m_gamepad)
    {
        if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_UP))
        {
            int v = math::clamp(static_cast<int>(m_input.vertical.mode.get()) - 1,
                                static_cast<int>(silk::stream::IMulti_Input::Vertical::Mode::THRUST_RATE),
                                static_cast<int>(silk::stream::IMulti_Input::Vertical::Mode::SPEED));

            m_input.vertical.mode.set(static_cast<silk::stream::IMulti_Input::Vertical::Mode>(v));
        }
        else if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_DOWN))
        {
            int v = math::clamp(static_cast<int>(m_input.vertical.mode.get()) + 1,
                                static_cast<int>(silk::stream::IMulti_Input::Vertical::Mode::THRUST_RATE),
                                static_cast<int>(silk::stream::IMulti_Input::Vertical::Mode::SPEED));

            m_input.vertical.mode.set(static_cast<silk::stream::IMulti_Input::Vertical::Mode>(v));
        }
    }
    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    if (kb.is_key_pressed('V') && kb.is_key_released('1'))
    {
        m_input.vertical.mode.set(silk::stream::IMulti_Input::Vertical::Mode::THRUST_RATE);
    }
    else if (kb.is_key_pressed('V') && kb.is_key_released('2'))
    {
        m_input.vertical.mode.set(silk::stream::IMulti_Input::Vertical::Mode::THRUST_OFFSET);
    }
    else if (kb.is_key_pressed('V') && kb.is_key_released('3'))
    {
        m_input.vertical.mode.set(silk::stream::IMulti_Input::Vertical::Mode::SPEED);
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

    m_input.horizontal.angle_rate.set(v);
}

void Multi_HUD_Widget::process_horizontal_angle()
{
    math::vec2f v;

    if (m_gamepad)
    {
        v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::RIGHT).value;
        v.set(-v.y, v.x); //vertical stick rotates along X axis, horizontal stick along Y axis

        constexpr float expo = 2.f;
        v = math::sgn(v) * math::vec2f(std::pow(v.x, expo), std::pow(v.y, expo)); //some expo

        v *= math::vec2f(math::anglef::pi / 4.f);
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    float kb_value = kb.is_key_pressed(qinput::Key_Code::SHIFT) ? math::radians(40.f) : math::radians(20.f);
    v.y -= kb.is_key_pressed(qinput::Key_Code::LEFT) ? kb_value : 0;
    v.y += kb.is_key_pressed(qinput::Key_Code::RIGHT) ? kb_value : 0;
    v.x -= kb.is_key_pressed(qinput::Key_Code::UP) ? kb_value : 0;
    v.x += kb.is_key_pressed(qinput::Key_Code::DOWN) ? kb_value : 0;

    m_input.horizontal.angle.set(v);
}

void Multi_HUD_Widget::process_horizontal_velocity()
{
    math::vec2f v;
    if (m_gamepad)
    {
        v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::RIGHT).value;
        v *= 5.f;
    }
    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    float kb_value = kb.is_key_pressed(qinput::Key_Code::SHIFT) ? math::radians(5.f) : math::radians(1.f);
    v.x -= kb.is_key_pressed(qinput::Key_Code::LEFT) ? kb_value : 0;
    v.x += kb.is_key_pressed(qinput::Key_Code::RIGHT) ? kb_value : 0;
    v.y -= kb.is_key_pressed(qinput::Key_Code::UP) ? kb_value : 0;
    v.y += kb.is_key_pressed(qinput::Key_Code::DOWN) ? kb_value : 0;

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

    if (m_gamepad)
    {
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

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    if (kb.is_key_pressed('H') && kb.is_key_released('1'))
    {
        m_input.horizontal.mode.set(silk::stream::IMulti_Input::Horizontal::Mode::ANGLE_RATE);
    }
    else if (kb.is_key_pressed('H') && kb.is_key_released('2'))
    {
        m_input.horizontal.mode.set(silk::stream::IMulti_Input::Horizontal::Mode::ANGLE);
    }
    else if (kb.is_key_pressed('H') && kb.is_key_released('3'))
    {
        m_input.horizontal.mode.set(silk::stream::IMulti_Input::Horizontal::Mode::VELOCITY);
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

    m_input.yaw.angle_rate.set(v);
}

void Multi_HUD_Widget::process_yaw_angle()
{
    process_yaw_angle_rate(); //same value as the angle rate
}

void Multi_HUD_Widget::process_yaw()
{
    switch (m_input.yaw.mode.value)
    {
    case silk::stream::IMulti_Input::Yaw::Mode::ANGLE_RATE:
        process_yaw_angle_rate();
        break;
    case silk::stream::IMulti_Input::Yaw::Mode::ANGLE:
        process_yaw_angle();
        break;
    }

    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    if (kb.is_key_pressed('Y') && kb.is_key_released('1'))
    {
        m_input.yaw.mode.set(silk::stream::IMulti_Input::Yaw::Mode::ANGLE_RATE);
    }
    else if (kb.is_key_pressed('Y') && kb.is_key_released('2'))
    {
        m_input.yaw.mode.set(silk::stream::IMulti_Input::Yaw::Mode::ANGLE);
    }
}

void Multi_HUD_Widget::process_mode_idle()
{
    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    if ((m_gamepad && m_gamepad->is_button_held(qinput::Gamepad::Button::HOME) && m_gamepad->is_button_released(qinput::Gamepad::Button::PS_X)) ||
        (kb.is_key_pressed(qinput::Key_Code::SHIFT) && kb.is_key_released('A')))
    {
        m_input.mode.set(silk::stream::IMulti_Input::Mode::ARMED);
        QLOGI("Trying to ARM");
    }
}

void Multi_HUD_Widget::process_mode_armed()
{
    const qinput::Keyboard& kb = m_input_mgr.get_keyboard();
    if ((m_gamepad && m_gamepad->is_button_held(qinput::Gamepad::Button::HOME) && m_gamepad->is_button_released(qinput::Gamepad::Button::PS_X)) ||
        (kb.is_key_pressed(qinput::Key_Code::SHIFT) && kb.is_key_released('A')))
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
    process_mode();

    m_comms.send_multi_input_value(m_input);
}

#define SYNC(x) \
{\
    auto& prev_remote = m_prev_state.input.value.x;\
    auto& remote = m_state.input.value.x;\
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
    SYNC(vertical.speed);

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
    for (auto const& s: samples)
    {
        m_state = s.value;
        decode_video(m_state.video.value);
    }

    util::coordinates::LLA lla_position = util::coordinates::ecef_to_lla(m_state.home_ecef_position.value);
    util::coordinates::enu_to_ecef_transform_and_inv(lla_position, m_uav.enu_to_ecef_transform, m_uav.ecef_to_enu_transform);

    m_uav.enu_position = math::vec3f(math::transform(m_uav.ecef_to_enu_transform, m_state.ecef_position.value));
    m_uav.enu_velocity = math::vec3f(math::rotate(m_uav.ecef_to_enu_transform, math::vec3d(m_state.ecef_velocity.value)));
    m_uav.local_to_enu_quat = m_state.frame.value;


    upload_video_frame();

    process_gamepad();

    sync_input();

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


    {
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
    }


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
    switch (m_input.mode.value)
    {
    case silk::stream::IMulti_Input::Mode::IDLE:
        str = q::util::format2<std::string>("M:Idle");
        break;
    case silk::stream::IMulti_Input::Mode::ARMED:
        str = q::util::format2<std::string>("M:Armed");
        break;
    }
    m_context.texter.draw_string(m_context.painter, str, math::vec2f(0, 20));

    float v_speed = m_uav.enu_velocity.z;
    switch (m_input.vertical.mode.value)
    {
    case silk::stream::IMulti_Input::Vertical::Mode::THRUST_RATE:
        str = q::util::format2<std::string>("V:Rate:{} | ALT:{} | SPD:{}", m_input.vertical.thrust_rate.value, m_uav.enu_position.z, v_speed);
        break;
    case silk::stream::IMulti_Input::Vertical::Mode::THRUST_OFFSET:
        str = q::util::format2<std::string>("V:Offset:{} | ALT:{} | SPD:{}", m_input.vertical.thrust_offset.value, m_uav.enu_position.z, v_speed);
        break;
    case silk::stream::IMulti_Input::Vertical::Mode::SPEED:
        str = q::util::format2<std::string>("V:Speed:{} | ALT:{} | SPD:{}", m_input.vertical.speed.value, m_uav.enu_position.z, v_speed);
        break;
    }
    m_context.texter.draw_string(m_context.painter, str, math::vec2f(0, 50));

    float h_speed = math::length(math::vec2f(m_uav.enu_velocity.x, m_uav.enu_velocity.y));
    switch (m_input.horizontal.mode.value)
    {
    case silk::stream::IMulti_Input::Horizontal::Mode::ANGLE_RATE:
        str = q::util::format2<std::string>("H:Rate:{} | SPD:{}", m_input.horizontal.angle_rate.value, h_speed);
        break;
    case silk::stream::IMulti_Input::Horizontal::Mode::ANGLE:
        str = q::util::format2<std::string>("H:Angle:{} | SPD:{}", math::degrees(m_input.horizontal.angle.value), h_speed);
        break;
    case silk::stream::IMulti_Input::Horizontal::Mode::VELOCITY:
        str = q::util::format2<std::string>("H:Velocity:{} | SPD:{}", m_input.horizontal.velocity.value, h_speed);
        break;
    }
    m_context.texter.draw_string(m_context.painter, str, math::vec2f(0, 80));

    switch (m_input.yaw.mode.value)
    {
    case silk::stream::IMulti_Input::Yaw::Mode::ANGLE_RATE:
        str = q::util::format2<std::string>("Y:Rate:{}", m_input.yaw.angle_rate.value);
        break;
    case silk::stream::IMulti_Input::Yaw::Mode::ANGLE:
        str = q::util::format2<std::string>("Y:Angle:{}", math::degrees(m_input.yaw.angle_rate.value));
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

