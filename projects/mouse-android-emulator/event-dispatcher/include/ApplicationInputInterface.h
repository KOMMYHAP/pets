#pragma once
#include <cstdint>

class ApplicationInputInterface
{
public:
	virtual ~ApplicationInputInterface() = default;

	virtual void UpdateFrame(int64_t elapsedMs) = 0;
	virtual void MouseMoved(int x, int y) = 0;
	virtual void RequestAvailableConnectionList() = 0;
};
