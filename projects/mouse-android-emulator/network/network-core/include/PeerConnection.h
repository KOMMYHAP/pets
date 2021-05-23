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

		uint16_t GetLocalPort() const;

		void SendPacket(uint32_t id, const std::string & data);
		void ProcessReceivedPackets();

	private:
		uint16_t										_localPort = 0;
		std::shared_ptr<PacketHandlerOperation>			_packetHandler;
		Peer &											_peer;
		OperationManager &								_operationManager;
	};
}
