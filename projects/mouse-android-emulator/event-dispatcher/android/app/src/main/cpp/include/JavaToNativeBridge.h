#pragma once
#include "ApplicationInputInterface.h"

class EventDispatcherApplication;

class JavaToNativeBridge : public ApplicationInputInterface
{
public:
	JavaToNativeBridge(EventDispatcherApplication & eventDispatcher);

	void UpdateFrame(int64_t elapsedMs) override;
	void MouseMoved(int x, int y) override;
	void RequestAvailableConnectionList() override;

private:
	EventDispatcherApplication &		_eventDispatcher;
};
