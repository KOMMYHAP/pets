#include "EventDispatcherRemoteApplication.h"

#include "network/IpAddress.hpp"

#include "NetworkInterface.h"
#include "Peer.h"

#include "ProtoPackagesNamespace.h"
#include "Connection.pb.h"
#include "Main.pb.h"
#include "tools/Timer.h"

// #define WIN32_LEAN_AND_MEAN
// #include <Windows.h>

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

void EventDispatcherRemoteApplication::Initialize(uint16_t localPort, const std::string& remoteIp, uint16_t remotePort, TimeState pingTime, TimeState pongTimeout)
{
	if (localPort != GetPeer().OpenLocalConnection({localPort}))
	{
		SetErrorState(Error::LocalPortBusy);
		return;
	}

	if (!GetPeer().OpenRemoteConnection(remotePort, remoteIp))
	{
		SetErrorState(Error::InvalidIp);
		return;
	}

	_networkInterface->StartPacketProcessing();
	_connectionStatus.ip = remoteIp;

	_pingTime = pingTime;
	_pingTimer = std::make_unique<Timer>(TypedCallback<>(_networkInterface, this, &EventDispatcherRemoteApplication::Ping), _pingTime, _operationManager);

	_pongTimeout = pongTimeout;
	_pongTimeoutTimer = std::make_unique<Timer>(TypedCallback<>(_networkInterface, this, &EventDispatcherRemoteApplication::OnPongTimedOut), _pongTimeout, _operationManager);

	SetState(State::Initialized);
}

void EventDispatcherRemoteApplication::TryConnect(TimeState timeout, uint32_t retries)
{
	if (_state == State::Initialized)
	{
		_connectionHandler.delay = timeout;
		_connectionHandler.retries = retries;
		_connectionRequestTimeoutTimer = std::make_unique<Timer>(TypedCallback<>(_networkInterface, this, &EventDispatcherRemoteApplication::OnConnectionRequestTimedOut), timeout, _operationManager);
		TryConnect();
		return;
	}
	std::cout << "EventDispatcherRemoteApplication::TryConnect called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventDispatcherRemoteApplication::SetScreen(int x, int y)
{
	_screenX = x;
	_screenY = y;
}

void EventDispatcherRemoteApplication::SendMousePosition(int x, int y)
{
	if (_state == State::Connected)
	{
		// static int s_width = GetSystemMetrics(SM_CXSCREEN);
		// static int s_height = GetSystemMetrics(SM_CYSCREEN);
		//
		// POINT point;
		// if (GetCursorPos(&point))
		// {
		// 	x = static_cast<float>(point.x);
		// 	y = static_cast<float>(point.y);
		//
		// 	ProtoPackets::MousePositionMessage message;
		// 	message.set_x(x / s_width);
		// 	message.set_y(y / s_height);
		// 	GetPeer().SendPacket(message);
		// }

		if (_screenX > 0 && _screenY > 0)
		{
			ProtoPackets::MousePositionMessage message;
			message.set_x(x / static_cast<float>(_screenX));
			message.set_y(y / static_cast<float>(_screenY));
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
	if (_connectionHandler.retries > 0)
	{
		_connectionHandler.retries -= 1;
		TryConnect();
	}
}

void EventDispatcherRemoteApplication::Disconnect()
{
	SetState(State::Disconnected);
}

void EventDispatcherRemoteApplication::DisconnectByTimeout()
{
	SetState(State::DisconnectedByTimeout);
}

void EventDispatcherRemoteApplication::Ping()
{
	_pongTimeoutTimer->Restart();
	ProtoPackets::ConnectionPing ping;
	GetPeer().SendPacket(ping);
}

void EventDispatcherRemoteApplication::OnConnectionResponse(const ProtoPackets::ConnectionResponse& connectionResponse)
{
	if (_state == State::WaitingForConnect)
	{
		_connectionStatus.wasConnected = true;
		_connectionStatus.ip = connectionResponse.ip();
		_connectionStatus.port = connectionResponse.port();
		_connectionRequestTimeoutTimer->Reset();
		SetState(State::Connected);

		Ping();
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
