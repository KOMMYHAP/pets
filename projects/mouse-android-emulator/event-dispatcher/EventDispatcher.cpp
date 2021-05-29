#include <iostream>
#include "EventDispatcherApplication.h"
#include "NativeJniBridge.h"
#include "application/ApplicationEvent.h"

int main(int argc, char **argv)
{
	NativeJniBridge nativeJniBridge(nullptr);
	auto application = nativeJniBridge.GetApplication();
	if (!application)
	{
		return 0;
	}

	std::cout
		<< "Hello from preudo android application!" << std::endl
		<< "I'll be listen to system events: mouse moving and clicking and send it to another application." << std::endl;

	application->ProcessCommandLine(argc, argv);

	ApplicationEvents::WindowResized resizeEvent;
	resizeEvent.newSizeX = 800;
	resizeEvent.newSizeY = 600;
	application->ProcessEvent(ApplicationEvent(resizeEvent));

	// sf::Window window(sf::VideoMode(600, 400), "Pseudo Android App");
	// // window.setFramerateLimit(25);
	//
	// sf::Clock clock;
	// sf::Time timeOnFrame = sf::milliseconds(15);
	// sf::Time elapsedTimeOnFrame;
	//
	// bool shouldClose = false;
	// while (!shouldClose)
	// {
	// 	sf::Event event;
	// 	while (!shouldClose && window.pollEvent(event))
	// 	{
	// 		application->ProcessEvent(event);
	// 		shouldClose = application->ShouldTerminate();
	// 	}
	//
	// 	auto elapsedTimeInternal = TimeState::Milliseconds(elapsedTimeOnFrame.asMilliseconds());
	// 	application->ProcessElapsedTime(elapsedTimeInternal);
	//
	// 	elapsedTimeOnFrame = clock.restart();
	// 	if (elapsedTimeOnFrame < timeOnFrame)
	// 	{
	// 		sf::sleep(timeOnFrame - elapsedTimeOnFrame);
	// 	}
	// }
	//
	// window.close();

	return 0;
}
