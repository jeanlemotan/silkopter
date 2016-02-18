#pragma once

#include <QDialog>

#include "sz_math.hpp"
#include "sz_Calibration_Data.hpp"

#include "HAL.h"
#include "Comms.h"

class Acceleration_Calibration_Wizard : public QDialog
{
public:
    Acceleration_Calibration_Wizard(silk::HAL& hal, silk::Comms& comms, silk::node::gs::Node_ptr node, size_t output_idx, QWidget* parent = 0);

private:
    void advance();
    void cancel();

    void prepare_step();

    void on_samples_received(silk::stream::gs::Acceleration::Samples const& samples);

    void set_calibration_points(sz::calibration::Acceleration_Points const& data);
    auto get_calibration_points() const -> sz::calibration::Acceleration_Points;

    auto compute_calibration_data(std::array<math::vec3<double>, 6> const& samples, math::vec3<double>& bias, math::vec3<double>& scale) -> bool;
    void calibrate_update_matrices(double dS[6], double JS[6][6], double beta[6], double data[3]);
    void calibrate_reset_matrices(double dS[6], double JS[6][6]);
    void calibrate_find_delta(double dS[6], double JS[6][6], double delta[6]);


    silk::HAL& m_hal;
    silk::Comms& m_comms;
    silk::node::gs::Node_ptr m_node;
    silk::node::gs::Node::Output m_output;
    silk::stream::gs::Acceleration_ptr m_stream;

    QWidget* m_content = nullptr;

    sz::calibration::Acceleration_Points m_initial_calibration;
    sz::calibration::Acceleration_Points m_crt_calibration;


    enum class Step
    {
        INTRO,
        RESET,
        SHOW_INSTRUCTIONS,
        COLLECT,
        DONE,
    };

    Step m_step = Step::INTRO;

    q::util::Scoped_Connection m_connection;

    std::vector<math::vec3f> m_samples;

    uint8_t m_collect_data_step = 0;
    std::array<math::vec3<double>, 6> m_averages;
};
