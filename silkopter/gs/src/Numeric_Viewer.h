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
    void add_samples(double const* src, bool is_healthy);
    void process();

private:
    void show_context_menu(QPoint const& point);

    Ui::Numeric_Viewer m_ui;
    size_t m_sample_rate = 0;
    double m_tp = 0.0;
    double m_dts = 0.0;
    bool m_is_started = false;

    struct View
    {
        size_t start_idx = 0;
        size_t end_idx = 0;
        bool is_at_end = true;
        double duration;
    } m_view;

    struct Sample
    {
        double tp;
        bool is_healthy = true;
    };
    std::vector<double> m_values;

    struct Stats
    {
        double min_value = 0;
        double max_value = 0;
        double average_value = 0;
    };

    struct FFT
    {
        std::shared_ptr<double> temp_input;
        std::shared_ptr<fftw_complex> temp_output;
        fftw_plan plan;
        size_t plan_sample_count = 0;
    };

    struct Graph
    {
        bool is_visible = true;
        std::string name;
        std::string unit;
        QColor color;

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
