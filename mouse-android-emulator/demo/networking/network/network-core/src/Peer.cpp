#include "Peer.h"

#include "PackageManager.h"
#include "ReceivedPacket.h"
#include "PeerConnection.h"
#include "PacketsId.pb.h"

namespace Network
{
	Peer::Peer(PackageManager& packageManager)
		: _packageManager(packageManager)
	{
	}

	void Peer::Connect(std::unique_ptr<PeerConnection> connection)
	{
		_connection = std::move(connection);
	}

	void Peer::ProcessReceivedPackets()
	{
		if (_connection)
		{
			_connection->ProcessReceivedPackets();
		}
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
		if (_connection)
		{
			_connection->SendPacket(packageId, std::move(data));
			return;
		}

		std::cerr << "Peer must be connected before sending packet." << std::endl;
	}
}
