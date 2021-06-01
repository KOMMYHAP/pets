#pragma once
#include <memory>

#include "EventReceiverRemoteApplication.h"
#include "application/ApplicationBase.h"

class EventReceiverRemoteApplication;
class NetworkInterface;
class OperationManager;
class RemoteApplicationBridge;
class ParsedCommandLine;

class EventReceiverApplication
{
public:
	EventReceiverApplication();
	~EventReceiverApplication();

	void ProcessCommandLine(int argc, char** argv);
	void ProcessEvent(const ApplicationEvent & event);
	void ProcessElapsedTime(TimeState elapsedTime);
	bool ShouldTerminate() const { return _shouldTerminate; }

private:
	void OnStateChanged(EventReceiverRemoteApplication::State state);

	std::unique_ptr<ParsedCommandLine>						_commandLine;
	std::unique_ptr<OperationManager>						_operationManager;

	std::shared_ptr<int>									_owner;
	std::unique_ptr<EventReceiverRemoteApplication>			_remoteApplication = nullptr;
	bool													_shouldTerminate = false;
};
