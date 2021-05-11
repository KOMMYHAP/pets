#include "RemoteApplication.h"

#include "NetworkInterface.h"
#include "Peer.h"

#include "ProtoPackagesNamespace.h"
#include "Main.pb.h"

RemoteApplication::RemoteApplication(NetworkInterface& networkInterface)
	: _networkInterface(networkInterface)
{
}

void RemoteApplication::SendMousePosition(float x, float y)
{
	ProtoPackets::MousePositionMessage message;
	message.set_x(x);
	message.set_y(y);
	_networkInterface.GetPeer().SendPacket(message);
}
