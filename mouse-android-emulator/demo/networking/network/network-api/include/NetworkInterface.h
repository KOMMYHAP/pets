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


class NetworkInterface
{
public:
	NetworkInterface(OperationManager & operationManager);
	~NetworkInterface();

	void TryConnect(const std::string & ip, uint16_t port, const TypedCallback<bool> & callback);
	Network::Peer * GetPeer() const;

private:
	void CreatePeer();

	std::unique_ptr<Network::PackageManager>	_packageManager;
	std::unique_ptr<Network::Peer>				_peer;
	OperationManager &							_operationManager;
};
