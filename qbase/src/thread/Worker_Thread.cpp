#include "QBaseStdAfx.h"
#include "QBase.h"
#include "thread/Worker_Thread.h"


using namespace q;
using namespace thread;

static void ThreadFunc(void* arg)
{
	Worker_Thread* w = (Worker_Thread*)arg;
	QASSERT(w);

    //std::thread::id id = std::this_thread::get_id();

	while (w->is_started())
	{
		auto const& task = w->get_task();
		if (task)
		{
			//QLOGI("Thread {0} got a new task", idx);
            //print("thread {0} active, task {1}", w->getThreadId(), task->GetTaskId());
			task->run();
			w->finish_task();
		}
		else
		{
			w->wait_for_new_task();
		}
	}
}

Worker_Thread::Worker_Thread(std::deque<thread::detail::Task_Data>* pendingTasks, std::mutex* pendingMutex)
	: m_pending_tasks(pendingTasks)
	, m_pending_mutex(pendingMutex)
	, m_is_started(false)
{
}

Worker_Thread::~Worker_Thread()
{
	stop();
}

void Worker_Thread::start()
{
	if (!m_thread)
	{
		m_thread.reset(new std::thread(std::bind(ThreadFunc, this)));
	}
	m_is_started = true;
}

void Worker_Thread::stop()
{
	if (m_thread)
	{
		m_is_started = false;

		m_task_semaphore.notify_all();

 		if (m_thread->joinable())
		{
			m_thread->join();
		}

		m_thread.reset();
	}
}

void Worker_Thread::notify_task_available()
{
	if (m_thread)
	{
		m_task_semaphore.notify_all();
	}
}

void Worker_Thread::wait_for_new_task()
{
	QASSERT(!m_task_data.m_task);

	std::unique_lock<std::mutex> ul(*m_pending_mutex);

	{
		if (!m_pending_tasks->empty())
		{
			m_task_data = m_pending_tasks->front();
			m_pending_tasks->pop_front();
		}
		else
		{
			m_task_data = detail::Task_Data();
			//no task available? then just go to sleep.
			//the scheduler will notify us
			if (m_thread != nullptr)
			{
				m_task_semaphore.wait(ul);
			}
		}
	}
}

void Worker_Thread::finish_task()
{
	if (m_task_data.m_task)
	{
		m_task_data.m_task->m_is_finished = true;
	}

	m_task_data = detail::Task_Data();
}

bool Worker_Thread::has_any_tasks() const
{
	return m_task_data.m_id != 0;
}

bool Worker_Thread::has_task(uint32_t id) const
{
	return m_task_data.m_id == id;
}
