#include "ListenerApplication.h"

#include <iostream>

#include "RemoteApplication.h"
#include "RemoteApplicationBridge.h"
#include "operations/OperationManager.h"
#include "tools/ParsedCommandLine.h"

ListenerApplication::ListenerApplication()
	: _owner(std::make_shared<int>(42))
	, _operationManager(std::make_unique<OperationManager>())
{
	_remoteBridge = std::make_unique<RemoteApplicationBridge>(*_operationManager);
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
	
	_remoteBridge->SubscribeOnStatusChanged(TypedCallback<RemoteBridgeStatus>(_owner, [this](RemoteBridgeStatus status)
	{
		if (status == RemoteBridgeStatus::Failed)
		{
			std::cerr << "Cannot connect to remote application" << std::endl;
			_shouldTerminate = true;
		}
	}));

	const int16_t localPort = _commandLine->GetIntOrDefault("local-port", 44332);
	const int16_t remotePort = _commandLine->GetIntOrDefault("remote-port",44331);
	const std::string remoteIp = _commandLine->GetOrDefault("remote-ip", "255.255.255.255");
	_remoteBridge->Initialize(localPort, remoteIp, remotePort);
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
			std::cout << '(' << event.mouseMove.x << ';' << event.mouseMove.y << ")\n";
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
