#include "operations/OperationManager.h"


#include "operations/Operation.h"
#include "operations/OperationDispatcher.h"

OperationManager::OperationManager()
	: _dispatcher(std::make_unique<OperationDispatcher>())
{
	_dispatcher->RegisterThread(OperationThreadIds::MainThread, "OM_MainThread");
	_dispatcher->RegisterThread(OperationThreadIds::NetworkThread, "OM_NetworkThread");
}

OperationManager::~OperationManager() = default;

void OperationManager::Schedule(const std::weak_ptr<Operation> & operation, OperationThreadIds where)
{
	Schedule(std::move(operation), TimeState(), where);
}

void OperationManager::Schedule(const std::weak_ptr<Operation> & operation, TimeState delay, OperationThreadIds where)
{
	_operations.emplace_back(operation, delay, where);
}

void OperationManager::Update(TimeState elapsedTime)
{
	auto completedIt = std::remove_if(_dispatchedOperations.begin(), _dispatchedOperations.end(), [](const std::weak_ptr<Operation> & weakOperation)
	{
		auto dispatchedOperation = weakOperation.lock();
		if (!dispatchedOperation)
		{
			return true;
		}
		if (dispatchedOperation->Completed())
		{
			dispatchedOperation->OnCompleted();
			return true;
		}
		return false;
	});
	_dispatchedOperations.erase(completedIt, _dispatchedOperations.end());
	
	auto expiredIt = std::remove_if(_operations.begin(), _operations.end(), [](const OperationStatus & status)
	{
		return status.operation.expired();
	});
	
	auto deferredIt = std::stable_partition(_operations.begin(), expiredIt, [&elapsedTime](const OperationStatus & status)
	{
		return status.delay >= elapsedTime;
	});

	std::for_each(_operations.begin(), deferredIt, [&elapsedTime](OperationStatus & status)
	{
		status.delay -= elapsedTime;
	});

	std::for_each(deferredIt, _operations.end(), [this](const OperationStatus & status)
	{
		Dispatch(status);
	});

	_operations.erase(deferredIt, _operations.end());
}

void OperationManager::Dispatch(const OperationStatus& status)
{
	_dispatcher->Dispath(status.operation, status.where);
	_dispatchedOperations.emplace_back(status.operation);
}

