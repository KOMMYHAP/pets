#pragma once
#include <memory>

#include "application/ApplicationBase.h"

class OperationManager;
class RemoteApplicationBridge;
class ParsedCommandLine;

class ListenerApplication : public ApplicationDelegate
{
public:
	ListenerApplication();
	~ListenerApplication() override;
	
	void ProcessCommandLine(int argc, char** argv) override;
	void ProcessEvent(const sf::Event & event) override;
	void ProcessElapsedTime(TimeState elapsedTime) override;
	bool ShouldTerminate() const override { return _shouldTerminate; }

private:
	void OnActivated() override;
	void OnDeactivated() override;

	std::unique_ptr<ParsedCommandLine>			_commandLine;
	std::unique_ptr<OperationManager>			_operationManager;
	std::unique_ptr<RemoteApplicationBridge>	_remoteBridge;
	
	std::shared_ptr<int>						_owner;
	
	bool										_shouldTerminate = false;
	
};
