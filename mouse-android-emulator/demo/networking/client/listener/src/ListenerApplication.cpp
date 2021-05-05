#include "ListenerApplication.h"

#include <iostream>
#include <map>


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
	_remoteBridge->Initialize(*_commandLine, _owner);
}

void ListenerApplication::ProcessEvent(const sf::Event& event)
{
	if (event.type != sf::Event::Closed)
	{
		if (auto remoteApp = _remoteBridge->GetRemoteApplication())
		{
			// remoteApp->SendMousePosition(0.0f, 0.0f);
		}
		if (event.type == sf::Event::MouseMoved)
		{
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
