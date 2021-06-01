#include "NetworkInterface.h"

#include "PackageManager.h"
#include "Peer.h"
#include "operations/OperationManager.h"
#include "operations/SimpleOperation.h"

class PacketProcessingOperation : public Operation
{
public:
	explicit PacketProcessingOperation(Network::Peer & peer)
		: _peer(peer)
	{
	}
	
	Result DoImpl() override
	{
		_peer.ProcessReceivedPackets();
		return Result::InProcess;
	}

private:
	Network::Peer &			_peer;
};

NetworkInterface::NetworkInterface(OperationManager& operationManager)
	: _operationManager(operationManager)
{
	_packageManager = std::make_unique<Network::PackageManager>();
	_packageManager->RegisterPackages();

	_peer = std::make_unique<Network::Peer>(*_packageManager, _operationManager);
}
NetworkInterface::~NetworkInterface() = default;

void NetworkInterface::StartPacketProcessing()
{
	if (!_packetProcessing)
	{
		_packetProcessing = std::make_shared<PacketProcessingOperation>(GetPeer());
		_operationManager.Schedule(_packetProcessing, OperationThreadIds::MainThread);
	}
}

Network::Peer & NetworkInterface::GetPeer() const
{
	return *_peer;
}
