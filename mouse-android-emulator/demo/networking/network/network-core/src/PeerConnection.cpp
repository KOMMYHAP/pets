#include "PeerConnection.h"
#include <atomic>
#include <condition_variable>
#include <queue>
#include <thread>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include "Peer.h"
#include "ReceivedPacket.h"


namespace Network
{
	class PeerConnection::Impl
	{
	public:
		Impl(PeerConnection & super)
			: _super(super)
		{
			_thread = std::thread(NetworkFunc, this);
		}

		~Impl()
		{
			_aborted = true;
			_packetsCondition.notify_one();
			_thread.join();
		}

		PeerConnectionStatus SetLocal(uint16_t localPort, const sf::IpAddress & localAddress)
		{
			if (localAddress == sf::IpAddress::None)
			{
				return PeerConnectionStatus::IpAddressIsInvalid;
			}

			auto socket = std::make_unique<sf::UdpSocket>();
			socket->setBlocking(false);
			auto status = socket->bind(localPort, localAddress);
			if (status == sf::Socket::Done)
			{
				std::unique_lock lock(_mutex);
				_socket = std::move(socket);
				_socketCondition.notify_one();
			}
			return status == sf::Socket::Done ? 
				PeerConnectionStatus::Good :
				PeerConnectionStatus::PortIsBusy;
		}
		
		PeerConnectionStatus SetRemote(uint16_t remotePort, const sf::IpAddress & remoteAddress)
		{
			if (remoteAddress == sf::IpAddress::None)
			{
				return PeerConnectionStatus::IpAddressIsInvalid;
			}
			_remotePort = remotePort;
			_remoteAddress = remoteAddress;
			return PeerConnectionStatus::Good;
		}

		void Send(uint32_t id, const std::string & data)
		{
			{
				std::scoped_lock lock(_mutex);
				_packetsToSend.emplace_back(sf::Packet() << id << data);
			}
			_packetsCondition.notify_one();
		}

	private:
		static void NetworkFunc(Impl * impl)
		{
			{
				std::unique_lock lock(impl->_mutex);
				impl->_socketCondition.wait(lock, [impl]()
				{
					return impl->_aborted || impl->_socket != nullptr;
				});
			}
			
			bool workInProcess = false;
			while (not impl->_aborted)
			{
				
				auto waitTime = workInProcess ? std::chrono::milliseconds(0) : std::chrono::milliseconds(25);
				workInProcess = false;

				{ // send packets
					std::vector<sf::Packet> packetsToSend;
					{
						std::unique_lock lock(impl->_mutex);
						auto status = impl->_packetsCondition.wait_for(lock, waitTime);
						workInProcess |= status == std::cv_status::no_timeout;
						packetsToSend = std::move(impl->_packetsToSend);
					}

					auto end = std::remove_if(packetsToSend.begin(), packetsToSend.end(), [&](sf::Packet & packet)
					{
						auto status = impl->_socket->send(packet, impl->_remoteAddress, impl->_remotePort);
						return status == sf::Socket::Done;
					});
					{
						std::unique_lock lock(impl->_mutex);
						impl->_packetsToSend.insert(impl->_packetsToSend.end(),
							std::make_move_iterator(packetsToSend.begin()),
							std::make_move_iterator(end));
					}
					packetsToSend.clear();
				}

				{ // receive packets
					sf::Packet packet;
					sf::IpAddress address;
					uint16_t port = 0;
					const auto status = impl->_socket->receive(packet, address, port);
					const bool packetProcessing = status == sf::Socket::Done || status == sf::Socket::Partial;
					const bool fromKnownUser = impl->IsTargetHost(port, address);
					if (packetProcessing && fromKnownUser)
					{
						workInProcess = true;
						impl->_receivingPacket.append(packet.getData(), packet.getDataSize());
						if (status == sf::Socket::Done)
						{
							impl->Received(packet);
						}
					}
				}
			}
		}

		bool IsTargetHost(uint16_t port, const sf::IpAddress & ip)
		{
			return
				_remotePort == port &&
				(_remoteAddress == sf::IpAddress::Broadcast || _remoteAddress == ip);
		}

		void Received(sf::Packet & packet)
		{
			uint32_t id;
			std::string data;
			if (packet >> id >> data)
			{
				_super.ReceivePacket(id, std::move(data));
			}
		}
		
		std::thread							_thread;
		std::condition_variable				_packetsCondition;
		std::condition_variable				_socketCondition;
		std::mutex							_mutex;
		std::atomic_bool					_aborted {false};

		std::vector<sf::Packet>				_packetsToSend;
		sf::Packet							_receivingPacket;
		
		std::unique_ptr<sf::UdpSocket>		_socket;
		uint16_t							_remotePort = 0;
		sf::IpAddress						_remoteAddress;
		
		PeerConnection &					_super;
	};

	PeerConnection::PeerConnection(Peer& peer)
		: _peer(peer)
	{
		_impl = std::make_unique<Impl>(*this);
	}
	
	PeerConnection::~PeerConnection() = default;

	PeerConnectionStatus PeerConnection::SetLocal(uint16_t localPort, const std::string & localAddress)
	{
		return _impl->SetLocal(localPort, localAddress);
	}

	PeerConnectionStatus PeerConnection::SetRemote(uint16_t remotePort, const std::string & remoteAddress)
	{
		return _impl->SetRemote(remotePort, remoteAddress);
	}

	void PeerConnection::SendPacket(uint32_t id, const std::string& data)
	{
		_impl->Send(id, data);
	}

	void PeerConnection::ProcessReceivedPackets()
	{
		decltype(_unprocessedPackets) unprocessedPackets;
		{
			std::scoped_lock lock(_unprocessedPacketsMutex);
			unprocessedPackets = std::move(_unprocessedPackets);
		}
		
		while (not unprocessedPackets.empty())
		{
			auto receivedPacket = std::move(unprocessedPackets.front());
			_peer.ReceivePacket(receivedPacket);
			unprocessedPackets.pop();
		}
	}

	void PeerConnection::ReceivePacket(uint32_t id, std::string data)
	{
		std::scoped_lock lock(_unprocessedPacketsMutex);
		_unprocessedPackets.emplace(id, std::move(data));
	}
}
