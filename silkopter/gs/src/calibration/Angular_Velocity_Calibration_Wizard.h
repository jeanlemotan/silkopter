#pragma once

#include <QDialog>

#include "Comms.h"

class Angular_Velocity_Calibration_Wizard : public QDialog
{
public:
    Angular_Velocity_Calibration_Wizard(silk::Comms& comms,
                                    std::string const& node_name,
                                    std::string const& stream_name,
                                    size_t stream_rate,
                                    std::shared_ptr<ts::IStruct_Value> config,
                                    std::shared_ptr<ts::IVector_Value> points,
                                    QWidget* parent = 0);
    ~Angular_Velocity_Calibration_Wizard();

private:
    void advance();
    void cancel();

    void prepare_step();

    void on_samples_received(std::vector<silk::stream::IAngular_Velocity::Sample> const& samples);

//    void set_calibration_points(sz::calibration::Angular_Velocity_Points const& data);
//    auto get_calibration_points() const -> sz::calibration::Angular_Velocity_Points;

    silk::Comms& m_comms;
    std::string m_node_name;
    std::string m_stream_name;
    size_t m_stream_rate = 0;
    std::shared_ptr<ts::IStruct_Value> m_config;
    std::shared_ptr<ts::IVector_Value> m_points;
    std::shared_ptr<ts::IVector_Value> m_initial_points;
    std::shared_ptr<ts::IVector_Value> m_new_points;

    QWidget* m_content = nullptr;

//    sz::calibration::Angular_Velocity_Points m_initial_calibration;
//    sz::calibration::Angular_Velocity_Points m_crt_calibration;


    enum class Step
    {
        RESET,
        SHOW_INSTRUCTIONS,
        COLLECT,
        SET_VALUE,
        DONE,
    };

    Step m_step = Step::RESET;

    q::util::Scoped_Connection m_connection;

    std::vector<math::vec3f> m_samples;
};
