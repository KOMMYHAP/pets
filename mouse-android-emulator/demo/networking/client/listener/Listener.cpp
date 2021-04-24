#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network.hpp>

#include "ProtoPackagesNamespace.h"
#include "PackageManager.h"
#include "Peer.h"
#include "PeerConnection.h"

#include "Connection.pb.h"
#include "operations/OperationManager.h"
#include "operations/SimpleOperation.h"

class NetworkOptions
{
public:
	NetworkOptions(const std::string & data);
	std::string ToStyledString() const;
	
	std::string		GetString(const std::string & field, const std::string & defaultValue);
	int64_t			GetInt(const std::string & field, int64_t defaultValue);

private:
	// json
};

class NetworkInterface
{
public:
	NetworkInterface(const NetworkOptions & options);
	std::shared_ptr<Network::Peer> GetPeer() const;

private:
	void CreatePeer();

	std::unique_ptr<NetworkOptions>				_options;
	std::unique_ptr<Network::PackageManager>	_packageManager;
	std::shared_ptr<Network::Peer>				_peer;
};

int main()
{
	OperationManager operationManager;
	auto owner = std::make_shared<int>(42);
	auto callback = TypedCallback<>(owner, []()
	{
		std::cout << "Hello from another thread!" << std::endl;
	});
	auto operation = std::make_shared<SimpleOperation>(callback);
	operationManager.Schedule(operation, TimeState::Seconds(5), OperationThreadIds::NetworkThread);

	sf::Clock clock;
	sf::Time frameTime = sf::milliseconds(15);
	while (not operation->Completed())
	{
		operationManager.Update(TimeState::Milliseconds(clock.getElapsedTime().asMilliseconds()));
		clock.restart();
		sf::sleep(frameTime);
	}

	std::cout << "Operation has been successfully completed. Bye!" << std::endl;
	
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
	return 0;
}
