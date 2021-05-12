#include "PacketHandlerOperation.h"

#include <algorithm>
#include <iostream>
#include <mutex>
#include <optional>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include "ReceivedPacket.h"

struct PacketHandlerOperation::Impl
{
	std::mutex									packetMutex;
	std::unique_ptr<sf::UdpSocket> 				socket;
	std::vector<sf::Packet>						packetsToSend;
	sf::IpAddress								remoteAddress;
	uint16_t									remotePort;
	
	std::vector<Network::ReceivedPacket>		unprocessedPackets;
	std::optional<sf::Packet>					partiallyProcessedPacket;
};

PacketHandlerOperation::PacketHandlerOperation()
	: _impl(std::make_unique<Impl>())
{
}

PacketHandlerOperation::~PacketHandlerOperation() = default;

NetworkErrorConstants PacketHandlerOperation::SetLocal(uint16_t localPort, const std::string& localAddress)
{
	auto socket = std::make_unique<sf::UdpSocket>();
	socket->setBlocking(false);
	sf::IpAddress ip(localAddress);
	if (ip == sf::IpAddress::None)
	{
		return NetworkErrorConstants::InvalidIp;
	}
	auto status = socket->bind(localPort, localAddress);
	if (status != sf::Socket::Done)
	{
		return NetworkErrorConstants::BusyPort;
	}
	_impl->socket = std::move(socket);
	return NetworkErrorConstants::NoError;
}

NetworkErrorConstants PacketHandlerOperation::SetRemote(uint16_t remotePort, const std::string& remoteAddress)
{
	sf::IpAddress ip(remoteAddress);
	if (ip == sf::IpAddress::None)
	{
		return NetworkErrorConstants::InvalidIp;
	}
	_impl->remotePort = remotePort;
	_impl->remoteAddress = remoteAddress;
	return NetworkErrorConstants::NoError;
}

void PacketHandlerOperation::Queue(uint32_t id, const std::string& data)
{
	std::scoped_lock lock(_impl->packetMutex);
	_impl->packetsToSend.emplace_back(sf::Packet() << id << data);
}

std::vector<Network::ReceivedPacket> PacketHandlerOperation::ExtractReceivedPackets()
{
	std::scoped_lock lock(_impl->packetMutex);
	return std::move(_impl->unprocessedPackets);
}

Operation::Result PacketHandlerOperation::DoImpl()
{
	if (not _impl->socket)
	{
		return Result::InProcess;
	}

	SendPackets();
	ReceivePackets();

	return Result::InProcess;
}

void PacketHandlerOperation::SendPackets()
{
	std::vector<sf::Packet> packetsToSend;
	{
		std::scoped_lock lock(_impl->packetMutex);
		packetsToSend = std::move(_impl->packetsToSend);
	}
	auto end = std::remove_if(packetsToSend.begin(), packetsToSend.end(), [&](sf::Packet & packet)
	{
		auto status = _impl->socket->send(packet, _impl->remoteAddress, _impl->remotePort);
		return status == sf::Socket::Done;
	});

	{
		std::scoped_lock lock(_impl->packetMutex);
		_impl->packetsToSend.insert(_impl->packetsToSend.begin(),
			std::make_move_iterator(packetsToSend.begin()),
			std::make_move_iterator(end));
	}
}

void PacketHandlerOperation::ReceivePackets()
{
	sf::Packet clearPacket;
	sf::Packet * packetPtr = &clearPacket;
	bool partialPacketCompleting = false;
	
	if (_impl->partiallyProcessedPacket)
	{
		packetPtr = &(*_impl->partiallyProcessedPacket);
		partialPacketCompleting = true;
	}

	sf::IpAddress address;
	uint16_t port = 0;
	const auto status = _impl->socket->receive(*packetPtr, address, port);
	const bool packetProcessed = status == sf::Socket::Done;
	const bool fromKnownUser = IsTargetHost(port, address.toString());
	if (packetProcessed && fromKnownUser)
	{
		uint32_t id;
		std::string data;
		if (*packetPtr >> id >> data)
		{
			// packet has been successfully received and contains id and data
			std::scoped_lock lock(_impl->packetMutex);
			_impl->unprocessedPackets.emplace_back(id, std::move(data));
		}
		if (partialPacketCompleting)
		{
			// packet has been successfully completed, but its content does not matter
			_impl->partiallyProcessedPacket.reset();
		}
	}
}

void PacketHandlerOperation::OnCompleted(std::exception_ptr exception)
{
	if (exception)
	{
		try
		{
			std::rethrow_exception(exception);
		}
		catch (const std::exception & e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

bool PacketHandlerOperation::IsTargetHost(uint16_t port, const std::string& ip) const
{
	return
		_impl->remotePort == port &&
		(_impl->remoteAddress == sf::IpAddress::Broadcast || _impl->remoteAddress == ip);
}

