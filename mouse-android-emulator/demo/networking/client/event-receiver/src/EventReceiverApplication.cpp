#include "EventReceiverApplication.h"

#include <iostream>

#include "NetworkInterface.h"
#include "EventReceiverRemoteApplication.h"
#include "operations/OperationManager.h"
#include "tools/Callback.h"
#include "tools/ParsedCommandLine.h"

EventReceiverApplication::EventReceiverApplication()
	: _operationManager(std::make_unique<OperationManager>())
{
	_remoteApplication = std::make_unique<EventReceiverRemoteApplication>(*_operationManager);
	_owner = std::make_shared<int>(42);
}

EventReceiverApplication::~EventReceiverApplication() = default;

void EventReceiverApplication::ProcessCommandLine(int argc, char** argv)
{
	_commandLine = std::make_unique<ParsedCommandLine>(argc, argv);

	auto localPort = static_cast<uint16_t>(_commandLine->GetIntOrDefault("local-port", 44331));
	auto remotePort = static_cast<uint16_t>(_commandLine->GetIntOrDefault("remote-port",44332));
	auto idleTimeout = static_cast<uint16_t>(_commandLine->GetIntOrDefault("idle-timeout", 5));

	_remoteApplication->SubscribeOnStatusChange(TypedCallback<EventReceiverRemoteApplication::State>(_owner, this, &EventReceiverApplication::OnStateChanged));
	_remoteApplication->Initialize(localPort, remotePort, TimeState::Seconds(idleTimeout));
}

void EventReceiverApplication::ProcessEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		_shouldTerminate = true;
	}
}

void EventReceiverApplication::ProcessElapsedTime(TimeState elapsedTime)
{
	_operationManager->Update(elapsedTime);
}

void EventReceiverApplication::OnActivated()
{
}

void EventReceiverApplication::OnDeactivated()
{
}

void EventReceiverApplication::OnStateChanged(EventReceiverRemoteApplication::State state)
{
	static EventReceiverRemoteApplication::State s_lastState = EventReceiverRemoteApplication::State::NotInitialized;
	auto StateToString = [](EventReceiverRemoteApplication::State state)
	{
		switch (state)
		{
		case EventReceiverRemoteApplication::State::NotInitialized:
			return "NotInitialized";
		case EventReceiverRemoteApplication::State::Connected:
			return "Connected";
		case EventReceiverRemoteApplication::State::Disconnected:
			return "Disconnected";
		case EventReceiverRemoteApplication::State::ErrorOccured:
			return "Error";
		case EventReceiverRemoteApplication::State::WaitingForConnectionRequest:
			return "WaitingForConnectionRequest";
		case EventReceiverRemoteApplication::State::DisconnectedByIdle:
			return "DisconnectedByIdle";
		default:
			return "Unknown";
		}
	};

	auto ErrorToString = [](EventReceiverRemoteApplication::Error error)
	{
		switch (error)
		{
		case EventReceiverRemoteApplication::Error::LocalPortBusy:
			return "LocalPortBusy";
		case EventReceiverRemoteApplication::Error::NoError:
			return "NoError";
		default:
			return "Unknown";
		}
	};

	std::cout << "State has been updated: " << StateToString(s_lastState) << " -> " << StateToString(state) << '\n';
	if (state == EventReceiverRemoteApplication::State::ErrorOccured)
	{
		std::cout << "... with error: " << ErrorToString(_remoteApplication->GetError()) << '\n';
	}
	s_lastState = state;
}
