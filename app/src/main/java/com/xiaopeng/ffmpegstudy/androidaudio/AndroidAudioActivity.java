package com.xiaopeng.ffmpegstudy.androidaudio;

import android.app.Activity;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.widget.LinearLayoutManager;

import com.github.ksoichiro.android.observablescrollview.ObservableRecyclerView;
import com.xiaopeng.ffmpegstudy.R;

import java.io.File;
import java.util.ArrayList;

/**
 * @author tony
 * @date 2018/11/22
 */
public class AndroidAudioActivity extends Activity {
    ObservableRecyclerView mRecyclerView;
    AudioAdapter mAdapter;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_android_audio);
        mRecyclerView = findViewById(R.id.rv_audio);
        mAdapter = new AudioAdapter();
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mRecyclerView.setAdapter(mAdapter);
        getData();
    }

    private void getData() {
        File dir = new File("/sdcard/Music/");
        ArrayList<File> list = new ArrayList<>();
        if (dir.exists() && dir.isDirectory()) {
            for (File file : dir.listFiles()) {
                list.add(file);
            }
        }
        mAdapter.setData(list);
    }
}
