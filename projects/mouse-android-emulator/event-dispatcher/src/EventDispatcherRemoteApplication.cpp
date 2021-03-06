#include "EventDispatcherRemoteApplication.h"

#include <utility>

#include "network/IpAddress.hpp"

#include "NetworkInterface.h"
#include "Peer.h"

#include "ProtoPackagesNamespace.h"
#include "Connection.pb.h"
#include "Main.pb.h"
#include "tools/Timer.h"

EventDispatcherRemoteApplication::EventDispatcherRemoteApplication(OperationManager & operationManager)
	: _operationManager(operationManager)
{
	_networkInterface = std::make_shared<NetworkInterface>(_operationManager);

	auto & peer = _networkInterface->GetPeer();
	peer.Subscribe(TypedCallback<const ProtoPackets::ConnectionResponse &>(_networkInterface, this, &EventDispatcherRemoteApplication::OnConnectionResponse));
	peer.Subscribe(TypedCallback<const ProtoPackets::ConnectionDisconnect &>(_networkInterface, this, &EventDispatcherRemoteApplication::OnDisconnect));
	peer.Subscribe(TypedCallback<const ProtoPackets::ConnectionPong &>(_networkInterface, this, &EventDispatcherRemoteApplication::OnPong));
}

EventDispatcherRemoteApplication::~EventDispatcherRemoteApplication() = default;

void EventDispatcherRemoteApplication::SubscribeOnStatusChange(TypedCallback<State> callback)
{
	_stateChangedCallback = std::move(callback);
}

//void EventDispatcherRemoteApplication::Initialize(uint16_t localPort, const std::string& remoteIp, uint16_t remotePort, TimeState pingTime, TimeState pongTimeout)
//{
//	if (localPort != GetPeer().OpenLocalConnection({localPort}))
//	{
//		SetErrorState(Error::LocalPortBusy);
//		return;
//	}
//
//	if (!GetPeer().OpenRemoteConnection(remotePort, remoteIp))
//	{
//		SetErrorState(Error::InvalidIp);
//		return;
//	}
//
//	_networkInterface->StartPacketProcessing();
//	_connectionStatus.ip = remoteIp;
//
//	_pingTime = pingTime;
//	_pingTimer = std::make_unique<Timer>(TypedCallback<>(_networkInterface, this, &EventDispatcherRemoteApplication::Ping), _pingTime, _operationManager);
//
//	_pongTimeout = pongTimeout;
//	_pongTimeoutTimer = std::make_unique<Timer>(TypedCallback<>(_networkInterface, this, &EventDispatcherRemoteApplication::OnPongTimedOut), _pongTimeout, _operationManager);
//
//	SetState(State::Idle);
//}

//void EventDispatcherRemoteApplication::TryConnect(TimeState timeout, uint32_t retries)
//{
//	if (_state == State::Idle)
//	{
//		_connectionHandler.delay = timeout;
//		_connectionHandler.retries = retries;
//		_connectionRequestTimeoutTimer = std::make_unique<Timer>(TypedCallback<>(_networkInterface, this, &EventDispatcherRemoteApplication::OnConnectionRequestTimedOut), timeout, _operationManager);
//		TryConnect();
//		return;
//	}
//	std::cout << "EventDispatcherRemoteApplication::TryConnect called from incorrect state " << static_cast<int>(_state) << ".\n";
//}

void EventDispatcherRemoteApplication::SetScreen(int x, int y)
{
	_screenX = x;
	_screenY = y;
}

void EventDispatcherRemoteApplication::SendMousePosition(int x, int y)
{
	if (_state == State::Connected)
	{
		if (_screenX > 0 && _screenY > 0)
		{
			ProtoPackets::MousePositionMessage message;
			message.set_x(static_cast<float>(x) / static_cast<float>(_screenX));
			message.set_y(static_cast<float>(y) / static_cast<float>(_screenY));
			GetPeer().SendPacket(message);
		}
	}
}

