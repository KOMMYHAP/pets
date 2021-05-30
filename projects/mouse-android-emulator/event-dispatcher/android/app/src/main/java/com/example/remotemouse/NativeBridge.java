package com.example.remotemouse;

import android.util.Log;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

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
    MutableLiveData<List<AvailableConnectionData>> _connectionList;


    static {
        System.loadLibrary("event-dispatcher");
    }

    public NativeBridge()
    {
        cache();
        updateFrame(42);
        touchAreaResize(42, 42);
        touchMoving(42, 42);
        touchTapping(42, 42, EventTouchType.ShortTap);
        requestAvailableConnections("q_q");
    }

    public LiveData<List<AvailableConnectionData>> getConnectionList()
    {
        return _connectionList;
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
        _connectionList.setValue(Arrays.asList(connectionsList));
    }

    public void onStateUpdated(ApplicationState state, ApplicationErrorState error)
    {
        _state = state;
        _errorState = error;
        Log.i(LOG_TAG, "State updated to " + state.toString());
        if (state == ApplicationState.ErrorOccurred)
        {
            Log.e(LOG_TAG, "Error state updated to " + error.toString());
        }
    }

    native public void cache();
    native public void updateFrame(long elapsedMs);
    native public void touchAreaResize(int width, int height);
    native public void touchMoving(int x, int y);
    native public void touchTapping(int x, int y, EventTouchType touchType);
    native public void requestAvailableConnections(String hostname);
}
