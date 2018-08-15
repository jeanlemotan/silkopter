#include "HUD.h"
#include "IHAL.h"
#include "imgui.h"

namespace silk
{
///////////////////////////////////////////////////////////////////////////////////////////////////

static const math::vec2f k_fov(math::radians(math::vec2f(62.2f, 48.8f)));

static void AddShadowText(ImDrawList& drawList, const ImVec2& pos, ImU32 col, const char* text)
{
    drawList.AddText(ImVec2(pos.x + 1.f, pos.y + 1.f), ImColor(0.f, 0.f, 0.f, ImColor(col).Value.w), text);
    drawList.AddText(pos, col, text);
}

HUD::HUD(silk::IHAL& hal)
    : m_hal(hal)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////

void HUD::draw_attitude(const stream::IMultirotor_State::Value& state)
{
    math::vec3f euler;
    state.local_frame.get_as_euler_zxy(euler);
    float pitch = euler.x;
    float roll = euler.y;
    float yaw = euler.z;

    math::vec2f display_size(m_hal.get_display_size());
    float cx = display_size.x / 2.f;
    float cy = display_size.y / 2.f;

    ImDrawList& drawList = *ImGui::GetWindowDrawList();

    {
        float h = display_size.y;
        float alpha_h = h / 1.5f;
        float roll_sin, roll_cos;
        math::sin_cos(roll, roll_sin, roll_cos);

        {
            float y = cy + pitch / k_fov.y * h;
            float alpha = math::clamp(1.f - math::abs(y - cy) / (alpha_h / 2.f), 0.f, 1.f) * 0.9f;
            float size = display_size.x * 0.25f;
            drawList.AddLine(ImVec2(cx - roll_cos * size, y - roll_sin * size),
                             ImVec2(cx + roll_cos * size, y + roll_sin * size),
                             ImColor(0.f, 0.f, 0.f, alpha), 5.f);
            drawList.AddLine(ImVec2(cx - roll_cos * size, y - roll_sin * size),
                             ImVec2(cx + roll_cos * size, y + roll_sin * size),
                             ImColor(0.f, 1.f, 0.f, alpha), 3.f);
        }
        for (int i = -18; i < 18; i++)
        {
            if (i == 0)
            {
                continue;
            }
            float y = cy + (pitch + math::radians((float)i * 10.f)) / k_fov.y * h;
            if (y < cy - h / 2.f || y > cy + h / 2.f)
            {
                continue;
            }
            float alpha = math::clamp(1.f - math::abs(y - cy) / (alpha_h / 2.f), 0.f, 1.f) * 0.9f;
            alpha = math::sqrt(alpha); //sharper gradient
            float size = display_size.x * 0.10f;
            drawList.AddLine(ImVec2(cx - roll_cos * size, y - roll_sin * size),
                             ImVec2(cx + roll_cos * size, y + roll_sin * size),
                             ImColor(0.f, 0.f, 0.f, alpha), 5.f);
            drawList.AddLine(ImVec2(cx - roll_cos * size, y - roll_sin * size),
                             ImVec2(cx + roll_cos * size, y + roll_sin * size),
                             ImColor(0.f, 1.f, 0.f, alpha), 3.f);
        }
    }

    {
        float max_size = display_size.y * 0.05f;
        float y = display_size.y / 10.f;

        {
            //drawList.AddRectFilled(ImVec2(cx - 10.f, y - 2.f), ImVec2(cx + 10.f, y + max_size + 2.f), ImColor(0.f, 1.f, 0.f, 0.5f), 3.f);
            drawList.AddLine(ImVec2(cx, y - 2.f), ImVec2(cx, y + max_size + 2.f), ImColor(0.f, 1.f, 0.f, 0.5f), 3.f);

            char buffer[64];
            sprintf(buffer, "%d'", (int)math::degrees(yaw));
            ImVec2 textSize = ImGui::CalcTextSize(buffer);
            AddShadowText(drawList, ImVec2(cx - textSize.x / 2, y - textSize.y - 10.f), ImColor(0.f, 1.f, 0.f, 1.f), buffer);
        }

        const char* cardinal_texts[8] = {"SE" ,"E", "NE", "N", "NW", "W", "SW", "S"};
        for (int i = -36; i < 36; i++) //every 5 degrees
        {
            float w = display_size.x / 3.f;
            float x = cx - (yaw + math::radians((float)(i) * 5.f)) / k_fov.x * w;
            if (x < cx - w / 2.f || x > cx + w / 2.f)
            {
                continue;
            }
            float alpha = 0.9f;//math::clamp(1.f - (x - w / 2.f) / (w / 2.f), 0.f, 1.f) * 0.9f;
            float size = max_size / 2.f;
            if (i % 9 == 0)
            {
                size = max_size;

                const char* text = cardinal_texts[(i + 36) / 9];
                ImVec2 textSize = ImGui::CalcTextSize(text);
                AddShadowText(drawList, ImVec2(x - textSize.x / 2.f, y + size + 4), ImColor(0.f, 1.f, 0.f, 1.f), text);
            }
            drawList.AddLine(ImVec2(x, y),
                             ImVec2(x, y + size),
                             ImColor(0.f, 0.f, 0.f, alpha), 5.f);
            drawList.AddLine(ImVec2(x, y),
                             ImVec2(x, y + size),
                             ImColor(0.f, 1.f, 0.f, alpha), 3.f);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void HUD::draw_altitude(const stream::IMultirotor_State::Value& state)
{
    util::coordinates::LLA home_lla;
    math::trans3dd home_ecef_to_enu_transform;
    if (state.home_ecef_position.is_initialized())
    {
        home_lla = util::coordinates::ecef_to_lla(*state.home_ecef_position);
        home_ecef_to_enu_transform = util::coordinates::ecef_to_enu_transform(home_lla);
    }
    math::vec3f enu_position = math::vec3f(math::transform(home_ecef_to_enu_transform, state.ecef_position));

    ImDrawList& drawList = *ImGui::GetWindowDrawList();

    math::vec2f display_size(m_hal.get_display_size());
    float cx = display_size.x / 2.f;
    float cy = display_size.y / 2.f;

    {
        float max_size = display_size.x * 0.03f;
        float x = display_size.x - display_size.x / 8.f;
        float h = display_size.y / 2.f;

        const float text_height = ImGui::CalcTextSize("A").y;
        const float line_height = text_height + 6.f;
        float text_y = display_size.y - line_height * 4;
        //drawList.AddRectFilled(ImVec2(x + 4, text_y), ImVec2(, y + max_size + 2.f), ImColor(0.f, 1.f, 0.f, 0.5f), 3.f);

        {
            drawList.AddLine(ImVec2(x + 2.f, cy), ImVec2(x - max_size - 2.f, cy), ImColor(0.f, 1.f, 0.f, 0.5f), 3.f);

            char buffer[64];
            if (!state.home_ecef_position.is_initialized())
            {
                sprintf(buffer, "ALT: ---");
            }
            else
            {
                sprintf(buffer, "ALT: %.1f m", enu_position.z);
            }
            AddShadowText(drawList, ImVec2(x + 4, text_y), ImColor(0.f, 1.f, 0.f, 1.f), buffer);
            text_y += line_height;
        }

        {
            char buffer[64];
            sprintf(buffer, "VSPD: %.1f m/s", state.enu_velocity.z);
            AddShadowText(drawList, ImVec2(x + 4, text_y), ImColor(0.f, 1.f, 0.f, 1.f), buffer);
            text_y += line_height;
        }
        {
            char buffer[64];
            sprintf(buffer, "HSPD: %.1f m/s", math::length(math::vec2f(state.enu_velocity)));
            AddShadowText(drawList, ImVec2(x + 4, text_y), ImColor(0.f, 1.f, 0.f, 1.f), buffer);
            text_y += line_height;
        }
        {
            char buffer[64];
            sprintf(buffer, "DIST: %.1f m", math::length(math::vec2f(enu_position)));
            AddShadowText(drawList, ImVec2(x + 4, text_y), ImColor(0.f, 1.f, 0.f, 1.f), buffer);
            text_y += line_height;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void HUD::draw_modes(const stream::IMultirotor_State::Value& state)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////

void HUD::draw()
{
    math::vec2f display_size(m_hal.get_display_size());
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(display_size.x, display_size.y));
    ImGui::SetNextWindowBgAlpha(0);
    ImGui::Begin("HUD", nullptr, ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoInputs);

    stream::IMultirotor_State::Value state = m_hal.get_comms().get_multirotor_state();

    draw_attitude(state);
    draw_altitude(state);
    draw_modes(state);

    ImGui::End();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
}
