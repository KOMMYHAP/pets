#include <iostream>
#include <SFML/Window.hpp>
#include "SFML/System.hpp"
#include "EventDispatcherApplication.h"

int main(int argc, char **argv)
{
	std::cout
		<< "Hello from preudo android application!" << std::endl
		<< "I'll be listen to system events: mouse moving and clicking and send it to another application." << std::endl;

	auto application = std::make_unique<EventDispatcherApplication>();
	application->ProcessCommandLine(argc, argv);

	sf::Window window(sf::VideoMode(600, 400), "Pseudo Android App");
	// window.setFramerateLimit(25);

	sf::Clock clock;
	sf::Time timeOnFrame = sf::milliseconds(15);
	sf::Time elapsedTimeOnFrame;

	bool shouldClose = false;
	while (not shouldClose)
	{

		sf::Event event;
		while (!shouldClose && window.pollEvent(event))
		{
			application->ProcessEvent(event);
			shouldClose = application->ShouldTerminate();
		}

		auto elapsedTimeInternal = TimeState::Milliseconds(elapsedTimeOnFrame.asMilliseconds());
		application->ProcessElapsedTime(elapsedTimeInternal);

		elapsedTimeOnFrame = clock.restart();
		if (elapsedTimeOnFrame < timeOnFrame)
		{
			sf::sleep(timeOnFrame - elapsedTimeOnFrame);
		}
	}

	window.close();

	return 0;
}
