#pragma once

namespace q
{
namespace util
{

class Fps
{
public:
	Fps(size_t fps_limit = 0);
    Fps(Fps const& other) = default;
    Fps& operator=(Fps const& other) = default;

	void	tick();
	float	get_fps() const;
	Clock::duration	get_frame_time() const;

private:
    int64_t		m_target_frame_time = 0;
	Clock::time_point		m_last_time;
	Clock::duration	m_frame_time;
    float	m_fps = 0;
};

inline Fps::Fps(size_t fps_limit /* = 0 */)
{
	m_last_time = Clock::now();
	m_frame_time = Clock::duration::zero();
	m_target_frame_time = fps_limit ? (1000000 / fps_limit) : 0;
}

inline void Fps::tick()
{
	Clock::time_point time;
	if (m_target_frame_time)
	{
		bool done = false;
		while (!done)
		{
			time = Clock::now();
			auto ft = std::chrono::duration_cast<std::chrono::microseconds>(time - m_last_time);

			int64_t wait_time = m_target_frame_time - ft.count();
			if (wait_time > 2000)
			{
				//print("\n{0}", waitTime);
				std::this_thread::sleep_for(std::chrono::microseconds(wait_time));
			}
			else if (wait_time > 0)
			{
				//print("\n{0}*", waitTime);
				std::this_thread::yield();
			}
			else
			{
				done = true;
			}
		}
	}
	else
	{
		time = Clock::now();
	}

	m_frame_time = time - m_last_time;
	m_fps = 1.f / std::chrono::duration_cast<Seconds>(m_frame_time).count();

	m_last_time = time;
}

inline float Fps::get_fps() const
{
	return m_fps;
}

inline Clock::duration Fps::get_frame_time() const
{
	return m_frame_time;
}

}
}
