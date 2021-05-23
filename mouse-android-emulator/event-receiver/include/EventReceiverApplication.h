#pragma once
#include <memory>

#include "EventReceiverRemoteApplication.h"
#include "application/ApplicationBase.h"

class EventReceiverRemoteApplication;
class NetworkInterface;
class OperationManager;
class RemoteApplicationBridge;
class ParsedCommandLine;

class EventReceiverApplication : public ApplicationDelegate
{
public:
	EventReceiverApplication();
	~EventReceiverApplication() override;

	void ProcessCommandLine(int argc, char** argv) override;
	void ProcessEvent(const sf::Event & event) override;
	void ProcessElapsedTime(TimeState elapsedTime) override;
	bool ShouldTerminate() const override { return _shouldTerminate; }

private:
	void OnActivated() override;
	void OnDeactivated() override;
	void OnStateChanged(EventReceiverRemoteApplication::State state);

	std::unique_ptr<ParsedCommandLine>						_commandLine;
	std::unique_ptr<OperationManager>						_operationManager;

	std::shared_ptr<int>									_owner;
	std::unique_ptr<EventReceiverRemoteApplication>			_remoteApplication = nullptr;
	bool													_shouldTerminate = false;
};