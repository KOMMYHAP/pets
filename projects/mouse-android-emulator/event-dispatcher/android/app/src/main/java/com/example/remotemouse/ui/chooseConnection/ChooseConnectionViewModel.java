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
        void onStatusUpdated(boolean connected);
        void onConnectionListReceived(List<AvailableConnectionData> connectionDataList);
    }

    private ConnectionCallback                              _onConnectedCallback;
    private NativeBridge                                    _nativeBridge;
    private MutableLiveData<AvailableConnectionData>         _connection;

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
        if (_onConnectedCallback != null)
        {
            _onConnectedCallback.onStatusUpdated(connected);
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