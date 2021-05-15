#include "RemoteApplicationBridge.h"

#include <memory>

#include "Connection.pb.h"
#include "NetworkInterface.h"
#include "Peer.h"
#include "RemoteApplicationBase.h"
#include "tools/Timer.h"

RemoteApplicationBridge::RemoteApplicationBridge(OperationManager& operationManager)
	: _operationManager(operationManager)
{
	_networkInterface = std::make_shared<NetworkInterface>(operationManager);
	_networkInterface->GetPeer().Subscribe(TypedCallback<const ProtoPackets::ConnectionResponse &>(_networkInterface, this, &RemoteApplicationBridge::OnConnected));
	
	_timeoutTimer = std::make_unique<Timer>(TypedCallback<>(_networkInterface, this, &RemoteApplicationBridge::OnConnectionTimeout), TimeState::Seconds(5), _operationManager);
}

RemoteApplicationBridge::~RemoteApplicationBridge() = default;

void RemoteApplicationBridge::Initialize(std::unique_ptr<RemoteApplicationBase> remoteApplication)
{
	_remoteApplication = std::move(remoteApplication);
	SetState(RemoteBridgeState::Initialized);
}

void RemoteApplicationBridge::OpenLocalConnection(const std::vector<uint16_t>& localPorts, const TypedCallback<RemoteBridgeState>& callback)
{
	_stateChangedCallback = callback;
	
	uint16_t localPort = _networkInterface->GetPeer().OpenLocalConnection(localPorts);
	if (localPort == 0)
	{
		SetErrorState(RemoteBridgeError::LocalPortsBusy);
		return;
	}
}

void RemoteApplicationBridge::StartBroadcasting(const std::vector<uint16_t>& remotePorts)
{
	DirectConnect("255.255.255.255", remotePorts);
}

void RemoteApplicationBridge::DirectConnect(const std::string& remoteIp, const std::vector<uint16_t>& remotePorts)
{
	Connect(remoteIp, remotePorts);
}

void RemoteApplicationBridge::CloseConnection()
{
	if (SetErrorStateIfNotInitialized())
	{
		return;
	}

	if (_remoteApplication->Connected())
	{
		_networkInterface->GetPeer().CloseRemoteConnection();
		_remoteApplication->Disconnect();
		SetState(RemoteBridgeState::Disconnected);
	}
}

void RemoteApplicationBridge::Reconnect()
{
	if (_connectionStatus.wasConnected)
	{
		std::vector<uint16_t> ports = {_connectionStatus.port};
		DirectConnect(_connectionStatus.ip, ports);
	}
}

RemoteApplicationBase* RemoteApplicationBridge::GetRemoteApplication() const
{
	return _remoteApplication.get();
}

RemoteBridgeError RemoteApplicationBridge::GetError() const
{
	return _error;
}

void RemoteApplicationBridge::Connect(const std::string& remoteIp, const std::vector<uint16_t>& remotePorts)
{
	if (SetErrorStateIfNotInitialized())
	{
		return;
	}

	if (remotePorts.empty())
	{
		SetErrorState(RemoteBridgeError::RemotePortEmpty);
		return;
	}

	_connectionStatus.ip = remoteIp;
	_timeoutTimer->Start();
	SetState(RemoteBridgeState::WaitingForConnect);

	auto & peer = _networkInterface->GetPeer();
	peer.OpenRemoteConnection(remotePorts[0], remoteIp);
	ProtoPackets::ConnectionRequest request;
	peer.SendPacket(request);
}

void RemoteApplicationBridge::OnConnected(const ProtoPackets::ConnectionResponse& response)
{
	_timeoutTimer->Reset();
	_connectionStatus.wasConnected = true;
	_connectionStatus.ip = response.ip();
	_connectionStatus.port = response.port();
	_remoteApplication->Connect();
	SetState(RemoteBridgeState::Connected);
}

void RemoteApplicationBridge::OnConnectionTimeout()
{
	SetState(RemoteBridgeState::ConnectionTimedOut);
}

void RemoteApplicationBridge::SetState(RemoteBridgeState state)
{
	_state = state;
	if (state != RemoteBridgeState::ErrorOccured)
	{
		_error = RemoteBridgeError::NoError;
	}
	_stateChangedCallback(state);
}

bool RemoteApplicationBridge::SetErrorStateIfNotInitialized()
{
	if (_state != RemoteBridgeState::NotInitialized)
	{
		return false;
	}
	SetErrorState(RemoteBridgeError::NotInitialized);
	return true;
}

void RemoteApplicationBridge::SetErrorState(RemoteBridgeError error)
{
	_error = error;
	SetState(RemoteBridgeState::ErrorOccured);
}
