#include "QInputStdAfx.h"
#include "QInput.h"

#include "OIS/OISInputManager.h"
#include "OIS/OISException.h"
#include "OIS/OISJoyStick.h"
#include "OIS/OISKeyboard.h"
#include "OIS/OISMouse.h"
#include "OIS/OISEvents.h"

using namespace qinput;

class Input_Mgr::Event_Handler : /*public OIS::KeyListener, public OIS::MouseListener,*/ public OIS::JoyStickListener, q::util::Noncopyable
{
    friend class Input_Mgr;
protected:
    std::shared_ptr<OIS::InputManager> m_ois;
    Input_Mgr& m_input_mgr;
    std::vector<std::shared_ptr<OIS::JoyStick>> m_joys;

    void update()
    {
        for (auto& j: m_joys)
        {
            j->capture();
        }
    }

public:
    Event_Handler(Input_Mgr& input_mgr)
        : m_input_mgr(input_mgr)
    {

    }

    bool buttonPressed(const OIS::JoyStickEvent &arg, int button)
    {
        //std::cout << std::endl << arg.device->vendor() << ". Button Pressed # " << button;
        Gamepad_Event event;
        event.gamepad_id = arg.device->getID();
        event.timestamp = q::Clock::now();
        event.type = Gamepad_Event::Type::BUTTON_PRESS;
        bool ok = true;
        switch (button)
        {
        case 3: event.button_id = Button_Id::OUYA_O; break;
        case 4: event.button_id = Button_Id::OUYA_U; break;
        case 5: event.button_id = Button_Id::OUYA_Y; break;
        case 6: event.button_id = Button_Id::OUYA_A; break;
        case 7: event.button_id = Button_Id::LEFT_BUMPER; break;
        case 8: event.button_id = Button_Id::RIGHT_BUMPER; break;
        case 9: event.button_id = Button_Id::LEFT_STICK; break;
        case 10: event.button_id = Button_Id::RIGHT_STICK; break;
        case 11: event.button_id = Button_Id::PAD_UP; break;
        case 12: event.button_id = Button_Id::PAD_DOWN; break;
        case 13: event.button_id = Button_Id::PAD_LEFT; break;
        case 14: event.button_id = Button_Id::PAD_RIGHT; break;
        default: ok = false; break;
        }
        if (ok)
        {
            m_input_mgr.add_event(event);
        }
        return true;
    }
    bool buttonReleased( const OIS::JoyStickEvent &arg, int button )
    {
        //std::cout << std::endl << arg.device->vendor() << ". Button Released # " << button;
        Gamepad_Event event;
        event.gamepad_id = arg.device->getID();
        event.timestamp = q::Clock::now();
        event.type = Gamepad_Event::Type::BUTTON_RELEASE;
        bool ok = true;
        switch (button)
        {
        case 3: event.button_id = Button_Id::OUYA_O; break;
        case 4: event.button_id = Button_Id::OUYA_U; break;
        case 5: event.button_id = Button_Id::OUYA_Y; break;
        case 6: event.button_id = Button_Id::OUYA_A; break;
        case 7: event.button_id = Button_Id::LEFT_BUMPER; break;
        case 8: event.button_id = Button_Id::RIGHT_BUMPER; break;
        case 9: event.button_id = Button_Id::LEFT_STICK; break;
        case 10: event.button_id = Button_Id::RIGHT_STICK; break;
        case 11: event.button_id = Button_Id::PAD_UP; break;
        case 12: event.button_id = Button_Id::PAD_DOWN; break;
        case 13: event.button_id = Button_Id::PAD_LEFT; break;
        case 14: event.button_id = Button_Id::PAD_RIGHT; break;
        default: ok = false; break;
        }
        if (ok)
        {
            m_input_mgr.add_event(event);
        }
        return true;
    }
    bool axisMoved( const OIS::JoyStickEvent &arg, int axis )
    {
        //Provide a little dead zone
        if( arg.state.mAxes[axis].abs > 2500 || arg.state.mAxes[axis].abs < -2500 )
        {
            //std::cout << std::endl << arg.device->vendor() << ". Axis # " << axis << " Value: " << arg.state.mAxes[axis].abs;
        }

        Gamepad_Event event;
        event.gamepad_id = arg.device->getID();
        event.timestamp = q::Clock::now();
        if (axis == 0 || axis == 1)
        {
            event.type = Gamepad_Event::Type::STICK_CHANGE;
            event.stick_id = Stick_Id::LEFT;
            event.stick_value.set(arg.state.mAxes[0].abs / 32768.f, arg.state.mAxes[1].abs / 32768.f);
        }
        else if (axis == 3 || axis == 4)
        {
            event.type = Gamepad_Event::Type::STICK_CHANGE;
            event.stick_id = Stick_Id::RIGHT;
            event.stick_value.set(arg.state.mAxes[3].abs / 32768.f, arg.state.mAxes[4].abs / 32768.f);
        }
        else if (axis == 2)
        {
            event.type = Gamepad_Event::Type::TRIGGER_CHANGE;
            event.trigger_id = Trigger_Id::LEFT;
            event.trigger_value = (arg.state.mAxes[axis].abs / 32768.f);
        }
        else if (axis == 5)
        {
            event.type = Gamepad_Event::Type::TRIGGER_CHANGE;
            event.trigger_id = Trigger_Id::RIGHT;
            event.trigger_value = (arg.state.mAxes[axis].abs / 32768.f);
        }
        m_input_mgr.add_event(event);

        return true;
    }
};

