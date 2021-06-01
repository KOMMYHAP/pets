package com.example.remotemouse;

public class AvailableConnectionData {
    public final String ip;
    public final String hostname;
    public final int port;

    public AvailableConnectionData(String ip, String hostname, short port) {
        this.ip = ip;
        this.hostname = hostname;
        this.port = port;
    }
}
