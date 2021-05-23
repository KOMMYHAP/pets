package com.example.remotemouse;

public class NativeBridge {
    static NativeBridge _instance;

    static {
        loadNativeLib();
    }

    private NativeBridge()
    {}

    public static NativeBridge getInstance() {
        if  (_instance == null)
        {
            _instance = new NativeBridge();
        }
        return _instance;
    }

    static private void loadNativeLib()
    {
        System.loadLibrary("event-dispatcher");
    }

    native public int Test();
}
