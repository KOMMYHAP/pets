#include "operations/SimpleOperation.h"

SimpleOperation::SimpleOperation(TypedCallback<> callback)
	: _callback(callback)
{
}

Operation::Result SimpleOperation::DoImpl()
{
	_callback();
	return Result::Completed;
}