EventDispatcherRemoteApplication::Error EventDispatcherRemoteApplication::GetError() const
{
	return _error;
}

Network::Peer & EventDispatcherRemoteApplication::GetPeer() const
{
	return _networkInterface->GetPeer();
}

void EventDispatcherRemoteApplication::TryConnect()
{
	_connectionRequestTimeoutTimer->Restart();
	SetState(State::WaitingForConnect);

	ProtoPackets::ConnectionRequest request;
	request.set_ip(sf::IpAddress::getLocalAddress().toString());
	request.set_port(GetPeer().GetLocalPort());
	GetPeer().SendPacket(request);
}

void EventDispatcherRemoteApplication::TryConnectAgain()
{
	if (_connectionStatus.retriesLeft > 0)
	{
		_connectionStatus.retriesLeft -= 1;
		TryConnect();
	}
}

void EventDispatcherRemoteApplication::Disconnect()
{
	// todo: send packet Disconnect
	SetState(State::Disconnected);
}

void EventDispatcherRemoteApplication::DisconnectByTimeout()
{
	SetState(State::DisconnectedByTimeout);
}

void EventDispatcherRemoteApplication::Ping()
{
	if (_state == State::Connected)
	{
		_pongTimeoutTimer->Restart();
		ProtoPackets::ConnectionPing ping;
		GetPeer().SendPacket(ping);
	}
}