Input_Mgr::Input_Mgr(q::String const& window_handle)
	: m_last_gamepad_id(0)
{
    m_event_handler.reset(new Event_Handler(*this));


    OIS::ParamList pl;

    pl.insert(std::make_pair( std::string("WINDOW"), window_handle.c_str() ));

    //This never returns null.. it will raise an exception on errors
    m_event_handler->m_ois.reset(OIS::InputManager::createInputSystem(pl), OIS::InputManager::destroyInputSystem);

    //Lets enable all addons that were compiled in:
    m_event_handler->m_ois->enableAddOnFactory(OIS::InputManager::AddOn_All);

    //Print debugging information
    uint32_t v = m_event_handler->m_ois->getVersionNumber();
    QLOG_INFO("Input_Mgr", "OIS Version: {}\nRelease Name: {}.{}.{}\nManager: {}\nTotal Keyboards: {}\nTotal Mice: {}\nTotal JoySticks: {}",
        (v>>16 ), ((v>>8) & 0x000000FF), (v & 0x000000FF),
        m_event_handler->m_ois->getVersionName(),
        m_event_handler->m_ois->inputSystemName(),
        m_event_handler->m_ois->getNumberOfDevices(OIS::OISKeyboard),
        m_event_handler->m_ois->getNumberOfDevices(OIS::OISMouse),
        m_event_handler->m_ois->getNumberOfDevices(OIS::OISJoyStick));

    //List all devices
    OIS::DeviceList list = m_event_handler->m_ois->listFreeDevices();

// 	g_kb = (Keyboard*)sInputManager->createInputObject( OISKeyboard, true );
// 	g_kb->setEventCallback( &handler );
//
// 	g_m = (Mouse*)sInputManager->createInputObject( OISMouse, true );
// 	g_m->setEventCallback( &handler );
// 	const MouseState &ms = g_m->getMouseState();
// 	ms.width = 100;
// 	ms.height = 100;

    try
    {
        //This demo uses at most 4 joysticks - use old way to create (i.e. disregard vendor)
        int numSticks = std::min(m_event_handler->m_ois->getNumberOfDevices(OIS::OISJoyStick), 4);
        for (int i = 0; i < numSticks; ++i)
        {
            m_event_handler->m_joys.emplace_back(
                (OIS::JoyStick*)m_event_handler->m_ois->createInputObject(OIS::OISJoyStick, true),  //constructor
                [&](OIS::JoyStick* joy){ m_event_handler->m_ois->destroyInputObject(joy); } //destructor
            );

            auto& joy = *m_event_handler->m_joys.back();
            joy.setEventCallback(m_event_handler.get());
            QLOG_INFO("Input_Mgr", "Creating Joystick {}\n\tAxes: {}\n\tSliders: {}\n\tPOV/HATs: {}\n\tButtons: {}\n\tVector3: {}\n\tBuffered: {}",
                    (i + 1),
                    joy.getNumberOfComponents(OIS::OIS_Axis),
                    joy.getNumberOfComponents(OIS::OIS_Slider),
                    joy.getNumberOfComponents(OIS::OIS_POV),
                    joy.getNumberOfComponents(OIS::OIS_Button),
                    joy.getNumberOfComponents(OIS::OIS_Vector3),
                    joy.buffered());

            Gamepad_Event event;
            event.gamepad_id = joy.getID();
            event.timestamp = q::Clock::now();
            event.type = Gamepad_Event::Type::CONNECT;
            add_event(event);
        }
    }
    catch(OIS::Exception &ex)
    {
        std::cout << "\nException raised on joystick creation: " << ex.eText << std::endl;
    }
}

