#pragma once
#include <memory>

#include "ProtoPackagesNamespace.h"
#include "tools/Callback.h"
#include "tools/TimeState.h"

class OperationManager;
class Timer;
class NetworkInterface;

namespace my::proto::package
{
	class ConnectionRequest;
	class ConnectionPing;
	class MousePositionMessage;
}

namespace Network {
	class Peer;
}

class EventReceiverRemoteApplication
{
public:
	enum class State
	{
		NotInitialized,
		WaitingForConnectionRequest,
		Connected,
		Disconnected,
		DisconnectedByTimeout,
		ErrorOccured
	};

	enum class Error
	{
		LocalPortBusy,
		RemoteIpInvalid,
		NoError,
	};
	
	EventReceiverRemoteApplication(OperationManager & operationManager);
	~EventReceiverRemoteApplication();

	void SubscribeOnStatusChange(TypedCallback<State> callback);
	void Initialize(uint16_t localPort, uint16_t remotePort, TimeState connectionTimeout);

	Error GetError() const;

private:
	Network::Peer & GetPeer() const;

	void DisconnectByTimeout();
	
	void OnConnectionRequested(const ProtoPackets::ConnectionRequest & connectionRequest);
	void OnPing(const ProtoPackets::ConnectionPing & connectionPing);
	void OnPingTimeout();

	void OnMouseMoved(const ProtoPackets::MousePositionMessage & mousePosition);

	void SetState(State state);
	void SetErrorState(Error error);

	State									_state = State::NotInitialized;
	Error									_error = Error::NoError;
	TypedCallback<State>					_stateChangedCallback;
	TimeState								_pingTimeout;
	
	OperationManager &						_operationManager;
	std::unique_ptr<Timer>					_pingTimeoutTimer;
	std::shared_ptr<NetworkInterface>		_networkInterface;
};
