#pragma once
#include "ApplicationOutputInterface.h"
#include "jni/jni.hpp"
#include "JniImpl.h"

class NativeJniBridge;

class NativeToJavaBridge : public ApplicationOutputInterface
{
public:
	NativeToJavaBridge(NativeJniBridge & nativeBridge, NativeBridgeObject & nativeBridgeObject);
	~NativeToJavaBridge();

	void ResponseAvailableConnectionList(const std::vector<AvailableConnectionData>& connectionList) override;
	void StateUpdated(const EventDispatcherState& state) override;

private:
	NativeJniBridge &							_nativeBridge;
	jni::WeakReference<NativeBridgeObject>		_globalNativeBridgeObject;
};
