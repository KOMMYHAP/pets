#pragma once
#include <memory>

#include "application/ApplicationBase.h"

class EmulatorDelegate;

class Emulator : public ApplicationDelegate
{
public:
	Emulator();
	~Emulator() override;

	void ProcessEvent(const sf::Event & event) override;
	bool ShouldTerminate() const override { return _shouldTerminate; }

private:
	void OnActivated() override;
	void OnDeactivated() override;

	std::unique_ptr<EmulatorDelegate>		_delegate;
	bool									_shouldTerminate = false;
};
