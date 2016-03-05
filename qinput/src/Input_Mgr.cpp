#include "QInputStdAfx.h"
#include "QInput.h"

#include <linux/joystick.h>
#include <unistd.h>
#include <fcntl.h>

using namespace qinput;


Input_Mgr::Input_Mgr(q::String const& /*window_handle*/)
{
    enumerate_gamepads();

    {
        auto& buttons = m_ouya_mapping.buttons;
        //auto& sticks = m_ouya_mapping.sticks;
        //auto& axes = m_ouya_mapping.axes;
        buttons[0] = Gamepad::Button::OUYA_O;
        buttons[1] = Gamepad::Button::OUYA_U;
        buttons[2] = Gamepad::Button::OUYA_Y;
        buttons[3] = Gamepad::Button::OUYA_A;

        buttons[8] = Gamepad::Button::LPAD_UP;
        buttons[9] = Gamepad::Button::LPAD_DOWN;
        buttons[10] = Gamepad::Button::LPAD_LEFT;
        buttons[11] = Gamepad::Button::LPAD_RIGHT;

        buttons[6] = Gamepad::Button::LEFT_STICK;
        buttons[7] = Gamepad::Button::RIGHT_STICK;

        buttons[4] = Gamepad::Button::LEFT_BUMPER;
        buttons[5] = Gamepad::Button::RIGHT_BUMPER;

        buttons[12] = Gamepad::Button::LEFT_TRIGGER;
        buttons[13] = Gamepad::Button::RIGHT_TRIGGER;
    }
    {
        auto& buttons = m_ps3_mapping.buttons;
        //auto& sticks = m_ps3_mapping.sticks;
        //auto& axes = m_ps3_mapping.axes;
        buttons[12] = Gamepad::Button::PS_TRIANGLE;
        buttons[13] = Gamepad::Button::PS_CIRCLE;
        buttons[14] = Gamepad::Button::PS_X;
        buttons[15] = Gamepad::Button::PS_SQUARE;

        buttons[4] = Gamepad::Button::LPAD_UP;
        buttons[5] = Gamepad::Button::LPAD_RIGHT;
        buttons[6] = Gamepad::Button::LPAD_DOWN;
        buttons[7] = Gamepad::Button::LPAD_LEFT;

        buttons[1] = Gamepad::Button::LEFT_STICK;
        buttons[2] = Gamepad::Button::RIGHT_STICK;

        buttons[10] = Gamepad::Button::LEFT_BUMPER;
        buttons[11] = Gamepad::Button::RIGHT_BUMPER;

        buttons[8] = Gamepad::Button::LEFT_TRIGGER;
        buttons[9] = Gamepad::Button::RIGHT_TRIGGER;
    }
    {
        auto& buttons = m_ps4_mapping.buttons;
        buttons[0] = Gamepad::Button::PS_SQUARE;
        buttons[1] = Gamepad::Button::PS_X;
        buttons[2] = Gamepad::Button::PS_CIRCLE;
        buttons[3] = Gamepad::Button::PS_TRIANGLE;

//        buttons[4] = Gamepad::Button::LPAD_UP;
//        buttons[5] = Gamepad::Button::LPAD_RIGHT;
//        buttons[6] = Gamepad::Button::LPAD_DOWN;
//        buttons[7] = Gamepad::Button::LPAD_LEFT;

        buttons[4] = Gamepad::Button::LEFT_BUMPER;
        buttons[5] = Gamepad::Button::RIGHT_BUMPER;

        buttons[6] = Gamepad::Button::LEFT_TRIGGER;
        buttons[7] = Gamepad::Button::RIGHT_TRIGGER;

        buttons[10] = Gamepad::Button::LEFT_STICK;
        buttons[11] = Gamepad::Button::RIGHT_STICK;

        buttons[12] = Gamepad::Button::HOME;
    }
}

Input_Mgr::~Input_Mgr()
{
    {
        std::lock_guard<std::mutex> lock(m_gamepad_mutex);
        for (auto& g: m_gamepads)
        {
            close(g.fd);
        }
    }
}

Touchscreen const& Input_Mgr::get_touchscreen() const
{
	return m_touchscreen;
}
Keyboard const& Input_Mgr::get_keyboard() const
{
	return m_keyboard;
}
Sensors const& Input_Mgr::get_sensors() const
{
	return m_sensors;
}

