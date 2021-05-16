#pragma once
#include <memory>

#include "EventDispatcherRemoteApplication.h"
#include "application/ApplicationBase.h"

class EventDispatcherRemoteApplication;
class NetworkInterface;
class OperationManager;
class RemoteApplicationBridge;
class ParsedCommandLine;

class EventDispatcherApplication : public ApplicationDelegate
{
public:
	EventDispatcherApplication();
	~EventDispatcherApplication() override;

	void ProcessCommandLine(int argc, char** argv) override;
	void ProcessEvent(const sf::Event & event) override;
	void ProcessElapsedTime(TimeState elapsedTime) override;
	bool ShouldTerminate() const override { return _shouldTerminate; }

private:
	void OnActivated() override;
	void OnDeactivated() override;
	void OnStateChanged(EventDispatcherRemoteApplication::State state);

	std::unique_ptr<ParsedCommandLine>							_commandLine;
	std::unique_ptr<OperationManager>							_operationManager;

	std::shared_ptr<int>										_owner;
	std::unique_ptr<EventDispatcherRemoteApplication>			_remoteApplication;

	bool														_shouldTerminate = false;
	
};
