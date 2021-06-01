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

private:
	NativeJniBridge &		_nativeJniBridge;
};
