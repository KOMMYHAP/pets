#pragma once
#include <memory>

namespace Network {
	class PackageManager;
	class Peer;
}

class NetworkOptions;

class NetworkInterface
{
public:
	NetworkInterface(std::unique_ptr<NetworkOptions> options);
	~NetworkInterface();

	std::shared_ptr<Network::Peer> GetPeer() const;

private:
	void CreatePeer();

	std::unique_ptr<NetworkOptions>				_options;
	std::unique_ptr<Network::PackageManager>	_packageManager;
	std::shared_ptr<Network::Peer>				_peer;
};
