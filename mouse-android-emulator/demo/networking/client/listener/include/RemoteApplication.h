#pragma once

class NetworkInterface;

class RemoteApplication
{
public:
	RemoteApplication(NetworkInterface & networkInterface);

	void SendMousePosition(float x, float y);

private:
	NetworkInterface &	_networkInterface;
};
