#include <iostream>
#include <SFML/Window.hpp>
#include "SFML/System.hpp"
#include "ListenerApplication.h"

int main(int argc, char **argv)
{
	std::cout
		<< "Hello from preudo android application!" << std::endl
		<< "I'll be listen to system events: mouse moving and clicking and send it to another application." << std::endl;

	auto application = std::make_unique<ListenerApplication>();
	application->ProcessCommandLine(argc, argv);

	sf::Window window(sf::VideoMode(600, 400), "Pseudo Android App");

	sf::Clock clock;
	sf::Time timeOnFrame = sf::milliseconds(15);
	sf::Time elapsedTimeFromLastFrame;
	
	bool shouldClose = false;
	while (not shouldClose)
	{
		sf::Event event;
		while (!shouldClose && window.pollEvent(event))
		{
			application->ProcessEvent(event);
			shouldClose = application->ShouldTerminate();
		}

		auto elapsedTimeInternal = TimeState::Milliseconds(elapsedTimeFromLastFrame.asMilliseconds());
		application->ProcessElapsedTime(elapsedTimeInternal);

		sf::Time elapsedTime = clock.getElapsedTime();
		elapsedTimeFromLastFrame = elapsedTime;
		if (elapsedTime < timeOnFrame)
		{
			sf::sleep(timeOnFrame - elapsedTime);
		}
		clock.restart();
	}

	window.close();
	
	return 0;
}
