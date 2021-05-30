#include "NativeJniBridge.h"

#include "EventDispatcherApplication.h"
#include "application/ApplicationEvent.h"
#include "JavaToNativeBridge.h"
#include "NativeToJavaBridge.h"
#include "JniImpl.h"

NativeJniBridge::NativeJniBridge(JavaVM *javaVm, jni::Object<NativeBridgeClass> & nativeBridgeObject)
	: _javaVm(javaVm)
	, _application(std::make_unique<EventDispatcherApplication>())
{
	_inputInterface = std::make_unique<JavaToNativeBridge>(*this);
	auto * env = GetJniEnv();
	if (env)
	{
		_outputInterface = std::make_unique<NativeToJavaBridge>(*this, nativeBridgeObject);
		_application->SetApplicationOutputInterface(_outputInterface.get());
	}
}

NativeJniBridge::~NativeJniBridge() = default;

EventDispatcherApplication* NativeJniBridge::GetApplication() const
{
	return _application.get();
}

ApplicationInputInterface* NativeJniBridge::GetApplicationInputInterface() const
{
	return _inputInterface.get();
}

ApplicationOutputInterface* NativeJniBridge::GetApplicationOutputInterface() const
{
	return _outputInterface.get();
}

JNIEnv* NativeJniBridge::GetJniEnv() const
{
	return details::GetEnv(_javaVm);
}

JavaVM* NativeJniBridge::GetJavaVM() const
{
	return _javaVm;
}
