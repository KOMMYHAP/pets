#include <memory>
#include <thread>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network.hpp>

#include "PackageManager.h"
#include "Peer.h"
#include "PeerConnection.h"

#include "Connection.pb.h"

int main()
{
	const uint16_t localPort = 44332;
	const uint16_t remotePort = 44333;
	
	auto packageManager = std::make_unique<Network::PackageManager>();
	packageManager->RegisterPackages();

	auto peer = std::make_unique<Network::Peer>(*packageManager);
	{
		auto udpSocket = std::make_unique<sf::UdpSocket>();
		udpSocket->setBlocking(false);
		auto status = udpSocket->bind(localPort, sf::IpAddress::Any);
		if (status != sf::Socket::Done)
		{
			std::cerr << "Port " << localPort << " is busy." << std::endl;
			return 1;
		}
		auto peerConnection = std::make_unique<Network::PeerConnection>(std::move(udpSocket), *peer);
		peerConnection->SetRemote(remotePort, sf::IpAddress::Broadcast);
		peer->Connect(std::move(peerConnection));
	}

	bool responseSent = false;
	auto owner = std::make_shared<int>(42);

	using Request = my::proto::package::ConnectionClientBroadcastSearchRequest;
	peer->Subscribe(Network::TypedResponseCallback<Request>(owner, [&peer, &responseSent, localPort](const Request & clientRequest)
	{
		my::proto::package::ConnectionClientBroadcastSearchResponse response;
		response.set_ip(sf::IpAddress::getLocalAddress().toString());
		response.set_port(localPort);
		peer->SendPacket(response);
		responseSent = true;
	}));

	while (not responseSent)
	{
		peer->ProcessReceivedPackets();
		sf::sleep(sf::milliseconds(15));
	}

	std::cout << "Request accepted. Response sent. Bye!" << std::endl;
	return 0;
}
