#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <string>

#include "ReceivedPacket.h"

namespace Network
{
	enum class PeerConnectionStatus
	{
		PortIsBusy,
		IpAddressIsInvalid,
		Good
	};
	class Peer;

	class PeerConnection
	{
	public:
		PeerConnection(Peer & peer);
		~PeerConnection();

		PeerConnectionStatus SetLocal(uint16_t localPort, const std::string & localAddress);
		PeerConnectionStatus SetRemote(uint16_t remotePort, const std::string & remoteAddress);
		void SendPacket(uint32_t id, const std::string & data);
		void ProcessReceivedPackets();

	private:
		void ReceivePacket(uint32_t id, std::string data);
		
		class Impl;
		std::unique_ptr<Impl>				_impl;
		std::queue<ReceivedPacket>			_unprocessedPackets;
		std::mutex							_unprocessedPacketsMutex;
		Peer &								_peer;
	};
}
