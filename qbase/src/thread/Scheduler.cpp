#include "QBaseStdAfx.h"
#include "QBase.h"
#include "thread/Worker_Thread.h"

using namespace q;
using namespace thread;

//////////////////////////////////////////////////////////////////////////

Scheduler::Scheduler(size_t threadCount)
	: m_last_id(0)
{
	for (size_t i = 0; i < threadCount; i++)
	{
		m_threads.emplace_back(new Worker_Thread(&m_pending_tasks, &m_pending_tasks_mutex));
	}
}

//////////////////////////////////////////////////////////////////////////

void Scheduler::init()
{
}

//////////////////////////////////////////////////////////////////////////

Scheduler::~Scheduler()
{
	m_threads.clear();
	m_pending_tasks.clear();
}

auto Scheduler::add_task(const Task_ptr& task) -> Handle
{
	QASSERT(task);
	if (!task)
	{
		return 0;
	}

	task->m_is_finished = false;
	task->m_id = m_last_id++;

	if (m_threads.empty())
	{
		task->run();
		return task->m_id;
	}

	detail::Task_Data td(task);

	{
		std::lock_guard<std::mutex> sm(m_pending_tasks_mutex);
		m_pending_tasks.push_back(td);
	}

	for (auto const& thread: m_threads)
	{
		thread->start();
		thread->notify_task_available();
	}

    //printf(" New task - %d.", td.mId);
	return task->get_id();
}

bool Scheduler::has_tasks() const
{
	{
		std::lock_guard<std::mutex> sm(m_pending_tasks_mutex);
		if (!m_pending_tasks.empty())
		{
			return true;
		}
	}
	for (auto const& thread: m_threads)
	{
		if (thread->is_started() && thread->m_task_data.m_id > 0)
		{
			return true;
		}
	}
	return false;
}

bool Scheduler::has_tasks(Handle id) const
{
	if (id == 0)
	{
		return false;
	}

	{
		std::lock_guard<std::mutex> sm(m_pending_tasks_mutex);
		if (std::find(m_pending_tasks.begin(), m_pending_tasks.end(), id) != m_pending_tasks.end())
		{
			return true;
		}
	}

	for (auto const& thread: m_threads)
	{
		if (thread->m_task_data.m_id == id)
		{
			return true;
		}
	}

	return false;
}

//Cancels the task.
//If it's pending, it will not be executed.
//If it's running, it will finish.
//If it's finished, it will be deleted
void Scheduler::cancel_task(Handle id)
{
	if (id == 0)
	{
		return;
	}

	{
		std::lock_guard<std::mutex> sm(m_pending_tasks_mutex);
		m_pending_tasks.erase(std::remove(m_pending_tasks.begin(), m_pending_tasks.end(), id), m_pending_tasks.end());
	}
}

//Cancels all tasks and returns instantly.
//The running task will finish though, but none other
//Any running task will stop, but all tasks (finished, running or pending) will be deleted
void Scheduler::cancel_all_tasks()
{
	{									
		std::lock_guard<std::mutex> sm(m_pending_tasks_mutex);
		m_pending_tasks.clear();
	}
}

void Scheduler::finish_task(Handle id)
{
	while (has_tasks(id))
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

//Waits for all tasks to finish.
//Any running task will stop, but all tasks (finished, running or pending) will be deleted
void Scheduler::finish_all_tasks()
{
	while (has_tasks())
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

