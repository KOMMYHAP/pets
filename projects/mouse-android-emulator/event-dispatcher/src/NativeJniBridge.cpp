#include "NativeJniBridge.h"

#include "EventDispatcherApplication.h"
#include "application/ApplicationEvent.h"

JavaToNativeBridge::JavaToNativeBridge(EventDispatcherApplication& eventDispatcher)
	: _eventDispatcher(eventDispatcher)
{
}

void JavaToNativeBridge::UpdateFrame(int64_t elapsedMs)
{
	_eventDispatcher.ProcessElapsedTime(TimeState::Milliseconds(elapsedMs));
}

void JavaToNativeBridge::MouseMoved(int x, int y)
{
	ApplicationEvents::MouseMoved mouseMoved;
	mouseMoved.x = static_cast<int>(x);
	mouseMoved.y = static_cast<int>(y);
	_eventDispatcher.ProcessEvent(ApplicationEvent(mouseMoved));
}

void JavaToNativeBridge::RequestAvailiableConnectionList()
{
	
}

NativeToJavaBridge::NativeToJavaBridge(NativeJniBridge& nativeBridge, jobject nativeBridgeObject)
	: _nativeBridge(nativeBridge)
{
	_cachedClassIds.fill(nullptr);
	_cachedFields.fill(nullptr);
	_cachedMethods.fill(nullptr);
	Cache(nativeBridgeObject);
}

NativeToJavaBridge::~NativeToJavaBridge()
{
	InvalidateCache();
}

void NativeToJavaBridge::ResponseAvailableConnectionList(const std::vector<AvailableConnectionData>& connectionList)
{
}

void NativeToJavaBridge::StateUpdated(const EventDispatcherState& state)
{
	auto * env = _nativeBridge.GetJniEnv();
	if (!env)
	{
		return;
	}

	jobject stateObject = nullptr;
	// todo: create java representation of native state
	
	jobject errorStateObject = nullptr;
	// todo: create java representation of native error state
	
	jmethodID stateUpdateMethod = _cachedMethods[StateUpdatedMethod];
	if (_globalNativeBridgeObject && stateUpdateMethod && stateObject && errorStateObject)
	{
		env->CallVoidMethod(_globalNativeBridgeObject, stateUpdateMethod, stateObject, errorStateObject);
	}
}

