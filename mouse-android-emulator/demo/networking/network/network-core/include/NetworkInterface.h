#pragma once
#include <memory>

class Operation;
class OperationManager;

namespace Network {
	class PackageManager;
	class Peer;
}

class NetworkInterface : public std::enable_shared_from_this<NetworkInterface>
{
public:
	NetworkInterface(OperationManager & operationManager);
	~NetworkInterface();

	void StartPacketProcessing();
	// todo: initiatilize with custom package creator to encapsulate packets by application type: client/server
	// void Initialize(PackageCreator);
	
	Network::Peer & GetPeer() const;

private:
	std::unique_ptr<Network::PackageManager>	_packageManager;
	std::unique_ptr<Network::Peer>				_peer;
	std::shared_ptr<Operation>					_packetProcessing;
	OperationManager &							_operationManager;
};
