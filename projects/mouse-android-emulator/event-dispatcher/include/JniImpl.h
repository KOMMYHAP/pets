#pragma once

#if defined(ANDROID)
#include <jni.h>
#else
#define JNIEXPORT
class _JavaVM {};
using JavaVM = _JavaVM;

class _jobject {};
using jobject = _jobject *;

class _jclass : public _jobject {};
using jclass = _jclass *;

class _jfieldID;
using jfieldID = _jfieldID *;
class _jmethodID;
using jmethodID = _jmethodID *;

using jbyte = int8_t;
using jint = int32_t;
using jfloat = float;
using jdouble = double;
using jchar = int16_t;
using jlong = int64_t;
using jshort = int16_t;
using jboolean = int8_t;

struct _JNIEnv
{
	jclass FindClass([[maybe_unused]] const char * className) { return nullptr; }
	jobject NewGlobalRef([[maybe_unused]] jobject object) { return nullptr; }
	void DeleteGlobalRef(jobject object);
	jmethodID GetMethodID(jclass clazz, const char *name, const char *sig) { return nullptr; }
	jfieldID GetStaticFieldID(jclass clazz, const char *name, const char *sig) { return nullptr; }
	void CallVoidMethod(jobject obj, jmethodID method, ...) {}
	jclass GetObjectClass(jobject object) { return nullptr; }
	jboolean IsSameObject(jobject ref1, jobject ref2) { return false; }
};
using JNIEnv = _JNIEnv;
#endif

extern "C"
{
	JNIEXPORT jint Java_com_example_remotemouse_NativeBridge_Test(JNIEnv *env, jobject thiz);
	JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved);
}

JNIEnv * GetEnv(JavaVM *vm);
