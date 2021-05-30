package com.example.remotemouse.ui.touchpad;

import androidx.lifecycle.ViewModel;

import com.example.remotemouse.NativeBridge;

public class TouchpadViewModel extends ViewModel {
    private NativeBridge _nativeBridge;

    public TouchpadViewModel() {
    }

    public void setNativeBridge(NativeBridge nativeBridge) {
        _nativeBridge = nativeBridge;
    }

    public void touchedAreaResized(int w, int h)
    {
        if (_nativeBridge != null)
        {
            _nativeBridge.touchAreaResize(w, h);
        }
    }

    public void touched(int x, int y)
    {
        if (_nativeBridge != null)
        {
            _nativeBridge.touchTapping(x, y, NativeBridge.EventTouchType.ShortTap);
        }
    }

    public void touchMoving(int x, int y)
    {
        if (_nativeBridge != null)
        {
            _nativeBridge.touchMoving(x, y);
        }
    }

}