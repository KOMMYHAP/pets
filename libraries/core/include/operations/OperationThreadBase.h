#pragma once

#include <string>
#include <thread>

class Operation;

class OperationThreadBase
{
public:
	virtual ~OperationThreadBase() = default;

	virtual void SetName(const std::string & name) = 0;
	virtual void Dispatch(std::weak_ptr<Operation> operation) = 0;
	virtual std::thread::id GetId() const = 0;
};
