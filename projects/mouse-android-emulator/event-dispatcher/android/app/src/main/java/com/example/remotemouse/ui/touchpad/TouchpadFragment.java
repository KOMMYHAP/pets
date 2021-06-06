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
    private int _prevX;
    private int _prevY;

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
                final boolean firstMove = _prevX == 0 && _prevY == 0;

                final int x = ((int) event.getX());
                final int y = ((int) event.getY());

                final int deltaX = x - _prevX;
                final int deltaY = y - _prevY;

                _prevX = x;
                _prevY = y;

                if (!firstMove)
                {
                    _touchpadViewModel.touchMoving(deltaX, deltaY);
                    return true;
                }
            }
            else if (event.getAction() == MotionEvent.ACTION_DOWN)
            {
                int x = ((int) event.getX());
                int y = ((int) event.getY());
                _touchpadViewModel.touched(x, y);
                return true;
            }
            return v.performClick();
        });

        _touchPadView.post(this::notifyTouchAreaSizeUpdated);
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