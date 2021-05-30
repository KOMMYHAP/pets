#include "NativeToJavaBridge.h"
#include "NativeJniBridge.h"

namespace jni {
    inline Local<AvailableConnectionDataObject>
    MakeAnything(ThingToMake<AvailableConnectionDataObject>, JNIEnv &env,
                 const AvailableConnectionData &data) {
        auto clazz = &Class<AvailableConnectionDataClass>::Singleton(env);
        auto constructor = clazz->GetConstructor<String, String, jint>(env);
        return clazz->New(env, constructor,
                         Make<String>(env, data.hostname),
                         Make<String>(env, data.ip),
                         data.port);
    }

    inline Local<NativeBridge$ApplicationStateObject>
    MakeAnything(ThingToMake<NativeBridge$ApplicationStateObject>,
                 JNIEnv &env,
                 const EventDispatcherRemoteApplication::State &state) {
        auto clazz = &Class<NativeBridge$ApplicationStateClass>::Singleton(env);
        const char * name = nullptr;
        switch (state) {
            case EventDispatcherRemoteApplication::State::NotInitialized:
                name = "NotInitialized";
                break;
            case EventDispatcherRemoteApplication::State::Initialized:
                name = "Initialized";
                break;
            case EventDispatcherRemoteApplication::State::WaitingForConnect:
                name = "WaitingForConnect";
                break;
            case EventDispatcherRemoteApplication::State::Connected:
                name = "Connected";
                break;
            case EventDispatcherRemoteApplication::State::ConnectionRequestTimeout:
                name = "ConnectionRequestTimeout";
                break;
            case EventDispatcherRemoteApplication::State::Disconnected:
                name = "Disconnected";
                break;
            case EventDispatcherRemoteApplication::State::DisconnectedByTimeout:
                name = "DisconnectedByTimeout";
                break;
            case EventDispatcherRemoteApplication::State::ErrorOccurred:
                name = "ErrorOccurred";
                break;
            default: ;
        }
        auto staticField = clazz->GetStaticField<NativeBridge$ApplicationStateObject>(env, name);
        return clazz->Get(env, staticField);
    }

    inline Local<NativeBridge$ApplicationErrorStateObject>
    MakeAnything(ThingToMake<NativeBridge$ApplicationErrorStateObject>,
                 JNIEnv &env,
                 const EventDispatcherRemoteApplication::Error &error) {
        auto clazz = &Class<NativeBridge$ApplicationErrorStateClass>::Singleton(env);
        const char * name = nullptr;
        switch (error) {
            case EventDispatcherRemoteApplication::Error::NotInitialized:
                name = "NotInitialized";
                break;
            case EventDispatcherRemoteApplication::Error::LocalPortBusy:
                name = "LocalPortBusy";
                break;
            case EventDispatcherRemoteApplication::Error::RemotePortEmpty:
                name = "RemotePortEmpty";
                break;
            case EventDispatcherRemoteApplication::Error::InvalidIp:
                name = "InvalidIp";
                break;
            case EventDispatcherRemoteApplication::Error::NoError:
                name = "NoError";
                break;
            default: ;
        }
        auto staticField = clazz->GetStaticField<NativeBridge$ApplicationErrorStateObject>(env, name);
        return clazz->Get(env, staticField);
    }

}


NativeToJavaBridge::NativeToJavaBridge(NativeJniBridge &nativeBridge,
                                       NativeBridgeObject &nativeBridgeObject)
        : _nativeBridge(nativeBridge)
        , _globalNativeBridgeObject(*_nativeBridge.GetJniEnv(), nativeBridgeObject) {

}

NativeToJavaBridge::~NativeToJavaBridge() = default;

void NativeToJavaBridge::ResponseAvailableConnectionList(
        const std::vector<AvailableConnectionData> &connectionList) {
    auto *env = _nativeBridge.GetJniEnv();
    if (!env) {
        return;
    }

    auto result = jni::Array<AvailableConnectionDataObject>::New(*env, connectionList.size());
    for (size_t i = 0; i < connectionList.size(); ++i) {
        result.Set(*env, i, jni::Make<AvailableConnectionDataObject>(*env, connectionList[i]));
    }

    static auto clazz = &jni::Class<NativeBridgeClass>::Singleton(*env);
    auto method = clazz->GetMethod<void(jni::Array<AvailableConnectionDataObject>)>(*env,
                                                                                    "onAvailableConnectionListResponse");
    auto nativeBridgeObject = _globalNativeBridgeObject.get(*env);
    nativeBridgeObject.Call(*env, method, result);
}

void NativeToJavaBridge::StateUpdated(const EventDispatcherState &state) {
    auto *env = _nativeBridge.GetJniEnv();
    if (!env) {
        return;
    }

    auto appState = jni::Make<NativeBridge$ApplicationStateObject>(*env, state.state);
    auto appErrorState = jni::Make<NativeBridge$ApplicationErrorStateObject>(*env, state.error);

    static auto clazz = &jni::Class<NativeBridgeClass>::Singleton(*env);
    auto method = clazz->GetMethod<void(NativeBridge$ApplicationStateObject , NativeBridge$ApplicationErrorStateObject)>(*env,
                                                                                    "onStateUpdated");
    auto nativeBridgeObject = _globalNativeBridgeObject.get(*env);
    nativeBridgeObject.Call(*env, method, appState, appErrorState);
}