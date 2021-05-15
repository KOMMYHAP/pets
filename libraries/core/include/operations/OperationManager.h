#pragma once
#include <memory>
#include <vector>

#include "tools/TimeState.h"
#include "OperationThreadIds.h"

class OperationDispatcher;
class Operation;

class OperationManager
{
public:
	OperationManager();
	~OperationManager();

	void Schedule(const std::shared_ptr<Operation> & operation, OperationThreadIds where);
	void Schedule(const std::shared_ptr<Operation> & operation, TimeState delay, OperationThreadIds where, bool sticky = false);

	void Update(TimeState elapsedTime);

private:
	struct OperationStatus
	{
		OperationStatus() = default;
		OperationStatus(std::weak_ptr<Operation> operation, TimeState delay, OperationThreadIds where)
			: operation(std::move(operation))
			, delay(delay)
			, where(where)
		{}
		
		std::weak_ptr<Operation>	operation;
		TimeState					delay;
		OperationThreadIds			where = OperationThreadIds::MainThread;
	};
	void Dispatch(const OperationStatus & status);

	void RemoveCompletedStickyOperations();
	void PerformCompletedOperations();
	void UpdateOperations(TimeState elapsedTime);

	std::vector<std::shared_ptr<Operation>>	_stickyOperations;
	std::vector<OperationStatus>			_operations;
	std::vector<std::weak_ptr<Operation>>	_dispatchedOperations;
	std::unique_ptr<OperationDispatcher>	_dispatcher;
};
