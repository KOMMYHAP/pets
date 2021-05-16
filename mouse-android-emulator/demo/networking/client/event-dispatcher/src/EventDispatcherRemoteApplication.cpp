#include "EventDispatcherRemoteApplication.h"

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
}

void EventDispatcherRemoteApplication::SubscribeOnStatusChange(TypedCallback<State> callback)
{
	_stateChangedCallback = std::move(callback);
}

void EventDispatcherRemoteApplication::Initialize(uint16_t localPort, const std::string& remoteIp, uint16_t remotePort)
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

	_connectionStatus.ip = remoteIp;
	SetState(State::Initialized);
}

void EventDispatcherRemoteApplication::TryConnect(TimeState timeout, uint32_t retries)
{
	_connectionHandler.delay = timeout;
	_connectionHandler.retries = retries;

	_connectionTimedOutTimer->Restart();
	SetState(State::WaitingForConnect);
	
	ProtoPackets::ConnectionRequest request;
	GetPeer().SendPacket(request);
}

void EventDispatcherRemoteApplication::SendMousePosition(float x, float y)
{
	ProtoPackets::MousePositionMessage message;
	message.set_x(x);
	message.set_y(y);
	GetPeer().SendPacket(message);
}

EventDispatcherRemoteApplication::Error EventDispatcherRemoteApplication::GetError() const
{
	return _error;
}

Network::Peer & EventDispatcherRemoteApplication::GetPeer() const
{
	return _networkInterface->GetPeer();
}

void EventDispatcherRemoteApplication::ReconnectByTimeout()
{
	if (_connectionHandler.retries > 0)
	{
		TryConnect(_connectionHandler.delay, _connectionHandler.retries - 1);
	}
}

void EventDispatcherRemoteApplication::OnConnectionResponse(const ProtoPackets::ConnectionResponse& connectionResponse)
{
	if (_state == State::WaitingForConnect)
	{
		_connectionStatus.wasConnected = true;
		_connectionStatus.ip = connectionResponse.ip();
		_connectionStatus.port = connectionResponse.port();
		_connectionTimedOutTimer->Reset();
		SetState(State::Connected);
		return;
	}
	std::cout << "EventDispatcherRemoteApplication::OnConnectionResponse called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventDispatcherRemoteApplication::OnConnectionRequestTimedOut()
{
	if (_state == State::WaitingForConnect)
	{
		SetState(State::ConnectionTimedOut);
		ReconnectByTimeout();
		return;
	}
	std::cout << "EventDispatcherRemoteApplication::OnConnectionRequestTimedOut called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventDispatcherRemoteApplication::SetState(State state)
{
	_state = state;
	if (state != State::ErrorOccured)
	{
		_error = Error::NoError;
	}
	_stateChangedCallback(state);
}

// bool EventDispatcherRemoteApplication::SetErrorStateIfNotInitialized()
// {
// 	if (_state != State::NotInitialized)
// 	{
// 		return false;
// 	}
// 	SetErrorState(Error::NotInitialized);
// 	return true;
// }

void EventDispatcherRemoteApplication::SetErrorState(Error error)
{
	_error = error;
	SetState(State::ErrorOccured);
}
