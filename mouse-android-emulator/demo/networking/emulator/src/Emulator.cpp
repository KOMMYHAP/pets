#include "Emulator.h"

#include <iostream>

#include "EmulatorDelegate.h"

class StubEmulatorDelegate : public EmulatorDelegate
{
	void ProcessEvents(const sf::Event& event) override
	{
		if (event.type == sf::Event::MouseMoved)
		{
			std::cout << '(' << event.mouseMove.x << ';' << event.mouseMove.y << ")\n";
		}
	}
};

Emulator::Emulator()
	: _delegate(std::make_unique<StubEmulatorDelegate>())
{
}

Emulator::~Emulator()
{
}

void Emulator::ProcessEvent(const sf::Event& event)
{
	if (event.type != sf::Event::Closed)
	{
		_delegate->ProcessEvents(event);
	}
	else
	{
		_shouldTerminate = true;
	}
}

void Emulator::OnActivated()
{
}

void Emulator::OnDeactivated()
{
}
