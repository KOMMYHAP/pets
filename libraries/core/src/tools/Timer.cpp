#include "tools/Timer.h"

#include "operations/OperationManager.h"
#include "operations/OperationThreadIds.h"
#include "operations/SimpleOperation.h"

Timer::Timer(const TypedCallback<> & callback, TimeState delay, OperationManager & operationManager)
	: _operationManager(operationManager)
	, _delay(delay)
	, _callback(callback)
{
}

void Timer::Restart()
{
	_operation = std::make_shared<SimpleOperation>(_callback);
	_operationManager.Schedule(_operation, _delay, OperationThreadIds::MainThread);
}

void Timer::Reset()
{
	_operation.reset();
}
