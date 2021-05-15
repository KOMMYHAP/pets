#pragma once

class RemoteApplicationBase
{
public:
	virtual ~RemoteApplicationBase() = default;

	virtual void Connect() = 0;
	virtual bool Connected() const = 0;
	virtual void Disconnect() = 0;
};