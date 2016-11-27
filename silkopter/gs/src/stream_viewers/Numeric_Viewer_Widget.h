#pragma once

#include <boost/thread.hpp>
#include "Comms.h"
#include "ui_Numeric_Viewer_Widget.h"

#include <QtCharts/QLineSeries>

class Numeric_Viewer_Widget : public QWidget
{
public:
    Numeric_Viewer_Widget(QWidget *parent);
    ~Numeric_Viewer_Widget();

    void init(std::string const& unit, uint32_t sample_rate, bool fft = true);

    void add_graph(std::string const& name, std::string const& unit, QColor color);
    void add_samples(float const* src, bool is_healthy);
    void process();

private:
    void show_context_menu(QPoint const& point);

    void setup_axis(QtCharts::QAbstractAxis* axis);

    Ui::Numeric_Viewer_Wdgiet m_ui;
    size_t m_sample_rate = 0;
    float m_tp = 0.0;
    float m_dts = 0.0;
    bool m_is_started = false;

    struct View
    {
        size_t start_idx = 0;
        size_t end_idx = 0;
        bool is_at_end = true;
        float duration;
    } m_view;

    struct Sample
    {
        float tp;
        bool is_healthy = true;
    };

    std::vector<float> m_temp_values;
    std::vector<float> m_values;

    struct Stats
    {
        float min_value = 0;
        float max_value = 0;
        float average_value = 0;
    };

    struct FFT
    {
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
        QColor color;

        struct Task
        {
            bool is_visible = false;
            FFT fft;
            Stats stats;
            QVector<QPointF> plot_points;
            QVector<QPointF> fft_points;
        } task;

        QtCharts::QLineSeries* plot_series = nullptr;
        QtCharts::QLineSeries* fft_series = nullptr;
    };

    QtCharts::QAbstractAxis* m_plot_x_axis = nullptr;
    QtCharts::QAbstractAxis* m_plot_y_axis = nullptr;
    QtCharts::QChart* m_plot_chart = nullptr;

    QtCharts::QAbstractAxis* m_fft_x_axis = nullptr;
    QtCharts::QAbstractAxis* m_fft_y_axis = nullptr;
    QtCharts::QChart* m_fft_chart = nullptr;

    std::vector<std::unique_ptr<Graph>> m_graphs;

    struct Task
    {
        boost::unique_future<void> future;
    } m_task;

    std::vector<Sample> m_temp_samples;
    std::vector<Sample> m_samples;

    void copy_data();
    void update_view();

    void process_fft_task(size_t idx, View const& view);
    void process_plot_task(size_t idx, View const& view);

    void start_task();
    void finish_task();
};
