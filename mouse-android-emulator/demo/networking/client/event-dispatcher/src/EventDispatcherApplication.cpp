#include "EventDispatcherApplication.h"

#include <iostream>

#include "NetworkInterface.h"
#include "EventDispatcherRemoteApplication.h"
#include "operations/OperationManager.h"
#include "tools/ParsedCommandLine.h"

EventDispatcherApplication::EventDispatcherApplication()
	: _operationManager(std::make_unique<OperationManager>())
{
	_remoteApplication = std::make_unique<EventDispatcherRemoteApplication>(*_operationManager);
	_owner = std::make_shared<int>(42);
}

EventDispatcherApplication::~EventDispatcherApplication() = default;

void EventDispatcherApplication::ProcessCommandLine(int argc, char** argv)
{
	_commandLine = std::make_unique<ParsedCommandLine>(argc, argv);

	auto localPort = static_cast<uint16_t>(_commandLine->GetIntOrDefault("local-port", 44332));
	auto remotePort = static_cast<uint16_t>(_commandLine->GetIntOrDefault("remote-port",44331));
	const std::string remoteIp = _commandLine->GetOrDefault("remote-ip", "255.255.255.255");
	
	auto pingTime = static_cast<uint16_t>(_commandLine->GetIntOrDefault("ping",3));
	auto pongTimeout = static_cast<uint16_t>(_commandLine->GetIntOrDefault("pong-timeout",5));
	
	auto retries = static_cast<uint16_t>(_commandLine->GetIntOrDefault("connection-request-retries", 12));
	auto timeout = static_cast<uint16_t>(_commandLine->GetIntOrDefault("connection-request-timeout", 5));

	_remoteApplication->SubscribeOnStatusChange(TypedCallback<EventDispatcherRemoteApplication::State>(_owner, this, &EventDispatcherApplication::OnStateChanged));
	_remoteApplication->Initialize(localPort, remoteIp, remotePort, TimeState::Seconds(pingTime), TimeState::Seconds(pongTimeout));
	_remoteApplication->TryConnect(TimeState::Seconds(timeout), retries);
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

void EventDispatcherApplication::OnStateChanged(EventDispatcherRemoteApplication::State state)
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
		case EventDispatcherRemoteApplication::State::ConnectionRequestTimeout:
			return "ConnectionTimedOut";
		case EventDispatcherRemoteApplication::State::Disconnected:
			return "Disconnected";
		case EventDispatcherRemoteApplication::State::ErrorOccured:
			return "Error";
		case EventDispatcherRemoteApplication::State::DisconnectedByTimeout:
			return "DisconnectedByTimeout";
		default:
			return "Unknown";
		}
	};

	auto ErrorToString = [](EventDispatcherRemoteApplication::Error error)
	{
		switch (error)
		{
		case EventDispatcherRemoteApplication::Error::NotInitialized:
			return "NotInitialized";
		case EventDispatcherRemoteApplication::Error::LocalPortBusy:
			return "LocalPortsBusy";
		case EventDispatcherRemoteApplication::Error::RemotePortEmpty:
			return "RemotePortEmpty";
		case EventDispatcherRemoteApplication::Error::NoError:
			return "NoError";
		case EventDispatcherRemoteApplication::Error::InvalidIp: 
			return "InvalidIp";
		default:
			return "Unknown";
		}
	};

	std::cout << "State has been updated: " << StateToString(s_lastState) << " -> " << StateToString(state) << '\n';
	if (state == EventDispatcherRemoteApplication::State::ErrorOccured)
	{
		std::cout << "... with error: " << ErrorToString(_remoteApplication->GetError()) << '\n';
	}
	s_lastState = state;
}