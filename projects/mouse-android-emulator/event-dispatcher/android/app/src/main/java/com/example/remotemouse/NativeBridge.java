package com.example.remotemouse;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import java.util.Arrays;
import java.util.List;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class NativeBridge {
    public final String LOG_TAG = "NativeBridge";

    public enum ApplicationState
    {
        NotInitialized,
        Idle,
        SearchingConnections,
        WaitingForConnect,
        Connected,
        ConnectionRequestTimeout,
        Disconnected,
        DisconnectedByTimeout,
        ErrorOccurred,
    }

    public enum ApplicationErrorState
    {
        NotInitialized,
        LocalPortBusy,
        RemotePortEmpty,
        InvalidIp,
        NoError,
        _InvalidNativeState,
    }

    public enum EventTouchType
    {
        ShortTap,
    }

    private static NativeBridge                         _instance;
    private final UiThreadExecutor                      _updateExecutor = new UiThreadExecutor();
    MutableLiveData<List<AvailableConnectionData>>      _connectionList = new MutableLiveData<>();
    MutableLiveData<ApplicationState>                   _state = new MutableLiveData<>();

    private NativeBridge()
    {
        System.loadLibrary("event-dispatcher");
        cache();
        initialize();

        ScheduledExecutorService _scheduledExecutorService = Executors.newSingleThreadScheduledExecutor();
        _scheduledExecutorService.scheduleWithFixedDelay(() -> _updateExecutor.execute(() -> updateFrame(15)), 0, 15, TimeUnit.MILLISECONDS);
    }

    static public NativeBridge getInstance()
    {
        if (_instance == null)
        {
            _instance = new NativeBridge();
        }
        return _instance;
    }

    public LiveData<List<AvailableConnectionData>> getConnectionList()
    {
        return _connectionList;
    }
    public LiveData<ApplicationState> getState() { return _state; }

    public void onAvailableConnectionListResponse(AvailableConnectionData[] connectionsList)
    {
        for (int i = 0; i < connectionsList.length; i++) {
            AvailableConnectionData connectionData = connectionsList[i];
            Log.i(LOG_TAG, String.format("%d. %s:%d %s",
                    i + 1,
                    connectionData.ip,
                    connectionData.port & 0xFFFFL,
                    connectionData.hostname));
        }
        _connectionList.setValue(Arrays.asList(connectionsList));
    }

    public void onStateUpdated(ApplicationState state, ApplicationErrorState error)
    {
        Log.i(LOG_TAG, "State updated to " + state.toString());
        if (state == ApplicationState.ErrorOccurred)
        {
            Log.e(LOG_TAG, "Error state updated to " + error.toString());
        }
        _state.setValue(state);
    }

    native private void cache();
    native private void initialize();
    native public void updateFrame(long elapsedMs);
    native public void touchAreaResize(int width, int height);
    native public void touchMoving(int x, int y);
    native public void touchTapping(int x, int y, EventTouchType touchType);
    native public void requestAvailableConnections(String hostname);
    native public void connect(String ip, short port);
}
