#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GS.h"
#include "qcustomplot.h"
#include "Comms.h"

class GS : public QMainWindow
{
public:
	GS(QWidget *parent = 0);
	~GS();

private:
	void process();

    void closeEvent(QCloseEvent* event);
    void read_settings();

	Ui::GSClass m_ui;

	boost::asio::io_service m_io_service;
    std::atomic<bool> m_stop_io_service_thread{ false };
	std::thread m_io_service_thread;
	std::shared_ptr<Video_Client> m_video_client;

    q::Clock::time_point m_last_time;
    //q::Clock::duration m_duration;

    silk::Comms m_comms;

	math::vec3f m_accel;
	math::vec3f m_accel_rotation;

	math::vec3f m_gyro;
	math::vec3f m_gyro_offset;
	math::vec3f m_gyro_vector;
    //int m_gyro_calibrarion_samples;

	math::vec3f m_rotation;

    void set_uav_address(std::string const& address);
    std::string m_uav_address;

    std::unique_ptr<qinput::Input_Mgr> m_input_mgr;

    void setup_motors();

    //uint32_t m_last_time_ms;
};


