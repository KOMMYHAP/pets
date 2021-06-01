#pragma once
#include <memory>

#include "EventDispatcherRemoteApplication.h"

namespace ApplicationEvents {
	struct WindowResized;
	struct CloseRequest;
	struct MouseClicked;
	struct MouseMoved;
}
class ApplicationEvent;
class ApplicationOutputInterface;
class EventDispatcherRemoteApplication;
class OperationManager;
class ParsedCommandLine;

class EventDispatcherApplication
{
public:
	EventDispatcherApplication();
	~EventDispatcherApplication();
	void Initialize();

	void SetApplicationOutputInterface(ApplicationOutputInterface * output);

	void ProcessCommandLine(int argc, char** argv);
	void ProcessEvent(const ApplicationEvent & event);
	void ProcessElapsedTime(TimeState elapsedTime);
	bool ShouldTerminate() const { return _shouldTerminate; }

	void RequestAvailableConnections(const std::string & hostname);
	void ConnectTo(const std::string & ip, uint16_t port);

private:
	void OnStateChanged(EventDispatcherRemoteApplication::State state);
	void OnConnectionListReceived(const std::vector<AvailableConnectionData> & connectionList);

	void OnMouseMoved(const ApplicationEvents::MouseMoved &);
	void OnMouseClicked(const ApplicationEvents::MouseClicked &);
	void OnCloseRequested(const ApplicationEvents::CloseRequest &);
	void OnWindowResized(const ApplicationEvents::WindowResized &);

	std::unique_ptr<ParsedCommandLine>							_commandLine;
	std::unique_ptr<OperationManager>							_operationManager;

	std::shared_ptr<int>										_owner;
	std::unique_ptr<EventDispatcherRemoteApplication>			_remoteApplication;
	ApplicationOutputInterface *								_outputInterface = nullptr;

	bool														_shouldTerminate = false;
};
