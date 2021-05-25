#pragma once
#include <memory>

#include "EventDispatcherRemoteApplication.h"
#include "application/ApplicationBase.h"

namespace ApplicationEvents {
	struct WindowResized;
	struct CloseRequest;
	struct MouseClicked;
	struct MouseMoved;
}

class EventDispatcherRemoteApplication;
class OperationManager;
class ParsedCommandLine;

class EventDispatcherApplication : public ApplicationDelegate
{
public:
	EventDispatcherApplication();
	~EventDispatcherApplication() override;

	void ProcessCommandLine(int argc, char** argv) override;
	void ProcessEvent(const ApplicationEvent & event) override;
	void ProcessElapsedTime(TimeState elapsedTime) override;
	bool ShouldTerminate() const override { return _shouldTerminate; }

private:
	void OnActivated() override;
	void OnDeactivated() override;
	void OnStateChanged(EventDispatcherRemoteApplication::State state);

	void OnMouseMoved(const ApplicationEvents::MouseMoved &);
	void OnMouseClicked(const ApplicationEvents::MouseClicked &);
	void OnCloseRequested(const ApplicationEvents::CloseRequest &);
	void OnWindowResized(const ApplicationEvents::WindowResized &);

	std::unique_ptr<ParsedCommandLine>							_commandLine;
	std::unique_ptr<OperationManager>							_operationManager;

	std::shared_ptr<int>										_owner;
	std::unique_ptr<EventDispatcherRemoteApplication>			_remoteApplication;

	bool														_shouldTerminate = false;

};
