#include "PacketHandlerOperation.h"

#include <algorithm>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "ReceivedPacket.h"

struct PacketHandlerOperation::Impl
{
	bool IsTargetHost(uint16_t port, const sf::IpAddress & ip) const;

	TracyLockable(std::mutex,				mutex);
	
	std::condition_variable_any					condition;
	std::unique_ptr<sf::UdpSocket> 				socket;
	std::vector<sf::Packet>						packetsToSend;
	sf::IpAddress								remoteAddress;
	uint16_t									remotePort;

	std::atomic_bool							hasUnprocessedPackets = false;
	std::vector<Network::ReceivedPacket>		unprocessedPackets;
	std::optional<sf::Packet>					partiallyProcessedPacket;
	bool										shouldCheckMorePackets = false;
	uint32_t									maxPacketsToReceive = 5;
};

bool PacketHandlerOperation::Impl::IsTargetHost(uint16_t port, const sf::IpAddress& ip) const
{
	const bool isTargetPort = remotePort == port;
	const bool isTargetIp = remoteAddress == ip;
	if (isTargetPort && isTargetIp)
	{
		return true;
	}

	const bool isCommonIp = 
		remoteAddress == sf::IpAddress::Broadcast || 
		remoteAddress == sf::IpAddress::LocalHost;
	return isTargetPort && isCommonIp;
}

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

	{
		std::scoped_lock lock(_impl->mutex);
		_impl->socket = std::move(socket);
	}
	return NetworkErrorConstants::NoError;
}

NetworkErrorConstants PacketHandlerOperation::SetRemote(uint16_t remotePort, const std::string& remoteAddress)
{
	sf::IpAddress ip(remoteAddress);
	if (ip == sf::IpAddress::None)
	{
		return NetworkErrorConstants::InvalidIp;
	}

	{
		std::scoped_lock lock(_impl->mutex);
		_impl->remotePort = remotePort;
		_impl->remoteAddress = remoteAddress;
	}
	return NetworkErrorConstants::NoError;
}

void PacketHandlerOperation::ResetRemote()
{
	std::scoped_lock lock(_impl->mutex);
	_impl->remotePort = 0;
	_impl->remoteAddress = sf::IpAddress::None;
}

void PacketHandlerOperation::Queue(uint32_t id, const std::string& data)
{
	ZoneScopedN("Queue Packet");
	std::scoped_lock lock(_impl->mutex);
	_impl->packetsToSend.emplace_back(sf::Packet() << id << data);
	_impl->condition.notify_one();
}

bool PacketHandlerOperation::HasUnprocessedPackets() const
{
	return _impl->hasUnprocessedPackets;
}

std::vector<Network::ReceivedPacket> PacketHandlerOperation::ExtractReceivedPackets()
{
	ZoneScopedN("Extract Received Packets");
	std::scoped_lock lock(_impl->mutex);
	_impl->hasUnprocessedPackets = false;
	return std::move(_impl->unprocessedPackets);
}

Operation::Result PacketHandlerOperation::DoImpl()
{
	ZoneScopedN("Packet Processing");

	if (not _impl->socket)
	{
		return Result::InProcess;
	}

	bool hasPacketsToSend;
	{
		ZoneScopedN("Sleeping")
		std::unique_lock lock(_impl->mutex);
		hasPacketsToSend = _impl->condition.wait_for(lock, std::chrono::milliseconds(25), [this]()
		{
			bool shouldSkipSleeping = _impl->shouldCheckMorePackets || !_impl->packetsToSend.empty();
			return shouldSkipSleeping;
		});
	}
	if (hasPacketsToSend)
	{
		SendPackets();
	}

	ReceivePackets();
	
	return Result::InProcess;
}

void PacketHandlerOperation::SendPackets()
{
	ZoneScopedN("Sending Packets");
	
	std::vector<sf::Packet> packetsToSend;
	{
		std::scoped_lock lock(_impl->mutex);
		packetsToSend = std::move(_impl->packetsToSend);
	}
	auto end = std::remove_if(packetsToSend.begin(), packetsToSend.end(), [&](sf::Packet & packet)
	{
		auto status = _impl->socket->send(packet, _impl->remoteAddress, _impl->remotePort);
		return status == sf::Socket::Done;
	});

	if (end != packetsToSend.end())
	{
		std::scoped_lock lock(_impl->mutex);
		_impl->packetsToSend.insert(_impl->packetsToSend.begin(),
			std::make_move_iterator(packetsToSend.begin()),
			std::make_move_iterator(end));
	}
}

void PacketHandlerOperation::ReceivePackets()
{
	_impl->shouldCheckMorePackets = false;
	int32_t maxPacketsToReceive = static_cast<int32_t>(_impl->maxPacketsToReceive);
	while (maxPacketsToReceive > 0 && ReceivePacket())
	{
		maxPacketsToReceive -= 1;
	}
	
	if (maxPacketsToReceive == 0)
	{
		// All available number of packets have been received,
		// so we should check more packets at the next frame
		_impl->shouldCheckMorePackets = true;
	}
}

bool PacketHandlerOperation::ReceivePacket()
{
	ZoneScopedN("Receiving Packets");
	
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
	const bool fromKnownUser = _impl->IsTargetHost(port, address.toString());
	if (packetProcessed && fromKnownUser)
	{
		uint32_t id;
		std::string data;
		if (*packetPtr >> id >> data)
		{
			// packet has been successfully received and contains id and data
			std::scoped_lock lock(_impl->mutex);
			_impl->hasUnprocessedPackets = true;
			_impl->unprocessedPackets.emplace_back(id, std::move(data));
		}
		if (partialPacketCompleting)
		{
			// packet has been successfully completed, but its content does not matter
			_impl->partiallyProcessedPacket.reset();
		}
	}

	return packetProcessed;
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

