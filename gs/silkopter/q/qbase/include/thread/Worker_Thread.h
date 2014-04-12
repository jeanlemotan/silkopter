#pragma once

#include "Task.h"
#include "util/Noncopyable.h"

namespace q
{
namespace thread
{
	class Worker_Thread : util::Noncopyable
	{
		friend class Scheduler;
	public:
		Worker_Thread(std::deque<detail::Task_Data>* pending_tasks, std::mutex* pending_mutex);
		~Worker_Thread();

		bool	is_started() const;
		bool	has_any_tasks() const;
		bool	has_task(uint32_t id) const;

	//used internally - do not call directly
		void	notify_task_available();
		Task_ptr const&	get_task();
		void	finish_task();
		void	wait_for_new_task();

		std::thread::id	get_thread_id() const;

	protected:
		void	start();
		void	stop();
		void	run(uint32_t time);

	private:
		detail::Task_Data	m_task_data;
		bool		m_is_started;
		std::unique_ptr<std::thread> m_thread;

		mutable std::condition_variable m_task_semaphore;
		std::deque<detail::Task_Data>* m_pending_tasks;
		std::mutex*	m_pending_mutex;
	};

	inline std::thread::id Worker_Thread::get_thread_id() const
	{
		return m_thread->get_id();
	}

	inline bool Worker_Thread::is_started() const
	{
		return m_is_started;
	}
	inline Task_ptr const& Worker_Thread::get_task()
	{
		return m_task_data.m_task;
	}

}
}