package com.example.remotemouse.ui.chooseConnection;

import androidx.lifecycle.ViewModel;

import com.example.remotemouse.NativeBridge;

public class ChooseConnectionViewModel extends ViewModel {
    private NativeBridge _nativeBridge;

    public ChooseConnectionViewModel() {
    }

    public void setNativeBridge(NativeBridge nativeBridge)
    {
        _nativeBridge = nativeBridge;
    }

    public void startSearching() {
        if (_nativeBridge != null)
        {
            _nativeBridge.requestAvailableConnections("o_o");
        }
    }
}