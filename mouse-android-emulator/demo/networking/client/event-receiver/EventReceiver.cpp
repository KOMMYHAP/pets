#include <iostream>
#include <SFML/Window.hpp>
#include "SFML/System.hpp"
#include "EventReceiverApplication.h"

int main(int argc, char **argv)
{
	std::cout
		<< "Event Receiver Application started." << '\n'
		<< "I'll receive events from event-dispatcher application and handle it." << std::endl;

	auto application = std::make_unique<EventReceiverApplication>();
	application->ProcessCommandLine(argc, argv);

	sf::Window window(sf::VideoMode(600, 400), "Event Receiver App");

	sf::Clock clock;
	sf::Time timeOnFrame = sf::milliseconds(15);
	sf::Time elapsedTimeOnFrame;

	bool shouldClose = false;
	while (not shouldClose)
	{
		FrameMarkNamed("Frame");
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
