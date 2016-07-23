#include "Numeric_Viewer_Widget.h"
#include "Comms.h"
#include "ui_Numeric_Viewer_Widget.h"

#include <QtCharts>

Numeric_Viewer_Widget::Numeric_Viewer_Widget(QWidget *parent)
    : QWidget(parent)
{
}

Numeric_Viewer_Widget::~Numeric_Viewer_Widget()
{
    m_task.future.wait();
}

void Numeric_Viewer_Widget::init(std::string const& unit, uint32_t sample_rate)
{
    m_sample_rate = sample_rate;
    m_dts = q::Seconds(1.f / float(sample_rate)).count();

    m_ui.setupUi(this);

    m_ui.plot->setCacheMode(QGraphicsView::CacheNone);
    m_ui.plot->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    m_ui.fft->setCacheMode(QGraphicsView::CacheNone);
    m_ui.fft->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    m_plot_x_axis = new QtCharts::QValueAxis();
    setup_axis(m_plot_x_axis);

    m_plot_y_axis = new QtCharts::QValueAxis();
    setup_axis(m_plot_y_axis);

    m_plot_chart = new QtCharts::QChart();
    m_plot_chart->setMargins(QMargins(2, 2, 2, 2));

    m_plot_chart->addAxis(m_plot_x_axis, Qt::AlignBottom);
    m_plot_chart->addAxis(m_plot_y_axis, Qt::AlignLeft);

    m_ui.plot->setChart(m_plot_chart);


    m_fft_x_axis = new QtCharts::QValueAxis();
    setup_axis(m_fft_x_axis);

    m_fft_y_axis = new QtCharts::QValueAxis();
    setup_axis(m_fft_y_axis);

    m_fft_chart = new QtCharts::QChart();
    m_fft_chart->setMargins(QMargins(2, 2, 2, 2));

    m_fft_chart->addAxis(m_fft_x_axis, Qt::AlignBottom);
    m_fft_chart->addAxis(m_fft_y_axis, Qt::AlignLeft);

    m_ui.fft->setChart(m_fft_chart);

    m_fft_x_axis->setRange(0.1f, m_sample_rate / 2.f);

    m_ui.progress->setValue(m_ui.progress->maximum());
    connect(m_ui.progress, &QScrollBar::valueChanged, [this](int pos)
    {
        if (pos == m_ui.progress->maximum())
        {
            m_view.is_at_end = true;
            update_view();
        }
        else
        {
            m_view.is_at_end = false;
            float ratio = static_cast<float>(pos - m_ui.progress->minimum()) / static_cast<float>(m_ui.progress->maximum() - m_ui.progress->minimum());
            size_t end = m_samples.size();
            m_view.end_idx = math::clamp(static_cast<size_t>(ratio * end), size_t(2), end);
            update_view();
        }
        process();
    });

    // myWidget is any QWidget-derived class
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, [this](QPoint const& pos)
    {
        show_context_menu(mapToGlobal(pos));
    });

    m_ui.plot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_ui.plot, &QWidget::customContextMenuRequested, [this](QPoint const& pos)
    {
        show_context_menu(m_ui.plot->mapToGlobal(pos));
    });

    m_ui.fft->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_ui.fft, &QWidget::customContextMenuRequested, [this](QPoint const& pos)
    {
        show_context_menu(m_ui.fft->mapToGlobal(pos));
    });

    //m_ui.window->setSingleStep(1.0 / float(m_sample_rate));
}

void Numeric_Viewer_Widget::setup_axis(QtCharts::QAbstractAxis* axis)
{
    QFont font;
    font.setPointSize(8);
    axis->setLabelsFont(font);
    axis->setTitleFont(font);
}

