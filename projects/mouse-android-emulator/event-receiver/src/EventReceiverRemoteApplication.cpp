#include "EventReceiverRemoteApplication.h"

#include "NetworkInterface.h"
#include "Peer.h"

#include "ProtoPackagesNamespace.h"
#include "Main.pb.h"
#include "Connection.pb.h"
#include "tools/Timer.h"

#define WIN32_LEAN_AND_MEAN
#include <array>
#include <Tracy.hpp>
#include <Windows.h>

#include "network/IpAddress.hpp"

extern BOOL SendMouseClick(bool left);

namespace
{
	std::string GetUserNameWrapper()
	{
		static std::string username;
		if (username.empty())
		{
			std::array<char, 32767> buffer;
			DWORD bufferCount = buffer.size();
			if (GetUserNameA(buffer.data(), &bufferCount))
			{
				if (bufferCount >= 1)
				{
					username.assign(buffer.data(), bufferCount - 1);
				}
			}
		}
		return username;
	}
}

EventReceiverRemoteApplication::EventReceiverRemoteApplication(OperationManager& operationManager)
	: _operationManager(operationManager)
{
	_networkInterface = std::make_shared<NetworkInterface>(_operationManager);
	
	auto & peer = _networkInterface->GetPeer();
	peer.Subscribe(TypedCallback<const ProtoPackets::ConnectionRequest &>(_networkInterface, this, &EventReceiverRemoteApplication::OnConnectionRequested));
	peer.Subscribe(TypedCallback<const ProtoPackets::ConnectionPing &>(_networkInterface, this, &EventReceiverRemoteApplication::OnPing));
	peer.Subscribe(TypedCallback<const ProtoPackets::MousePositionMessage &>(_networkInterface, this, &EventReceiverRemoteApplication::OnMouseMoved));
	peer.Subscribe(TypedCallback<const ProtoPackets::MouseClickMessage &>(_networkInterface, this, &EventReceiverRemoteApplication::OnMouseClicked));
}

EventReceiverRemoteApplication::~EventReceiverRemoteApplication() = default;

void EventReceiverRemoteApplication::SubscribeOnStatusChange(TypedCallback<State> callback)
{
	_stateChangedCallback = std::move(callback);
}

void EventReceiverRemoteApplication::Initialize(uint16_t localPort, uint16_t remotePort, TimeState idleTimeout)
{
	if (localPort != GetPeer().OpenLocalConnection({localPort}))
	{
		SetErrorState(Error::LocalPortBusy);
		return;
	}

	_connectionStatus.remotePort = remotePort;
	_idleTimeout = idleTimeout;

	ChangeStateWaitingForConnect();

	_networkInterface->StartPacketProcessing();
}

EventReceiverRemoteApplication::Error EventReceiverRemoteApplication::GetError() const
{
	return _error;
}

Network::Peer & EventReceiverRemoteApplication::GetPeer() const
{
	return _networkInterface->GetPeer();
}

void EventReceiverRemoteApplication::DisconnectByIdle()
{
	if (_state == State::Connected)
	{
		// bug: Packet sending won't be performed because CloseRemoteConnection will close connection earlier.
		// ProtoPackets::ConnectionDisconnect disconnect;
		// GetPeer().SendPacket(disconnect);
		// GetPeer().CloseRemoteConnection();

		SetState(State::DisconnectedByIdle);

		ChangeStateWaitingForConnect();
		return;
	}

	std::cout << "EventReceiverRemoteApplication::DisconnectByTimeout called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventReceiverRemoteApplication::OnConnectionRequested([[maybe_unused]] const ProtoPackets::ConnectionRequest& connectionRequest)
{
	if (_state == State::WaitingForConnectionRequest)
	{
		const bool remoteChanged = GetPeer().OpenRemoteConnection(connectionRequest.port(), connectionRequest.ip());
		if (!remoteChanged){
			SetErrorState(Error::RemoteIpInvalid);
			return;
		}

		ProtoPackets::ConnectionResponse response;
		response.set_ip(sf::IpAddress::getLocalAddress().toString());
		response.set_port(GetPeer().GetLocalPort());
		response.set_hostname(GetUserNameWrapper());
		GetPeer().SendPacket(response);
		
		if (!connectionRequest.search())
		{
			_idleTimer = std::make_unique<Timer>(TypedCallback<>(_networkInterface, this, &EventReceiverRemoteApplication::DisconnectByIdle), _idleTimeout, _operationManager);
			_idleTimer->Restart();
			SetState(State::Connected);
		}
		
		return;
	}

	if (_state == State::Connected)
	{
		std::cout << "Another connection request has been ignored due to already established connection.\n";
		return;
	}
	std::cout << "EventReceiverRemoteApplication::OnConnectionRequested called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventReceiverRemoteApplication::OnPing([[maybe_unused]] const ProtoPackets::ConnectionPing& connectionPing)
{
	if (_state == State::Connected)
	{
		_idleTimer->Restart();

		ProtoPackets::ConnectionPong pong;
		GetPeer().SendPacket(pong);
		return;
	}
	std::cout << "EventReceiverRemoteApplication::OnPing called from incorrect state " << static_cast<int>(_state) << ".\n";
}

void EventReceiverRemoteApplication::OnMouseMoved(const ProtoPackets::MousePositionMessage& mousePosition)
{
	ZoneScopedN("On Mouse Moved");
	if (_state == State::Connected)
	{
		_idleTimer->Restart();

		if (_prevX == 0 && _prevY == 0)
		{
			POINT pos;
			if (GetCursorPos(&pos))
			{
				_prevX = pos.x;
				_prevY = pos.y;
			}
		}

		static int s_width = GetSystemMetrics(SM_CXSCREEN);
		static int s_height = GetSystemMetrics(SM_CYSCREEN);
		
		float offsetX = mousePosition.x() * s_width;
		float offsetY = mousePosition.y() * s_height;

		_prevX += offsetX;
		_prevY += offsetY;

		SetCursorPos(_prevX, _prevY);
	}
}

void EventReceiverRemoteApplication::OnMouseClicked(const ProtoPackets::MouseClickMessage& mouseClick)
{
	if (_state == State::Connected)
	{
		_idleTimer->Restart();

		const bool leftButton = mouseClick.button() == my::proto::package::MouseClickMessage_ButtonType_LEFT_BUTTON;
		SendMouseClick(leftButton);
	}
}

void EventReceiverRemoteApplication::ChangeStateWaitingForConnect()
{
	GetPeer().OpenRemoteConnection(_connectionStatus.remotePort, sf::IpAddress::Any.toString());
	SetState(State::WaitingForConnectionRequest);
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
