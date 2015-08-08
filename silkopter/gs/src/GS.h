#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GS.h"
#include "qcustomplot.h"
#include "Comms.h"
#include "Render_Widget.h"

class Multi_Config_Window;
class HAL_Window;
class Sim_Window;
class IHUD_Widget;

class GS : public QMainWindow
{
public:
	GS(QWidget *parent = 0);
	~GS();

private:
	void process();

    void closeEvent(QCloseEvent* event);
    void read_settings();

    void init_graphics();

    std::shared_ptr<qinput::Input_Mgr> m_input_mgr;

    q::Clock::time_point m_last_tp;
    //q::Clock::duration m_duration;

    Ui::GS m_ui;

    silk::HAL m_hal;
    silk::Comms m_comms;

    Render_Context m_context;

    HAL_Window* m_hal_window = nullptr;
    Multi_Config_Window* m_multi_config_window = nullptr;
    IHUD_Widget* m_hud = nullptr;

    void set_remote_address(std::string const& address);
    std::string m_remote_address;
};