void Numeric_Viewer_Widget::show_context_menu(QPoint const& pos)
{
    QMenu menu;
    for (std::unique_ptr<Graph>& g: m_graphs)
    {
        QImage image(10, 10, QImage::Format_ARGB32_Premultiplied);
        image.fill(g->color);
        QImage pressed_image(8, 8, QImage::Format_ARGB32_Premultiplied);
        pressed_image.fill(g->color);

        QIcon icon;
        icon.addPixmap(QPixmap::fromImage(pressed_image), QIcon::Normal, QIcon::On);
        icon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::Off);

        QAction* action = menu.addAction(icon, g->name.c_str());
        action->setCheckable(true);
        action->setChecked(g->is_visible);
        Graph& graph = *g;
        connect(action, &QAction::toggled, [this, &graph](bool yes)
        {
            graph.is_visible = yes;
            process();
        });
    }

    menu.addSeparator();

    QAction* action = menu.addAction(QIcon(":/icons/fft.png"), "Logarithmic FFT");
    action->setCheckable(true);
    action->setChecked(dynamic_cast<QtCharts::QLogValueAxis*>(m_fft_x_axis) != nullptr);
    connect(action, &QAction::toggled, [this](bool yes)
    {
        m_fft_chart->removeAxis(m_fft_x_axis);
        delete m_fft_x_axis;

        if (yes)
        {
            QtCharts::QLogValueAxis* axis = new QtCharts::QLogValueAxis();
            setup_axis(axis);
            //axisY->setLabelFormat("%g");
            //axisY->setTitleText("Values");
            axis->setBase(10);
            m_fft_x_axis = axis;

//            m_ui.fft->xAxis->setScaleType(QCPAxis::stLogarithmic);
//            m_ui.fft->xAxis->setScaleLogBase(10);
//            m_ui.fft->xAxis->setNumberFormat("fb");
//            m_ui.fft->xAxis->setNumberPrecision(0);
//            m_ui.fft->xAxis->setSubTickCount(10);
        }
        else
        {
            QtCharts::QValueAxis* axis = new QtCharts::QValueAxis();
            setup_axis(axis);
            //axisY->setLabelFormat("%g");
            //axisY->setTitleText("Values");
            m_fft_x_axis = axis;
//            m_ui.fft->xAxis->setScaleType(QCPAxis::stLinear);
//            m_ui.fft->xAxis->setNumberFormat(m_ui.fft->yAxis->numberFormat());
//            m_ui.fft->xAxis->setNumberPrecision(m_ui.fft->yAxis->numberPrecision());
//            m_ui.fft->xAxis->setSubTickCount(m_ui.fft->yAxis->subTickCount());
        }

        m_fft_chart->addAxis(m_fft_x_axis, Qt::AlignBottom);
        m_fft_x_axis->setRange(0.1f, m_sample_rate / 2.f);

        for (std::unique_ptr<Graph>& g: m_graphs)
        {
            g->fft_series->attachAxis(m_fft_x_axis);
        }

        process();
    });

    menu.exec(pos);
}

void Numeric_Viewer_Widget::add_graph(std::string const& name, std::string const& unit, QColor color)
{
    QASSERT(!m_is_started);
    if (m_is_started)
    {
        return;
    }

    QLineSeries* plot_series = new QLineSeries();
    plot_series->setUseOpenGL(true);
    plot_series->setPen(QPen(color, 2));
    plot_series->setName((name + " " + unit).c_str());

    m_plot_chart->addSeries(plot_series);
    plot_series->attachAxis(m_plot_x_axis);
    plot_series->attachAxis(m_plot_y_axis);

    QLineSeries* fft_series = new QLineSeries();
    fft_series->setUseOpenGL(true);
    fft_series->setPen(QPen(color, 2));
    fft_series->setName((name + " " + unit).c_str());

    m_fft_chart->addSeries(fft_series);
    fft_series->attachAxis(m_fft_x_axis);
    fft_series->attachAxis(m_fft_y_axis);

    m_graphs.emplace_back(new Graph());
    Graph& graph = *m_graphs.back();
    graph.name = name;
    graph.unit = unit;
    graph.color = color;
    graph.plot_series = plot_series;
    graph.fft_series = fft_series;
}

void Numeric_Viewer_Widget::add_samples(float const* src, bool is_healthy)
{
    size_t off = m_temp_values.size();
    m_temp_values.resize(m_temp_values.size() + m_graphs.size());
//    std::copy(src, src + m_graphs.size(), m_temp_values.begin() + off);
    auto dst = m_temp_values.begin() + off;

    QASSERT(src != nullptr);
    for (size_t i = 0; i < m_graphs.size(); i++)
    {
        if (math::is_finite(src[i]))
        {
            *dst++ = static_cast<float>(src[i]);
        }
        else
        {
            //QASSERT(0);
            *dst++ = 0;//std::numeric_limits<float>::max();
        }
    }

    Sample sample;
    sample.tp = m_tp;
    sample.is_healthy = is_healthy;
    m_temp_samples.emplace_back(std::move(sample));
    m_tp += m_dts;
}

