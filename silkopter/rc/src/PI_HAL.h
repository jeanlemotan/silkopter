#pragma once

#include "IHAL.h"

namespace silk
{

class PI_HAL : virtual public IHAL
{
public:
    PI_HAL();
    ~PI_HAL();

    ts::Result<void> init() override;
    void shutdown() override;

    Comms& get_comms() override;
    math::vec2u32 get_display_size() const override;
    void set_backlight(float brightness) override; //0..1
    float get_temperature() const override;
    void set_fan_speed(float speed) override; //0..1

    settings::Settings& get_settings() override;
    void save_settings() override;

    bool process() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    bool init_pigpio();
    void shutdown_pigpio();
    bool init_display();
    void shutdown_display();
    bool update_display();
    bool init_ts();
    void shutdown_ts();
    void update_ts();
    void update_fan();
    void update_temperature();

    void generate_settings_file();
    ts::Result<void> load_settings();
};

}
