#include "QInputStdAfx.h"
#include "QInput.h"

using namespace qinput;


Gamepad::Gamepad(q::String const& name, Type type)
    : m_name(name)
    , m_type(type)
{
}

auto Gamepad::get_name() const -> q::String const&
{
    return m_name;
}
auto Gamepad::get_type() const -> Type
{
    return m_type;
}

bool Gamepad::is_connected() const
{
	return m_is_connected;
}

auto Gamepad::get_stick_data(Stick id) const -> Gamepad::Stick_Data
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if ((size_t)id < m_sticks.size())
	{
		return m_sticks[(size_t)id];
	}
	return Stick_Data();
}
auto Gamepad::get_axis_data(Axis id) const -> Gamepad::Axis_Data
{
	std::lock_guard<std::mutex> lock(m_mutex);
    if ((size_t)id < m_axes.size())
	{
        return m_axes[(size_t)id];
	}
    return Axis_Data();
}

bool Gamepad::is_button_inactive(Button button) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_buttons_pressed.find(button) == m_buttons_pressed.end() && m_buttons_released.find(button) == m_buttons_released.end();
}
bool Gamepad::is_button_pressed(Button button) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_buttons_pressed.find(button) != m_buttons_pressed.end();
}
bool Gamepad::is_button_released(Button button) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_buttons_released.find(button) != m_buttons_released.end();
}
auto Gamepad::get_all_pressed_buttons() const -> std::vector<Button>
{
	std::lock_guard<std::mutex> sm(m_mutex);
    std::vector<Button> res;
	res.reserve(16);
	for (auto k: m_buttons_pressed)
	{
		res.push_back(k);
	}
	return res;
}
auto Gamepad::get_all_released_buttons() const -> std::vector<Button>
{
	std::lock_guard<std::mutex> sm(m_mutex);
    std::vector<Button> res;
	res.reserve(16);
	for (auto k: m_buttons_released)
	{
		res.push_back(k);
	}
	return res;
}
auto Gamepad::get_sensors() const -> Sensors const&
{
	return m_sensors;
}

void Gamepad::update(q::Clock::duration/* dt*/)
{
	{
		std::lock_guard<std::mutex> sm(m_mutex);
		m_buttons_released.clear();
	}
}

void Gamepad::set_stick_data(Stick stick, Stick_Data const& data)
{
    m_sticks[static_cast<int>(stick)] = data;
}
void Gamepad::set_axis_data(Axis axis, Axis_Data const& data)
{
    m_axes[static_cast<int>(axis)] = data;
}
void Gamepad::set_button_pressed(Button button)
{
    m_buttons_pressed.insert(button);
}
void Gamepad::set_button_released(Button button)
{
    m_buttons_pressed.erase(button);
    m_buttons_released.insert(button);
}

void Gamepad::set_connected(bool yes)
{
    m_is_connected = yes;
}
