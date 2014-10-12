#pragma once

#include "ui_Sensors.h"
#include "Comms.h"

class Sensors : public QWidget
{
public:
	Sensors(QWidget *parent = 0);
	~Sensors();

    void init(silk::Comms* comms);

public:
	void process();

private:

	void set_stream_messages();

    void update_calibration();
	void start_accelerometer_calibration();
    void update_accelerometer_calibration();

    void start_gyroscope_calibration();
    void update_gyroscope_calibration();

    void start_compass_calibration();
    void update_compass_calibration();

    boost::optional<math::vec3f> collect_vec3f_sample(math::vec3f const& data);

	//code copied from arducopter
    bool calibrate_accelerometer(std::array<math::vec3<double>, 6> const& samples, math::vec3<double>& bias, math::vec3<double>& scale);
	void calibrate_update_matrices(double dS[6], double JS[6][6], double beta[6], double data[3]);
	void calibrate_reset_matrices(double dS[6], double JS[6][6]);
	void calibrate_find_delta(double dS[6], double JS[6][6], double delta[6]);


	Ui::Sensors m_ui;

    q::Clock::time_point m_last_time;
    float m_plot_x_value = 0;

    silk::Comms* m_comms;

    float m_average_sonar = 0;
    float m_average_barometer = 0;
    float m_average_thermometer = 0;

    struct Calibration
    {
        enum class Sensor
        {
            ACCELEROMETER,
            GYROSCOPE,
            COMPASS
        };

        enum class Step
        {
            IDLE,
            START,
            RESET,
            COLLECT_DATA,
            SHOW_INSTRUCTIONS,
            SET,
        };

        Sensor sensor = Sensor::ACCELEROMETER;
        Step step = Step::IDLE;

        q::Clock::time_point step_timepoint;
        uint32_t last_sample_idx = 0;

        std::vector<math::vec3f> samples;
        q::Clock::duration last_ping_time;

        math::vec3f new_bias;
        math::vec3f new_scale;

        uint8_t collect_data_step = 0;
        std::array<math::vec3<double>, 6> averages;

        QMessageBox message_box;

        q::util::Connection connection;
        bool is_received_data_valid = false;
        math::vec3f received_bias;
        math::vec3f received_scale;

    } m_calibration;

    static void remove_plot_data_before(QCustomPlot& plot, q::Clock::duration length);
    static void clear_plot(QCustomPlot& plot);
    static void add_plot_sample(QCustomPlot& plot, float key, math::vec3f const& sample);
    static void add_plot_sample(QCustomPlot& plot, float key, float sample);
};