Touchscreen const& Input_Mgr::get_touchscreen() const
{
	return m_touchscreen;
}
Keyboard const& Input_Mgr::get_keybard() const
{
	return m_keyboard;
}
Sensors const& Input_Mgr::get_sensors() const
{
	return m_sensors;
}

std::vector<Gamepad_cptr> Input_Mgr::get_all_gamepads() const
{
	std::vector<Gamepad_cptr> gamepads;
	gamepads.reserve(m_gamepads.size());
	for (auto const& g: m_gamepads)
	{
		gamepads.emplace_back(g);
	}						  
	return gamepads;
}
Gamepad_cptr Input_Mgr::find_gamepad_by_id(uint32_t id) const
{
	auto it = std::find_if(m_gamepads.begin(), m_gamepads.end(), [&](Gamepad_ptr const& g) { return g->get_id() == id; });
	if (it != m_gamepads.end())
	{
		return *it;
	}
	return Gamepad_cptr();
}

void Input_Mgr::remove_disconnected_gamepads()
{
	m_gamepads.erase(std::remove_if(m_gamepads.begin(), m_gamepads.end(), [](Gamepad_ptr const& g)
	{ 
		return !g->is_connected(); 
	}), m_gamepads.end());
}

void Input_Mgr::update_gamepads(q::Clock::duration dt)
{
	for (auto const& g: m_gamepads)
	{
		g->update(dt);
	}
}

void Input_Mgr::connect_new_gamepads(q::Clock::duration dt)
{
	std::lock_guard<std::mutex> lock(m_new_gamepad_event_mutex);
	std::vector<Gamepad_ptr> new_gamepads;
	for (auto const& event: m_new_gamepad_events)
	{
		if (event.type == Gamepad_Event::Type::CONNECT)
		{
			Gamepad_ptr g = std::make_shared<Gamepad>(event.gamepad_id);
			m_gamepads.emplace_back(g);
			g->add_event(event);
			new_gamepads.emplace_back(g);
			continue;
		}

		auto it = std::find_if(m_gamepads.begin(), m_gamepads.end(), [&](Gamepad_ptr const& g) { return g->get_id() == event.gamepad_id; });
		if (it != m_gamepads.end())
		{
			(*it)->add_event(event);
		}
		else
		{
			QLOG_WARNING("Q", "Ignored event for gamepad {0}: {1}", event.gamepad_id, (int)event.type); //ignored event
		}
    };

	//update all new gamepads
	for (auto const g: new_gamepads)
	{
		g->update(dt);
	}
	m_new_gamepad_events.clear();
}

void Input_Mgr::update(q::Clock::duration dt)
{
    m_event_handler->update();

	m_touchscreen.update(dt);
	m_keyboard.update(dt);
	m_sensors.update(dt);

	remove_disconnected_gamepads();
	update_gamepads(dt);
	connect_new_gamepads(dt);
}

//////////////////////////////////////////////////////////////////////////
// Methods called from the platform code

void Input_Mgr::add_event(Touchscreen_Event const& event)
{
	m_touchscreen.add_event(event);
}
void Input_Mgr::add_event(Keyboard_Event const& event)
{
	m_keyboard.add_event(event);
}
void Input_Mgr::add_event(Sensor_Event const& event)
{
	m_sensors.add_event(event);
}
void Input_Mgr::add_event(Gamepad_Event const& event)
{
	auto it = std::find_if(m_gamepads.begin(), m_gamepads.end(), [&](Gamepad_ptr const& g) { return g->get_id() == event.gamepad_id; });
	if (it != m_gamepads.end())
	{
		(*it)->add_event(event);
	}
	else
	{
		std::lock_guard<std::mutex> lock(m_new_gamepad_event_mutex);
		m_new_gamepad_events.emplace_back(event);
	}
}
