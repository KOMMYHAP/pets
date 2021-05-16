#pragma once
#include <map>
#include <string>
#include <thread>

#include "OperationThreadIds.h"

class OperationThreadBase;
class Operation;

class OperationDispatcher
{
public:
	OperationDispatcher();
	~OperationDispatcher();

	void RegisterThread(OperationThreadIds where, const std::string & name);
	void Dispath(std::weak_ptr<Operation> operation, OperationThreadIds where);
	void UpdateMainThread();

private:
	OperationThreadBase * GetThread(OperationThreadIds ids) const;
	
	std::map<OperationThreadIds, std::unique_ptr<OperationThreadBase>>	_threadIds;
};
