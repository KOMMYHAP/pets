#include "operations/OperationMainThread.h"

#include <algorithm>


#include "operations/Operation.h"
#include "tools/Utils.h"

OperationMainThread::OperationMainThread()
	: _id(std::this_thread::get_id())
{
}

void OperationMainThread::SetName(const std::string& name)
{
	Utils::SetThreadName(name);
}

void OperationMainThread::Dispatch(std::weak_ptr<Operation> weakOperation)
{
	if (auto operation = weakOperation.lock())
	{
		operation->PreAction();
		_operations.emplace_back(weakOperation);
	}
}

std::thread::id OperationMainThread::GetId() const
{
	return _id;
}

void OperationMainThread::Update()
{
	auto completedIt = std::remove_if(_operations.begin(), _operations.end(), [](const std::weak_ptr<Operation> & weakoperation)
	{
		auto operation = weakoperation.lock();
		if (!operation)
		{
			return true;
		}
		auto result = operation->Do();
		if (result == Operation::Result::Completed)
		{
			operation->PostAction();
			return true;
		}
		return false;
	});
	_operations.erase(completedIt, _operations.end());
}
