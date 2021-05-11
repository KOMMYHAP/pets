#include "NetworkInterface.h"

#include "ProtoPackagesNamespace.h"
#include "Connection.pb.h"

#include "PackageManager.h"
#include "Peer.h"

NetworkInterface::NetworkInterface(OperationManager& operationManager)
	: _operationManager(operationManager)
{
	_packageManager = std::make_unique<Network::PackageManager>();
	_packageManager->RegisterPackages();

	_peer = std::make_unique<Network::Peer>(*_packageManager);
}
NetworkInterface::~NetworkInterface() = default;

NetworkConnection::Status NetworkInterface::Initialize(uint16_t localPort)
{
	std::vector<uint16_t> localPorts;
	localPorts.emplace_back(localPort);

	uint16_t openPort = _peer->OpenLocalConnection(localPorts);
	bool ok = openPort == localPort;
	return ok ? NetworkConnection::Connected : NetworkConnection::BusyPort;
}

void NetworkInterface::TryConnect(const std::string& ip, uint16_t port, const TypedCallback<NetworkConnection::Status>& callback)
{
	bool ok = _peer->OpenRemoteConnection(port, ip);
	if (not ok)
	{
		callback(NetworkConnection::InvalidIpAddress);
		return;
	}

	using Response = ProtoPackets::ConnectionResponse;
	_peer->Subscribe(Network::TypedResponseCallback<Response>(callback.Owner(), [this, callback](const Response & /*response*/)
	{
		callback(NetworkConnection::Connected);
	}));

	ProtoPackets::ConnectionRequest requestClient;
	_peer->SendPacket(requestClient);
}

Network::Peer & NetworkInterface::GetPeer() const
{
	return *_peer;
}