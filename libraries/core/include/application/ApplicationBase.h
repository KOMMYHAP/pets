#pragma once
#include "tools/TimeState.h"

class ApplicationEvent;

class ApplicationDelegate
{
public:
	virtual ~ApplicationDelegate() = default;

	virtual void ProcessCommandLine(int argc, char **argv) = 0;
	virtual void ProcessEvent(const ApplicationEvent & event) = 0;
	virtual void ProcessElapsedTime(TimeState elapsedTime) = 0;

	virtual bool ShouldTerminate() const = 0;

	virtual void OnActivated() = 0;
	virtual void OnDeactivated() = 0;
};