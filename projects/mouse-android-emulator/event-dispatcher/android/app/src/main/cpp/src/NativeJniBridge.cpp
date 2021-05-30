#include "NativeJniBridge.h"

#include "EventDispatcherApplication.h"
#include "application/ApplicationEvent.h"
#include "ApplicationInputInterface.h"
#include "ApplicationOutputInterface.h"

NativeJniBridge::NativeJniBridge(/*JavaVM* javaVm*/)
//	: _javaVm(javaVm)
	: _eventDispatcher(std::make_unique<EventDispatcherApplication>())
{
}

NativeJniBridge::~NativeJniBridge() = default;

EventDispatcherApplication* NativeJniBridge::GetApplication() const
{
	return _eventDispatcher.get();
}

ApplicationInputInterface* NativeJniBridge::GetApplicationInputInterface() const
{
	return _inputInterface.get();
}

ApplicationOutputInterface* NativeJniBridge::GetApplicationOutputInterface() const
{
	return _outputInterface.get();
}

//JNIEnv* NativeJniBridge::GetJniEnv() const
//{
//	return GetEnv(_javaVm);
//}
//
//JavaVM* NativeJniBridge::GetJavaVm() const
//{
//	return _javaVm;
//}
