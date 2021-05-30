#include "NativeToJavaBridge.h"

NativeToJavaBridge::NativeToJavaBridge(NativeJniBridge& nativeBridge, jni::Object<NativeBridgeClass> & nativeBridgeObject)
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
    // auto * env = _nativeBridge.GetJniEnv();
    // if (!env)
    // {
    // 	return;
    // }
    //
    // ScopeLocalFrame arrayLocalFrame(env, connectionList.size() * 2);
    // for (const auto & connectionData : connectionList)
    // {
    // 	ScopeLocalFrame tempVarsLocalFrame(env, 16);
    // 	jobject ipObject = CreateJavaString(env, connectionData.ip);
    // 	if (!ipObject)
    // 	{
    // 		break;
    // 	}
    // 	jobject hostnameObject = CreateJavaString(env, connectionData.hostname);
    // 	if (!hostnameObject)
    // 	{
    // 		break;
    // 	}
    // 	jint port = connectionData.port;
    // }
    //
    // if (env->ExceptionCheck())
    // {
    // 	return;
    // }

    // call
}

void NativeToJavaBridge::StateUpdated(const EventDispatcherState& state)
{
    // auto * env = _nativeBridge.GetJniEnv();
    // if (!env)
    // {
    // 	return;
    // }
    //
    // jobject stateObject = nullptr;
    // switch (state.state)
    // {
    // case EventDispatcherRemoteApplication::State::NotInitialized:
    // 	stateObject = env->GetStaticObjectField(_cachedClassIds[ApplicationStateClass], _cachedFields[ApplicationStateNotInitialized]);
    // 	break;
    // case EventDispatcherRemoteApplication::State::Initialized:
    // 	stateObject = env->GetStaticObjectField(_cachedClassIds[ApplicationStateClass], _cachedFields[ApplicationStateInitialized]);
    // 	break;
    // case EventDispatcherRemoteApplication::State::WaitingForConnect:
    // 	stateObject = env->GetStaticObjectField(_cachedClassIds[ApplicationStateClass], _cachedFields[ApplicationStateWaitingForConnect]);
    // 	break;
    // case EventDispatcherRemoteApplication::State::Connected:
    // 	stateObject = env->GetStaticObjectField(_cachedClassIds[ApplicationStateClass], _cachedFields[ApplicationStateConnected]);
    // 	break;
    // case EventDispatcherRemoteApplication::State::ConnectionRequestTimeout:
    // 	stateObject = env->GetStaticObjectField(_cachedClassIds[ApplicationStateClass], _cachedFields[ApplicationStateConnectionRequestTimeout]);
    // 	break;
    // case EventDispatcherRemoteApplication::State::Disconnected:
    // 	stateObject = env->GetStaticObjectField(_cachedClassIds[ApplicationStateClass], _cachedFields[ApplicationStateDisconnected]);
    // 	break;
    // case EventDispatcherRemoteApplication::State::DisconnectedByTimeout:
    // 	stateObject = env->GetStaticObjectField(_cachedClassIds[ApplicationStateClass], _cachedFields[ApplicationStateDisconnectedByTimeout]);
    // 	break;
    // case EventDispatcherRemoteApplication::State::ErrorOccurred:
    // 	stateObject = env->GetStaticObjectField(_cachedClassIds[ApplicationStateClass], _cachedFields[ApplicationStateErrorOccurred]);
    // 	break;
    // default: ;
    // }
    // if (!stateObject || env->ExceptionCheck())
    // {
    // 	return;
    // }
    //
    // jobject errorStateObject = nullptr;
    // switch (state.error)
    // {
    // case EventDispatcherRemoteApplication::Error::NotInitialized:
    // 	errorStateObject = env->GetStaticObjectField(_cachedClassIds[ApplicationErrorStateClass], _cachedFields[ApplicationErrorStateNotInitialized]);
    // 	break;
    // case EventDispatcherRemoteApplication::Error::LocalPortBusy:
    // 	errorStateObject = env->GetStaticObjectField(_cachedClassIds[ApplicationErrorStateClass], _cachedFields[ApplicationErrorStateLocalPortBusy]);
    // 	break;
    // case EventDispatcherRemoteApplication::Error::RemotePortEmpty:
    // 	errorStateObject = env->GetStaticObjectField(_cachedClassIds[ApplicationErrorStateClass], _cachedFields[ApplicationErrorStateRemotePortEmpty]);
    // 	break;
    // case EventDispatcherRemoteApplication::Error::InvalidIp:
    // 	errorStateObject = env->GetStaticObjectField(_cachedClassIds[ApplicationErrorStateClass], _cachedFields[ApplicationErrorStateInvalidIp]);
    // 	break;
    // case EventDispatcherRemoteApplication::Error::NoError:
    // 	errorStateObject = env->GetStaticObjectField(_cachedClassIds[ApplicationErrorStateClass], _cachedFields[ApplicationErrorStateNoError]);
    // 	break;
    // default: ;
    // }
    // if (!errorStateObject || env->ExceptionCheck())
    // {
    // 	return;
    // }
    //
    // jmethodID stateUpdateMethod = _cachedMethods[StateUpdatedMethod];
    // if (!stateUpdateMethod)
    // {
    // 	return;
    // }
    //
    // if (_globalNativeBridgeObject && stateUpdateMethod && stateObject && errorStateObject)
    // {
    // 	env->CallVoidMethod(_globalNativeBridgeObject, stateUpdateMethod, stateObject, errorStateObject);
    // }
    //
    // env->DeleteLocalRef(stateObject);
    // env->DeleteLocalRef(errorStateObject);
}

