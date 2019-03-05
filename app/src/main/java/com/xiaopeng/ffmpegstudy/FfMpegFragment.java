package com.xiaopeng.ffmpegstudy;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.github.ksoichiro.android.observablescrollview.ObservableRecyclerView;
import com.xiaopeng.xannotation.annotation.AutoNull;

/**
 * @author tony
 * @date 2018/12/20
 */
@AutoNull
public class FfMpegFragment extends Fragment {
     ObservableRecyclerView rv;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.activity_android_audio, container, false);
    }

    @Override
    public void onResume() {
        rv = getView().findViewById(R.id.rv_audio);
        super.onResume();
        FfMpegFragment$$Proxy proxy=new FfMpegFragment$$Proxy();
    }
}
