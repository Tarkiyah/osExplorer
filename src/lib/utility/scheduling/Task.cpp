#include "utility/scheduling/Task.h"

#include "utility/logging/logging.h"
#include "utility/scheduling/TaskScheduler.h"

void Task::dispatch(std::shared_ptr<Task> task)
{
	TaskScheduler::getInstance()->pushTask(task);
}

Task::Task()
	: m_state(STATE_NEW)
{
}

Task::~Task()
{
}

Task::TaskState Task::getState() const
{
	return m_state;
}

Task::TaskState Task::process(bool interruptTask)
{
	if (interruptTask)
	{
		switch (m_state)
		{
		case STATE_NEW:
		case STATE_CANCELED:
			break;
		case STATE_RUNNING:
			interrupt();
			exit();
			break;
		case STATE_FINISHED:
			revert();
			break;
		}

		setState(STATE_CANCELED);
	}
	else
	{
		switch (m_state)
		{
		case STATE_NEW:
		case STATE_CANCELED:
			enter();
		case STATE_RUNNING:
			{
				TaskState newState = update();
				if (newState == STATE_NEW || newState == STATE_CANCELED)
				{
					LOG_ERROR("Task can't change to state NEW or CANCELLED");
					return m_state;
				}

				setState(newState);
				if (m_state == STATE_FINISHED)
				{
					exit();
				}
			}
			break;
		case STATE_FINISHED:
			break;
		}
	}

	return m_state;
}

void Task::execute()
{
	TaskState state;
	do
	{
		state = process(false);
	}
	while (state != STATE_FINISHED);
}

void Task::setState(TaskState state)
{
	m_state = state;
}