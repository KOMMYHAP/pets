package com.example.remotemouse.ui.chooseConnection;

import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ScrollView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.example.remotemouse.AvailableConnectionData;
import com.example.remotemouse.MainActivity;
import com.example.remotemouse.NativeBridge;
import com.example.remotemouse.R;
import com.google.android.material.floatingactionbutton.FloatingActionButton;

import java.util.List;

public class ChooseConnectionFragment extends Fragment {
    private ChooseConnectionViewModel _chooseConnectionViewModel;
    private ScrollView _scrollConnectionList;
    private FloatingActionButton _searchButton;
    private boolean _connecting = false;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        _chooseConnectionViewModel =
                new ViewModelProvider(this).get(ChooseConnectionViewModel.class);
        View root = inflater.inflate(R.layout.fragment_choose_connection, container, false);

        _searchButton = root.findViewById(R.id.search_button);
        _searchButton.setOnClickListener(view ->
        {
            _chooseConnectionViewModel.startSearching();
            Toast.makeText(getContext(), "Searching...", Toast.LENGTH_SHORT).show();
        });

        _scrollConnectionList = root.findViewById(R.id.scrollConnectionList);

        Context context = getContext();
        if (context instanceof MainActivity)
        {
            NativeBridge nativeBridge = ((MainActivity) context).getNativeBridge();
            _chooseConnectionViewModel.setNativeBridge(getViewLifecycleOwner(), nativeBridge);
            _chooseConnectionViewModel.setConnectionCallback(new ChooseConnectionViewModel.ConnectionCallback() {
                @Override
                public void onStatusUpdated(boolean connected) {
                    notifyConnectionChanged(connected);
                }

                @Override
                public void onConnectionListReceived(List<AvailableConnectionData> connectionDataList) {
                    if (_scrollConnectionList != null)
                    {
                        _scrollConnectionList.removeAllViews();
                        for (AvailableConnectionData connection : connectionDataList)
                        {
                            // todo: create custom widget
                            Button button = new Button(getContext());
                            button.setText(connection.hostname);
                            button.setOnClickListener(v -> connectionClicked(connection));
                            _scrollConnectionList.addView(button);
                        }
                    }
                }
            });
        }

        return root;
    }

    public void connectionClicked(AvailableConnectionData connectionData)
    {
        if (_scrollConnectionList != null && _chooseConnectionViewModel != null)
        {
            _connecting = true;
            _scrollConnectionList.setEnabled(false);
            _searchButton.setEnabled(false);
            _chooseConnectionViewModel.connect(connectionData);
        }
    }

    private void notifyConnectionChanged(boolean connected)
    {
        if (!_connecting)
        {
            return;
        }
        if (connected)
        {
            onConnected();
        }
        else
        {
            onConnectionFailed();
        }

        _scrollConnectionList.setEnabled(true);
        _searchButton.setEnabled(true);
        _connecting = false;
    }

    private void onConnected()
    {
        Toast.makeText(getContext(), "Connected!", Toast.LENGTH_SHORT).show();
    }

    private void onConnectionFailed()
    {
        Toast.makeText(getContext(), "Connection failed", Toast.LENGTH_SHORT).show();
    }
}