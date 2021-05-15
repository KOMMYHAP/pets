#include "operations/SimpleOperation.h"

SimpleOperation::SimpleOperation(TypedCallback<> callback)
	: SimpleOperation(std::move(callback), TypedCallback<std::exception_ptr>())
{
}

SimpleOperation::SimpleOperation(TypedCallback<> callback, TypedCallback<std::exception_ptr> onCompleted)
	: _callback(std::move(callback))
	, _onCompleted(std::move(onCompleted))
{
}

SimpleOperation::SimpleOperation(TypedCallback<> onCompleted, SafeOperationTag)
{
	auto owner = onCompleted.Owner();
	auto onCompletedIgnoringException = [onCompleted = std::move(onCompleted)](std::exception_ptr)
	{
		onCompleted();
	};
	_onCompleted = TypedCallback<std::exception_ptr>(std::move(owner), std::move(onCompletedIgnoringException));
}

Operation::Result SimpleOperation::DoImpl()
{
	_callback();
	return Result::Completed;
}

void SimpleOperation::OnCompleted(std::exception_ptr exception)
{
	if (_onCompleted.Valid())
	{
		_onCompleted(exception);
	}
	else if (exception)
	{
		std::rethrow_exception(exception);
	}
}