std::vector<Gamepad_cptr> Input_Mgr::get_all_gamepads() const
{
    enumerate_gamepads();

	std::vector<Gamepad_cptr> gamepads;
    {
        std::lock_guard<std::mutex> lock(m_gamepad_mutex);
        gamepads.reserve(m_gamepads.size());
        for (auto const& g: m_gamepads)
        {
            gamepads.emplace_back(g.gamepad);
        }
    }
    return gamepads;
}

void Input_Mgr::remove_disconnected_gamepads()
{
    std::lock_guard<std::mutex> lock(m_gamepad_mutex);

//	m_gamepads.erase(std::remove_if(m_gamepads.begin(), m_gamepads.end(), [](Gamepad_ptr const& g)
//	{
//		return !g->is_connected();
//	}), m_gamepads.end());
}

void Input_Mgr::process_ouya_gamepad_event(Gamepad_Data const& data, js_event const& ev)
{
    uint8_t type = ev.type & ~JS_EVENT_INIT;
    if (type == JS_EVENT_BUTTON)
    {
        auto it = m_ouya_mapping.buttons.find(ev.number);
        if (it == m_ouya_mapping.buttons.end())
        {
            QLOGW("unhandled ouya button {}", ev.number);
            return;
        }
        if (ev.value)
        {
            data.gamepad->set_button_pressed(it->second);
        }
        else
        {
            data.gamepad->set_button_released(it->second);
        }
    }
    else if (type == JS_EVENT_AXIS)
    {
        switch (ev.number)
        {
            case 0:
            {
                auto stick = data.gamepad->get_stick_data(Gamepad::Stick::LEFT);
                stick.value.x = ev.value / 32767.f;
                data.gamepad->set_stick_data(Gamepad::Stick::LEFT, stick);
            }
            break;
            case 1:
            {
                auto stick = data.gamepad->get_stick_data(Gamepad::Stick::LEFT);
                stick.value.y = -ev.value / 32767.f;
                data.gamepad->set_stick_data(Gamepad::Stick::LEFT, stick);
            }
            break;
            case 3:
            {
                auto stick = data.gamepad->get_stick_data(Gamepad::Stick::RIGHT);
                stick.value.x = ev.value / 32767.f;
                data.gamepad->set_stick_data(Gamepad::Stick::RIGHT, stick);
            }
            break;
            case 4:
            {
                auto stick = data.gamepad->get_stick_data(Gamepad::Stick::RIGHT);
                stick.value.y = -ev.value / 32767.f;
                data.gamepad->set_stick_data(Gamepad::Stick::RIGHT, stick);
            }
            break;
            case 2:
            {
                Gamepad::Axis_Data v;
                v.value = ev.value / 32767.f;
                data.gamepad->set_axis_data(Gamepad::Axis::LEFT_TRIGGER, v);
            }
            break;
            case 5:
            {
                Gamepad::Axis_Data v;
                v.value = ev.value / 32767.f;
                data.gamepad->set_axis_data(Gamepad::Axis::RIGHT_TRIGGER, v);
            }
            break;
            default:
            {
                QLOGW("unknown ouya axis {}", ev.number);
            }
            break;
        }
    }
}

