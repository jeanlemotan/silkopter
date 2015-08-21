#pragma once

#include <QDialog>

#include "sz_math.hpp"
#include "sz_Calibration_Data.hpp"

#include "HAL.h"
#include "Comms.h"

class Magnetic_Field_Calibration_Wizard : public QDialog
{
public:
    Magnetic_Field_Calibration_Wizard(silk::HAL& hal, silk::Comms& comms, silk::node::Node_ptr node, size_t output_idx, QWidget* parent = 0);

private:
    void advance();
    void cancel();

    void prepare_step();

    void on_samples_received(silk::node::stream::Magnetic_Field::Samples const& samples);

    void set_calibration_points(sz::calibration::Magnetic_Field_Points const& data);
    auto get_calibration_points() const -> sz::calibration::Magnetic_Field_Points;

    silk::HAL& m_hal;
    silk::Comms& m_comms;
    silk::node::Node_ptr m_node;
    silk::node::Node::Output m_output;
    silk::node::stream::Magnetic_Field_ptr m_stream;

    QWidget* m_content = nullptr;

    sz::calibration::Magnetic_Field_Points m_initial_calibration;
    sz::calibration::Magnetic_Field_Points m_crt_calibration;


    enum class Step
    {
        RESET,
        SHOW_INSTRUCTIONS,
        COLLECT,
        DONE,
    };

    Step m_step = Step::RESET;

    q::util::Connection m_connection;

    std::vector<math::vec3f> m_samples;
    math::aabb3f m_box;
};
