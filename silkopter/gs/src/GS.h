#pragma once

#include <QtWidgets/QMainWindow>
//#include "ui_GS.h"
#include "qcustomplot.h"
#include "Comms.h"

class HAL_Window;

class GS : public QMainWindow
{
public:
	GS(QWidget *parent = 0);
	~GS();

private:
	void process();

    void closeEvent(QCloseEvent* event);
    void read_settings();

	boost::asio::io_service m_io_service;
    std::atomic<bool> m_stop_io_service_thread{ false };
	std::thread m_io_service_thread;

    q::Clock::time_point m_last_time;
    //q::Clock::duration m_duration;

    silk::HAL m_hal;
    silk::Comms m_comms;

    HAL_Window* m_hal_window = nullptr;

    void set_remote_address(std::string const& address);
    std::string m_remote_address;
};


