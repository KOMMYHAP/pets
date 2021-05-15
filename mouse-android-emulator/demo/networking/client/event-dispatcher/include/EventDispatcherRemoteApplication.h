#pragma once
#include <memory>

#include "RemoteApplicationBase.h"

namespace Network {
	class Peer;
}

class NetworkInterface;

class EventDispatcherRemoteApplication : public RemoteApplicationBase
{
public:
	void Configurate(const std::shared_ptr<NetworkInterface> & networkInterface) override;

	void SendMousePosition(float x, float y);

	void Connect() override;
	bool Connected() const override;
	void Disconnect() override;
private:
	Network::Peer * GetPeer() const;
	
	std::weak_ptr<NetworkInterface>			_networkInterface;
	bool									_connected = false;
};
