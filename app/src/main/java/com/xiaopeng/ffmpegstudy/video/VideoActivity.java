package com.xiaopeng.ffmpegstudy.video;

import android.Manifest;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;

import com.xiaopeng.ffmpegstudy.R;
import com.xiaopeng.ffmpegstudy.model.VideoModel;

import java.io.File;

import permissions.dispatcher.NeedsPermission;
import permissions.dispatcher.RuntimePermissions;

/**
 * @author tony
 * @date 2018/11/6
 */
public class VideoActivity extends AppCompatActivity {
    private static final String TAG = "VideoActivity";
    VideoModel mModel;
    VideoView mVideoView;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video);
        mModel = new VideoModel();
        mVideoView =(VideoView) findViewById(R.id.player_view);
        mVideoView.setModel(mModel);
        new Thread(new Runnable() {
            @Override
            public void run() {
              mVideoView.play();
            }
        }).start();


    }
}
