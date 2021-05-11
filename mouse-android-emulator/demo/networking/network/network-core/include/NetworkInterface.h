#pragma once
#include <memory>
#include <string>

#include "tools/Callback.h"

class OperationManager;

namespace Network {
	class PackageManager;
	class Peer;
}

class NetworkOptions;

struct NetworkConnection
{
	enum Status
	{
		Connected,
		BusyPort,
		InvalidIpAddress
	};
};

class NetworkInterface
{
public:
	NetworkInterface(OperationManager & operationManager);
	~NetworkInterface();

	NetworkConnection::Status Initialize(uint16_t localPort);
	void TryConnect(const std::string & ip, uint16_t port, const TypedCallback<NetworkConnection::Status> & callback);
	Network::Peer & GetPeer() const;

private:
	std::unique_ptr<Network::PackageManager>	_packageManager;
	std::unique_ptr<Network::Peer>				_peer;
	OperationManager &							_operationManager;
};
