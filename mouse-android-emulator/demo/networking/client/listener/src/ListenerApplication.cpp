#include "ListenerApplication.h"

#include <iostream>

#include "NetworkInterface.h"
#include "RemoteApplication.h"
#include "RemoteApplicationBridge.h"
#include "operations/OperationManager.h"
#include "tools/ParsedCommandLine.h"

ListenerApplication::ListenerApplication()
	: _operationManager(std::make_unique<OperationManager>())
{
	_remoteBridge = std::make_unique<RemoteApplicationBridge>(*_operationManager);
	_owner = std::make_shared<int>(42);
}

ListenerApplication::~ListenerApplication() = default;

void ListenerApplication::SetWindowSize(int32_t w, int32_t h)
{
	_width = w;
	_height = h;
}

void ListenerApplication::ProcessCommandLine(int argc, char** argv)
{
	_commandLine = std::make_unique<ParsedCommandLine>(argc, argv);

	auto localPort = static_cast<uint16_t>(_commandLine->GetIntOrDefault("local-port", 44332));
	auto remotePort = static_cast<uint16_t>(_commandLine->GetIntOrDefault("remote-port",44331));
	const std::string remoteIp = _commandLine->GetOrDefault("remote-ip", "255.255.255.255");

	_remoteBridge->Initialize({localPort}, TypedCallback<RemoteBridgeState>(_owner, this, &ListenerApplication::OnRemoteBridgeStatusChanged));
	_remoteBridge->DirectConnect(remoteIp, {remotePort});
}

void ListenerApplication::ProcessEvent(const sf::Event& event)
{
	if (event.type != sf::Event::Closed)
	{
		if (event.type == sf::Event::MouseMoved)
		{
			if (auto remoteApp = _remoteBridge->GetRemoteApplication())
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

void ListenerApplication::ProcessElapsedTime(TimeState elapsedTime)
{
	_operationManager->Update(elapsedTime);
}

void ListenerApplication::OnActivated()
{
}

void ListenerApplication::OnDeactivated()
{
}

void ListenerApplication::OnRemoteBridgeStatusChanged(RemoteBridgeState state)
{
	static RemoteBridgeState s_lastState = RemoteBridgeState::NotInitialized;
	auto ToString = [](RemoteBridgeState state)
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

	std::cout << "Remote bridge's state has been updated: " << ToString(s_lastState) << " -> " << ToString(state) << '\n';
	s_lastState = state;
}
