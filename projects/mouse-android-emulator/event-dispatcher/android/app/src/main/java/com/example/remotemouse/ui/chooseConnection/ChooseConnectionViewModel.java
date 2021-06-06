package com.example.remotemouse.ui.chooseConnection;

import androidx.lifecycle.LifecycleOwner;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.remotemouse.AvailableConnectionData;
import com.example.remotemouse.NativeBridge;

import java.util.List;

public class ChooseConnectionViewModel extends ViewModel {
    public interface ConnectionCallback
    {
        void onSearchStatusUpdated(boolean searching);
        void onConnectionStatusUpdated(boolean connected);
        void onConnectionListReceived(List<AvailableConnectionData> connectionDataList);
    }

    private ConnectionCallback                              _onConnectedCallback;
    private NativeBridge                                    _nativeBridge;

    public ChooseConnectionViewModel() {
    }

    public void setNativeBridge(LifecycleOwner lifecycleOwner, NativeBridge nativeBridge)
    {
        _nativeBridge = nativeBridge;
        _nativeBridge.getState().observe(lifecycleOwner, this::onApplicationStateChanged);
        _nativeBridge.getConnectionList().observe(lifecycleOwner, this::onConnectionListReceived);
    }

    public void startSearching() {
        if (_nativeBridge != null)
        {
            _nativeBridge.requestAvailableConnections("Mobile Tester");
        }
    }

    public void connect(AvailableConnectionData connection)
    {
        if (_nativeBridge != null) {
            _nativeBridge.connect(connection.ip, ((short) connection.port));
        }
    }

    public void setConnectionCallback(ConnectionCallback connectionCallback)
    {
        _onConnectedCallback = connectionCallback;
    }

    private void onApplicationStateChanged(NativeBridge.ApplicationState state)
    {
        final boolean connected = state == NativeBridge.ApplicationState.Connected;
        final boolean searching = state == NativeBridge.ApplicationState.SearchingConnections;
        final boolean idle = state == NativeBridge.ApplicationState.Idle;
        final boolean error = state == NativeBridge.ApplicationState.ErrorOccurred;
        if (_onConnectedCallback != null)
        {
            if (connected || error)
            {
                _onConnectedCallback.onConnectionStatusUpdated(connected);
            }
            if (searching || idle || error)
            {
                _onConnectedCallback.onSearchStatusUpdated(searching);
            }
        }
    }

    private void onConnectionListReceived(List<AvailableConnectionData> connectionDataList)
    {
        if (_onConnectedCallback != null)
        {
            _onConnectedCallback.onConnectionListReceived(connectionDataList);
        }
    }
}