void NativeToJavaBridge::Cache(jni::Object<NativeBridgeClass> & initializerObject)
{
    // auto * env = _nativeBridge.GetJniEnv();
    // if (!env)
    // {
    // 	return;
    // }
    //
    // jclass applicationStateClass;
    // jclass applicationErrorStateClass;
    // jclass availableConnectionClass;
    //
    // jclass nativeBridgeClass = SafeFindClass(env, "com/example/remotemouse/NativeBridge");
    // jclass initializerClass = static_cast<jclass>(env->GetObjectClass(initializerObject));
    // if (!env->IsSameObject(initializerClass, nativeBridgeClass))
    // {
    // 	return;
    // }
    // {
    //
    // 	applicationStateClass =			env->FindClass("com/example/remotemouse/NativeBridge$ApplicationState");
    // 	applicationErrorStateClass =	env->FindClass("com/example/remotemouse/NativeBridge$ApplicationErrorState");
    // 	availableConnectionClass =		env->FindClass("com/example/remotemouse/AvailableConnectionData");
    //
    // 	_cachedClassIds[NativeBridgeClass] =						static_cast<jclass>(env->NewGlobalRef(nativeBridgeClass));
    // 	_cachedClassIds[ApplicationStateClass] =					static_cast<jclass>(env->NewGlobalRef(applicationStateClass));
    // 	_cachedClassIds[ApplicationErrorStateClass] =				static_cast<jclass>(env->NewGlobalRef(applicationErrorStateClass));
    // 	_cachedClassIds[AvailableConnectionDataClass] =				static_cast<jclass>(env->NewGlobalRef(availableConnectionClass));
    //
    // 	_globalNativeBridgeObject =									env->NewGlobalRef(initializerObject);
    // }
    //
    //
    //
    // _cachedMethods[AvailableConnectionResponseMethod] =			env->GetMethodID(nativeBridgeClass, "OnAvailableConnectionListResponse",	"([Lcom/example/remotemouse/NativeBridge$AvailableConnectionData;)V");
    // _cachedMethods[StateUpdatedMethod] =						env->GetMethodID(nativeBridgeClass, "StateUpdated",							"(Lcom/example/remotemouse/NativeBridge$ApplicationState;Lcom/example/remotemouse/NativeBridge$ApplicationErrorState;)V");
    //
    // _cachedFields[ApplicationStateNotInitialized] =				env->GetStaticFieldID(applicationStateClass, "NotInitialized",				"Lcom/example/remotemouse/NativeBridge$ApplicationState");
    // _cachedFields[ApplicationStateInitialized] =				env->GetStaticFieldID(applicationStateClass, "Initialized",					"Lcom/example/remotemouse/NativeBridge$ApplicationState");
    // _cachedFields[ApplicationStateWaitingForConnect] =			env->GetStaticFieldID(applicationStateClass, "WaitingForConnect",			"Lcom/example/remotemouse/NativeBridge$ApplicationState");
    // _cachedFields[ApplicationStateConnected] =					env->GetStaticFieldID(applicationStateClass, "Connected",					"Lcom/example/remotemouse/NativeBridge$ApplicationState");
    // _cachedFields[ApplicationStateConnectionRequestTimeout] =	env->GetStaticFieldID(applicationStateClass, "ConnectionRequestTimeout",	"Lcom/example/remotemouse/NativeBridge$ApplicationState");
    // _cachedFields[ApplicationStateDisconnected] =				env->GetStaticFieldID(applicationStateClass, "Disconnected",				"Lcom/example/remotemouse/NativeBridge$ApplicationState");
    // _cachedFields[ApplicationStateDisconnectedByTimeout] =		env->GetStaticFieldID(applicationStateClass, "DisconnectedByTimeout",		"Lcom/example/remotemouse/NativeBridge$ApplicationState");
    // _cachedFields[ApplicationStateErrorOccurred] =				env->GetStaticFieldID(applicationStateClass, "ErrorOccurred",				"Lcom/example/remotemouse/NativeBridge$ApplicationState");
    //
    // _cachedFields[ApplicationErrorStateNotInitialized] =		env->GetStaticFieldID(applicationErrorStateClass, "NotInitialized",			"Lcom/example/remotemouse/NativeBridge$ApplicationErrorState");
    // _cachedFields[ApplicationErrorStateLocalPortBusy] =			env->GetStaticFieldID(applicationErrorStateClass, "LocalPortBusy",			"Lcom/example/remotemouse/NativeBridge$ApplicationErrorState");
    // _cachedFields[ApplicationErrorStateRemotePortEmpty] =		env->GetStaticFieldID(applicationErrorStateClass, "RemotePortEmpty",		"Lcom/example/remotemouse/NativeBridge$ApplicationErrorState");
    // _cachedFields[ApplicationErrorStateInvalidIp] =				env->GetStaticFieldID(applicationErrorStateClass, "InvalidIp",				"Lcom/example/remotemouse/NativeBridge$ApplicationErrorState");
    // _cachedFields[ApplicationErrorStateNoError] =				env->GetStaticFieldID(applicationErrorStateClass, "NoError",				"Lcom/example/remotemouse/NativeBridge$ApplicationErrorState");
    //
    // _cachedFields[EventTouchShortType] =						env->GetStaticFieldID(applicationErrorStateClass, "ShortTap",				"Lcom/example/remotemouse/NativeBridge$EventTouchType");
}

void NativeToJavaBridge::InvalidateCache()
{
    // auto * env = _nativeBridge.GetJniEnv();
    // if (!env)
    // {
    // 	return;
    // }
    //
    // for (jclass classGlobalRef : _cachedClassIds)
    // {
    // 	if (classGlobalRef)
    // 	{
    // 		env->DeleteGlobalRef(classGlobalRef);
    // 	}
    // }
    //
    // if (_globalNativeBridgeObject)
    // {
    // 	env->DeleteGlobalRef(_globalNativeBridgeObject);
    // }
    //
    // _cachedClassIds.fill(nullptr);
    // _cachedFields.fill(nullptr);
    // _cachedMethods.fill(nullptr);
    // _globalNativeBridgeObject = nullptr;
}
