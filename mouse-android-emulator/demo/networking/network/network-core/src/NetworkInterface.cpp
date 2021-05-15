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

	_peer = std::make_unique<Network::Peer>(*_packageManager, _operationManager);

	// using Response = ProtoPackets::ConnectionResponse;
	// _peer->Subscribe(Network::TypedResponseCallback<Response>(_owner, [this](const Response & /*response*/)
	// {
	// 	UpdateStatus(NetworkConnection::Connected);
	// }));
}
NetworkInterface::~NetworkInterface() = default;

// NetworkConnection::Status NetworkInterface::Initialize(uint16_t localPort)
// {
// 	std::vector<uint16_t> localPorts;
// 	localPorts.emplace_back(localPort);
//
// 	uint16_t openPort = _peer->OpenLocalConnection(localPorts);
// 	bool ok = openPort == localPort;
// 	return ok ? NetworkConnection::Connected : NetworkConnection::BusyPort;
// }
//
// void NetworkInterface::TryConnect(const std::string& ip, uint16_t port)
// {
// 	bool ok = _peer->OpenRemoteConnection(port, ip);
// 	if (not ok)
// 	{
// 		UpdateStatus(NetworkConnection::InvalidIpAddress);
// 		return;
// 	}
//
// 	ProtoPackets::ConnectionRequest requestClient;
// 	_peer->SendPacket(requestClient);
// }
//
// void NetworkInterface::SubscribeOnStatusChanged(const TypedCallback<NetworkConnection::Status>& callback)
// {
// 	_statusSubscribers.emplace_back(callback);
// }
//
Network::Peer & NetworkInterface::GetPeer() const
{
	return *_peer;
}
//
// void NetworkInterface::UpdateStatus(NetworkConnection::Status status)
// {
// 	decltype(_statusSubscribers) subscribers = std::move(_statusSubscribers);
// 	for (const auto & subscriber : _statusSubscribers)
// 	{
// 		if (subscriber.Valid())
// 		{
// 			subscriber(status);
// 			_statusSubscribers.emplace_back(subscriber);
// 		}
// 	}
// }
