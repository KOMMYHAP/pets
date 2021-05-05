#include "RemoteApplicationBridge.h"
#include "RemoteApplication.h"
#include "NetworkInterface.h"
#include "tools/ParsedCommandLine.h"

RemoteApplicationBridge::RemoteApplicationBridge(OperationManager& operationManager)
	: _operationManager(operationManager)
{
	_networkInterface = std::make_unique<NetworkInterface>(operationManager);
}

RemoteApplicationBridge::~RemoteApplicationBridge() = default;

void RemoteApplicationBridge::Initialize(const ParsedCommandLine& commandLine, std::weak_ptr<void> owner)
{
	const int16_t port = commandLine.GetIntOrDefault("port", 4242);
	const std::string ip = commandLine.GetOrDefault("ip", "255.255.255.255");

	_networkInterface->TryConnect(ip, port, TypedCallback<bool>(owner, [this](bool success)
	{
		_status = success ? RemoteBridgeStatus::Connected : RemoteBridgeStatus::Failed;
	}));
}

void RemoteApplicationBridge::SubscribeOnStatusChanged(TypedCallback<RemoteBridgeStatus> callback)
{
	_subscriber = std::move(callback);
}

RemoteApplication* RemoteApplicationBridge::GetRemoteApplication() const
{
	return _remoteApplication.get();
}

void RemoteApplicationBridge::ChangeStatus(RemoteBridgeStatus status)
{
	_status = status;
	_subscriber(std::move(status));
}
