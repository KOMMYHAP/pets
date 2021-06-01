#pragma once
#include <string>
#include "ApplicationInputInterface.h"

class NativeJniBridge;

class JavaToNativeBridge : public ApplicationInputInterface
{
public:
	JavaToNativeBridge(NativeJniBridge & nativeJniBridge);

	void CanBeInitialized() override;
	void UpdateFrame(int64_t elapsedMs) override;
	void ProcessEvent(const ApplicationEvent & event) override;
	void RequestAvailableConnectionList(const std::string & hostname) override;
	void Connect(const std::string &ip, uint16_t port) override;

private:
	NativeJniBridge &		_nativeJniBridge;
};
