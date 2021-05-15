#pragma once

class NetworkInterface;

class RemoteApplicationBase
{
public:
	virtual ~RemoteApplicationBase() = default;

	virtual void Configurate(const std::shared_ptr<NetworkInterface> & networkInterface) = 0;
	
	virtual void Connect() = 0;
	virtual bool Connected() const = 0;
	virtual void Disconnect() = 0;
};