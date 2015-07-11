#pragma once

#include <QDialog>

#include "HAL.h"
#include "Comms.h"

class Acceleration_Calibration_Wizard : public QDialog
{
public:
    explicit Acceleration_Calibration_Wizard(silk::HAL& hal, silk::Comms& comms, silk::node::stream::Acceleration_ptr stream, QWidget* parent = 0);

private:
    void advance();
    void cancel();

    silk::node::stream::Acceleration_ptr m_stream;
    silk::node::Node_ptr m_node;

    silk::HAL& m_hal;
    silk::Comms& m_comms;

    QWidget* m_content = nullptr;


    enum class Step
    {
        INTRO,
        START,
        RESET,
        COLLECT_DATA,
        SHOW_INSTRUCTIONS,
        SET,
    };

    Step m_step = Step::INTRO;

    q::Clock::time_point m_step_timepoint;
    uint32_t m_last_sample_idx = 0;

    std::vector<math::vec3f> m_samples;

    math::vec3f m_new_bias;
    math::vec3f m_new_scale;
    math::vec3f m_received_bias;
    math::vec3f m_received_scale;

    uint8_t m_collect_data_step = 0;
    std::array<math::vec3<double>, 6> m_averages;
};
