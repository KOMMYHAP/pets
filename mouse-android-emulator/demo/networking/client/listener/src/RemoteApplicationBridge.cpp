#include "RemoteApplicationBridge.h"

#include <memory>

#include "RemoteApplication.h"
#include "NetworkInterface.h"
#include "tools/ParsedCommandLine.h"

RemoteApplicationBridge::RemoteApplicationBridge(OperationManager& operationManager)
	: _operationManager(operationManager)
{
	_networkInterface = std::make_unique<NetworkInterface>(operationManager);
}

RemoteApplicationBridge::~RemoteApplicationBridge() = default;

void RemoteApplicationBridge::Initialize(uint16_t localPort, const std::string& remoteIp, uint16_t remotePort)
{
	_packetOwner = std::make_shared<int>(42);

	auto status = _networkInterface->Initialize(localPort);
	if (status == NetworkConnection::BusyPort)
	{
		ChangeStatus(RemoteBridgeStatus::Failed);
		return;
	}

	_networkInterface->TryConnect(remoteIp, remotePort, TypedCallback<NetworkConnection::Status>(_packetOwner, [this](NetworkConnection::Status status)
	{
		if (status == NetworkConnection::Connected)
		{
			_remoteApplication = std::make_unique<RemoteApplication>(*_networkInterface);
			ChangeStatus(RemoteBridgeStatus::Connected);
		}
		else
		{
			ChangeStatus(RemoteBridgeStatus::Failed);
		};
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
