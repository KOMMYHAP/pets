package com.example.remotemouse.ui.touchpad;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.example.remotemouse.R;

public class TouchpadFragment extends Fragment {

    private TouchpadViewModel _touchpadViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        _touchpadViewModel =
                new ViewModelProvider(this).get(TouchpadViewModel.class);
        View root = inflater.inflate(R.layout.fragment_touchpad, container, false);
        return root;
    }
}