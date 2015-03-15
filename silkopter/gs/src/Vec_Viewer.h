#pragma once

#include "HAL.h"
#include "Comms.h"
#include "ui_Vec_Viewer.h"

class Vec_Viewer : public QWidget
{
public:
    Vec_Viewer(std::string const& unit, uint32_t sample_rate, QWidget *parent);
    ~Vec_Viewer();
    void set_components(math::vec4<bool> const& components);
    void add_sample(q::Clock::duration dt, math::vec4f const& value);
    void process();

private:
    Ui::Vec_Viewer m_ui;
    math::vec4<bool> m_components = math::vec4<bool>(true);
    math::vec4<bool> m_allowed_components = math::vec4<bool>(true);
    std::string m_unit;
    q::Clock::duration m_time = q::Clock::duration(0);
    q::Clock::time_point m_last_time_point = q::Clock::now();

    struct Sample
    {
        q::Clock::duration time;
        math::vec4f value;
    };
    std::vector<Sample> m_samples;

    struct Range
    {
        size_t start_idx = 0;
        size_t end_idx = 0;
        bool is_at_end = true;
        q::Clock::duration time;
        math::vec4f min_value;
        math::vec4f max_value;
        math::vec4f average_value;
        float min_value_scalar = 0;
        float max_value_scalar = 0;
        float average_value_scalar = 0;
    } m_range;

    struct FFT_Data
    {
        size_t sample_rate = 0;

        static const size_t MAX_INPUT_SIZE = 65536;
        std::shared_ptr<float> temp_input;
        std::shared_ptr<fftwf_complex> temp_output;
        fftwf_plan plan;
        size_t plan_sample_count = 0;
    } m_fft;

    void process_fft(FFT_Data& fft);
    void process_plot();
};
