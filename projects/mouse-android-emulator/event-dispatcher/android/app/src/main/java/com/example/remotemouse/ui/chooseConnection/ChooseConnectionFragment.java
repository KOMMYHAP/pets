package com.example.remotemouse.ui.chooseConnection;

import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ScrollView;
import android.widget.TextView;
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
    private TextView _searchHintView;
    private boolean _connecting = false;
    private boolean _searching = false;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        _chooseConnectionViewModel =
                new ViewModelProvider(this).get(ChooseConnectionViewModel.class);
        View root = inflater.inflate(R.layout.fragment_choose_connection, container, false);

        _searchButton = root.findViewById(R.id.search_button);
        _searchButton.setOnClickListener(view -> startSearching());

        _searchHintView = root.findViewById(R.id.header_text);

        _scrollConnectionList = root.findViewById(R.id.scrollConnectionList);

        Context context = getContext();
        if (context instanceof MainActivity)
        {
            NativeBridge nativeBridge = ((MainActivity) context).getNativeBridge();
            _chooseConnectionViewModel.setNativeBridge(getViewLifecycleOwner(), nativeBridge);
            _chooseConnectionViewModel.setConnectionCallback(new ChooseConnectionViewModel.ConnectionCallback() {
                @Override
                public void onSearchStatusUpdated(boolean searching) {
                    notifySearchStateChanged(searching);
                }

                @Override
                public void onConnectionStatusUpdated(boolean connected) {
                    notifyConnectionStateChanged(connected);
                }

                @Override
                public void onConnectionListReceived(List<AvailableConnectionData> connectionDataList) {
                    if (_searchHintView != null && connectionDataList.isEmpty())
                    {
                        _searchHintView.setVisibility(View.VISIBLE);
                    }
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

    public void startSearching()
    {
        if (_chooseConnectionViewModel != null)
        {
            _searching = true;
            _chooseConnectionViewModel.startSearching();
        }
    }

    private void searchStarted()
    {
//        if (_searchHintView != null)
//        {
//            _searchHintView.setVisibility(View.GONE);
//        }
        _searchButton.setEnabled(false);
        Toast.makeText(getContext(), "Searching...", Toast.LENGTH_LONG).show();
    }

    private void searchStopped()
    {
        _searching = false;
        _searchButton.setEnabled(true);
    }

    public void notifySearchStateChanged(boolean searching)
    {
        if (!_searching)
        {
            return;
        }
        if (searching)
        {
            searchStarted();
        }
        else
        {
            searchStopped();
        }
    }

    private void connectionClicked(AvailableConnectionData connectionData)
    {
        if (_scrollConnectionList != null && _chooseConnectionViewModel != null)
        {
            _connecting = true;
            _scrollConnectionList.setEnabled(false);
            _chooseConnectionViewModel.connect(connectionData);
        }
    }

    public void notifyConnectionStateChanged(boolean connected)
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