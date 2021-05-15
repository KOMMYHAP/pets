#pragma once
#include "Operation.h"
#include "tools/Callback.h"

class SimpleOperation : public Operation
{
public:
	SimpleOperation(TypedCallback<> callback);
	SimpleOperation(TypedCallback<> callback, TypedCallback<std::exception_ptr> onCompleted);

	struct SafeOperationTag {};
	SimpleOperation(TypedCallback<> onCompleted, SafeOperationTag);

private:
	Result DoImpl() override;
	void OnCompleted(std::exception_ptr exception) override;
	
	TypedCallback<>							_callback;
	TypedCallback<std::exception_ptr>		_onCompleted;
};
