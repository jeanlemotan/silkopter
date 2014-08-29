#pragma once

#include "Types.h"
#include "util/Noncopyable.h"

namespace q
{
namespace thread
{
	class Task;
	DECLARE_CLASS_PTR(Task);

	enum class Type : uint8_t
	{
		MAIN = 0, //always first
		RENDERING,
		STREAMING,
		USER,
	};

	class Task : util::Noncopyable
	{
		friend class Scheduler;
		friend class Worker_Thread;
	public:
        virtual ~Task() {}
		virtual void	run() = 0;
		virtual void	reset(); //call this to reset the task to it's initial state
		bool			is_finished() const;
		uint32_t		get_id() const;
	protected:
		Task();
		bool	m_is_finished;
		uint32_t		m_id;
	};

	class Function_Task : public Task
	{
	public:
		Function_Task(std::function<void()> const& func) : m_function(func) {}
		virtual void run() { m_function(); }

	private:
		std::function<void()> m_function;
	};

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	class Scheduler;
	class Worker_Thread;

	namespace detail
	{
		class Task_Data
		{
			friend class thread::Scheduler;
			friend class thread::Worker_Thread;
		public:
			Task_Data() : m_id(0) {}
			Task_Data(const Task_ptr& task) : m_id(task->get_id()), m_task(task) {}
			bool operator==(uint32_t id) const { return m_id == id; }
		protected:
			uint32_t	m_id;
			Task_ptr	m_task;
		};
	}
}
}
