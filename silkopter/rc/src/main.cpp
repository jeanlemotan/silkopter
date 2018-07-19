#include "Comms.h"
#include "Video_Decoder.h"

//#include "Menu_System.h"
//#include "Splash_Menu_Page.h"

//#include "Main_Menu_Page.h"

#include "utils/Clock.h"
#include "IHAL.h"

#ifdef RASPBERRY_PI
#   include "PI_HAL.h"
#else
#   include "GLFW_HAL.h"
#endif

#include "imgui.h"

namespace silk
{

int s_version_major = 1;
int s_version_minor = 0;

std::string s_program_path;

std::unique_ptr<silk::IHAL> s_hal;

}

///////////////////////////////////////////////////////////////////////////////////////////////////

//This prints an "Assertion failed" message and aborts.
void __assert_fail(const char *__assertion, const char *__file, unsigned int __line, const char *__function)
{
    QASSERT_MSG(false, "assert: {}:{}: {}: {}", __file, __line, __function, __assertion);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void signal_callback_handler(int signum)
{
    printf("Caught signal %d\n", signum);
    if (silk::s_hal)
    {
        silk::s_hal->shutdown();
    }
    exit(signum);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));
    q::logging::set_decorations(q::logging::Decorations(q::logging::Decoration::TIMESTAMP, q::logging::Decoration::LEVEL, q::logging::Decoration::TOPIC));

    //    boost::shared_ptr<asio::io_service::work> work(new asio::io_service::work(s_async_io_service));
    //    std::thread_group worker_threads;
    //    for(int x = 0; x < 4; ++x)
    //    {
    //        worker_threads.create_thread(boost::bind(&asio::io_service::run, &s_async_io_service));
    //    }

    signal(SIGHUP, signal_callback_handler);
    signal(SIGINT, signal_callback_handler);
    signal(SIGCONT, signal_callback_handler);
    signal(SIGTERM, signal_callback_handler);

    silk::s_program_path = argv[0];
    size_t off = silk::s_program_path.find_last_of('/');
    if (off != std::string::npos)
    {
        silk::s_program_path = silk::s_program_path.substr(0, off);
    }
    QLOGI("Program path: {}.", silk::s_program_path);

#ifdef RASPBERRY_PI
    silk::s_hal.reset(new silk::PI_HAL());
#else
    silk::s_hal.reset(new silk::GLFW_HAL());
#endif

    silk::IHAL& hal = *silk::s_hal;
    ts::Result<void> result = hal.init();
    if (result != ts::success)
    {
        QLOGE("Cannot init hal: {}.", result.error().what());
        exit(1);
    }

    math::vec2u32 size = hal.get_display_size();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScrollbarSize = size.x / 80.f;
    style.TouchExtraPadding = ImVec2(style.ScrollbarSize * 2.f, style.ScrollbarSize * 2.f);
    //style.ItemSpacing = ImVec2(size.x / 200, size.x / 200);
    //style.ItemInnerSpacing = ImVec2(style.ItemSpacing.x / 2, style.ItemSpacing.y / 2);

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 2.f;

    Video_Decoder decoder;

    size_t render_frames = 0;

    q::Clock::time_point last_swapped_tp = q::Clock::now();

    math::vec2u16 resolution;
    typedef std::vector<uint8_t> Video_Packet;
    typedef Pool<Video_Packet>::Ptr Video_Packet_ptr;
    Pool<Video_Packet> video_packet_pool;
    Queue<Video_Packet_ptr> video_packet_queue(32);

    //FILE* f = fopen("video.h264", "wb");

    hal.get_comms().on_video_data_received = [&video_packet_pool, &video_packet_queue, &resolution](void const* data, size_t size, math::vec2u16 const& res)
    {
        //fwrite(data, size, 1, f);
        //fflush(f);
        Video_Packet_ptr packet = video_packet_pool.acquire();
        packet->resize(size);
        memcpy(packet->data(), data, size);
        video_packet_queue.push_back(packet, true);

        resolution = res;
    };

    q::Clock::time_point last_tp = q::Clock::now();
    size_t process_frames = 0;

    float temperature = 0.f;
    std::vector<float> temperature_history;

    float brightness = 50.f;
    float fan_speed = 20.f;

    while (hal.process())
    {
        ImGui::NewFrame();
        {
            static int counter = 0;
            ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
            if (ImGui::SliderFloat("Brightness", &brightness, 10.f, 100.f, "%.0f%%"))
            {
                hal.set_backlight(brightness / 100.f);
            }
            if (ImGui::SliderFloat("Fan", &fan_speed, 10.f, 100.f, "%.0f%%"))
            {
                hal.set_fan_speed(fan_speed / 100.f);
            }
            temperature = hal.get_temperature();
            temperature_history.push_back(temperature);
            while (temperature_history.size() > 60 * 5)
            {
                temperature_history.erase(temperature_history.begin());
            }
            //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
            //ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::Text("Temperature = %.2f'C", temperature);
            if (!temperature_history.empty())
            {
                ImGui::PlotLines("History",
                                 temperature_history.data(), temperature_history.size(),
                                 0, NULL,
                                 *std::min_element(temperature_history.begin(), temperature_history.end()),
                                 *std::max_element(temperature_history.begin(), temperature_history.end()),
                                 ImVec2(0, size.y / 20));
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
        //ImGui::EndFrame();

        process_frames++;
        if (q::Clock::now() - last_tp >= std::chrono::seconds(1))
        {
            last_tp = q::Clock::now();
            QLOGI("P FPS: {}, R FPS: {}", process_frames, render_frames);
            process_frames = 0;
            render_frames = 0;
        }

        bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);

        //std::this_thread::sleep_for(std::chrono::microseconds(1));
        ImGui::Render();
    }

    hal.shutdown();

    return 0;
}
