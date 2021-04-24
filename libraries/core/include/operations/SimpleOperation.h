#pragma once
#include "Operation.h"
#include "tools/Callback.h"

class SimpleOperation : public Operation
{
public:
	SimpleOperation(TypedCallback<> callback);

private:
	Result DoImpl() override;
	
	TypedCallback<>			_callback;
};
