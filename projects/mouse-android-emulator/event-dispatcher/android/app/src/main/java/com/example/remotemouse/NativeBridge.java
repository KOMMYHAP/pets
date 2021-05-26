package com.example.remotemouse;

public class NativeBridge {
    static {
        System.loadLibrary("event-dispatcher");
    }

    public NativeBridge()
    {}

    native public int Test();
}
