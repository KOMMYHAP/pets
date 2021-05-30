package com.example.remotemouse;

import android.util.Log;

public class NativeBridge {
    public final String LOG_TAG = "NativeBridge";

    public enum ApplicationState
    {
        NotInitialized,
        Initialized,
        WaitingForConnect,
        Connected,
        ConnectionRequestTimeout,
        Disconnected,
        DisconnectedByTimeout,
        ErrorOccurred
    }

    public enum ApplicationErrorState
    {
        NotInitialized,
        LocalPortBusy,
        RemotePortEmpty,
        InvalidIp,
        NoError,
    }

    public enum EventTouchType
    {
        ShortTap,
    }


    private ApplicationState        _state;
    private ApplicationErrorState   _errorState;


    static {
        System.loadLibrary("event-dispatcher");
    }

    public NativeBridge()
    {
        cache();
    }

    public void onAvailableConnectionListResponse(AvailableConnectionData[] connectionsList)
    {
        for (int i = 0; i < connectionsList.length; i++) {
            AvailableConnectionData connectionData = connectionsList[i];
            Log.i(LOG_TAG, String.format("%d. %s:%d %s",
                    i + 1,
                    connectionData.ip,
                    connectionData.port,
                    connectionData.hostname));
        }
    }

    public void onStateUpdated(ApplicationState state, ApplicationErrorState error)
    {

    }

    native public void cache();
    native public void updateFrame(long elapsedMs);
    native public void touchAreaResize(int width, int height);
    native public void touchMoving(int x, int y);
    native public void touchTapping(int x, int y, EventTouchType touchType);
    native public void requestAvailableConnections(String hostname);
}
