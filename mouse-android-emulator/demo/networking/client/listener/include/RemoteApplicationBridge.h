#pragma once
#include <memory>
#include <optional>
#include <string>
#include "ProtoPackagesNamespace.h"
#include "tools/Callback.h"

class Timer;
class ParsedCommandLine;
class OperationManager;
class NetworkInterface;
class RemoteApplication;

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

	void Initialize(const std::vector<uint16_t> & localPorts, const TypedCallback<RemoteBridgeState> & callback);
	void StartBroadcasting(const std::vector<uint16_t> & remotePorts);
	void DirectConnect(const std::string & remoteIp, const std::vector<uint16_t> & remotePorts);

	void CloseConnection();
	void Reconnect();

	RemoteApplication * GetRemoteApplication() const;
	RemoteBridgeError GetError() const;

private:
	void OnConnected(const ProtoPackets::ConnectionResponse & response);
	void OnConnectionTimeout();

	void SetState(RemoteBridgeState state);
	void SetErrorState(RemoteBridgeError error);

	RemoteBridgeState					_state = RemoteBridgeState::NotInitialized;
	RemoteBridgeError					_error = RemoteBridgeError::NoError;
	ConnectionStatus					_connectionStatus;
	TypedCallback<RemoteBridgeState>	_stateChangedCallback;

	std::unique_ptr<Timer>				_timeoutTimer;
	
	std::shared_ptr<NetworkInterface>	_networkInterface;
	std::unique_ptr<RemoteApplication> 	_remoteApplication;
	OperationManager &					_operationManager;
};
