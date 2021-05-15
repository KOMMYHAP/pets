#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "NetworkErrorConstants.h"
#include "ReceivedPacket.h"

class PacketHandlerOperation;
class OperationManager;

namespace Network
{
	class Peer;

	class PeerConnection
	{
	public:
		PeerConnection(Peer & peer, OperationManager & operationManager);
		~PeerConnection();

		NetworkErrorConstants SetLocal(uint16_t localPort, const std::string & localAddress);
		NetworkErrorConstants SetRemote(uint16_t remotePort, const std::string & remoteAddress);
		void ResetRemote();

		void SendPacket(uint32_t id, const std::string & data);
		void ProcessReceivedPackets();

	private:
		std::shared_ptr<PacketHandlerOperation>			_packetHandler;
		Peer &											_peer;
		OperationManager &								_operationManager;
	};
}
