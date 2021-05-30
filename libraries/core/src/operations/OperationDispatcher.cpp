#include "operations/OperationDispatcher.h"

#include <iostream>
#include "Tracy.hpp"

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
	
	_threadIds[where] = std::move(thread);
}

void OperationDispatcher::Dispath(std::weak_ptr<Operation> operation, OperationThreadIds where)
{
	if (auto thread = GetThread(where))
	{
		thread->Dispatch(operation);
		return;
	}
	std::cerr << "Cannot dispatch specified operation: thread id " << static_cast<int>(where) << " is unknown!" << std::endl;
}

void OperationDispatcher::UpdateMainThread()
{
	ZoneScopedN("Update Main Thread");
	auto mainThread = dynamic_cast<OperationMainThread *>(GetThread(OperationThreadIds::MainThread));
	if (mainThread)
	{
		mainThread->Update();
	}
}

OperationThreadBase* OperationDispatcher::GetThread(OperationThreadIds ids) const
{
	auto it = _threadIds.find(ids);
	if (it != _threadIds.end())
	{
		return it->second.get();
	}
	return nullptr;
}
