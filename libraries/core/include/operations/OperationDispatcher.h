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

private:
	std::map<OperationThreadIds, std::thread::id>	_threadIds;
	std::map<std::thread::id, std::unique_ptr<OperationThreadBase>> _threads;
};
