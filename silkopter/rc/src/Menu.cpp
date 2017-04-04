#include "Menu.h"
#include "Input.h"
#include "IRotary_Encoder.h"
#include "IButton.h"
#include "Adafruit_GFX.h"

namespace silk
{

constexpr size_t MENU_WIDTH = 200;

///////////////////////////////////////////////////////////////////////////////////////////////////

Menu::Menu()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::push_submenu(std::vector<std::string> const& entries, size_t selected, int16_t y)
{
    if (entries.empty())
    {
        QASSERT(false);
        return;
    }

    //max 1 active transition
    while (m_submenus.size() > 1)
    {
        m_submenus.erase(m_submenus.begin());
    }

    Submenu submenu;
    submenu.entries = entries;
    submenu.crt_entry = std::min(selected, entries.size() - 1);
    submenu.top_entry = 0;
    submenu.x = MENU_WIDTH * m_submenus.size();
    submenu.y = y;

    m_submenus.push_back(submenu);
    m_crt_submenu_idx = m_submenus.size() - 1;

    m_target_x = static_cast<float>(m_crt_submenu_idx * MENU_WIDTH);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::pop_submenu()
{
    if (m_submenus.empty())
    {
        QASSERT(false);
        return;
    }

    Submenu& old_submenu = m_submenus.back();
    old_submenu.popped = true;

    m_crt_submenu_idx = m_submenus.size() - 2;

    if (m_crt_submenu_idx < m_submenus.size())
    {
        m_target_x = static_cast<float>(m_crt_submenu_idx * MENU_WIDTH);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::set_submenu_entry(size_t idx, std::string const& entry)
{
    if (m_submenus.empty() || m_crt_submenu_idx >= m_submenus.size())
    {
        return;
    }

    Submenu& crt_submenu = m_submenus[m_crt_submenu_idx];

    if (idx >= crt_submenu.entries.size())
    {
        return;
    }

    crt_submenu.entries[idx] = entry;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

boost::optional<size_t> Menu::process(Input& input)
{
    if (m_submenus.empty() || m_crt_submenu_idx >= m_submenus.size())
    {
        return boost::none;
    }

    Submenu& crt_submenu = m_submenus[m_crt_submenu_idx];

    int32_t delta = input.get_menu_encoder().get_delta();

    int32_t entry = static_cast<int>(crt_submenu.crt_entry) + delta;
    if (entry < 0)
    {
        entry = crt_submenu.entries.size() - 1;
    }
    crt_submenu.crt_entry = static_cast<size_t>(entry);

    if (crt_submenu.crt_entry >= crt_submenu.entries.size())
    {
        crt_submenu.crt_entry = 0;
    }

    if (input.get_menu_switch().was_released())
    {
        return crt_submenu.crt_entry;
    }

    return boost::none;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::render(Adafruit_GFX& display, size_t max_entries)
{
    if (m_submenus.empty())
    {
        return;
    }

    m_crt_x = math::lerp(m_crt_x, m_target_x, 0.1f);

    for (Submenu& submenu: m_submenus)
    {
        int16_t x = submenu.x - static_cast<int16_t>(m_crt_x + 0.5f);
        int16_t y = submenu.y;

        if (x < display.width() && x > -display.width() &&
            y < display.height() && x > -display.height())
        {
            render(display, submenu, max_entries);
        }
        else if (submenu.popped)
        {
            submenu.finished = true;
        }
    }

    auto it = std::remove_if(m_submenus.begin(), m_submenus.end(), [](Submenu const& submenu) { return submenu.finished; });
    m_submenus.erase(it, m_submenus.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::render(Adafruit_GFX& display, Submenu& submenu, size_t max_entries)
{
    int16_t parent_x = static_cast<int16_t>(m_crt_x + 0.5f);
    int16_t x = submenu.x - parent_x;
    int16_t y = submenu.y;

    const int16_t border_h = 2;
    const int16_t entry_h = 10;

    if (max_entries == 0)
    {
        max_entries = std::min<size_t>((display.height() - y - border_h * 2) / entry_h, submenu.entries.size());
    }

    //const int16_t total_entry_h = entry_h * max_entries;
    const int16_t total_h = entry_h * max_entries + border_h * 2; //+borders

    if (submenu.crt_entry < submenu.top_entry)
    {
        submenu.top_entry = submenu.crt_entry;
    }

    if (submenu.crt_entry - submenu.top_entry >= max_entries)
    {
        submenu.top_entry = submenu.crt_entry - max_entries + 1;
    }

    display.setTextWrap(false);

    for (int16_t xx = 0; xx < display.width(); xx += 3)
    {
        display.drawPixelWhite(x + xx, y);
        display.drawPixelWhite(x + xx, y + max_entries*entry_h + border_h*2);
    }

    //show scroll bar
    if (max_entries < submenu.entries.size())
    {
        int16_t scroll_x = x + display.width() - 1;
        int16_t scroll_y = y + submenu.top_entry * total_h / submenu.entries.size();
        int16_t scroll_h = max_entries * total_h / submenu.entries.size();
        display.drawLine(scroll_x, scroll_y, scroll_x, scroll_y + scroll_h, 1);
    }

    y += border_h;

    for (size_t i = 0; i < max_entries; i++)
    {
        size_t entry_idx = submenu.top_entry + i;

        if (submenu.crt_entry == entry_idx)
        {
            int16_t rx = x + 2;
            int16_t rw = display.width() - rx - 2;
            display.fillRect(rx, y, rw, entry_h, 1);
            display.setTextColor(0, 0);
        }
        else
        {
            display.setTextColor(1, 1);
        }
        display.setCursor(x + 4, y + (entry_h - 8) / 2);
        display.print(submenu.entries[entry_idx].c_str());
        y += entry_h;
    }

    display.setTextColor(1, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////


}
