#pragma once
#include <memory>

namespace Network {
	class Peer;
}

class NetworkInterface;

class RemoteApplication
{
public:
	RemoteApplication(const std::shared_ptr<NetworkInterface> & networkInterface);

	void SendMousePosition(float x, float y);

private:
	Network::Peer * GetPeer() const;
	
	std::weak_ptr<NetworkInterface>			_networkInterface;
};
