#pragma once

#include "HAL.h"
#include "Comms.h"
#include "ui_Numeric_Viewer.h"

class Numeric_Viewer : public QWidget
{
public:
    Numeric_Viewer(std::string const& unit, uint32_t sample_rate, QWidget *parent);
    ~Numeric_Viewer();
    void add_graph(std::string const& name, std::string const& unit, QColor color);
    void add_samples(q::Clock::duration dt, float const* src);
    void process();

private:
    Ui::Numeric_Viewer m_ui;
    size_t m_sample_rate = 0;
    q::Clock::duration m_time = q::Clock::duration(0);
    bool m_is_started = false;

    struct View
    {
        size_t start_idx = 0;
        size_t end_idx = 0;
        bool is_at_end = true;
        double time;
    } m_view;

    struct Sample
    {
        double time;
    };
    std::vector<float> m_values;

    struct Stats
    {
        float min_value = 0;
        float max_value = 0;
        float average_value = 0;
    };

    struct FFT
    {
        static const size_t MAX_INPUT_SIZE = 2000000;
        std::shared_ptr<float> temp_input;
        std::shared_ptr<fftwf_complex> temp_output;
        fftwf_plan plan;
        size_t plan_sample_count = 0;
    };

    struct Graph
    {
        bool is_visible = true;
        std::string name;
        std::string unit;

        boost::unique_future<void> future;

        QCPDataMap plot_data_map;
        QCPDataMap fft_data_map;

        Stats stats;
        FFT fft;

        QCPGraph* plot = nullptr;
        QCPGraph* fft_plot = nullptr;
    };

    std::vector<std::unique_ptr<Graph>> m_graphs;

    std::vector<Sample> m_temp_samples;
    std::vector<Sample> m_samples;

    void copy_samples();
    void update_view();

    void process_fft_task(size_t idx, View const& view);
    void process_plot_task(size_t idx, View const& view);

    void start_tasks();
    void finish_tasks();
};
