#include "NetworkInterface.h"

#include "PackageManager.h"
#include "Peer.h"

NetworkInterface::NetworkInterface(OperationManager& operationManager)
	: _operationManager(operationManager)
{
	_packageManager = std::make_unique<Network::PackageManager>();
	_packageManager->RegisterPackages();

	_peer = std::make_unique<Network::Peer>(*_packageManager, _operationManager);
}
NetworkInterface::~NetworkInterface() = default;

Network::Peer & NetworkInterface::GetPeer() const
{
	return *_peer;
}
