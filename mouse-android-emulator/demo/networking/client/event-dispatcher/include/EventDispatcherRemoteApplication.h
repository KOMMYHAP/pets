#pragma once
#include <memory>
#include <string>

#include "ProtoPackagesNamespace.h"
#include "RemoteApplicationBase.h"
#include "tools/Callback.h"
#include "tools/TimeState.h"

class OperationManager;
class Timer;
class NetworkInterface;

namespace my::proto::package
{
	class ConnectionResponse;
}

namespace Network {
	class Peer;
}

struct ConnectionStatus
{
	bool			wasConnected = false;
	uint16_t		port = 0;
	std::string		ip;
};

struct ConnectionHandler
{
	uint32_t		retries = 0;
	TimeState		delay;
};

class EventDispatcherRemoteApplication
{
public:
	enum class State
	{
		NotInitialized,
		Initialized,
		WaitingForConnect,
		Connected,
		ConnectionTimedOut,
		Disconnected,
		ErrorOccured
	};

	enum class Error
	{
		NotInitialized,
		LocalPortBusy,
		RemotePortEmpty,
		InvalidIp,
		NoError,
	};
	
	EventDispatcherRemoteApplication(OperationManager & operationManager);

	void SubscribeOnStatusChange(TypedCallback<State> callback);
	void Initialize(uint16_t localPort, const std::string & remoteIp, uint16_t remotePort);

	void TryConnect(TimeState timeout, uint32_t retries);
	void SendMousePosition(float x, float y);

	Error GetError() const;

private:
	Network::Peer & GetPeer() const;

	void ReconnectByTimeout();
	
	void OnConnectionResponse(const ProtoPackets::ConnectionResponse & connectionResponse);
	void OnConnectionRequestTimedOut();

	void SetState(State state);
	// bool SetErrorStateIfNotInitialized();
	void SetErrorState(Error error);

	State									_state = State::NotInitialized;
	Error									_error = Error::NoError;
	ConnectionStatus						_connectionStatus;
	ConnectionHandler						_connectionHandler;
	TypedCallback<State>					_stateChangedCallback;
	
	OperationManager &						_operationManager;
	std::unique_ptr<Timer>					_connectionTimedOutTimer;
	std::shared_ptr<NetworkInterface>		_networkInterface;
};
