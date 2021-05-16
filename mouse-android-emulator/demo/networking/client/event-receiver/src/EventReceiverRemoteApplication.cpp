#include "EventReceiverRemoteApplication.h"

#include <SFML/Network/IpAddress.hpp>

#include "NetworkInterface.h"
#include "Peer.h"

#include "ProtoPackagesNamespace.h"
#include "Main.pb.h"
#include "Connection.pb.h"
#include "tools/Timer.h"

EventReceiverRemoteApplication::EventReceiverRemoteApplication(OperationManager& operationManager)
	: _operationManager(operationManager)
{
	_networkInterface = std::make_shared<NetworkInterface>(_operationManager);
	
	auto & peer = _networkInterface->GetPeer();
	peer.Subscribe(TypedCallback<const ProtoPackets::ConnectionRequest &>(_networkInterface, this, &EventReceiverRemoteApplication::OnConnectionRequested));
	peer.Subscribe(TypedCallback<const ProtoPackets::ConnectionPing &>(_networkInterface, this, &EventReceiverRemoteApplication::OnPing));
	peer.Subscribe(TypedCallback<const ProtoPackets::MousePositionMessage &>(_networkInterface, this, &EventReceiverRemoteApplication::OnMouseMoved));
}

EventReceiverRemoteApplication::~EventReceiverRemoteApplication() = default;

void EventReceiverRemoteApplication::SubscribeOnStatusChange(TypedCallback<State> callback)
{
	_stateChangedCallback = std::move(callback);
}

void EventReceiverRemoteApplication::Initialize(uint16_t localPort, uint16_t remotePort, TimeState connectionTimeout)
{
	if (localPort != GetPeer().OpenLocalConnection({localPort}))
	{
		SetErrorState(Error::LocalPortBusy);
		return;
	}

	GetPeer().OpenRemoteConnection(remotePort, "255.255.255.255");
	_pingTimeout = connectionTimeout;
	SetState(State::WaitingForConnectionRequest);
}

EventReceiverRemoteApplication::Error EventReceiverRemoteApplication::GetError() const
{
	return _error;
}

Network::Peer & EventReceiverRemoteApplication::GetPeer() const
{
	return _networkInterface->GetPeer();
}

void EventReceiverRemoteApplication::DisconnectByTimeout()
{
	if (_state == State::Connected)
	{
		SetState(State::DisconnectedByTimeout);
		SetState(State::WaitingForConnectionRequest);
		return;
	}

	std::cout << "EventReceiverRemoteApplication::DisconnectByTimeout called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventReceiverRemoteApplication::OnConnectionRequested([[maybe_unused]] const ProtoPackets::ConnectionRequest& connectionRequest)
{
	if (_state == State::WaitingForConnectionRequest)
	{
		ProtoPackets::ConnectionResponse response;
		response.set_ip(sf::IpAddress::getLocalAddress().toString());
		response.set_port(GetPeer().GetLocalPort());
		GetPeer().SendPacket(response);

		_pingTimeoutTimer = std::make_unique<Timer>(TypedCallback<>(_networkInterface, this, &EventReceiverRemoteApplication::OnPingTimeout), _pingTimeout, _operationManager);
		_pingTimeoutTimer->Restart();

		SetState(State::Connected);
		return;
	}

	std::cout << "EventReceiverRemoteApplication::OnConnectionRequested called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventReceiverRemoteApplication::OnPing([[maybe_unused]] const ProtoPackets::ConnectionPing& connectionPing)
{
	if (_state == State::Connected)
	{
		ProtoPackets::ConnectionPong pong;
		GetPeer().SendPacket(pong);

		_pingTimeoutTimer->Restart();
		return;
	}
	std::cout << "EventReceiverRemoteApplication::OnPing called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventReceiverRemoteApplication::OnPingTimeout()
{
	DisconnectByTimeout();
}

void EventReceiverRemoteApplication::OnMouseMoved(const ProtoPackets::MousePositionMessage& mousePosition)
{
	float x = mousePosition.x();
	float y = mousePosition.y();
	std::cout << '(' << x << ", " << y << ')' << '\n';
}

void EventReceiverRemoteApplication::SetState(State state)
{
	_state = state;
	if (state != State::ErrorOccured)
	{
		_error = Error::NoError;
	}
	_stateChangedCallback(state);
}

void EventReceiverRemoteApplication::SetErrorState(Error error)
{
	_error = error;
	SetState(State::ErrorOccured);
}
