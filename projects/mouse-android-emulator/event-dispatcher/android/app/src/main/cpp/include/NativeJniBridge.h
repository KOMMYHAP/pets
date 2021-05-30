#pragma once
#include <memory>

//class JavaVM;
class ApplicationInputInterface;
class ApplicationOutputInterface;
class EventDispatcherApplication;

class NativeJniBridge
{
public:
	NativeJniBridge(/*JavaVM * javaVm*/);
	~NativeJniBridge();

	EventDispatcherApplication *	GetApplication() const;
	ApplicationInputInterface *		GetApplicationInputInterface() const;
	ApplicationOutputInterface *	GetApplicationOutputInterface() const;

//	JNIEnv * GetJniEnv() const;
//	JavaVM * GetJavaVm() const;

private:
//	JavaVM *											_javaVm = nullptr;
	std::unique_ptr<EventDispatcherApplication>			_eventDispatcher;
	std::unique_ptr<ApplicationInputInterface>			_inputInterface;
	std::unique_ptr<ApplicationOutputInterface>			_outputInterface;
};
