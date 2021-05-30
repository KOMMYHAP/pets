#include <jni.h>
#include "jni/jni.hpp"
#include "NativeJniBridge.h"
#include "ApplicationInputInterface.h"
#include "ApplicationOutputInterface.h"
#include "JniImpl.h"

namespace {
    std::unique_ptr<NativeJniBridge> s_nativeJniBridge;
}

extern "C"
{

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* /*reserved*/) {
    jni::JNIEnv & env = jni::GetEnv(*vm);

    auto cache = [vm](jni::JNIEnv &env, jni::Object<NativeBridgeClass> & nativeBridgeObject)
    {
        if (s_nativeJniBridge)
        {
            s_nativeJniBridge.reset();
        }
        s_nativeJniBridge = std::make_unique<NativeJniBridge>(vm, nativeBridgeObject);
    };
    auto updateFrame = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> & clazz, jni::jlong elapsedMs)
    {
        if (s_nativeJniBridge)
        {
            auto * input = s_nativeJniBridge->GetApplicationInputInterface();
            if (input)
            {
                input->UpdateFrame(elapsedMs);
            }
        }
    };
    auto touchAreaResize = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> & clazz, jni::jint width, jni::jint height)
    {
        if (s_nativeJniBridge)
        {
            auto * input = s_nativeJniBridge->GetApplicationInputInterface();
            if (input)
            {
                ApplicationEvents::WindowResized windowResized{width, height};
                input->ProcessEvent(ApplicationEvent(windowResized));
            }
        }
    };
    auto touchMoving = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> & clazz, jni::jint x, jni::jint y)
    {
        if (s_nativeJniBridge)
        {
            auto * input = s_nativeJniBridge->GetApplicationInputInterface();
            if (input)
            {
                ApplicationEvents::MouseMoved mouseClicked{x, y};
                input->ProcessEvent(ApplicationEvent(mouseClicked));
            }
        }
    };
    auto touchTapping = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> & clazz, jni::jint x, jni::jint y, jni::Object<EventTouchTypeClass> & eventTouchType)
    {
        if (s_nativeJniBridge)
        {
            auto * input = s_nativeJniBridge->GetApplicationInputInterface();
            if (input)
            {
                auto mouseButton = ApplicationEvents::MouseClicked::MouseButton::LeftButton;
                ApplicationEvents::MouseClicked mouseClicked{x, y, mouseButton};
                input->ProcessEvent(ApplicationEvent(mouseClicked));
            }
        }
    };
    auto requestAvailableConnections = [](jni::JNIEnv &env, jni::String & hostname)
    {
        if (s_nativeJniBridge)
        {
            auto * input = s_nativeJniBridge->GetApplicationInputInterface();
            if (input)
            {
                input->RequestAvailableConnectionList(jni::Make<std::string>(env, hostname));
            }
        }
    };

#define REGISTER_NATIVE(clazz, method) \
    jni::RegisterNatives(env, *jni::Class<clazz>::Find(env), jni::MakeNativeMethod(#method, method));

    REGISTER_NATIVE(NativeBridgeClass, cache);
    REGISTER_NATIVE(NativeBridgeClass, updateFrame);
    REGISTER_NATIVE(NativeBridgeClass, touchAreaResize);
    REGISTER_NATIVE(NativeBridgeClass, touchMoving);
    REGISTER_NATIVE(NativeBridgeClass, touchTapping);
    REGISTER_NATIVE(NativeBridgeClass, requestAvailableConnections);

#undef REGISTER_NATIVE

    return jni::Unwrap(jni::jni_version_1_6);
}
}

namespace details
{
    JNIEnv * GetEnv(JavaVM *vm) {
        if (vm) {
            jni::GetEnv(*vm, jni::jni_version_1_6);
        }
        return nullptr;
    }
}
