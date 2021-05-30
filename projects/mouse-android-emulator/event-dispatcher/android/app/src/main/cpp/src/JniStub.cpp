#include "JniImpl.h"

extern "C"
{
	JNIEXPORT jint Java_com_example_remotemouse_NativeBridge_Test(JNIEnv *, jobject)
	{
		return 0;
	}

	JNIEXPORT jint JNI_OnLoad(JavaVM* , void*)
	{
		return 0;
	}
}

JNIEnv * GetEnv(JavaVM *)
{
	return nullptr;
}