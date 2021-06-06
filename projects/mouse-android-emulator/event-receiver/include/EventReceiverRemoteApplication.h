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
	class MouseClickMessage;
}

namespace Network {
	class Peer;
}

struct ConnectionStatus
{
	uint16_t		remotePort = 0;
};

class EventReceiverRemoteApplication
{
public:
	enum class State
	{
		NotInitialized,
		WaitingForConnectionRequest,
		Connected,
		Disconnected,
		DisconnectedByIdle,
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
	void Initialize(uint16_t localPort, uint16_t remotePort, TimeState idleTimeout);

	Error GetError() const;

private:
	Network::Peer & GetPeer() const;

	void DisconnectByIdle();
	
	void OnConnectionRequested(const ProtoPackets::ConnectionRequest & connectionRequest);
	void OnPing(const ProtoPackets::ConnectionPing & connectionPing);

	void OnMouseMoved(const ProtoPackets::MousePositionMessage & mousePosition);
	void OnMouseClicked(const ProtoPackets::MouseClickMessage & mouseClick);

	void ChangeStateWaitingForConnect();
	
	void SetState(State state);
	void SetErrorState(Error error);

	State									_state = State::NotInitialized;
	Error									_error = Error::NoError;
	ConnectionStatus						_connectionStatus;
	float									_prevX = 0.0f;
	float									_prevY = 0.0f;
	TypedCallback<State>					_stateChangedCallback;
	TimeState								_idleTimeout;
	
	OperationManager &						_operationManager;
	std::unique_ptr<Timer>					_idleTimer;
	std::shared_ptr<NetworkInterface>		_networkInterface;
};
