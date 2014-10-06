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

    q::Clock::duration m_last_uav_alive_duration;
	bool m_stream_messages_set = false;
    q::Clock::time_point m_last_time;
    silk::Comms* m_comms;

    struct FFT_Data
    {
        size_t sample_rate = 0;
        std::vector<math::vec3f> input;
        std::vector<math::vec3f> filtered_input;

        std::vector<math::vec3f> output;
        std::vector<math::vec3f> filtered_output;

        static const size_t MAX_INPUT_SIZE = 65536;
        std::shared_ptr<double> temp_input;
        std::shared_ptr<fftw_complex> temp_output;
        fftw_plan plan;
        size_t plan_sample_count = 0;
    };

    void process_fft(FFT_Data& fft);
    float m_gyroscope_sample_time = 0;
    FFT_Data m_gyroscope_fft;

    void process_accelerometer_fft(FFT_Data& fft);
    float m_accelerometer_sample_time = 0;
    FFT_Data m_accelerometer_fft;

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
        q::Clock::time_point last_data_timepoint;

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

    struct History
    {
        size_t last_file_idx = 0;

        //the float is the time in seconds
        std::vector<std::pair<float, math::vec3f>> gyroscope_samples;
        std::vector<std::pair<float, math::vec3f>> gyroscope_filtered_samples;

        std::vector<std::pair<float, math::vec3f>> accelerometer_samples;
        std::vector<std::pair<float, math::vec3f>> accelerometer_filtered_samples;

        std::vector<std::pair<float, math::vec3f>> compass_samples;
        std::vector<std::pair<float, math::vec3f>> compass_filtered_samples;
    } m_history;

    struct History_Playback : History
    {
        bool is_playing = false;
        bool is_loaded = false;
        float time = 0;
        size_t last_gyroscope_idx = 0;
        size_t last_accelerometer_idx = 0;
        size_t last_compass_idx = 0;
    } m_playback;

    void dump_history_to_file();
    void load_history_from_file(q::Path const& filepath);
    void load_history();
    void clear_history();
    void rewind();

    static void remove_plot_data_before(QCustomPlot& plot, q::Clock::duration length);
    static void clear_plot(QCustomPlot& plot);
    static void add_plot_sample(QCustomPlot& plot, float key, math::vec3f const& sample);
    static void add_plot_filtered_sample(QCustomPlot& plot, float key, math::vec3f const& sample);
    static void prepare_fft(QCustomPlot& plot, std::vector<std::pair<float, math::vec3f>> const& samples, std::vector<math::vec3f>& output);
    static void display_fft(QCustomPlot& plot, FFT_Data const& fft);
    static void display_filtered_fft(QCustomPlot& plot, FFT_Data const& fft);
};