void Numeric_Viewer_Widget::process()
{
    //launch any pending task
    if (!m_is_started)
    {
        start_task();
        return;
    }

    //check if all ready
    if (m_task.future.is_ready())
    {
        finish_task();
        start_task();
    }
}

void Numeric_Viewer_Widget::copy_data()
{
    {
        size_t off = m_samples.size();
        m_samples.resize(m_samples.size() + m_temp_samples.size());
        std::copy(m_temp_samples.begin(), m_temp_samples.end(), m_samples.begin() + off);
        m_temp_samples.clear();
    }
    {
        size_t off = m_values.size();
        m_values.resize(m_values.size() + m_temp_values.size());
        std::copy(m_temp_values.begin(), m_temp_values.end(), m_values.begin() + off);
        m_temp_values.clear();
    }
}

void Numeric_Viewer_Widget::update_view()
{
    //update range
    int time = m_ui.window->value();
    int sample_count = static_cast<int>(math::round(time * float(m_sample_rate)));
    if (m_view.is_at_end)
    {
        m_view.end_idx = m_samples.size();
    }

    int start_idx = math::max(static_cast<int>(m_view.end_idx) - sample_count, 0);
    m_view.start_idx = static_cast<size_t>(start_idx);

    if (!m_samples.empty())
    {
        m_view.duration = m_samples[m_view.end_idx - 1].tp - m_samples[m_view.start_idx].tp;
    }
    else
    {
        m_view.duration = 0.0;
    }
}

void Numeric_Viewer_Widget::process_plot_task(size_t graph_idx, View const& view)
{
    Graph& graph = *m_graphs[graph_idx];
    Graph::Task& task = graph.task;

    task.stats.average_value = 0;
    task.stats.min_value = std::numeric_limits<float>::max();
    task.stats.max_value = std::numeric_limits<float>::lowest();

    task.plot_points.clear();
    task.plot_points.reserve(100000);

    //graph.plot_data_map.resize(0);
    //graph.plot_data_map.reserve(100000);

    float pixels_per_second = static_cast<float>(m_ui.plot->width()) / view.duration;
    size_t skip = static_cast<int>(m_sample_rate / pixels_per_second) + 1;
    size_t offset = view.start_idx * m_graphs.size();
    for (size_t i = view.start_idx; i < view.end_idx; i++)
    {
        Sample const& s = m_samples[i];
        float v = m_values[offset + graph_idx];
        offset += m_graphs.size();
        task.stats.max_value = math::max(task.stats.max_value, v);
        task.stats.min_value = math::min(task.stats.min_value, v);
        if (i % skip == 0)
        {
            task.plot_points.push_back(QPointF(s.tp, v));
        }
    }
    task.stats.average_value = (task.stats.max_value + task.stats.min_value) * 0.5f;
}

void Numeric_Viewer_Widget::process_fft_task(size_t graph_idx, View const& view)
{
    Graph& graph = *m_graphs[graph_idx];
    Graph::Task& task = graph.task;

    size_t sample_count = view.end_idx - view.start_idx;

    task.fft_points.clear();
    task.fft_points.reserve(100000);

    if (sample_count < m_sample_rate)
    {
        return;
    }

    size_t output_size = sample_count / 2 + 1;
    float div = 4.f / float(sample_count);
    float freq_div = m_sample_rate / float(sample_count);

    size_t offset = view.start_idx * m_graphs.size();
    float* temp_input = task.fft.temp_input.get();
    for (size_t i = 0; i < sample_count; i++)
    {
        temp_input[i] = m_values[offset + graph_idx];
        offset += m_graphs.size();
    }
    fftwf_execute(task.fft.plan);
    fftwf_complex* temp_output = task.fft.temp_output.get();

    float pixels_per_second = static_cast<float>(m_ui.plot->width()) / view.duration;
    {
        float last_key = 0;
        float max_value = std::numeric_limits<float>::lowest();
        for (size_t i = 1; i < output_size; i++)
        {
            float x = temp_output[i][0] * div;
            float y = temp_output[i][1] * div;
            float v = math::sqrt(x*x + y*y);
            max_value = math::max(max_value, v);

            float key = i * freq_div;
            float dt = key - last_key;
            if (dt * pixels_per_second >= 0.2)
            {
                last_key = key;
                task.fft_points.push_back(QPointF(key, max_value));
                max_value = std::numeric_limits<float>::lowest();
            }
        }
    }
}

