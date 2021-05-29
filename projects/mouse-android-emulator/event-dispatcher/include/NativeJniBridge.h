#pragma once
#include <array>
#include <memory>
#include <vector>


#include "EventDispatcherRemoteApplication.h"
#include "JniImpl.h"

class NativeJniBridge;

class EventDispatcherApplication;

class ApplicationInputInterface
{
public:
	virtual ~ApplicationInputInterface() = default;

	virtual void UpdateFrame(int64_t elapsedMs) = 0;
	virtual void MouseMoved(int x, int y) = 0;
	virtual void RequestAvailiableConnectionList() = 0;
};

// todo: optimize to avoid using of dynamic memory here and minimize jni marshaling
struct AvailableConnectionData
{
	std::string		ip;
	std::string		hostname;
	int				port;
};

struct EventDispatcherState
{
	EventDispatcherRemoteApplication::State		state;
	EventDispatcherRemoteApplication::Error		error;
};

class ApplicationOutputInterface
{
public:
	virtual ~ApplicationOutputInterface() = default;

	virtual void ResponseAvailableConnectionList(const std::vector<AvailableConnectionData> & connectionList) = 0;
	virtual void StateUpdated(const EventDispatcherState & state) = 0;
};

class JavaToNativeBridge : public ApplicationInputInterface
{
public:
	JavaToNativeBridge(EventDispatcherApplication & eventDispatcher);

	void UpdateFrame(int64_t elapsedMs) override;
	void MouseMoved(int x, int y) override;
	void RequestAvailiableConnectionList() override;

private:
	EventDispatcherApplication &		_eventDispatcher;
};

class NativeToJavaBridge : ApplicationOutputInterface
{
public:
	NativeToJavaBridge(NativeJniBridge & nativeBridge, jobject nativeBridgeObject);
	~NativeToJavaBridge();

	void ResponseAvailableConnectionList(const std::vector<AvailableConnectionData>& connectionList) override;
	void StateUpdated(const EventDispatcherState& state) override;

private:
	enum CachedFieldIds
	{
		ApplicationStateNotInitialized,
		ApplicationStateInitialized,
		ApplicationStateWaitingForConnect,
		ApplicationStateConnected,
		ApplicationStateConnectionRequestTimeout,
		ApplicationStateDisconnected,
		ApplicationStateDisconnectedByTimeout,
		ApplicationStateErrorOccurred,

		ApplicationErrorStateNotInitialized,
		ApplicationErrorStateLocalPortBusy,
		ApplicationErrorStateRemotePortEmpty,
		ApplicationErrorStateInvalidIp,
		ApplicationErrorStateNoError,

		EventTouchShortType,

		CachedFieldNumber
	};
	enum CachedMethodIds
	{
		AvailableConnectionResponseMethod,
		StateUpdatedMethod,
		CachedMethodNumber
	};
	enum CachedClassIds
	{
		NativeBridgeClass,
		ApplicationStateClass,
		ApplicationErrorStateClass,
		CachedClassNumber
	};

	void Cache(jobject initializerObject);
	void InvalidateCache();

	NativeJniBridge &							_nativeBridge;
	std::array<jclass, CachedClassNumber>		_cachedClassIds;
	std::array<jfieldID, CachedFieldNumber>		_cachedFields;
	std::array<jmethodID, CachedMethodNumber>	_cachedMethods;
	jobject										_globalNativeBridgeObject;
};

class NativeJniBridge
{
public:
	NativeJniBridge(JavaVM * javaVm);

	EventDispatcherApplication *	GetApplication() const;
	ApplicationInputInterface *		GetApplicationInputInterface() const;
	ApplicationOutputInterface *	GetApplicationOutputInterface() const;

	JNIEnv * GetJniEnv() const;
	JavaVM * GetJavaVm() const;

private:
	JavaVM *											_javaVm = nullptr;
	std::unique_ptr<EventDispatcherApplication>			_eventDispatcher;
	std::unique_ptr<ApplicationInputInterface>			_inputInterface;
	std::unique_ptr<ApplicationOutputInterface>			_outputInterface;
};