void NativeToJavaBridge::Cache(jobject initializerObject)
{
	auto * env = _nativeBridge.GetJniEnv();
	if (!env)
	{
		return;
	}

	jclass nativeBridgeClass = env->FindClass("com/example/remotemouse/NativeBridge");
	if (!nativeBridgeClass)
	{
		return;
	}

	jclass initializerClass = static_cast<jclass>(env->GetObjectClass(initializerObject));
	if (!env->IsSameObject(initializerClass, nativeBridgeClass))
	{
		return;
	}

	_globalNativeBridgeObject = env->NewGlobalRef(initializerObject);
	
	jclass applicationStateClass =								env->FindClass("com/example/remotemouse/NativeBridge$ApplicationState");
	jclass applicationErrorStateClass =							env->FindClass("com/example/remotemouse/NativeBridge$ApplicationErrorState");
	
	_cachedClassIds[NativeBridgeClass] =						static_cast<jclass>(env->NewGlobalRef(nativeBridgeClass));
	_cachedClassIds[ApplicationStateClass] =					static_cast<jclass>(env->NewGlobalRef(applicationStateClass));
	_cachedClassIds[ApplicationErrorStateClass] =				static_cast<jclass>(env->NewGlobalRef(applicationErrorStateClass));

	_cachedMethods[AvailableConnectionResponseMethod] =			env->GetMethodID(nativeBridgeClass, "OnAvailableConnectionListResponse",	"([Lcom/example/remotemouse/NativeBridge$AvailableConnectionData;)V");
	_cachedMethods[StateUpdatedMethod] =						env->GetMethodID(nativeBridgeClass, "StateUpdated",							"(Lcom/example/remotemouse/NativeBridge$ApplicationState;Lcom/example/remotemouse/NativeBridge$ApplicationErrorState;)V");
	
	_cachedFields[ApplicationStateNotInitialized] =				env->GetStaticFieldID(applicationStateClass, "NotInitialized",				"Lcom/example/remotemouse/NativeBridge$ApplicationState");
	_cachedFields[ApplicationStateInitialized] =				env->GetStaticFieldID(applicationStateClass, "Initialized",					"Lcom/example/remotemouse/NativeBridge$ApplicationState");
	_cachedFields[ApplicationStateWaitingForConnect] =			env->GetStaticFieldID(applicationStateClass, "WaitingForConnect",			"Lcom/example/remotemouse/NativeBridge$ApplicationState");
	_cachedFields[ApplicationStateConnected] =					env->GetStaticFieldID(applicationStateClass, "Connected",					"Lcom/example/remotemouse/NativeBridge$ApplicationState");
	_cachedFields[ApplicationStateConnectionRequestTimeout] =	env->GetStaticFieldID(applicationStateClass, "ConnectionRequestTimeout",	"Lcom/example/remotemouse/NativeBridge$ApplicationState");
	_cachedFields[ApplicationStateDisconnected] =				env->GetStaticFieldID(applicationStateClass, "Disconnected",				"Lcom/example/remotemouse/NativeBridge$ApplicationState");
	_cachedFields[ApplicationStateDisconnectedByTimeout] =		env->GetStaticFieldID(applicationStateClass, "DisconnectedByTimeout",		"Lcom/example/remotemouse/NativeBridge$ApplicationState");
	_cachedFields[ApplicationStateErrorOccurred] =				env->GetStaticFieldID(applicationStateClass, "ErrorOccurred",				"Lcom/example/remotemouse/NativeBridge$ApplicationState");

	_cachedFields[ApplicationErrorStateNotInitialized] =		env->GetStaticFieldID(applicationErrorStateClass, "NotInitialized",			"Lcom/example/remotemouse/NativeBridge$ApplicationErrorState");
	_cachedFields[ApplicationErrorStateLocalPortBusy] =			env->GetStaticFieldID(applicationErrorStateClass, "LocalPortBusy",			"Lcom/example/remotemouse/NativeBridge$ApplicationErrorState");
	_cachedFields[ApplicationErrorStateRemotePortEmpty] =		env->GetStaticFieldID(applicationErrorStateClass, "RemotePortEmpty",		"Lcom/example/remotemouse/NativeBridge$ApplicationErrorState");
	_cachedFields[ApplicationErrorStateInvalidIp] =				env->GetStaticFieldID(applicationErrorStateClass, "InvalidIp",				"Lcom/example/remotemouse/NativeBridge$ApplicationErrorState");
	_cachedFields[ApplicationErrorStateNoError] =				env->GetStaticFieldID(applicationErrorStateClass, "NoError",				"Lcom/example/remotemouse/NativeBridge$ApplicationErrorState");
	
	_cachedFields[EventTouchShortType] =						env->GetStaticFieldID(applicationErrorStateClass, "ShortTap",				"Lcom/example/remotemouse/NativeBridge$EventTouchType");
}

void NativeToJavaBridge::InvalidateCache()
{
	auto * env = _nativeBridge.GetJniEnv();
	if (!env)
	{
		return;
	}

	for (jclass classGlobalRef : _cachedClassIds)
	{
		if (classGlobalRef)
		{
			env->DeleteGlobalRef(classGlobalRef);
		}
	}

	if (_globalNativeBridgeObject)
	{
		env->DeleteGlobalRef(_globalNativeBridgeObject);
	}

	_cachedClassIds.fill(nullptr);
	_cachedFields.fill(nullptr);
	_cachedMethods.fill(nullptr);
	_globalNativeBridgeObject = nullptr;
}

NativeJniBridge::NativeJniBridge(JavaVM* javaVm)
	: _javaVm(javaVm)
	, _eventDispatcher(std::make_unique<EventDispatcherApplication>())
{
}

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

JNIEnv* NativeJniBridge::GetJniEnv() const
{
	return GetEnv(_javaVm);
}

JavaVM* NativeJniBridge::GetJavaVm() const
{
	return _javaVm;
}
