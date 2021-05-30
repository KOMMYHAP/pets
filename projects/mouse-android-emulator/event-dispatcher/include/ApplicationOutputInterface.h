#pragma once

#include <string>
#include <vector>
#include "EventDispatcherRemoteApplication.h"

// todo: optimize to avoid using of dynamic memory here and minimize jni marshaling
struct AvailableConnectionData
{
	std::string		ip;
	std::string		hostname;
	int				port;
};

struct EventDispatcherState
{
	EventDispatcherRemoteApplication::State		state;
	EventDispatcherRemoteApplication::Error		error;
};

class ApplicationOutputInterface
{
public:
	virtual ~ApplicationOutputInterface() = default;

	virtual void ResponseAvailableConnectionList(const std::vector<AvailableConnectionData> & connectionList) = 0;
	virtual void StateUpdated(const EventDispatcherState & state) = 0;
};
