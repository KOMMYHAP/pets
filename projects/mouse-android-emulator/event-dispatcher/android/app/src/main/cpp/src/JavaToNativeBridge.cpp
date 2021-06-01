#include "JavaToNativeBridge.h"
#include "EventDispatcherApplication.h"
#include "application/ApplicationEvent.h"
#include "NativeJniBridge.h"

JavaToNativeBridge::JavaToNativeBridge(NativeJniBridge & nativeJniBridge)
        : _nativeJniBridge(nativeJniBridge)
{
}

void JavaToNativeBridge::UpdateFrame(int64_t elapsedMs)
{
    auto * application = _nativeJniBridge.GetApplication();
    if (application)
    {
        application->ProcessElapsedTime(TimeState::Milliseconds(elapsedMs));
    }
}

void JavaToNativeBridge::ProcessEvent(const ApplicationEvent & event)
{
    auto * application = _nativeJniBridge.GetApplication();
    if (application)
    {
        application->ProcessEvent(event);
    }
}

void JavaToNativeBridge::RequestAvailableConnectionList(const std::string & hostname)
{
    auto * application = _nativeJniBridge.GetApplication();
    if (application)
    {
        application->RequestAvailableConnections(hostname);
    }
}

void JavaToNativeBridge::CanBeInitialized()
{
    auto * application = _nativeJniBridge.GetApplication();
    if (application)
    {
        application->Initialize();
    }
}
