#pragma once

#include <string>
#include <vector>

#include "operations/Operation.h"
#include "NetworkErrorConstants.h"

namespace Network {
	struct ReceivedPacket;
}

class PacketHandlerOperation : public Operation
{
public:
	PacketHandlerOperation();
	~PacketHandlerOperation() override;

	NetworkErrorConstants SetLocal(uint16_t localPort, const std::string & localAddress);
	NetworkErrorConstants SetRemote(uint16_t remotePort, const std::string & remoteAddress);

	void Queue(uint32_t id, const std::string & data);
	std::vector<Network::ReceivedPacket> ExtractReceivedPackets();

protected:
	Result DoImpl() override;
	void SendPackets();
	void ReceivePackets();
	
	void OnCompleted(std::exception_ptr exception) override;

private:
	bool IsTargetHost(uint16_t port, const std::string & ip) const;
	
	struct Impl;
	std::unique_ptr<Impl>		_impl;
};
