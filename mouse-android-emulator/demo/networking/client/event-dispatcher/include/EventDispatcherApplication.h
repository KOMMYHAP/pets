#pragma once
#include <memory>

#include "application/ApplicationBase.h"

class EventDispatcherRemoteApplication;
enum class RemoteBridgeState;
class NetworkInterface;
class OperationManager;
class RemoteApplicationBridge;
class ParsedCommandLine;

class EventDispatcherApplication : public ApplicationDelegate
{
public:
	EventDispatcherApplication();
	~EventDispatcherApplication() override;

	void SetWindowSize(int32_t w, int32_t h);
	void ProcessCommandLine(int argc, char** argv) override;
	void ProcessEvent(const sf::Event & event) override;
	void ProcessElapsedTime(TimeState elapsedTime) override;
	bool ShouldTerminate() const override { return _shouldTerminate; }

private:
	void OnActivated() override;
	void OnDeactivated() override;
	
	void OnRemoteBridgeStatusChanged(RemoteBridgeState state);
	EventDispatcherRemoteApplication * GetRemoteApplication() const;

	std::unique_ptr<ParsedCommandLine>			_commandLine;
	std::unique_ptr<OperationManager>			_operationManager;
	std::unique_ptr<RemoteApplicationBridge>	_remoteBridge;

	std::shared_ptr<int>						_owner;
	int32_t										_width = 0;
	int32_t										_height = 0;

	bool										_shouldTerminate = false;
	
};
