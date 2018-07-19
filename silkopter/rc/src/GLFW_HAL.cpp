#include "GLFW_HAL.h"

#if !defined RASPBERRY_PI

#include "def_lang/Serialization.h"
#include "def_lang/JSON_Serializer.h"

#include "settings.def.h"
#include <fstream>

#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

extern uint8_t s_font_droid_sans[];

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace silk
{

static const std::string k_settings_filename("settings.json");
extern std::string s_program_path;
static uint32_t s_width = 1920;
static uint32_t s_height = 1080;
static bool s_rotate = false;

struct GLFW_HAL::Impl
{
    Impl(GLFW_HAL& hal)
        : comms(hal)
        , settings()
    {}

    Comms comms;

    GLFWwindow* window = nullptr;

    settings::Settings settings;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

static void video_error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool GLFW_HAL::init_display()
{
    if (!glfwInit())
    {
        return false;
    }
    glfwSetErrorCallback(video_error_callback);

    {
        int dw = (int)s_width;
        int dh = (int)s_height;
        if (s_rotate)
        {
            std::swap(dw, dh);
        }
        m_impl->window = glfwCreateWindow(dw, dh, "RC", NULL, NULL);
        if (!m_impl->window)
        {
            return false;
        }
    }
    glfwMakeContextCurrent(m_impl->window);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = s_width;
    io.DisplaySize.y = s_height;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void GLFW_HAL::shutdown_display()
{
    ImGui::DestroyContext();
    if (m_impl->window)
    {
        glfwDestroyWindow(m_impl->window);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool GLFW_HAL::update_display()
{
    glfwPollEvents();

    static bool s_mouse_just_pressed[5] = { false, false, false, false, false };
    // Update buttons
    ImGuiIO& io = ImGui::GetIO();
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
    {
        // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        io.MouseDown[i] = s_mouse_just_pressed[i] || glfwGetMouseButton(m_impl->window, i) != 0;
        s_mouse_just_pressed[i] = false;
    }

    // Update mouse position
    const ImVec2 mouse_pos_backup = io.MousePos;
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    if (glfwGetWindowAttrib(m_impl->window, GLFW_FOCUSED))
    {
        if (io.WantSetMousePos)
        {
            glfwSetCursorPos(m_impl->window, (double)mouse_pos_backup.x, (double)mouse_pos_backup.y);
        }
        else
        {
            double mouse_x, mouse_y;
            glfwGetCursorPos(m_impl->window, &mouse_x, &mouse_y);
            if (s_rotate)
            {
                io.MousePos = ImVec2((float)mouse_y, (float)s_height - (float)mouse_x);
            }
            else
            {
                io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
            }
        }
    }

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData(), s_rotate);

    glfwMakeContextCurrent(m_impl->window);
    glfwSwapBuffers(m_impl->window);

    return !glfwWindowShouldClose(m_impl->window);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GLFW_HAL::generate_settings_file()
{
    get_settings() = settings::Settings();
    //get_settings().get_comms().set_video_wlan_interfaces({"wlan1", "wlan2"});
    save_settings();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ts::Result<void> GLFW_HAL::load_settings()
{
    std::string filename = s_program_path + "/" + k_settings_filename;
    QLOGI("Loading settings from '{}'", filename);

    std::string data;

    {
        //read the data
        std::ifstream fs(filename, std::ifstream::in | std::ifstream::binary);
        if (!fs.is_open())
        {
            QLOGW("Failed to load '{}'", filename);
            generate_settings_file();
            return ts::Error("Failed to load '" + filename + "'");
        }

        fs.seekg (0, fs.end);
        size_t size = fs.tellg();
        fs.seekg (0, fs.beg);

        data.resize(size + 1);
        fs.read(&data[0], size);
    }

    ts::Result<ts::sz::Value> json_result = ts::sz::from_json(data);
    if (json_result != ts::success)
    {
        return ts::Error("Failed to load '" + filename + "': " + json_result.error().what());
    }

    settings::Settings settings;
    auto reserialize_result = settings::deserialize(settings, json_result.payload());
    if (reserialize_result != ts::success)
    {
        return ts::Error("Failed to deserialize settings: " + reserialize_result.error().what());
    }

    get_settings() = settings;

    return ts::success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void GLFW_HAL::save_settings()
{
    std::string filename = s_program_path + "/" + k_settings_filename;
    QLOGI("Saving settings to '{}'", filename);

    ts::sz::Value sz_value = settings::serialize(m_impl->settings);

    std::string json = ts::sz::to_json(sz_value, true);

    std::ofstream fs(filename);
    if (fs.is_open())
    {
        fs.write(json.data(), json.size());
        fs.flush();
    }
    else
    {
        QLOGE("Cannot open '{}' to save settings.", filename);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

GLFW_HAL::GLFW_HAL()
{
    m_impl.reset(new Impl(*this));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

GLFW_HAL::~GLFW_HAL()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////

ts::Result<void> GLFW_HAL::init()
{
    ts::Result<void> result = load_settings();
    if (result != ts::success)
    {
        return result.error();
    }

    save_settings();

    if (!init_display())
    {
        return ts::Error("Cannot initialize EGL");
    }

    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromMemoryTTF(s_font_droid_sans, 16, 16.f);
    io.Fonts->Build();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_NewFrame();

    if (!m_impl->comms.start())
    {
        return ts::Error("Cannot start comms.");
    }

    return ts::success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void GLFW_HAL::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();

    shutdown_display();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Comms& GLFW_HAL::get_comms()
{
    return m_impl->comms;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

math::vec2u32 GLFW_HAL::get_size() const
{
    return math::vec2u32(s_width, s_height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

settings::Settings& GLFW_HAL::get_settings()
{
    return m_impl->settings;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool GLFW_HAL::process()
{
    m_impl->comms.process();
    return update_display();
}


}

#endif
