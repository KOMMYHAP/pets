#include "RemoteApplication.h"

#include "NetworkInterface.h"
#include "Peer.h"

#include "ProtoPackagesNamespace.h"
#include "Main.pb.h"

RemoteApplication::RemoteApplication(const std::shared_ptr<NetworkInterface>& networkInterface)
	: _networkInterface(networkInterface)
{
}

void RemoteApplication::SendMousePosition(float x, float y)
{
	if (auto peer = GetPeer())
	{
		ProtoPackets::MousePositionMessage message;
		message.set_x(x);
		message.set_y(y);
		peer->SendPacket(message);
	}
}

Network::Peer* RemoteApplication::GetPeer() const
{
	if (auto networkInterface = _networkInterface.lock())
	{
		return &networkInterface->GetPeer();
	}
	return nullptr;
}
