#include "Battery_Calibration_Menu_Page.h"
#include "Adafruit_GFX.h"
#include "Input.h"
#include "IBattery_Info.h"
#include "IRotary_Encoder.h"
#include "IButton.h"

#include "Menu_System.h"
#include "HAL.h"

#include "settings.def.h"

namespace silk
{

constexpr size_t LOW_VOLTAGE_ENTRY = 2;
constexpr size_t CRITICAL_VOLTAGE_ENTRY = 3;
constexpr size_t BIAS_ENTRY = 4;
constexpr size_t SCALE_ENTRY = 5;

///////////////////////////////////////////////////////////////////////////////////////////////////

Battery_Calibration_Menu_Page::Battery_Calibration_Menu_Page(HAL& hal)
    : m_hal(hal)
{
    m_menu.push_submenu({
                         "<-",
                         "Save",
                         "Low Voltage",
                         "Critical Voltage",
                         "Bias",
                         "Scale",
                        }, 0, 12);

    refresh_menu();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Battery_Calibration_Menu_Page::refresh_menu()
{
    IBattery_Info const& bi = m_hal.get_battery_info();

    size_t se = m_selected_entry == boost::none ? size_t(-1) : *m_selected_entry;

    char buffer[128];

    sprintf(buffer, "Low: %s%.1fV", se == LOW_VOLTAGE_ENTRY ? ">" : " ", bi.get_low_voltage());
    m_menu.set_submenu_entry(LOW_VOLTAGE_ENTRY, buffer);

    sprintf(buffer, "Critical: %s%.1fV", se == CRITICAL_VOLTAGE_ENTRY ? ">" : " ", bi.get_critical_voltage());
    m_menu.set_submenu_entry(CRITICAL_VOLTAGE_ENTRY, buffer);

    sprintf(buffer, "Bias: %s%.2f", se == BIAS_ENTRY ? ">" : " ", bi.get_calibration_data().bias);
    m_menu.set_submenu_entry(BIAS_ENTRY, buffer);

    sprintf(buffer, "Scale: %s%.2f", se == SCALE_ENTRY ? ">" : " ", bi.get_calibration_data().scale);
    m_menu.set_submenu_entry(SCALE_ENTRY, buffer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Battery_Calibration_Menu_Page::process(Input& input, Menu_System& menu_system)
{
    if (m_selected_entry == boost::none)
    {
        m_selected_entry = m_menu.process(input);
        if (m_selected_entry != boost::none)
        {
            if (*m_selected_entry == 0)
            {
                return false;
            }
            else if (*m_selected_entry == 1)
            {
                m_hal.save_settings();
                m_selected_entry = boost::none;
            }
        }
    }
    else
    {
        if (input.get_menu_switch().was_released())
        {
            m_selected_entry = boost::none;
            refresh_menu();
        }
        else
        {
            IBattery_Info& bi = m_hal.get_battery_info();

            if (*m_selected_entry == LOW_VOLTAGE_ENTRY)
            {
                float v = bi.get_low_voltage();
                bi.set_low_voltage(std::max(v + input.get_menu_encoder().get_delta() / 10.f, 1.f));
                refresh_menu();
            }
            else if (*m_selected_entry == CRITICAL_VOLTAGE_ENTRY)
            {
                float v = bi.get_critical_voltage();
                bi.set_critical_voltage(std::max(v + input.get_menu_encoder().get_delta() / 10.f, 1.f));
                refresh_menu();
            }
            else if (*m_selected_entry == BIAS_ENTRY)
            {
                IBattery_Info::Calibration_Data cb = bi.get_calibration_data();
                cb.bias = cb.bias + input.get_menu_encoder().get_delta() / 50.f;
                bi.set_calibration_data(cb);
                refresh_menu();
            }
            else if (*m_selected_entry == SCALE_ENTRY)
            {
                IBattery_Info::Calibration_Data cb = bi.get_calibration_data();
                cb.scale = std::max(cb.scale + input.get_menu_encoder().get_delta() / 50.f, 0.f);
                bi.set_calibration_data(cb);
                refresh_menu();
            }
        }
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Battery_Calibration_Menu_Page::render(Adafruit_GFX& display)
{
    IBattery_Info& bi = m_hal.get_battery_info();

    display.setCursor(0, 0);
    display.printf("Battery: %.1fV", bi.get_voltage());

    m_menu.render(display, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

}
