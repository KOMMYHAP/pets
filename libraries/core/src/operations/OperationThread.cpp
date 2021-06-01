#include "operations/OperationThread.h"


#include <algorithm>
#include <iostream>

#include "operations/Operation.h"
#include "tools/Utils.h"

OperationThread::OperationThread()
	: _state(std::make_shared<State>())
{
	_state->stopped = false;
	std::weak_ptr<State> weakState = _state;
	_thread = std::thread(ThreadFunc, weakState);
}

OperationThread::~OperationThread()
{
	_state->stopped = true;
	_state->condition.notify_one();
	_state.reset();

	_thread.detach();
}

void OperationThread::SetName(const std::string& name)
{
	std::scoped_lock lock(_state->mutex);
	_state->changedName = name;
}

void OperationThread::Dispatch(std::weak_ptr<Operation> operation)
{
	std::unique_lock lock(_state->mutex);
	_state->operations.emplace_back(operation);
	_state->condition.notify_one();
}

std::thread::id OperationThread::GetId() const
{
	return _thread.get_id();
}

void OperationThread::ThreadFunc(std::weak_ptr<State> weakState)
{
	auto state = weakState.lock();
	if (!state)
	{
		return;
	}

	std::vector<std::weak_ptr<Operation>> operationsInProcess;

	while (!state->stopped)
	{
		decltype(state->operations) operationsToStart;
		{
			std::unique_lock lock(state->mutex);
			if (state->changedName)
			{
				Utils::SetThreadName(*state->changedName);
				state->changedName.reset();
			}
			if (state->operations.empty() && operationsInProcess.empty())
			{
				state->condition.wait(lock);
			}
			operationsToStart = std::move(state->operations);
		}

		for (const auto & weakOperation : operationsToStart)
		{
			auto operation = weakOperation.lock();
			if (operation)
			{
				operation->PreAction();
				operationsInProcess.emplace_back(weakOperation);
			}
		}

		auto completedIt = std::remove_if(operationsInProcess.begin(), operationsInProcess.end(), [](const std::weak_ptr<Operation> & weakOperation)
		{
			auto operation = weakOperation.lock();
			if (operation)
			{
				auto result = operation->Do();
				bool completed = result == Operation::Result::Completed;
				if (completed)
				{
					operation->PostAction();
				}
				return completed;
			}
			return true;
		});

		operationsInProcess.erase(completedIt, operationsInProcess.end());
	}
}
