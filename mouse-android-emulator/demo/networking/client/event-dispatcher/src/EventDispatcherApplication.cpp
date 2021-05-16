#include "EventDispatcherApplication.h"

#include <iostream>

#include "NetworkInterface.h"
#include "EventDispatcherRemoteApplication.h"
#include "RemoteApplicationBridge.h"
#include "operations/OperationManager.h"
#include "tools/ParsedCommandLine.h"

EventDispatcherApplication::EventDispatcherApplication()
	: _operationManager(std::make_unique<OperationManager>())
{
	_owner = std::make_shared<int>(42);
}

EventDispatcherApplication::~EventDispatcherApplication() = default;

void EventDispatcherApplication::ProcessCommandLine(int argc, char** argv)
{
	_commandLine = std::make_unique<ParsedCommandLine>(argc, argv);

	auto localPort = static_cast<uint16_t>(_commandLine->GetIntOrDefault("local-port", 44332));
	auto remotePort = static_cast<uint16_t>(_commandLine->GetIntOrDefault("remote-port",44331));
	const std::string remoteIp = _commandLine->GetOrDefault("remote-ip", "255.255.255.255");

	_remoteApplication->SubscribeOnStatusChange(TypedCallback<EventDispatcherRemoteApplication::State>(_owner, this, &EventDispatcherApplication::OnRemoteBridgeStatusChanged));
	_remoteApplication->Initialize(localPort, remoteIp, remotePort);
	_remoteApplication->TryConnect(TimeState::Seconds(5), 12);
}

void EventDispatcherApplication::ProcessEvent(const sf::Event& event)
{
	if (event.type != sf::Event::Closed)
	{
		if (event.type == sf::Event::MouseMoved)
		{
			float x = event.mouseMove.x;
			float y = event.mouseMove.y;
			_remoteApplication->SendMousePosition(x, y);
		}
	}
	else
	{
		_shouldTerminate = true;
	}
}

void EventDispatcherApplication::ProcessElapsedTime(TimeState elapsedTime)
{
	_operationManager->Update(elapsedTime);
}

void EventDispatcherApplication::OnActivated()
{
}

void EventDispatcherApplication::OnDeactivated()
{
}

void EventDispatcherApplication::OnRemoteBridgeStatusChanged(EventDispatcherRemoteApplication::State state)
{
	static EventDispatcherRemoteApplication::State s_lastState = EventDispatcherRemoteApplication::State::NotInitialized;
	auto StateToString = [](EventDispatcherRemoteApplication::State state)
	{
		switch (state)
		{
		case EventDispatcherRemoteApplication::State::NotInitialized:
			return "NotInitialized";
		case EventDispatcherRemoteApplication::State::Initialized:
			return "Initialized";
		case EventDispatcherRemoteApplication::State::WaitingForConnect:
			return "WaitingForConnect";
		case EventDispatcherRemoteApplication::State::Connected:
			return "Connected";
		case EventDispatcherRemoteApplication::State::ConnectionTimedOut:
			return "ConnectionTimedOut";
		case EventDispatcherRemoteApplication::State::Disconnected:
			return "Disconnected";
		case EventDispatcherRemoteApplication::State::ErrorOccured:
			return "Error";
		default:
			return "Unknown";
		}
	};

	auto ErrorToString = [](EventDispatcherRemoteApplication::Error error)
	{
		switch (error)
		{
		case RemoteBridgeError::NotInitialized:
			return "NotInitialized";
		case RemoteBridgeError::LocalPortsBusy:
			return "LocalPortsBusy";
		case RemoteBridgeError::RemotePortEmpty:
			return "RemotePortEmpty";
		case RemoteBridgeError::NoError:
			return "NoError";
		default:
			return "Unknown";
		}
	};

	std::cout << "Remote bridge's state has been updated: " << StateToString(s_lastState) << " -> " << StateToString(state) << '\n';
	if (state == EventDispatcherRemoteApplication::State::ErrorOccured)
	{
		std::cout << "Remote bridge's error: " << ErrorToString(_remoteApplication->GetError()) << '\n';
	}
	s_lastState = state;
}