void Input_Mgr::process_ps3_gamepad_event(Gamepad_Data const& data, js_event const& ev)
{
    uint8_t type = ev.type & ~JS_EVENT_INIT;
    if (type == JS_EVENT_BUTTON)
    {
        auto it = m_ps3_mapping.buttons.find(ev.number);
        if (it == m_ps3_mapping.buttons.end())
        {
            QLOGW("unhandled ps3 button {}", ev.number);
            return;
        }
        if (ev.value)
        {
            data.gamepad->set_button_pressed(it->second);
        }
        else
        {
            data.gamepad->set_button_released(it->second);
        }
    }
    else if (type == JS_EVENT_AXIS)
    {
        if (ev.number == 0)
        {
            auto stick = data.gamepad->get_stick_data(Gamepad::Stick::LEFT);
            stick.value.x = ev.value / 32767.f;
            data.gamepad->set_stick_data(Gamepad::Stick::LEFT, stick);
        }
        else if (ev.number == 1)
        {
            auto stick = data.gamepad->get_stick_data(Gamepad::Stick::LEFT);
            stick.value.y = -ev.value / 32767.f;
            data.gamepad->set_stick_data(Gamepad::Stick::LEFT, stick);
        }
        else if (ev.number == 2)
        {
            auto stick = data.gamepad->get_stick_data(Gamepad::Stick::RIGHT);
            stick.value.x = ev.value / 32767.f;
            data.gamepad->set_stick_data(Gamepad::Stick::RIGHT, stick);
        }
        else if (ev.number == 3)
        {
            auto stick = data.gamepad->get_stick_data(Gamepad::Stick::RIGHT);
            stick.value.y = -ev.value / 32767.f;
            data.gamepad->set_stick_data(Gamepad::Stick::RIGHT, stick);
        }
        else if (ev.number == 12)
        {
            Gamepad::Axis_Data v;
            v.value = ev.value / 65535.f + 0.5f;
            data.gamepad->set_axis_data(Gamepad::Axis::LEFT_TRIGGER, v);
        }
        else if (ev.number == 13)
        {
            Gamepad::Axis_Data v;
            v.value = ev.value / 65535.f + 0.5f;
            data.gamepad->set_axis_data(Gamepad::Axis::RIGHT_TRIGGER, v);
        }
        else
        {
            //QLOGW("unknown ps3 axis {}", ev.number);
        }
    }
}

void Input_Mgr::process_ps4_gamepad_event(Gamepad_Data const& data, js_event const& ev)
{
    uint8_t type = ev.type & ~JS_EVENT_INIT;
    if (type == JS_EVENT_BUTTON)
    {
        auto it = m_ps4_mapping.buttons.find(ev.number);
        if (it == m_ps4_mapping.buttons.end())
        {
            QLOGW("unhandled ps4 button {}", ev.number);
            return;
        }
        if (ev.value)
        {
            data.gamepad->set_button_pressed(it->second);
        }
        else
        {
            data.gamepad->set_button_released(it->second);
        }
    }
    else if (type == JS_EVENT_AXIS)
    {
        switch (ev.number)
        {
            case 0:
            {
                auto stick = data.gamepad->get_stick_data(Gamepad::Stick::LEFT);
                stick.value.x = ev.value / 32767.f;
                data.gamepad->set_stick_data(Gamepad::Stick::LEFT, stick);
            }
            break;
            case 1:
            {
                auto stick = data.gamepad->get_stick_data(Gamepad::Stick::LEFT);
                stick.value.y = -ev.value / 32767.f;
                data.gamepad->set_stick_data(Gamepad::Stick::LEFT, stick);
            }
            break;
            case 2:
            {
                auto stick = data.gamepad->get_stick_data(Gamepad::Stick::RIGHT);
                stick.value.x = ev.value / 32767.f;
                data.gamepad->set_stick_data(Gamepad::Stick::RIGHT, stick);
            }
            break;
            case 5:
            {
                auto stick = data.gamepad->get_stick_data(Gamepad::Stick::RIGHT);
                stick.value.y = -ev.value / 32767.f;
                data.gamepad->set_stick_data(Gamepad::Stick::RIGHT, stick);
            }
            break;
            case 3:
            {
                Gamepad::Axis_Data v;
                v.value = ev.value / 65535.f + 0.5f;
                data.gamepad->set_axis_data(Gamepad::Axis::LEFT_TRIGGER, v);
            }
            break;
            case 4:
            {
                Gamepad::Axis_Data v;
                v.value = ev.value / 65535.f + 0.5f;
                data.gamepad->set_axis_data(Gamepad::Axis::RIGHT_TRIGGER, v);
            }
            break;
            case 6:
            {
                if (ev.value < -15000)
                {
                    data.gamepad->set_button_pressed(Gamepad::Button::LPAD_LEFT);
                }
                else if (ev.value > 15000)
                {
                    data.gamepad->set_button_pressed(Gamepad::Button::LPAD_RIGHT);
                }
                else
                {
                    data.gamepad->set_button_released(Gamepad::Button::LPAD_LEFT);
                    data.gamepad->set_button_released(Gamepad::Button::LPAD_RIGHT);
                }
            }
            break;
            case 7:
            {
                if (ev.value < -15000)
                {
                    data.gamepad->set_button_pressed(Gamepad::Button::LPAD_UP);
                }
                else if (ev.value > 15000)
                {
                    data.gamepad->set_button_pressed(Gamepad::Button::LPAD_DOWN);
                }
                else
                {
                    data.gamepad->set_button_released(Gamepad::Button::LPAD_UP);
                    data.gamepad->set_button_released(Gamepad::Button::LPAD_DOWN);
                }
            }
            break;
            default:
            {
                //QLOGW("unknown ps4 axis {} / {}", ev.number, ev.value / 32767.f);
            }
            break;
        }
    }
}

