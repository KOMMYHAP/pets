#include "operations/OperationManager.h"
#include "Tracy.hpp"
#include "operations/Operation.h"
#include "operations/OperationDispatcher.h"

OperationManager::OperationManager()
	: _dispatcher(std::make_unique<OperationDispatcher>())
{
	_dispatcher->RegisterThread(OperationThreadIds::MainThread, "OM_MainThread");
	_dispatcher->RegisterThread(OperationThreadIds::NetworkThread, "OM_NetworkThread");
	_dispatcher->RegisterThread(OperationThreadIds::HelperThread, "OM_HelperThread");
}

OperationManager::~OperationManager() = default;

void OperationManager::Schedule(const std::shared_ptr<Operation> & operation, OperationThreadIds where)
{
	Schedule(operation, TimeState(), where);
}

void OperationManager::Schedule(const std::shared_ptr<Operation> & operation, TimeState delay, OperationThreadIds where, bool sticky)
{
	_operations.emplace_back(operation, delay, where);
	if (sticky)
	{
		_stickyOperations.emplace_back(operation);
	}
}

void OperationManager::UpdateOperations(TimeState elapsedTime)
{
	// extract all operations to temp variable to avoid coruption of memory,
	// when one of operation intenally calls OperationManager::Schedule
	auto operations = std::move(_operations);

	// move all expired operations to the end
	auto expiredIt = std::remove_if(operations.begin(), operations.end(), [](const OperationStatus & status)
	{
		return status.operation.expired();
	});

	// split alive operations on two partitions:
	// a) deferred operation with execution time more than elapsed time
	// b) ready to dispath operations
	auto deferredIt = std::stable_partition(operations.begin(), expiredIt, [&elapsedTime](const OperationStatus & status)
	{
		return status.delay >= elapsedTime;
	});

	// update delay time of all deferred operations
	std::for_each(operations.begin(), deferredIt, [&elapsedTime](OperationStatus & status)
	{
		status.delay -= elapsedTime;
	});

	// dispath all ready operations
	std::for_each(deferredIt, operations.end(), [this](const OperationStatus & status)
	{
		Dispatch(status);
	});

	// actually remove all expired operations
	operations.erase(deferredIt, operations.end());

	// move all new operations which has been queued in this update function's call to the end
	operations.insert(operations.end(), std::make_move_iterator(_operations.begin()), std::make_move_iterator(_operations.end()));

	// and store operations back in the main buffer
	std::swap(operations, _operations);
}

void OperationManager::Update(TimeState elapsedTime)
{
	ZoneScopedN("Operation Manager Update");
	_dispatcher->UpdateMainThread();
	UpdateDispatchedOperations();
	RemoveCompletedStickyOperations();
	UpdateOperations(elapsedTime);
}

void OperationManager::Dispatch(const OperationStatus& status)
{
	_dispatcher->Dispath(status.operation, status.where);
	_dispatchedOperations.emplace_back(status.operation);
}

void OperationManager::RemoveCompletedStickyOperations()
{
	auto stickyEndIt = std::remove_if(_stickyOperations.begin(), _stickyOperations.end(), [](const std::shared_ptr<Operation> & operation)
	{
		return operation->Completed();
	});
	_stickyOperations.erase(stickyEndIt, _stickyOperations.end());
}

void OperationManager::UpdateDispatchedOperations()
{
	// call OnCompleted callback for all completed operations
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
}