void Numeric_Viewer_Widget::start_task()
{
    copy_data();
    update_view();

    if (m_sample_rate == 0 || m_samples.empty())
    {
        return;
    }

    View view = m_view; //make a copy as it change change while the task is progressing

    size_t sample_count = view.end_idx - view.start_idx;
    if (sample_count < 2)
    {
        return;
    }

    bool any_graph_visible = false;
    for (size_t i = 0; i < m_graphs.size(); i++)
    {
        Graph& graph = *m_graphs[i];
        Graph::Task& task = graph.task;
        task.is_visible = graph.is_visible; //copy this in case it changes when we thread
        if (graph.is_visible)
        {
            if (sample_count != task.fft.plan_sample_count)
            {
                task.fft.temp_input.reset(static_cast<float*>(fftwf_malloc(sample_count * sizeof(float))), fftwf_free);
                task.fft.temp_output.reset(static_cast<fftwf_complex*>(fftwf_malloc(sample_count * sizeof(fftwf_complex))), fftwf_free);
                task.fft.plan = fftwf_plan_dft_r2c_1d(sample_count, task.fft.temp_input.get(), task.fft.temp_output.get(), FFTW_ESTIMATE);
                task.fft.plan_sample_count = sample_count;
            }
            any_graph_visible = true;
        }
        else
        {
            graph.plot_series->clear();
            graph.fft_series->clear();
        }
    }

    if (!any_graph_visible)
    {
        return;
    }

    m_is_started = true;

    m_task.future = silk::async([this, view]()
    {
        for (size_t i = 0; i < m_graphs.size(); i++)
        {
            Graph::Task& task = m_graphs[i]->task;
            if (task.is_visible)
            {
                process_plot_task(i, view);
                process_fft_task(i, view);
            }
        }
    });
}

void Numeric_Viewer_Widget::finish_task()
{
    float min_value = std::numeric_limits<float>::max();
    float max_value = std::numeric_limits<float>::lowest();
    float max_average_value = std::numeric_limits<float>::lowest();
    float max_value_range = std::numeric_limits<float>::lowest();
    bool has_range = false;

    for (size_t i = 0; i < m_graphs.size(); i++)
    {
        Graph& graph = *m_graphs[i];
        Graph::Task& task = graph.task;
        if (graph.is_visible)
        {
            has_range = true;
            min_value = math::min(min_value, task.stats.min_value);
            max_value = math::max(max_value, task.stats.max_value);
            max_average_value = math::max(max_average_value, task.stats.average_value);
            max_value_range = math::max(max_value_range, task.stats.max_value - task.stats.min_value);
        }
    }

    if (!has_range)
    {
        min_value = 0;
        max_value = 0;
        max_average_value = 0;
    }

    m_ui.data_range->setValue(max_value_range);
    m_ui.average->setValue(max_average_value);
    //m_ui.plot->xAxis->setRange(m_samples[m_view.start_idx].tp, m_samples[m_view.start_idx].tp + m_view.duration);
    m_plot_x_axis->setRange(m_samples[m_view.start_idx].tp, m_samples[m_view.start_idx].tp + m_view.duration);

    float display_range = m_ui.display_range->value();
    if (math::is_zero(display_range, math::epsilon<float>()))
    {
        m_plot_y_axis->setRange(min_value, max_value);
        m_fft_y_axis->setRange(0, max_value - min_value);
    }
    else
    {
        float average_value = (max_value + min_value) * 0.5;
        m_plot_y_axis->setRange(average_value - display_range / 2, average_value + display_range / 2);
        m_fft_y_axis->setRange(0, display_range);
    }

    for (size_t i = 0; i < m_graphs.size(); i++)
    {
        Graph& graph = *m_graphs[i];
        m_graphs[i]->plot_series->replace(graph.task.plot_points);
        m_graphs[i]->fft_series->replace(graph.task.fft_points);
    }

//    m_ui.fft->update();
//    update();
}
