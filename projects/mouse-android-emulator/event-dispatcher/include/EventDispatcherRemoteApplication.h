#pragma once
#include <memory>
#include <string>

#include "ProtoPackagesNamespace.h"
#include "tools/Callback.h"
#include "tools/TimeState.h"

class OperationManager;
class Timer;
class NetworkInterface;

namespace my::proto::package
{
	class ConnectionResponse;
	class ConnectionPong;
	class ConnectionDisconnect;
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

struct EventDispatcherOptions
{
	uint16_t 		localPort = 44332;

	TimeState 		pingTime = TimeState::Seconds(3);
	TimeState 		pongTimeout = TimeState::Seconds(5);

	TimeState 		connectionTimeout = TimeState::Seconds(3);
	uint32_t 		connectionRetries = 5;
};

struct AvailableConnectionData
{
	std::string		ip;
	std::string		hostname;
	uint16_t		port = 0;
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
		ConnectionRequestTimeout,
		Disconnected,
		DisconnectedByTimeout,
		ErrorOccurred
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
	~EventDispatcherRemoteApplication();

	void SubscribeOnStatusChange(TypedCallback<State> callback);

	void Initialize(const EventDispatcherOptions & options);
	void Initialize(uint16_t localPort, const std::string & remoteIp, uint16_t remotePort, TimeState pingTime, TimeState pongTimeout);

	void RequestConnectionList();
	void SubscribeOnConnectionList(TypedCallback<const std::vector<AvailableConnectionData> &> callback);

	void Connect(const std::string & ip, uint16_t port);

	void SetScreen(int x, int y);
	void SendMousePosition(int x, int y);

	Error GetError() const;

private:
	Network::Peer & GetPeer() const;

	void TryConnect();
	void TryConnectAgain();
	
	void Disconnect();
	void DisconnectByTimeout();
	void Ping();
	
	void OnConnectionResponse(const ProtoPackets::ConnectionResponse & connectionResponse);
	void OnPong(const ProtoPackets::ConnectionPong & pong);
	void OnDisconnect(const ProtoPackets::ConnectionDisconnect & disconnect);
	void OnPongTimedOut();
	void OnConnectionRequestTimedOut();

	void SetState(State state);
	void SetErrorState(Error error);

	State									_state = State::NotInitialized;
	Error									_error = Error::NoError;
	ConnectionStatus						_connectionStatus;
	ConnectionHandler						_connectionHandler;
	TypedCallback<State>					_stateChangedCallback;
	int										_screenX = 0;
	int										_screenY = 0;
	
	std::unique_ptr<Timer>					_connectionRequestTimeoutTimer;
	TimeState								_pingTime;
	std::unique_ptr<Timer>					_pingTimer;
	TimeState								_pongTimeout;
	std::unique_ptr<Timer>					_pongTimeoutTimer;
	
	OperationManager &						_operationManager;
	std::shared_ptr<NetworkInterface>		_networkInterface;
};
