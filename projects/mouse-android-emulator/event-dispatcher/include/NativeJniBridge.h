#pragma once
#include <memory>
#include "JniImpl.h"

class EventDispatcherApplication;

class NativeJniBridge
{
public:
	NativeJniBridge(JavaVM * javaVm);

	EventDispatcherApplication * GetEventDispatcherApplication() const;
	JNIEnv * GetJniEnv() const;

private:
	JavaVM *											_javaVm = nullptr;
	std::unique_ptr<EventDispatcherApplication>			_eventDispatcher;
};
