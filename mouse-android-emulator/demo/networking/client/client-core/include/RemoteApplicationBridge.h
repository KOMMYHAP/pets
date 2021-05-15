#pragma once
#include <memory>
#include <optional>
#include <string>
#include "ProtoPackagesNamespace.h"
#include "tools/Callback.h"

class RemoteApplicationBase;
class Timer;
class ParsedCommandLine;
class OperationManager;
class NetworkInterface;
class EventDispatcherRemoteApplication;

namespace my::proto::package
{
	class ConnectionResponse;
}

enum class RemoteBridgeState
{
	NotInitialized,
	Initialized,
	WaitingForConnect,
	Connected,
	ConnectionTimedOut,
	Disconnected,
	ErrorOccured
};

enum class RemoteBridgeError
{
	NotInitialized,
	LocalPortsBusy,
	RemotePortEmpty,
	NoError,
};

struct ConnectionStatus
{
	bool			wasConnected = false;
	uint16_t		port = 0;
	std::string		ip;
};

class RemoteApplicationBridge
{
public:
	RemoteApplicationBridge(OperationManager & operationManager);
	~RemoteApplicationBridge();

	void Initialize(std::unique_ptr<RemoteApplicationBase> remoteApplication);
	void OpenLocalConnection(const std::vector<uint16_t> & localPorts, const TypedCallback<RemoteBridgeState> & callback);
	void StartBroadcasting(const std::vector<uint16_t> & remotePorts);
	void DirectConnect(const std::string & remoteIp, const std::vector<uint16_t> & remotePorts);

	void CloseConnection();
	void Reconnect();

	RemoteApplicationBase * GetRemoteApplication() const;
	RemoteBridgeError GetError() const;

private:
	void Connect(const std::string & remoteIp, const std::vector<uint16_t> & remotePorts);
	
	void OnConnected(const ProtoPackets::ConnectionResponse & response);
	void OnConnectionTimeout();

	void SetState(RemoteBridgeState state);
	bool SetErrorStateIfNotInitialized();
	void SetErrorState(RemoteBridgeError error);

	RemoteBridgeState						_state = RemoteBridgeState::NotInitialized;
	RemoteBridgeError						_error = RemoteBridgeError::NoError;
	ConnectionStatus						_connectionStatus;
	TypedCallback<RemoteBridgeState>		_stateChangedCallback;

	std::unique_ptr<Timer>					_timeoutTimer;
	
	std::shared_ptr<NetworkInterface>		_networkInterface;
	std::unique_ptr<RemoteApplicationBase> 	_remoteApplication;
	OperationManager &						_operationManager;
};
