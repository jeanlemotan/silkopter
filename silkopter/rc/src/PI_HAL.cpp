#include "PI_HAL.h"

#if defined RASPBERRY_PI

#include "def_lang/Serialization.h"
#include "def_lang/JSON_Serializer.h"

#include "settings.def.h"
#include <fstream>
#include <future>
#include <atomic>

#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include <tslib.h>

extern "C"
{
#include "interface/vcos/vcos.h"
#include <bcm_host.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglext_brcm.h>
}

extern "C"
{
#include "utils/hw/pigpio.h"
#include "utils/hw/bcm2835.h"
}

#include "utils/hw/UART_Dev.h"

extern uint8_t s_font_droid_sans[];
constexpr uint32_t FAN_GPIO = 4;

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace silk
{

static const std::string k_settings_filename("settings.json");
extern std::string s_program_path;
static uint32_t s_width = 1920;
static uint32_t s_height = 1080;

struct PI_HAL::Impl
{
    Impl(PI_HAL& hal)
        : comms(hal)
        , settings()
    {}

    Comms comms;

    EGLDisplay display = nullptr;
    EGLSurface surface = nullptr;
    EGLContext context = nullptr;
    tsdev* ts = nullptr;

    constexpr static size_t MAX_TOUCHES = 3;
    constexpr static size_t MAX_SAMPLES = 10;

    ts_sample_mt** ts_samples;

    struct Touch
    {
        int id = 0;
        int32_t x = 0;
        int32_t y = 0;
        bool is_pressed = false;
    };
    std::array<Touch, MAX_TOUCHES> touches;

    settings::Settings settings;

    bool pigpio_is_isitialized = false;

    float target_fan_speed = 0.2f;
    float fan_speed = 0.0f;
    Clock::time_point fan_speed_tp = Clock::now();

    float target_backlight = 0.2f;
    float backlight = 0.0f;
    std::future<void> backlight_future;
    std::atomic_bool backlight_future_cancelled;
    Clock::time_point backlight_tp = Clock::now();
    util::hw::UART_Dev backlight_uart;

    float temperature_filtered = 0.f;
    Clock::time_point temperature_filtered_tp = Clock::now();
    float temperature = 0.f;
    std::future<float> temperature_future;
    Clock::time_point temperature_tp = Clock::now();
};

///////////////////////////////////////////////////////////////////////////////////////////////////


bool PI_HAL::init_pigpio()
{
    if (m_impl->pigpio_is_isitialized)
    {
        return true;
    }

    QLOGI("Initializing pigpio");
    if (gpioCfgClock(2, PI_CLOCK_PCM, 0) < 0 ||
            gpioCfgPermissions(static_cast<uint64_t>(-1)))
    {
        QLOGE("Cannot configure pigpio");
        return false;
    }
    if (gpioInitialise() < 0)
    {
        QLOGE("Cannot init pigpio");
        return false;
    }

    gpioSetPWMfrequency(FAN_GPIO, 20000);

    m_impl->pigpio_is_isitialized = true;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void PI_HAL::shutdown_pigpio()
{
    if (m_impl->pigpio_is_isitialized)
    {
        gpioTerminate();
    }
    m_impl->pigpio_is_isitialized = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool PI_HAL::init_display()
{
    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    static const EGLint attribute_list[] =
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 0,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };
    static const EGLint context_attributes[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLConfig config;

    // get an EGL display connection
    m_impl->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(m_impl->display!=EGL_NO_DISPLAY);

    // init the EGL display connection
    EGLBoolean result = eglInitialize(m_impl->display, NULL, NULL);
    assert(EGL_FALSE != result);

    // get an appropriate EGL frame buffer configuration
    EGLint num_config;
    result = eglChooseConfig(m_impl->display, attribute_list, &config, 1, &num_config);
    assert(EGL_FALSE != result);

    // create an EGL rendering context
    m_impl->context = eglCreateContext(m_impl->display, config, EGL_NO_CONTEXT, context_attributes);
    assert(m_impl->context!=EGL_NO_CONTEXT);

    // create an EGL window surface
    uint32_t width = 0;
    uint32_t height = 0;
    int32_t success = graphics_get_display_size(0 /* LCD */, &width, &height);
    assert( success >= 0 );

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = width;
    dst_rect.height = height;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = width << 16;
    src_rect.height = height << 16;

    dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
    dispman_update = vc_dispmanx_update_start( 0 );

    VC_DISPMANX_ALPHA_T alpha;
    memset(&alpha, 0, sizeof(alpha));
    alpha.flags = (DISPMANX_FLAGS_ALPHA_T)(DISPMANX_FLAGS_ALPHA_FROM_SOURCE | DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS);
    alpha.opacity = 255;
    alpha.mask = 0;

    dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
    0/*layer*/, &dst_rect, 0/*src*/,
    &src_rect, DISPMANX_PROTECTION_NONE, &alpha /*alpha*/, 0/*clamp*/, DISPMANX_NO_ROTATE/*transform*/);

    static EGL_DISPMANX_WINDOW_T nativewindow;
    nativewindow.element = dispman_element;
    nativewindow.width = width;
    nativewindow.height = height;
    vc_dispmanx_update_submit_sync( dispman_update );

    m_impl->surface = eglCreateWindowSurface(m_impl->display, config, &nativewindow, NULL);
    assert(m_impl->surface != EGL_NO_SURFACE);

    // connect the context to the surface
    result = eglMakeCurrent(m_impl->display, m_impl->surface, m_impl->surface, m_impl->context);
    assert(EGL_FALSE != result);

    eglSwapInterval(m_impl->display, 0);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = s_width;
    io.DisplaySize.y = s_height;

    {
        ts::Result<void> result = m_impl->backlight_uart.init("/dev/ttyACM0", util::hw::UART_Dev::Baud::_115200);
        if (result != ts::success)
        {
            QLOGW("Failed to initialize backlight uart: {}", result.error().what());
            return false;
        }
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void PI_HAL::shutdown_display()
{
    // clear screen
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(m_impl->display, m_impl->surface);

    // Release OpenGL resources
    eglMakeCurrent(m_impl->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(m_impl->display, m_impl->surface);
    eglDestroyContext(m_impl->display, m_impl->context);
    eglTerminate(m_impl->display);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool PI_HAL::update_display()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData(), true);

    eglSwapBuffers(m_impl->display, m_impl->surface);

    Clock::time_point now = Clock::now();
    if (m_impl->target_backlight != m_impl->backlight &&
            now - m_impl->backlight_tp >= std::chrono::milliseconds(50))
    {
        m_impl->backlight = m_impl->target_backlight;
        m_impl->backlight_tp = now;

        float b = m_impl->target_backlight;
        std::atomic_bool& cancelled = m_impl->backlight_future_cancelled;

        cancelled = true;
        if (m_impl->backlight_future.valid())
        {
            m_impl->backlight_future.wait();
        }
        cancelled = false;

        m_impl->backlight_future = std::async(std::launch::async, [this, b, &cancelled]()
        {
            if (!cancelled)
            {
                std::string command = "backlight " + std::to_string((int)(b * 100.f + 0.5f)) + "\r";
                m_impl->backlight_uart.write((const uint8_t*)command.data(), command.size());
            }
        });
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool PI_HAL::init_ts()
{
    m_impl->ts = ts_open("/dev/input/event0", 1);
    if (!m_impl->ts)
    {
        perror("ts_open");
        return false;
    }
    int res = ts_config(m_impl->ts);
    if (res < 0)
    {
        perror("ts_config");
        return false;
    }

    m_impl->ts_samples = new ts_sample_mt*[Impl::MAX_SAMPLES];
    for (size_t i = 0; i < Impl::MAX_SAMPLES; i++)
    {
        m_impl->ts_samples[i] = new ts_sample_mt[Impl::MAX_TOUCHES];
        memset(m_impl->ts_samples[i], 0, sizeof(ts_sample_mt));
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void PI_HAL::shutdown_ts()
{
    ts_close(m_impl->ts);
    m_impl->ts = nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void PI_HAL::update_ts()
{
//    for (Impl::Touch& touch: m_impl->touches)
//    {
//        touch.is_pressed = false;
//    }

    int ret = ts_read_mt(m_impl->ts, (ts_sample_mt**)m_impl->ts_samples, Impl::MAX_TOUCHES, Impl::MAX_SAMPLES);
    for (int sampleIndex = 0; sampleIndex < ret; sampleIndex++)
    {
        for (size_t slotIndex = 0; slotIndex < Impl::MAX_TOUCHES; slotIndex++)
        {
            Impl::Touch& touch = m_impl->touches[slotIndex];

            ts_sample_mt& sample = m_impl->ts_samples[sampleIndex][slotIndex];
            if (sample.valid < 1)
            {
                continue;
            }
//            printf("%ld.%06ld: %d %6d %6d %6d\n",
//                   sample.tv.tv_sec,
//                   sample.tv.tv_usec,
//                   sample.slot,
//                   sample.x,
//                   sample.y,
//                   sample.pressure);

            touch.is_pressed = sample.pressure > 0;
            touch.x = sample.x;
            touch.y = sample.y;
            touch.id = sample.slot;
        }
    }

    Impl::Touch& touch = m_impl->touches[0];

    // Update buttons
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[0] = touch.is_pressed;

    // Update mouse position
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    float mouse_x = touch.y;
    float mouse_y = s_height - touch.x;
    io.MousePos = ImVec2(mouse_x, mouse_y);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void PI_HAL::generate_settings_file()
{
    get_settings() = settings::Settings();
    //get_settings().get_comms().set_video_wlan_interfaces({"wlan1", "wlan2"});
    save_settings();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ts::Result<void> PI_HAL::load_settings()
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

void PI_HAL::save_settings()
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

PI_HAL::PI_HAL()
{
    m_impl.reset(new Impl(*this));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PI_HAL::~PI_HAL()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////

ts::Result<void> PI_HAL::init()
{
    ts::Result<void> result = load_settings();
    if (result != ts::success)
    {
        return result.error();
    }

    save_settings();

    bcm_host_init();

    if (!init_pigpio())
    {
        return ts::Error("Cannot initialize pigpio");
    }
    QLOGI("Initialized pigpio");
    if (!init_display())
    {
        return ts::Error("Cannot initialize EGL");
    }
    QLOGI("Initialized EGL");
    if (!init_ts())
    {
        return ts::Error("Cannot initialize touch screen");
    }
    QLOGI("Initialized TS");

    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
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

void PI_HAL::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();

    shutdown_pigpio();
    shutdown_ts();
    shutdown_display();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Comms& PI_HAL::get_comms()
{
    return m_impl->comms;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

math::vec2u32 PI_HAL::get_display_size() const
{
    return math::vec2u32(s_width, s_height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void PI_HAL::set_backlight(float brightness)
{
    m_impl->target_backlight = math::clamp(brightness, 0.f, 1.f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float PI_HAL::get_temperature() const
{
    return m_impl->temperature_filtered;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void PI_HAL::set_fan_speed(float speed)
{
    m_impl->target_fan_speed = math::clamp(speed, 0.f, 1.f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

settings::Settings& PI_HAL::get_settings()
{
    return m_impl->settings;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void PI_HAL::update_temperature()
{
    Clock::time_point now = Clock::now();

    if (m_impl->temperature_future.valid())
    {
        if (m_impl->temperature_future.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready)
        {
            float t = m_impl->temperature_future.get();
            if (t > -1000.f)
            {
                m_impl->temperature = t;
            }
        }
    }
    else
    {
        if (now - m_impl->temperature_tp >= std::chrono::milliseconds(500))
        {
            m_impl->temperature_tp = now;
            m_impl->temperature_future = std::async(std::launch::async, []()
            {
                double T;
                FILE* fp = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
                if (fp != nullptr)
                {
                    double t;
                    fscanf(fp, "%lf", &t);
                    fclose(fp);
                    return (float)(t / 1000.0);
                }
                return -1000000.f;
            });
        }
    }

    Clock::duration dt = now - m_impl->temperature_filtered_tp;
    if (dt >= std::chrono::milliseconds(30))
    {
        m_impl->temperature_filtered_tp = now;
        float dtsf = std::chrono::duration<float>(dt).count();
        m_impl->temperature_filtered = math::lerp(m_impl->temperature_filtered, m_impl->temperature, math::min(dtsf, 1.f));
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void PI_HAL::update_fan()
{
    Clock::time_point now = Clock::now();
    if (//m_impl->target_fan_speed != m_impl->fan_speed &&
            now - m_impl->fan_speed_tp >= std::chrono::milliseconds(50))
    {
        m_impl->fan_speed = m_impl->target_fan_speed;
        m_impl->fan_speed_tp = now;

        float min_temp = 50.f;
        float max_temp = 80.f;
        float temp_mu = math::clamp((get_temperature() - min_temp) / (max_temp - min_temp), 0.f, 1.f);
        float speed = m_impl->target_fan_speed * temp_mu;
        gpioPWM(FAN_GPIO, math::clamp((uint32_t)(speed * 255.f), 0u, 255u));
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool PI_HAL::process()
{
    m_impl->comms.process();
    update_ts();
    update_temperature();
    update_fan();
    return update_display();
}


}

#endif
