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
	_remoteBridge = std::make_unique<RemoteApplicationBridge>(*_operationManager);
	_owner = std::make_shared<int>(42);
}

EventDispatcherApplication::~EventDispatcherApplication() = default;

void EventDispatcherApplication::SetWindowSize(int32_t w, int32_t h)
{
	_width = w;
	_height = h;
}

void EventDispatcherApplication::ProcessCommandLine(int argc, char** argv)
{
	_commandLine = std::make_unique<ParsedCommandLine>(argc, argv);

	auto localPort = static_cast<uint16_t>(_commandLine->GetIntOrDefault("local-port", 44332));
	auto remotePort = static_cast<uint16_t>(_commandLine->GetIntOrDefault("remote-port",44331));
	const std::string remoteIp = _commandLine->GetOrDefault("remote-ip", "255.255.255.255");

	_remoteBridge->Initialize(std::make_unique<EventDispatcherRemoteApplication>());
	_remoteBridge->OpenLocalConnection({localPort}, TypedCallback<RemoteBridgeState>(_owner, this, &EventDispatcherApplication::OnRemoteBridgeStatusChanged));
	_remoteBridge->DirectConnect(remoteIp, {remotePort});
}

void EventDispatcherApplication::ProcessEvent(const sf::Event& event)
{
	if (event.type != sf::Event::Closed)
	{
		if (event.type == sf::Event::MouseMoved)
		{
			if (auto remoteApp = GetRemoteApplication())
			{
				float x = event.mouseMove.x / static_cast<float>(_width);
				float y = event.mouseMove.y / static_cast<float>(_height);
				remoteApp->SendMousePosition(x, y);
			}
			// std::cout << '(' << event.mouseMove.x << ';' << event.mouseMove.y << ")\n";
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
	_remoteBridge->Reconnect();
}

void EventDispatcherApplication::OnDeactivated()
{
	_remoteBridge->CloseConnection();
}

void EventDispatcherApplication::OnRemoteBridgeStatusChanged(RemoteBridgeState state)
{
	static RemoteBridgeState s_lastState = RemoteBridgeState::NotInitialized;
	auto StateToString = [](RemoteBridgeState state)
	{
		switch (state)
		{
		case RemoteBridgeState::NotInitialized:
			return "NotInitialized";
		case RemoteBridgeState::Initialized:
			return "Initialized";
		case RemoteBridgeState::WaitingForConnect:
			return "WaitingForConnect";
		case RemoteBridgeState::Connected: 
			return "Connected";
		case RemoteBridgeState::ConnectionTimedOut: 
			return "ConnectionTimedOut";
		case RemoteBridgeState::Disconnected:
			return "Disconnected";
		case RemoteBridgeState::ErrorOccured:
			return "Error";
		default:
			return "Unknown";
		}
	};

	auto ErrorToString = [](RemoteBridgeError error)
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
	if (state == RemoteBridgeState::ErrorOccured)
	{
		std::cout << "Remote bridge's error: " << ErrorToString(_remoteBridge->GetError()) << '\n';
	}
	s_lastState = state;
}

EventDispatcherRemoteApplication* EventDispatcherApplication::GetRemoteApplication() const
{
	return dynamic_cast<EventDispatcherRemoteApplication*>(_remoteBridge->GetRemoteApplication());
}
