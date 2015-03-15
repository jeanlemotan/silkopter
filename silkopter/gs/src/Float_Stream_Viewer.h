#pragma once

#include "HAL.h"
#include "Comms.h"
#include "ui_Float_Stream_Viewer.h"

class Float_Stream_Viewer : public QWidget
{
public:
    Float_Stream_Viewer(std::string const& unit, uint32_t sample_rate, QWidget *parent);
    ~Float_Stream_Viewer();
    void add_sample(q::Clock::duration dt, float value);
    void process();

private:
    Ui::Float_Stream_Viewer m_ui;
    QCPGraph* m_fft_plottable = nullptr;
    std::string m_unit;
    q::Clock::duration m_time = q::Clock::duration(0);
    q::Clock::time_point m_last_time_point = q::Clock::now();

    struct Sample
    {
        q::Clock::duration time;
        float value = 0;
    };
    std::vector<Sample> m_samples;

    struct Range
    {
        size_t start_idx = 0;
        size_t end_idx = 0;
        bool is_at_end = true;
        q::Clock::duration time;
        float min_value = 0;
        float max_value = 0;
        float average_value = 0;
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
