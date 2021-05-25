#pragma once

#if defined(ANDROID)
#include <jni.h>
#else
#define JNIEXPORT
class _JNIEnv {};
class _JavaVM {};
class _jobject {};
class _jclass : public _jobject {};

using JavaVM = _JavaVM;
using JNIEnv = _JNIEnv;

using jobject = _jobject *;
using jclass = _jclass *;

using jbyte = int8_t;
using jint = int32_t;
using jfloat = float;
using jdouble = double;
using jchar = int16_t;
using jlong = int64_t;
using jshort = int16_t;
using jboolean = int8_t;
#endif

extern "C"
{
	JNIEXPORT jint Java_com_example_remotemouse_NativeBridge_Test(JNIEnv *env, jobject thiz);
	JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved);
}

JNIEnv * GetEnv(JavaVM *vm);
