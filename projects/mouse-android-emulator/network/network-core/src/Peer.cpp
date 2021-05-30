#include "Peer.h"

#include "PackageManager.h"
#include "ReceivedPacket.h"
#include "PeerConnection.h"
#include "PacketsId.pb.h"
#include "Tracy.hpp"

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

	void Peer::CloseRemoteConnection()
	{
		_connection->ResetRemote();
	}

	uint16_t Peer::GetLocalPort() const
	{
		return _connection->GetLocalPort();
	}

	void Peer::ProcessReceivedPackets()
	{
		ZoneScopedN("Process Received Packets");
		_connection->ProcessReceivedPackets();
	}

	void Peer::ReceivePacket(ReceivedPacket receivedPacket)
	{
		ZoneScopedN("Receive Packet");

		if (receivedPacket.id != my::proto::package::MOUSE_POSITION_MESSAGE)
		{
			std::cout << "Packet has been received: " << my::proto::package::PackageId_Name(receivedPacket.id) << '\n';
		}

		auto package = _packageManager.Create(receivedPacket.id);
		if (!package)
		{
			std::cerr << "Received package was not registeted in package manager." << std::endl;
			return;
		}
		if (!package->ParseFromString(receivedPacket.data))
		{
			std::cerr << "Cannot parse received package." << std::endl;
			return;
		}

		if (receivedPacket.id != my::proto::package::MOUSE_POSITION_MESSAGE)
		{
			std::cout
				<< "Packet's message: {\n"
				<< package->DebugString() << "}\n";
		}
		
		ReceivePacket(receivedPacket.id, *package);
	}

	void Peer::ReceivePacket(uint32_t packageId, const google::protobuf::Message& response)
	{
		ZoneScopedN("Call Packet's Subscribers");
		auto it = _subscribers.find(packageId);
		if (it !=  _subscribers.end())
		{
			std::vector<ResponseCallback> oldCallbacks = std::move(it->second);
			auto invalidCallbackIt = std::remove_if(oldCallbacks.begin(), oldCallbacks.end(), [&response](const ResponseCallback & callback)
			{
				callback(response);
				return !callback.Valid();
			});
			oldCallbacks.erase(invalidCallbackIt, oldCallbacks.end());
			oldCallbacks.insert(oldCallbacks.end(), std::make_move_iterator(it->second.begin()), std::make_move_iterator(it->second.end()));
			it->second = std::move(oldCallbacks);
		}
	}

	void Peer::SendPacket(uint32_t packageId, const google::protobuf::Message& request)
	{
		ZoneScopedN("Send Packet");
		if (packageId != my::proto::package::MOUSE_POSITION_MESSAGE)
		{
			std::cout << "Packet will be sent: " << my::proto::package::PackageId_Name(packageId) << '\n';
		}

		if (request.SerializeToString(&_buffer))
		{
			_connection->SendPacket(packageId, _buffer);
		}
	}
}
