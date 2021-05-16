#pragma once
#include "Callback.h"
#include "TimeState.h"

class SimpleOperation;
class OperationManager;

class Timer
{
public:
	Timer(const TypedCallback<> & callback, TimeState delay, OperationManager & operationManager);

	void Restart();
	void Reset();

private:
	OperationManager &					_operationManager;
	TimeState							_delay;
	TypedCallback<>						_callback;
	std::shared_ptr<SimpleOperation>	_operation;
};
