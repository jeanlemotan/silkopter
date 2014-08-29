#include "QBaseStdAfx.h"
#include "QBase.h"

using namespace q;
using namespace thread;

Task::Task() 
	: m_is_finished(false)
	, m_id(0)
{
	;
}
void Task::reset()
{
	m_is_finished = false;
}
bool Task::is_finished() const
{
	return m_is_finished;
}
uint32_t Task::get_id() const
{
	return m_id;
}
