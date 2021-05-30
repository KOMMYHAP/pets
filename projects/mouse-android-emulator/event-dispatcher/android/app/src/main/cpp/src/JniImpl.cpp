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
//public void OnAvailableConnectionListResponse(AvailableConnectionData[] connectionsList);
//public void OnStateUpdated(ApplicationState state, ApplicationErrorState error);

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* /*reserved*/) {
    jni::JNIEnv & env = jni::GetEnv(*vm);

    s_nativeJniBridge = std::make_unique<NativeJniBridge>(/*vm*/);

    auto cache = [](jni::JNIEnv &env, jni::Class<NativeBridgeClass> & clazz)
    {};
    auto updateFrame = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> & clazz, jni::jlong)
    {};
    auto touchAreaResize = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> & clazz, jni::jint width, jni::jint height)
    {};
    auto touchMoving = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> & clazz, jni::jint x, jni::jint y)
    {};
    auto touchTapping = [](jni::JNIEnv &env, jni::Object<NativeBridgeClass> & clazz, jni::jint x, jni::jint y, jni::Object<EventTouchTypeClass> & eventTouchType)
    {};

#define REGISTER_NATIVE(clazz, method) \
    jni::RegisterNatives(env, *jni::Class<clazz>::Find(env), jni::MakeNativeMethod(#method, method));

    REGISTER_NATIVE(NativeBridgeClass, cache);
    REGISTER_NATIVE(NativeBridgeClass, updateFrame);
    REGISTER_NATIVE(NativeBridgeClass, touchAreaResize);
    REGISTER_NATIVE(NativeBridgeClass, touchMoving);
    REGISTER_NATIVE(NativeBridgeClass, touchTapping);

#undef REGISTER_NATIVE

    return jni::Unwrap(jni::jni_version_1_6);
}
}

JNIEnv *GetEnv(JavaVM *vm) {
    if (vm) {
        JNIEnv *env;
        if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
            return nullptr;
        }
        return env;
    }
    return nullptr;
}
