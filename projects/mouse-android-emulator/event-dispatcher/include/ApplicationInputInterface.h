#pragma once
#include <cstdint>

namespace ApplicationEvents {
	struct WindowResized;
	struct CloseRequest;
	struct MouseClicked;
	struct MouseMoved;
}
class ApplicationEvent;

class ApplicationInputInterface
{
public:
	virtual ~ApplicationInputInterface() = default;

	virtual void CanBeInitialized() = 0;
	virtual void UpdateFrame(int64_t elapsedMs) = 0;
	virtual void ProcessEvent(const ApplicationEvent & event) = 0;

	virtual void RequestAvailableConnectionList(const std::string & hostname) = 0;
    virtual void Connect(const std::string &ip, uint16_t port) = 0;
};
