#pragma once
#include <memory>
#include "jni/jni.hpp"
#include "JniImpl.h"

class ApplicationInputInterface;
class ApplicationOutputInterface;
class EventDispatcherApplication;

class NativeJniBridge
{
public:
	NativeJniBridge(JavaVM * javaVm, jni::Object<NativeBridgeClass> & nativeBridgeObject);
	~NativeJniBridge();

	EventDispatcherApplication *	GetApplication() const;
	ApplicationInputInterface *		GetApplicationInputInterface() const;
	ApplicationOutputInterface *	GetApplicationOutputInterface() const;

	JavaVM * GetJavaVM() const;
	JNIEnv * GetJniEnv() const;

private:
	JavaVM *											_javaVm = nullptr;
	std::unique_ptr<EventDispatcherApplication>			_application;
	std::unique_ptr<ApplicationInputInterface>			_inputInterface;
	std::unique_ptr<ApplicationOutputInterface>			_outputInterface;
};
