#pragma once

namespace q
{
namespace anim
{

	class Controller
	{
	public:
		void		set_interval(Duration start, Duration end);
		Duration	get_interval_start() const;
		Duration	get_interval_end() const;

		void		set_duration(Duration duration);

		//time ca be outside the interval - it's not clamped!!
		void		set_time(Duration time);
		Duration	get_time() const;

		void		rewind(); //convienience

		void		update(Duration dt);

	private:
	};

}
}