void Input_Mgr::update_gamepads(q::Clock::duration dt)
{
    std::lock_guard<std::mutex> lock(m_gamepad_mutex);

    for (auto const& g: m_gamepads)
	{
        g.gamepad->update(dt);

        struct js_event ev;
        while (read (g.fd, &ev, sizeof(ev)) > 0)
        {
            switch (g.gamepad->get_type())
            {
            case Gamepad::Type::OUYA: process_ouya_gamepad_event(g, ev); break;
            case Gamepad::Type::PS3: process_ps3_gamepad_event(g, ev); break;
            case Gamepad::Type::PS4: process_ps4_gamepad_event(g, ev); break;
            default: break;
            }
        }
	}
}

void Input_Mgr::process(q::Clock::duration dt)
{
	m_touchscreen.update(dt);
	m_keyboard.update(dt);
	m_sensors.update(dt);

	remove_disconnected_gamepads();
	update_gamepads(dt);
//	connect_new_gamepads(dt);
}

//////////////////////////////////////////////////////////////////////////
// Methods called from the platform code

void Input_Mgr::add_event(Touchscreen_Event const& event)
{
	m_touchscreen.add_event(event);
}
void Input_Mgr::add_event(Keyboard_Event const& event)
{
    m_keyboard.add_event(event);
}
void Input_Mgr::add_event(Sensor_Event const& event)
{
	m_sensors.add_event(event);
}

//-----------------------------------------------------------------------------//

struct Gamepad_Info
{
    Gamepad::Type type;
    q::String name;
    size_t button_count = 0;
    size_t axes_count = 0;
};

static boost::optional<Gamepad_Info> get_gamepad_info(int device_id)
{
    Gamepad_Info info;

    char name[1024];
    if (ioctl(device_id, JSIOCGNAME(sizeof(name)), name) < 0)
    {
        QLOGW("Cannot read device name: {}", strerror(errno));
        return boost::none;
    }

    info.name = name;

    if (info.name.find("ouya") != q::String::npos)
    {
        info.type = Gamepad::Type::OUYA;
    }
    else if (info.name.find("sony playstation(r)3") != q::String::npos)
    {
        info.type = Gamepad::Type::PS3;
    }
    else if (info.name.find("Wireless Controller") != q::String::npos)
    {
        info.type = Gamepad::Type::PS4;
    }
    else
    {
        return boost::none;
    }

    char count = 0;
    if (ioctl(device_id, JSIOCGAXES, &count) == -1)
    {
        QLOGW("Cannot read device info: {}", strerror(errno));
        return boost::none;
    }
    info.axes_count = count;

    if (ioctl(device_id, JSIOCGBUTTONS, &count) == -1)
    {
        QLOGW("Cannot read device info: {}", strerror(errno));
        return boost::none;
    }
    info.button_count = count;

    return info;
}


void Input_Mgr::enumerate_gamepads() const
{
    std::lock_guard<std::mutex> lock(m_gamepad_mutex);

    for (size_t i = 0; i < 64; i++)
    {
        auto it = std::find_if(m_gamepads.begin(), m_gamepads.end(), [i](Gamepad_Data const& g) { return g.id == i; });
        if (it != m_gamepads.end())
        {
            return;
        }

        auto name = q::util::format<std::string>("/dev/input/js{}", i);
        int fd = open(name.c_str(), O_RDONLY | O_NONBLOCK);
        if (fd == -1)
        {
            continue;
        }

        auto info = get_gamepad_info(fd);
        if (!info)
        {
            close(fd);
            continue;
        }

        QLOGI("Found {} gamepad/joystick with {} axes and {} buttons", info->name, info->axes_count, info->button_count);

        Gamepad_Data g;
        g.gamepad = std::make_shared<Gamepad>(info->name, info->type);
        g.id = i;
        g.fd = fd;
        m_gamepads.push_back(g);
    }
}
