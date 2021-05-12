#include "operations/Operation.h"

void Operation::PreAction()
{
	try
	{
		_state = State::Started;
		PreActionImpl();
	}
	catch (const std::exception &)
	{
		_exception = std::current_exception();
	}
}

Operation::Result Operation::Do()
{
	try
	{
		if (Good())
		{
			return DoImpl();
		}
	}
	catch (const std::exception &)
	{
		_exception = std::current_exception();
	}

	// fallback in error case
	return Result::Completed;
}

void Operation::PostAction()
{
	try
	{
		if (Good())
		{
			PostActionImpl();
		}
	}
	catch (const std::exception &)
	{
		_exception = std::current_exception();
	}
	_state = State::Completed;
}

void Operation::OnCompleted()
{
	OnCompleted(_exception);
}

void Operation::OnCompleted(std::exception_ptr exception)
{
	if (exception)
	{
		std::rethrow_exception(exception);
	}
}
