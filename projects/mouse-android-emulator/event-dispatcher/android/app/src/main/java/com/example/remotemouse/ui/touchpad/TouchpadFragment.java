package com.example.remotemouse.ui.touchpad;

import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.example.remotemouse.MainActivity;
import com.example.remotemouse.R;

public class TouchpadFragment extends Fragment {
    private TouchpadViewModel _touchpadViewModel;
    private View _touchPadView;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        _touchpadViewModel =
                new ViewModelProvider(this).get(TouchpadViewModel.class);
        View root = inflater.inflate(R.layout.fragment_touchpad, container, false);
        _touchPadView = root.findViewById(R.id.touchPadView);

        Context context = getContext();
        if (context instanceof MainActivity)
        {
            _touchpadViewModel.setNativeBridge(((MainActivity) context).getNativeBridge());
        }

        _touchPadView.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_MOVE)
            {
                int x = ((int) event.getX());
                int y = ((int) event.getY());
                _touchpadViewModel.touchMoving(x, y);
                return true;
            }
            if (event.getAction() == MotionEvent.ACTION_DOWN)
            {
                int x = ((int) event.getX());
                int y = ((int) event.getY());
                _touchpadViewModel.touched(x, y);
                return true;
            }
            return v.performClick();
        });

        notifyTouchAreaSizeUpdated();

        return root;
    }

    public void notifyTouchAreaSizeUpdated()
    {
        if (_touchPadView != null)
        {
            _touchpadViewModel.touchedAreaResized(_touchPadView.getWidth(), _touchPadView.getHeight());
        }
    }
}