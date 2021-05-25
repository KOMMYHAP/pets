#include "NativeJniBridge.h"

#include "EventDispatcherApplication.h"

NativeJniBridge::NativeJniBridge(JavaVM* javaVm)
	: _javaVm(javaVm)
	, _eventDispatcher(std::make_unique<EventDispatcherApplication>())
{
}

EventDispatcherApplication* NativeJniBridge::GetEventDispatcherApplication() const
{
	return _eventDispatcher.get();
}

JNIEnv* NativeJniBridge::GetJniEnv() const
{
	return GetEnv(_javaVm);
}
