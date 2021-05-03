#include <iostream>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include "Emulator.h"

int main()
{
	std::cout
		<< "Hello from preudo android application!" << std::endl
		<< "I'll be listen to system events: mouse moving and clicking and send it to another application." << std::endl;

	auto emulator = std::make_unique<Emulator>();

	sf::Window window(sf::VideoMode(600, 400), "Pseudo Android App");
	window.setFramerateLimit(25);

	bool shouldClose = false;
	while (not shouldClose)
	{
		sf::Event event;
		while (!shouldClose && window.pollEvent(event))
		{
			emulator->ProcessEvent(event);
			shouldClose = emulator->ShouldTerminate();
		}
	}

	window.close();
	
	return 0;
}
