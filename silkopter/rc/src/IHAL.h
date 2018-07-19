#pragma once

#include "Comms.h"
#include "def_lang/Result.h"

namespace silk
{
namespace settings
{
class Settings;
}


class IHAL
{
public:
    virtual ~IHAL() = default;

    virtual ts::Result<void> init() = 0;
    virtual void shutdown() = 0;

    virtual Comms& get_comms() = 0;

    virtual math::vec2u32 get_display_size() const = 0;
    virtual void set_backlight(float brightness) = 0; //0..1
    virtual float get_temperature() const = 0;
    virtual void set_fan_speed(float speed) = 0; //0..1

    virtual settings::Settings& get_settings() = 0;
    virtual void save_settings() = 0;

    virtual bool process() = 0;
};


}
