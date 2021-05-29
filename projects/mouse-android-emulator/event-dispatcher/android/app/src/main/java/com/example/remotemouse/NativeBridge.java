package com.example.remotemouse;

import android.util.Log;

import java.util.ArrayList;

public class NativeBridge {
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

    public static class AvailableConnectionData
    {
        public final String     ip;
        public final String     hostname;
        public final int        port;

        public AvailableConnectionData(String ip, String hostname, int port) {
            this.ip = ip;
            this.hostname = hostname;
            this.port = port;
        }
    }


    static {
        System.loadLibrary("event-dispatcher");
        Cache();
    }

    public NativeBridge()
    {}

    public void OnAvailableConnectionListResponse(AvailableConnectionData[] connectionsList)
    {
        for (int i = 0; i < connectionsList.length; i++) {
            AvailableConnectionData connectionData = connectionsList[i];
            Log.i("XXX", String.format("%d. %s:%d %s",
                    i + 1,
                    connectionData.ip,
                    connectionData.port,
                    connectionData.hostname));
        }
    }

    public void OnStateUpdated(ApplicationState state, ApplicationErrorState error)
    {

    }

    native static public void Cache();
    native public void UpdateFrame(long elapsedMs);
    native public void TouchAreaResize(int width, int height);
    native public void MouseMoved(int x, int y);
    native public void MouseClicked(int x, int y, EventTouchType touchType);
}
