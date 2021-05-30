#pragma once
#include <thread>
#include <vector>

#include "OperationThreadBase.h"

class OperationMainThread : public OperationThreadBase
{
public:
	OperationMainThread();
	
	void SetName(const std::string& name) override;
	void Dispatch(std::weak_ptr<Operation> operation) override;
	std::thread::id GetId() const override;

	void Update();

private:
	std::thread::id							_id;
	std::vector<std::weak_ptr<Operation>>	_operations;
};
