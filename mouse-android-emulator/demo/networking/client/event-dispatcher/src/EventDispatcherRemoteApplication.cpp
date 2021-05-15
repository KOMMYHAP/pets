#include "EventDispatcherRemoteApplication.h"

#include "NetworkInterface.h"
#include "Peer.h"

#include "ProtoPackagesNamespace.h"
#include "Main.pb.h"

EventDispatcherRemoteApplication::EventDispatcherRemoteApplication(const std::shared_ptr<NetworkInterface>& networkInterface)
	: _networkInterface(networkInterface)
{
}

void EventDispatcherRemoteApplication::SendMousePosition(float x, float y)
{
	if (auto peer = GetPeer())
	{
		ProtoPackets::MousePositionMessage message;
		message.set_x(x);
		message.set_y(y);
		peer->SendPacket(message);
	}
}

void EventDispatcherRemoteApplication::Connect()
{
	_connected = true;
}

bool EventDispatcherRemoteApplication::Connected() const
{
	return _connected;
}

void EventDispatcherRemoteApplication::Disconnect()
{
	_connected = false;
}

Network::Peer* EventDispatcherRemoteApplication::GetPeer() const
{
	if (auto networkInterface = _networkInterface.lock())
	{
		return &networkInterface->GetPeer();
	}
	return nullptr;
}
