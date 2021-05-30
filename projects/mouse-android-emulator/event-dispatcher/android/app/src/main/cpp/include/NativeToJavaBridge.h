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
	/*enum CachedFieldIds
	{
//		ApplicationStateNotInitialized,
//		ApplicationStateInitialized,
//		ApplicationStateWaitingForConnect,
//		ApplicationStateConnected,
//		ApplicationStateConnectionRequestTimeout,
//		ApplicationStateDisconnected,
//		ApplicationStateDisconnectedByTimeout,
//		ApplicationStateErrorOccurred,
//
//		ApplicationErrorStateNotInitialized,
//		ApplicationErrorStateLocalPortBusy,
//		ApplicationErrorStateRemotePortEmpty,
//		ApplicationErrorStateInvalidIp,
//		ApplicationErrorStateNoError,
//
//		EventTouchShortType,

		CachedFieldNumber
	};
	enum CachedMethodIds
	{
//		AvailableConnectionResponseMethod,
//		StateUpdatedMethod,
		CachedMethodNumber
	};
	enum CachedClassIds
	{
//		NativeBridgeClass,
//		ApplicationStateClass,
//		ApplicationErrorStateClass,
//		AvailableConnectionDataClass,
		CachedClassNumber
	};*/

	void Cache();
	void InvalidateCache();

	NativeJniBridge &							_nativeBridge;
//	std::array<jclass, CachedClassNumber>		_cachedClassIds{};
//	std::array<jfieldID, CachedFieldNumber>		_cachedFields{};
//	std::array<jmethodID, CachedMethodNumber>	_cachedMethods{};
	jni::WeakReference<NativeBridgeObject>		_globalNativeBridgeObject;
};
