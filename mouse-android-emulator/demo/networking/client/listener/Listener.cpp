#include <atomic>
#include <thread>

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <SFML/Network.hpp>

struct SharedState
{
	sf::Mutex				mutex;
	std::string				message;
};

struct ListenerData
{
	sf::UdpSocket				broadcast;
	SharedState					sharedState;
	std::atomic_bool			needShutdown = false;
};

static void Listener(std::shared_ptr<ListenerData> data)
{
	if (!data)
	{
		sf::err() << "Listener was not initialized!" << std::endl << std::flush;
		return;
	}
	if (data->broadcast.isBlocking())
	{
		sf::err() << "Socket must be not blocking!" << std::endl << std::flush;
		return;
	}

	std::string fullMessage;
	sf::Clock clock;
	sf::Time waitingTime = sf::milliseconds(10);
	while (!data->needShutdown)
	{
		clock.restart();

		sf::Packet packet;
		sf::IpAddress address;
		sf::Uint16 port = 0;
		auto status = data->broadcast.receive(packet, address, port);

		std::string messsage;
		if (packet >> messsage)
		{
			fullMessage.append(messsage);
		}
		if (status == sf::Socket::Done)
		{
			sf::Lock lock(data->sharedState.mutex);
			data->sharedState.message = std::move(fullMessage);
			sf::err() << "Message received from socket: address " << address.toString() << ", port " << port << std::endl << std::flush;
		}
		else if (status == sf::Socket::Error)
		{
			sf::err() << "An error occurred while listening socket: address " << address.toString() << ", port " << port << std::endl << std::flush;;
		}
		else if (status == sf::Socket::Disconnected)
		{
			sf::err() << "Socket was been disconnected: address " << address.toString() << ", port " << port << std::endl << std::flush;;
		}

		sf::Time timeToSleep = waitingTime - clock.getElapsedTime();
		if (timeToSleep.asMicroseconds() > 0)
		{
			sf::sleep(timeToSleep);
		}
	}
	sf::err() << "Listener stopped." << std::endl << std::flush;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Listener");
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);

	auto listenerData = std::make_shared<ListenerData>();
	listenerData->broadcast.setBlocking(false);
	listenerData->broadcast.bind(sf::Socket::AnyPort);
	sf::err() << "Socket bind to port " << listenerData->broadcast.getLocalPort() << std::endl << std::flush;

	std::thread listener(&Listener, listenerData);

	sf::Clock deltaClock;
	std::vector<std::string> data;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		{ // game update
			if (sf::Lock lock(listenerData->sharedState.mutex); !listenerData->sharedState.message.empty())
			{
				data.emplace_back(std::move(listenerData->sharedState.message));
			}
		}
		{ // imgui update
			ImGui::Begin("Received data: ");
			int i = 0;
			for (const auto & s : data)
			{
				++i;
				ImGui::Text("%d. %s", i, s.c_str());
			}
			ImGui::End();
		}
		window.clear();
		{
			// game render
		}
		{
			// imgui render
			ImGui::SFML::Render(window);
		}
		window.display();
	}

	ImGui::SFML::Shutdown();

	sf::err() << "Stopping..." << std::endl << std::flush;
	listenerData->needShutdown = true;
	listener.join();
	sf::err() << "Stopped." << std::endl << std::flush;

	return 0;
}
