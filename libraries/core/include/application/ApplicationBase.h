#pragma once
#include "SFML/Window/Event.hpp"

class ApplicationDelegate
{
public:
	virtual ~ApplicationDelegate() = default;

	virtual void ProcessEvent(const sf::Event & event) = 0;

	virtual bool ShouldTerminate() const = 0;

	virtual void OnActivated() = 0;
	virtual void OnDeactivated() = 0;
};