void EventDispatcherRemoteApplication::OnConnectionResponse(const ProtoPackets::ConnectionResponse& connectionResponse)
{
	if (_state == State::WaitingForConnect)
	{
		_connectionStatus.ip = connectionResponse.ip();
		_connectionStatus.port = connectionResponse.port();
		_connectionStatus.retriesLeft = _options.connectionRetries;
		_connectionRequestTimeoutTimer->Reset();
		SetState(State::Connected);

		Ping();
		return;
	}
	if (_state == State::SearchingConnections)
	{
		if (!connectionResponse.ip().empty() && !connectionResponse.hostname().empty())
		{
			_availableConnectionList.emplace_back(connectionResponse.ip(), connectionResponse.hostname(), connectionResponse.port());
		}
		return;
	}
	std::cout << "EventDispatcherRemoteApplication::OnConnectionResponse called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventDispatcherRemoteApplication::OnPong([[maybe_unused]] const ProtoPackets::ConnectionPong& pong)
{
	if (_state == State::Connected)
	{
		_pingTimer->Restart();
		_pongTimeoutTimer->Reset();
		return;
	}
	std::cout << "EventDispatcherRemoteApplication::OnPong called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventDispatcherRemoteApplication::OnDisconnect([[maybe_unused]] const ProtoPackets::ConnectionDisconnect& disconnect)
{
	if (_state == State::Connected)
	{
		Disconnect();
		return;
	}
	std::cout << "EventDispatcherRemoteApplication::OnDisconnect called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventDispatcherRemoteApplication::OnPongTimedOut()
{
	if (_state == State::Connected)
	{
		DisconnectByTimeout();
		return;
	}
	std::cout << "EventDispatcherRemoteApplication::OnPongTimedOut called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventDispatcherRemoteApplication::OnConnectionRequestTimedOut()
{
	if (_state == State::WaitingForConnect)
	{
		SetState(State::ConnectionRequestTimeout);
		TryConnectAgain();
		return;
	}
	std::cout << "EventDispatcherRemoteApplication::OnConnectionRequestTimedOut called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventDispatcherRemoteApplication::SetState(State state)
{
	_state = state;
	if (state != State::ErrorOccurred)
	{
		_error = Error::NoError;
	}
	_stateChangedCallback(state);
}

void EventDispatcherRemoteApplication::SetErrorState(Error error)
{
	_error = error;
	SetState(State::ErrorOccurred);
}

void EventDispatcherRemoteApplication::Initialize(const EventDispatcherOptions &options) {
	_options = options;

	const uint16_t localPort = _options.localPort;
    if (localPort != GetPeer().OpenLocalConnection({localPort}))
    {
        SetErrorState(Error::LocalPortBusy);
        return;
    }

    if (!GetPeer().OpenRemoteConnection(_options.remotePort, sf::IpAddress::Broadcast.toString()))
    {
        SetErrorState(Error::InvalidIp);
        return;
    }

    _networkInterface->StartPacketProcessing();
    _pingTimer = std::make_unique<Timer>(TypedCallback<>(_networkInterface, this, &EventDispatcherRemoteApplication::Ping), _options.pingTime, _operationManager);
    _pongTimeoutTimer = std::make_unique<Timer>(TypedCallback<>(_networkInterface, this, &EventDispatcherRemoteApplication::OnPongTimedOut), _options.pongTimeout, _operationManager);
	_availableConnectionTimer = std::make_unique<Timer>(TypedCallback<>(_networkInterface, this, &EventDispatcherRemoteApplication::OnConnectionsSearchTimedOut), _options.searchConnectionsTime, _operationManager);
	_connectionRequestTimeoutTimer = std::make_unique<Timer>(TypedCallback<>(_networkInterface, this, &EventDispatcherRemoteApplication::OnConnectionRequestTimedOut), _options.connectionTimeout, _operationManager);

    SetState(State::Idle);
}

void EventDispatcherRemoteApplication::RequestConnectionList() {
//    if (_state != State::Idle && _state != State::ErrorOccurred)
//    {
//        SetErrorState(Error::NoError);
//        return;
//    }

    _availableConnectionList.clear();
	_availableConnectionTimer->Restart();
	SetState(State::SearchingConnections);

    ProtoPackets::ConnectionRequest request;
    request.set_ip(sf::IpAddress::getLocalAddress().toString());
    request.set_port(GetPeer().GetLocalPort());
    request.set_hostname(_options.name);
    request.set_search(true);
    GetPeer().SendPacket(request);
}

void EventDispatcherRemoteApplication::SubscribeOnConnectionList(TypedCallback<const std::vector<AvailableConnectionData> &> callback) {
    _availableConnectionCallback = std::move(callback);
}

void EventDispatcherRemoteApplication::Connect(const std::string &ip, uint16_t port) {
	if (_state == State::Connected)
	{
		Disconnect();
	}

//	const bool idle = _state == State::Idle;
//	const bool disconnected = _state == State::Disconnected || _state != State::DisconnectedByTimeout;
//	const bool error = _state == State::ErrorOccurred;
//	const bool fromValidState = idle || disconnected || error;
//	if (!fromValidState)
//	{
//		SetErrorState(Error::InvalidState);
//		return;
//	}

    if (!GetPeer().OpenRemoteConnection(port, ip))
    {
        SetErrorState(Error::InvalidIp);
        return;
    }
	_connectionStatus.port = port;
	_connectionStatus.ip = ip;
	TryConnect();
}

void EventDispatcherRemoteApplication::OnConnectionsSearchTimedOut() {
	SetState(State::Idle);
	_availableConnectionCallback(_availableConnectionList);
}

void EventDispatcherRemoteApplication::SendMouseClick(bool leftButton)
{
	if (_state == State::Connected)
	{
		ProtoPackets::MouseClickMessage message;
		if (leftButton)
		{
			message.set_button(my::proto::package::MouseClickMessage_ButtonType_LEFT_BUTTON);
		}
		else
		{
			message.set_button(my::proto::package::MouseClickMessage_ButtonType_RIGHT_BUTTON);
		}
		GetPeer().SendPacket(message);
	}
}

AvailableConnectionData::AvailableConnectionData(std::string ip, std::string hostname, uint16_t port) noexcept
	: ip(std::move(ip))
	, hostname(std::move(hostname))
	, port(port)
{}
