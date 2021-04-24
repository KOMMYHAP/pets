#include "operations/OperationDispatcher.h"

#include <iostream>

#include "operations/OperationMainThread.h"
#include "operations/OperationThread.h"

namespace
{
	std::unique_ptr<OperationThreadBase> CreateThread(bool mainThread)
	{
		if (!mainThread)
		{
			return std::make_unique<OperationThread>();
		}
		return std::make_unique<OperationMainThread>();
	}
}


OperationDispatcher::OperationDispatcher() = default;
OperationDispatcher::~OperationDispatcher() = default;

void OperationDispatcher::RegisterThread(OperationThreadIds where, const std::string& name)
{
	auto thread = CreateThread(where == OperationThreadIds::MainThread);
	thread->SetName(name);
	auto threadId = thread->GetId();
	
	_threadIds[where] = threadId;
	_threads[threadId] = std::move(thread);
}

void OperationDispatcher::Dispath(std::weak_ptr<Operation> operation, OperationThreadIds where)
{
	auto it = _threadIds.find(where);
	if (it != _threadIds.end())
	{
		auto threadId = it->second;
		auto threadIt = _threads.find(threadId);
		if (threadIt != _threads.end())
		{
			const auto & thread = threadIt->second;
			thread->Dispatch(operation);
			return;
		}
	}
	std::cerr << "Cannot dispatch specified operation: thread id " << static_cast<int>(where) << " is unknown!" << std::endl;
}
