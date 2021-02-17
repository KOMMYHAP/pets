#include <atomic>
#include <iomanip>
#include <thread>

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network.hpp>

#include "packets/Example.pb.h"

struct SharedState
{
	sf::Mutex				mutex;
	std::string				message;
	sf::Uint16				port;
};

struct WriterData
{
	sf::UdpSocket				broadcast;
	SharedState					sharedState;
	std::atomic_bool			needShutdown = false;
};

static void Writer(std::shared_ptr<WriterData> data)
{
	if (!data)
	{
		sf::err() << "Writer was not initialized!" << std::endl << std::flush;
		return;
	}
	if (data->broadcast.isBlocking())
	{
		sf::err() << "Socket must be not blocking!" << std::endl << std::flush;
		return;
	}

	const auto address = sf::IpAddress::Broadcast;
	sf::Uint16 port = 0;

	std::string message;
	sf::Clock clock;
	sf::Time waitingTime = sf::milliseconds(10);
	while (!data->needShutdown)
	{
		clock.restart();

		if (sf::Lock lock(data->sharedState.mutex); !data->sharedState.message.empty())
		{
			message = std::move(data->sharedState.message);
			port = data->sharedState.port;
		}

		if (!message.empty())
		{
			auto packet = sf::Packet() << message;
			auto status = data->broadcast.send(packet, sf::IpAddress::Broadcast, port);

			if (status == sf::Socket::Done)
			{
				sf::err() << "Message " << std::quoted(message) << " has been sent: address " << address.toString() << ", port " << port << std::endl << std::flush;
			}
			else if (status == sf::Socket::Error)
			{
				sf::err() << "An error occurred while writing to socket: address " << address.toString() << ", port " << port << std::endl << std::flush;;
			}
			else if (status == sf::Socket::Disconnected)
			{
				sf::err() << "Socket was been disconnected: address " << address.toString() << ", port " << port << std::endl << std::flush;;
			}

			message.clear();
		}

		sf::Time timeToSleep = waitingTime - clock.getElapsedTime();
		if (timeToSleep.asMicroseconds() > 0)
		{
			sf::sleep(timeToSleep);
		}
	}
	sf::err() << "Writer stopped." << std::endl << std::flush;
}

int main()
{
	Request r;
	r.set_question("qwe");
	
	sf::RenderWindow window(sf::VideoMode(640, 480), "Writer");
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);

	auto writerData = std::make_shared<WriterData>();
	writerData->broadcast.setBlocking(false);
	writerData->broadcast.bind(sf::Socket::AnyPort);
	sf::err() << "Socket bind to port " << writerData->broadcast.getLocalPort() << std::endl << std::flush;

	std::thread writer(&Writer, writerData);

	sf::Clock deltaClock;
	std::string message;
	std::string buffer;
	int port = 0;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed ||
				event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		{ // game update
			if (!message.empty())
			{
				sf::Lock lock(writerData->sharedState.mutex);
				writerData->sharedState.message = std::move(message);
			}
		}
		{ // imgui update
			ImGui::InputText("Message", &buffer);
			if (ImGui::Button("Send", ImVec2(50.0f, 30.0f)))
			{
				message = std::move(buffer);
			}
			if (ImGui::InputInt("Port", &port, 1, 1000) && port <= std::numeric_limits<sf::Uint16>::max())
			{
				sf::Lock lock(writerData->sharedState.mutex);
				writerData->sharedState.port = port;
			}
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
	writerData->needShutdown = true;
	writer.join();
	sf::err() << "Stopped." << std::endl << std::flush;

	return 0;
}
