package com.example.remotemouse.ui.chooseConnection;

import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.example.remotemouse.MainActivity;
import com.example.remotemouse.R;
import com.google.android.material.floatingactionbutton.FloatingActionButton;

public class ChooseConnectionFragment extends Fragment {

    private ChooseConnectionViewModel _chooseConnectionViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        _chooseConnectionViewModel =
                new ViewModelProvider(this).get(ChooseConnectionViewModel.class);
        View root = inflater.inflate(R.layout.fragment_choose_connection, container, false);

        FloatingActionButton fab = root.findViewById(R.id.search_button);
        fab.setOnClickListener(view -> Toast.makeText(getContext(), "Searching...", Toast.LENGTH_SHORT).show());

        Context context = getContext();
        if (context instanceof MainActivity)
        {
            _chooseConnectionViewModel.setNativeBridge(((MainActivity) context).getNativeBridge());
        }

        return root;
    }
}