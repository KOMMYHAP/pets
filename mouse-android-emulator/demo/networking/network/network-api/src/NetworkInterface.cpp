#include "NetworkInterface.h"
#include "PackageManager.h"
#include "Peer.h"

NetworkInterface::NetworkInterface(OperationManager& operationManager)
	: _operationManager(operationManager)
{
}
NetworkInterface::~NetworkInterface() = default;

void NetworkInterface::TryConnect(const std::string& ip, uint16_t port, const TypedCallback<bool>& callback)
{
}

Network::Peer* NetworkInterface::GetPeer() const
{
	return nullptr;
}

void NetworkInterface::CreatePeer()
{
}

// const uint16_t localPort = 44333;
// const uint16_t remotePort = 44332;
//
// auto packageManager = std::make_unique<Network::PackageManager>();
// packageManager->RegisterPackages();
//
// auto peer = std::make_unique<Network::Peer>(*packageManager);
// {
// 	auto udpSocket = std::make_unique<sf::UdpSocket>();
// 	udpSocket->setBlocking(false);
// 	auto status = udpSocket->bind(localPort, sf::IpAddress::Any);
// 	if (status != sf::Socket::Done)
// 	{
// 		std::cerr << "Port " << localPort << " is busy." << std::endl;
// 		return 1;
// 	}
// 	auto peerConnection = std::make_unique<Network::PeerConnection>(std::move(udpSocket), *peer);
// 	peerConnection->SetRemote(remotePort, sf::IpAddress::Broadcast);
// 	peer->Connect(std::move(peerConnection));
// }
//
// bool userFound = false;
// auto owner = std::make_shared<int>(42);
// ProtoPackets::ConnectionClientBroadcastSearchRequest requestClient;
//
// using Response = ProtoPackets::ConnectionClientBroadcastSearchResponse;
// peer->Subscribe(Network::TypedResponseCallback<Response>(owner, [&peer, &userFound](const Response & response)
// {
// 	auto udpSocket = std::make_unique<sf::UdpSocket>();
// 	udpSocket->setBlocking(false);
// 	udpSocket->bind(response.port(), sf::IpAddress(response.ip()));
// 	auto peerConnection = std::make_unique<Network::PeerConnection>(std::move(udpSocket), *peer);
// 	peer->Connect(std::move(peerConnection));
// 	std::cout << "Connection response from " << response.ip() << ":" << response.port() << "." << std::endl;
// 	userFound = true;
// }));
//
// sf::Clock clock;
// sf::Time requestClientDelay = sf::seconds(5);
//
// peer->SendPacket(requestClient);
// while (not userFound)
// {
// 	if (clock.getElapsedTime() > requestClientDelay)
// 	{
// 		peer->SendPacket(requestClient);
// 		clock.restart();
// 	}
// 	peer->ProcessReceivedPackets();
// 	sf::sleep(sf::milliseconds(15));
// }
//
// std::cout << "User found. Bye!" << std::endl;
