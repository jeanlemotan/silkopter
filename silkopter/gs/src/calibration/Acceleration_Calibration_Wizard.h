#pragma once

#include <QDialog>

#include "Comms.h"

class Acceleration_Calibration_Wizard : public QDialog
{
public:
    Acceleration_Calibration_Wizard(silk::Comms& comms,
                                    std::string const& node_name,
                                    std::string const& stream_name,
                                    size_t stream_rate,
                                    std::shared_ptr<ts::IVector_Value> points,
                                    QWidget* parent = 0);
    ~Acceleration_Calibration_Wizard();

private:
    void advance();
    void cancel();

    void prepare_step();

    void on_samples_received(std::vector<silk::stream::IAcceleration::Sample> const& samples);

    //void set_calibration_points(sz::calibration::Acceleration_Points const& data);
    //auto get_calibration_points() const -> sz::calibration::Acceleration_Points;

    auto compute_calibration_data(std::array<math::vec3<double>, 6> const& samples, math::vec3<double>& bias, math::vec3<double>& scale) -> bool;
    void calibrate_update_matrices(double dS[6], double JS[6][6], double beta[6], double data[3]);
    void calibrate_reset_matrices(double dS[6], double JS[6][6]);
    void calibrate_find_delta(double dS[6], double JS[6][6], double delta[6]);


    silk::Comms& m_comms;
    std::string m_node_name;
    std::string m_stream_name;
    size_t m_stream_rate = 0;
    std::shared_ptr<ts::IVector_Value> m_initial_points;
    std::shared_ptr<ts::IVector_Value> m_crt_points;

    QWidget* m_content = nullptr;

//    sz::calibration::Acceleration_Points m_initial_calibration;
//    sz::calibration::Acceleration_Points m_crt_calibration;

    enum class Step
    {
        INTRO,
        RESET,
        SHOW_INSTRUCTIONS,
        COLLECT,
        SET_VALUE,
        DONE,
    };

    Step m_step = Step::INTRO;

    q::util::Scoped_Connection m_connection;

    std::vector<math::vec3f> m_samples;

    uint8_t m_collect_data_step = 0;
    std::array<math::vec3<double>, 6> m_averages;
};
