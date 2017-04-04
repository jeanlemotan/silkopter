#pragma once

#include <vector>
#include <string>
#include <boost/optional.hpp>

class Adafruit_GFX;

namespace silk
{

class Input;

class Menu
{
public:
    Menu();

    void push_submenu(std::vector<std::string> const& entries, size_t selected, int16_t y);
    void pop_submenu();

    void set_submenu_entry(size_t idx, std::string const& entry);

    boost::optional<size_t> process(Input& input);

    void render(Adafruit_GFX& display, size_t max_entries);

private:
    struct Submenu
    {
        std::vector<std::string> entries;
        size_t crt_entry = 0;
        size_t top_entry = 0;
        int16_t x = 0;
        int16_t y = 0;
        bool popped = false;
        bool finished = false;
    };

    std::vector<Submenu> m_submenus;
    size_t m_crt_submenu_idx = 0;

    float m_target_x = 0;
    float m_crt_x = 0;

    void render(Adafruit_GFX& display, Submenu& data, size_t max_entries);
};

}
