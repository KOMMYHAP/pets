#include "Peer.h"

#include <SFML/Network/IpAddress.hpp>


#include "PackageManager.h"
#include "ReceivedPacket.h"
#include "PeerConnection.h"
#include "PacketsId.pb.h"

namespace Network
{
	Peer::Peer(PackageManager& packageManager, OperationManager & operationManager)
		: _packageManager(packageManager)
		, _operationManager(operationManager)
	{
		_connection = std::make_unique<PeerConnection>(*this, _operationManager);
	}

	Peer::~Peer() = default;

	uint16_t Peer::OpenLocalConnection(const std::vector<uint16_t>& localPorts)
	{
		for (auto port : localPorts)
		{
			auto status = _connection->SetLocal(port, "0.0.0.0");
			if (status == NetworkErrorConstants::NoError)
			{
				return port;
			}
		}
		return 0;
	}

	bool Peer::OpenRemoteConnection(uint16_t remotePort, const std::string& ip)
	{
		return _connection->SetRemote(remotePort, ip) != NetworkErrorConstants::InvalidIp;
	}

	void Peer::ProcessReceivedPackets()
	{
		_connection->ProcessReceivedPackets();
	}

	void Peer::ReceivePacket(ReceivedPacket receivedPacket)
	{
		std::cout << "Received package with id " << receivedPacket.id << "." << std::endl;

		auto package = _packageManager.Create(receivedPacket.id);
		if (not package)
		{
			std::cerr << "Received package was not registeted in package manager." << std::endl;
			return;
		}
		if (not package->ParseFromString(receivedPacket.data))
		{
			std::cerr << "Cannot parse received package." << std::endl;
			return;
		}
		
		ReceivePacket(receivedPacket.id, *package);
	}

	void Peer::ReceivePacket(uint32_t packageId, const google::protobuf::Message& response)
	{
		// auto it = _responseCallbacks.find(packageId);
		// if (it != _responseCallbacks.end())
		// {
		// 	auto & callback = it->second;
		// 	callback(response);
		// }
		for (auto it = _subscribers.begin(); it != _subscribers.end();)
		{
			auto & callback = *it;
			if (callback.Valid())
			{
				callback(response);
				++it;
			}
			else
			{
				it = _subscribers.erase(it);
			}
		}
	}

	void Peer::SendPacket(uint32_t packageId, std::string data)
	{
		_connection->SendPacket(packageId, std::move(data));
	}
}
