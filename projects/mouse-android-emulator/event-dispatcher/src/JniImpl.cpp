#include "JniImpl.h"

namespace
{
	std::unique_ptr<NativeJniBridge>		s_nativeJniBridge;
}

extern "C"
{
	JNIEXPORT jint
	Java_com_example_remotemouse_NativeBridge_MouseMoved(JNIEnv *env, jobject thiz, jint x, jint y) {
		if (s_nativeJniBridge)
		{
			s_nativeJniBridge->MouseMoved(static_cast<int>(x), static_cast<int>(y));
		}
	}

    JNIEXPORT jint
    Java_com_example_remotemouse_NativeBridge_Test(JNIEnv *env, jobject thiz) {
        return 42;
    }

	JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
		JNIEnv* env = GetEnv(vm);

	    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
	    jclass c = env->FindClass("com/example/remotemouse/NativeBridge");
	    if (c == nullptr) return JNI_ERR;

	    // Register your class' native methods.
	    static const JNINativeMethod methods[] = {
	        {"Test", "()I", reinterpret_cast<void*>(Java_com_example_remotemouse_NativeBridge_Test)},
	        // {"nativeBar", "(Ljava/lang/String;I)Z", reinterpret_cast<void*>(nativeBar)},
	    };
	    int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
	    if (rc != JNI_OK) return rc;

		s_nativeJniBridge = std::make_unique<NativeJniBridge>(vm);

	    return JNI_VERSION_1_6;
	}
}

JNIEnv * GetEnv(JavaVM * vm)
{
	JNIEnv* env;
	if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
		return nullptr;
	}
	return env;
}
