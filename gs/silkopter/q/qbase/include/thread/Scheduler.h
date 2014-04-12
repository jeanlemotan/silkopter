#pragma once

#include "util/Noncopyable.h"
#include "thread/Task.h"

namespace q
{
namespace thread
{
	class Thread;

	class Scheduler : util::Noncopyable
	{
		friend class Thread;
	public:
		Scheduler(size_t thread_count = 1);
		~Scheduler();

		typedef uint32_t Handle;

		//Adds a new task. 
		Handle	add_task(const Task_ptr& task);

		//Cancels the task.
		//If it's pending, it will not be executed.
		//If it's running, it will finish.
		//If it's finished, it will be deleted
		void	cancel_task(Handle id);

		//Waits until the task is finished.
		void	finish_task(Handle id);

		//Cancels all tasks and returns instantly.
		//The runnining task will finish though, but none other
		//Any running task will stop, but all tasks (finished, running or pending) will be deleted
		void	cancel_all_tasks();

		//Waits for all tasks to finish.
		//Any running task will stop, but all tasks (finished, running or pending) will be deleted
		void	finish_all_tasks();

		//returns true if the task id is wither pending or running
		bool	has_tasks(Handle id) const;

		//returns true if there are tasks pending or running
		bool	has_tasks() const;
	protected:
		void	init();

	private:
		Task_ptr		find_task_by_id(Handle id) const;

		std::vector<std::unique_ptr<Worker_Thread>> m_threads;

		std::deque<detail::Task_Data> m_pending_tasks;
		mutable std::mutex m_pending_tasks_mutex;

		Handle				m_last_id;
	};

}
}
