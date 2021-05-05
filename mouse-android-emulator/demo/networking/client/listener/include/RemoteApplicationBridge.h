#pragma once
#include <memory>
#include <string>

#include "tools/Callback.h"

class ParsedCommandLine;
class OperationManager;
class NetworkInterface;
class RemoteApplication;

enum class RemoteBridgeStatus
{
	NotInitialized,
	WaitingForConnect,
	Connected,
	Failed
};

class RemoteApplicationBridge
{
public:
	RemoteApplicationBridge(OperationManager & operationManager);
	~RemoteApplicationBridge();

	void Initialize(const ParsedCommandLine & commandLine, std::weak_ptr<void> owner);
	void SubscribeOnStatusChanged(TypedCallback<RemoteBridgeStatus> callback);

	RemoteBridgeStatus GetStatus() const { return _status; }
	RemoteApplication * GetRemoteApplication() const;

private:
	void ChangeStatus(RemoteBridgeStatus status);

	RemoteBridgeStatus					_status = RemoteBridgeStatus::NotInitialized;
	TypedCallback<RemoteBridgeStatus>	_subscriber;
	std::unique_ptr<NetworkInterface>	_networkInterface;
	std::unique_ptr<RemoteApplication> 	_remoteApplication;
	OperationManager &					_operationManager;
};
