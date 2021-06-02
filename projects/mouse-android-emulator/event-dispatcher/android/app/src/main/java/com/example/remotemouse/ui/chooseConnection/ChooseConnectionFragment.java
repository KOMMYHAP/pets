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

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        _chooseConnectionViewModel =
                new ViewModelProvider(this).get(ChooseConnectionViewModel.class);
        View root = inflater.inflate(R.layout.fragment_choose_connection, container, false);

        FloatingActionButton fab = root.findViewById(R.id.search_button);
        fab.setOnClickListener(view ->
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
                    if (connected) {
                        onConnected();
                    }
                    else {
                        onConnectionFailed();
                    }
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
            _scrollConnectionList.setEnabled(false);
            _chooseConnectionViewModel.connect(connectionData);
        }
    }

    private void onConnected()
    {
        if (_scrollConnectionList != null)
        {
            _scrollConnectionList.setEnabled(true);
        }
    }

    private void onConnectionFailed()
    {
        Toast.makeText(getContext(), "Connection failed", Toast.LENGTH_SHORT).show();
    }
}