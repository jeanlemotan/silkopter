#pragma once

#include "Render_Widget.h"
#include "HAL.h"
#include "Comms.h"
#include "IHUD_Widget.h"
#include "Video_Decoder.h"

class Multi_HUD_Widget : public IHUD_Widget
{
public:
    Multi_HUD_Widget(silk::HAL& hal, silk::Comms& comms, qinput::Input_Mgr& input_mgr, Render_Context& context, QWidget* parent);

    virtual void process() override;

private:
    silk::HAL& m_hal;
    silk::Comms& m_comms;
    qinput::Input_Mgr& m_input_mgr;
    Render_Context& m_context;
    q::scene::Camera m_camera;

    qinput::Gamepad_cptr m_gamepad;

    silk::stream::IMulti_Input::Value m_input;
    silk::stream::IMulti_State::Value m_prev_state;
    silk::stream::IMulti_State::Value m_state;

    struct UAV
    {
        math::trans3dd enu_to_ecef_transform; //relative to home
        math::trans3dd ecef_to_enu_transform; //relative to home
        math::vec3f enu_position; //this is in the home enu space
        math::vec3f enu_velocity; //this is in the home enu space
        math::quatf local_to_enu_quat; //the uav rotation basically.
    } m_uav;


    q::Clock::time_point m_arm_start_tp = q::Clock::now();

    Render_Widget* m_render_widget = nullptr;

    struct Video
    {
        Video_Decoder decoder;
        math::vec2u32 size;
        q::video::Texture_ptr texture;
        std::vector<uint8_t> data;
    } m_video;

    void decode_video(silk::stream::gs::Video::Value const& frame);
    void upload_video_frame();


    void process_vertical_thrust_rate();
    void process_vertical_thrust_offset();
    void process_vertical_climb_rate();
    void process_vertical();

    void process_horizontal_angle_rate();
    void process_horizontal_angle();
    void process_horizontal_velocity();
    void process_horizontal();

    void process_yaw_angle_rate();
    void process_yaw_angle();
    void process_yaw();

    void process_mode_idle();
    void process_mode_armed();
    void process_mode();

    void sync_input();

    void acquire_gamepad();
    void process_gamepad();

    //////////////////////////

    q::text::Style m_border_text_style;

    void render();
    void render_ground();
    void render_hud();
    void render_modes();
    void render_horizon();
    void render_altitude();
    void render_video();
};

