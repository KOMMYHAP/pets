#include <jni.h>
#include "jni/jni.hpp"
#include "application/ApplicationEvent.h"
#include "NativeJniBridge.h"
#include "ApplicationInputInterface.h"
#include "ApplicationOutputInterface.h"
#include "JniImpl.h"

namespace {
    std::unique_ptr<NativeJniBridge> s_nativeJniBridge;
}

extern "C"
{

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void * /*reserved*/) {
    jni::JNIEnv &env = jni::GetEnv(*vm);

    auto cache = [vm](jni::JNIEnv &env, jni::Object<NativeBridgeClass> &nativeBridgeObject) {
        if (s_nativeJniBridge) {
            s_nativeJniBridge.reset();
        }
        s_nativeJniBridge = std::make_unique<NativeJniBridge>(vm, nativeBridgeObject);
    };
    auto updateFrame = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> &clazz,
                          jni::jlong elapsedMs) {
        if (s_nativeJniBridge) {
            auto *input = s_nativeJniBridge->GetApplicationInputInterface();
            if (input) {
                input->UpdateFrame(elapsedMs);
            }
        }
    };
    auto touchAreaResize = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> &clazz,
                              jni::jint width, jni::jint height) {
        if (s_nativeJniBridge) {
            auto *input = s_nativeJniBridge->GetApplicationInputInterface();
            if (input) {
                ApplicationEvents::WindowResized windowResized{width, height};
                input->ProcessEvent(ApplicationEvent(windowResized));
            }
        }
    };
    auto touchMoving = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> &clazz, jni::jint x,
                          jni::jint y) {
        if (s_nativeJniBridge) {
            auto *input = s_nativeJniBridge->GetApplicationInputInterface();
            if (input) {
                ApplicationEvents::MouseMoved mouseClicked{x, y};
                input->ProcessEvent(ApplicationEvent(mouseClicked));
            }
        }
    };
    auto touchTapping = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> &clazz, jni::jint x,
                           jni::jint y, jni::Object<EventTouchTypeClass> &eventTouchType) {
        if (s_nativeJniBridge) {
            auto *input = s_nativeJniBridge->GetApplicationInputInterface();
            if (input) {
                auto mouseButton = ApplicationEvents::MouseClicked::MouseButton::LeftButton;
                ApplicationEvents::MouseClicked mouseClicked{x, y, mouseButton};
                input->ProcessEvent(ApplicationEvent(mouseClicked));
            }
        }
    };

    auto requestAvailableConnections = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> &clazz, jni::String &hostname) {
        if (s_nativeJniBridge) {
            auto *input = s_nativeJniBridge->GetApplicationInputInterface();
            if (input) {
                input->RequestAvailableConnectionList(jni::Make<std::string>(env, hostname));
            }
        }
    };

    auto initialize = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> &clazz)
    {
        if (s_nativeJniBridge) {
            auto * input = s_nativeJniBridge->GetApplicationInputInterface();
            if (input)
            {
                input->CanBeInitialized();
            }
        }
    };

    auto connect = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> &clazz, jni::String & ip, jni::jshort port)
    {
        if (s_nativeJniBridge) {
            auto * input = s_nativeJniBridge->GetApplicationInputInterface();
            if (input)
            {
                input->Connect(jni::Make<std::string>(env, ip), port);
            }
        }
    };


#define MAKE_NATIVE(method) \
   jni::MakeNativeMethod(#method, method)

    jni::RegisterNatives(env, *jni::Class<NativeBridgeClass>::Find(env),
                         MAKE_NATIVE(cache),
                         MAKE_NATIVE(updateFrame),
                         MAKE_NATIVE(touchAreaResize),
                         MAKE_NATIVE(touchMoving),
                         MAKE_NATIVE(touchTapping),
                         MAKE_NATIVE(requestAvailableConnections),
                         MAKE_NATIVE(initialize),
                         MAKE_NATIVE(connect)
    );
#undef MAKE_NATIVE

    return jni::Unwrap(jni::jni_version_1_6);
}
}

namespace details {
    JNIEnv *GetEnv(JavaVM *vm) {
        if (vm) {
            return &jni::GetEnv(*vm, jni::jni_version_1_6);
        }
        return nullptr;
    }
}
