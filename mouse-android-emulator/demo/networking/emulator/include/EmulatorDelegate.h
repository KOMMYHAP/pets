#pragma once
#include <SFML/Window/Event.hpp>

// filters events and processes it
class EmulatorDelegate
{
public:
	virtual ~EmulatorDelegate() = default;
	virtual void ProcessEvents(const sf::Event & event) = 0;
};
