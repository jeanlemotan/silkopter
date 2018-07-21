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

    math::vec2u32 display_size = hal.get_display_size();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScrollbarSize = display_size.x / 80.f;
    style.TouchExtraPadding = ImVec2(style.ScrollbarSize * 2.f, style.ScrollbarSize * 2.f);
    //style.ItemSpacing = ImVec2(size.x / 200, size.x / 200);
    //style.ItemInnerSpacing = ImVec2(style.ItemSpacing.x / 2, style.ItemSpacing.y / 2);

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 2.f;

    Video_Decoder decoder;
    decoder.init();

    Clock::time_point last_tp = Clock::now();

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
        video_packet_queue.push_back(packet, false);

        resolution = res;
    };

    float temperature = 0.f;

    Clock::time_point last_temperature_history_tp = Clock::now();
    std::vector<float> temperature_history;

    Clock::time_point last_dBm_history_tp = Clock::now();
    std::vector<float> tx_dBm_history;
    std::vector<float> rx_dBm_history;

    float brightness = 50.f;
    float fan_speed = 20.f;

    silk::stream::ICamera_Commands::Value camera_commands;

    while (hal.process())
    {
        Video_Packet_ptr packet;
        while (video_packet_queue.pop_front(packet, false))
        {
            decoder.decode_data(packet->data(), packet->size(), resolution);
            packet = nullptr;
        }
        decoder.release_buffers();

        Clock::time_point now = Clock::now();
        Clock::duration dt = now - last_tp;
        last_tp = now;
        io.DeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(dt).count();

        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(display_size.x, display_size.y));
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoInputs);

        ImGui::Image((void*)(decoder.get_video_texture_id() | 0x80000000), ImVec2(display_size.x, display_size.y));

        ImGui::Begin("HAL");
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

            if (now - last_temperature_history_tp >= std::chrono::seconds(1))
            {
                last_temperature_history_tp = now;
                temperature_history.push_back(temperature);
                while (temperature_history.size() > 60) { temperature_history.erase(temperature_history.begin()); }
            }
            ImGui::Text("Temperature = %.2f'C", temperature);
            if (!temperature_history.empty())
            {
                ImGui::PlotLines("Temperature",
                                 temperature_history.data(), temperature_history.size(),
                                 0, NULL,
                                 30.f, 90.f,
                                 ImVec2(0, display_size.y / 20));
            }
            if (now - last_dBm_history_tp >= std::chrono::milliseconds(100))
            {
                last_dBm_history_tp = now;
                tx_dBm_history.push_back(hal.get_comms().get_tx_dBm());
                while (tx_dBm_history.size() > 10 * 5) { tx_dBm_history.erase(tx_dBm_history.begin()); }
                rx_dBm_history.push_back(hal.get_comms().get_rx_dBm());
                while (rx_dBm_history.size() > 10 * 5) { rx_dBm_history.erase(rx_dBm_history.begin()); }
            }
            ImGui::Text("TX = %ddBm", (int)hal.get_comms().get_tx_dBm());
            if (!tx_dBm_history.empty())
            {
                ImGui::PlotLines("History",
                                 tx_dBm_history.data(), tx_dBm_history.size(),
                                 0, NULL,
                                 -128.f, 128.f,
                                 ImVec2(0, display_size.y / 20));
            }
            ImGui::Text("RX = %ddBm", (int)hal.get_comms().get_rx_dBm());
            if (!rx_dBm_history.empty())
            {
                ImGui::PlotLines("History",
                                 rx_dBm_history.data(), rx_dBm_history.size(),
                                 0, NULL,
                                 -128.f, 128.f,
                                 ImVec2(0, display_size.y / 20));
            }

            bool hq = camera_commands.quality == silk::stream::ICamera_Commands::Quality::HIGH;
            ImGui::Checkbox("HQ Video", &hq);
            camera_commands.quality = hq ? silk::stream::ICamera_Commands::Quality::HIGH : silk::stream::ICamera_Commands::Quality::LOW;
            ImGui::SameLine();
            ImGui::Checkbox("Record Video", &camera_commands.recording);

            hal.get_comms().send_camera_commands_value(camera_commands);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
        ImGui::End();

        ImGui::End();
        //std::this_thread::sleep_for(std::chrono::microseconds(1));
        ImGui::Render();
    }

    hal.shutdown();

    return 0;
}
