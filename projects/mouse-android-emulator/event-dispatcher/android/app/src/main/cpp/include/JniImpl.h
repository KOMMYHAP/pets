#pragma once

struct NativeBridgeClass { static constexpr auto Name() { return "com/example/remotemouse/NativeBridge"; } };
struct NativeBridge$ApplicationStateClass { static constexpr auto Name() { return "com/example/remotemouse/NativeBridge$ApplicationState"; } };
struct NativeBridge$ApplicationErrorStateClass { static constexpr auto Name() { return "com/example/remotemouse/NativeBridge$ApplicationErrorState"; } };
struct EventTouchTypeClass { static constexpr auto Name() { return "com/example/remotemouse/NativeBridge$EventTouchType"; } };
struct AvailableConnectionDataClass { static constexpr auto Name() { return "com/example/remotemouse/AvailableConnectionData"; } };

using NativeBridgeObject = jni::Object<NativeBridgeClass>;
using NativeBridge$ApplicationStateObject = jni::Object<NativeBridge$ApplicationStateClass>;
using NativeBridge$ApplicationErrorStateObject = jni::Object<NativeBridge$ApplicationErrorStateClass>;
using EventTouchTypeObject = jni::Object<EventTouchTypeClass>;
using AvailableConnectionDataObject = jni::Object<AvailableConnectionDataClass>;

namespace details
{
    JNIEnv * GetEnv(JavaVM * jvm);
}

