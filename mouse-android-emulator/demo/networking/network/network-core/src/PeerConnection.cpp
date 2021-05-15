#include "PeerConnection.h"
#include <memory>
#include <queue>

#include "PacketHandlerOperation.h"
#include "Peer.h"
#include "ReceivedPacket.h"
#include "operations/OperationManager.h"

namespace Network
{
	PeerConnection::PeerConnection(Peer& peer, OperationManager & operationManager)
		: _packetHandler(std::make_shared<PacketHandlerOperation>())
		, _peer(peer)
		, _operationManager(operationManager)
	{
		_operationManager.Schedule(_packetHandler, OperationThreadIds::NetworkThread);
	}

	PeerConnection::~PeerConnection() = default;

	NetworkErrorConstants PeerConnection::SetLocal(uint16_t localPort, const std::string & localAddress)
	{
		return _packetHandler->SetLocal(localPort, localAddress);
	}

	NetworkErrorConstants PeerConnection::SetRemote(uint16_t remotePort, const std::string & remoteAddress)
	{
		return _packetHandler->SetRemote(remotePort, remoteAddress);
	}

	void PeerConnection::ResetRemote()
	{
		_packetHandler->ResetRemote();
	}

	void PeerConnection::SendPacket(uint32_t id, const std::string& data)
	{
		_packetHandler->Queue(id, data);
	}

	void PeerConnection::ProcessReceivedPackets()
	{
		auto packets = _packetHandler->ExtractReceivedPackets();
		for (const auto & packet : packets)
		{
			_peer.ReceivePacket(packet);
		}
	}
}
