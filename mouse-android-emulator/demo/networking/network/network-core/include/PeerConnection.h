#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <string>

#include "ReceivedPacket.h"

namespace sf {
	class UdpSocket;
	class IpAddress;
}

namespace Network
{
	class Peer;

	class PeerConnection
	{
	public:
		PeerConnection(std::unique_ptr<sf::UdpSocket> socket, Peer & peer);
		~PeerConnection();

		void SetRemote(uint16_t remotePort, const sf::IpAddress & remoteAddress);
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
