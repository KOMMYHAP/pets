#include "JavaToNativeBridge.h"
#include "EventDispatcherApplication.h"
#include "application/ApplicationEvent.h"

JavaToNativeBridge::JavaToNativeBridge(EventDispatcherApplication& eventDispatcher)
        : _eventDispatcher(eventDispatcher)
{
}

void JavaToNativeBridge::UpdateFrame(int64_t elapsedMs)
{
    _eventDispatcher.ProcessElapsedTime(TimeState::Milliseconds(elapsedMs));
}

void JavaToNativeBridge::MouseMoved(int x, int y)
{
    ApplicationEvents::MouseMoved mouseMoved{x, y};
    _eventDispatcher.ProcessEvent(ApplicationEvent(mouseMoved));
}

void JavaToNativeBridge::RequestAvailableConnectionList() {

}
