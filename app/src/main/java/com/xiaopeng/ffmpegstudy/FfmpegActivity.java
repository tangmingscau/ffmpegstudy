package com.xiaopeng.ffmpegstudy;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.TextView;

import com.xiaopeng.ffmpegstudy.androidaudio.AndroidAudioActivity;
import com.xiaopeng.ffmpegstudy.audio.AudioMainActivity;
import com.xiaopeng.ffmpegstudy.video.VideoActivity;

import permissions.dispatcher.NeedsPermission;
import permissions.dispatcher.RuntimePermissions;

/**
 * @author tony
 * @date 2018/10/23
 */
@RuntimePermissions
public class FfmpegActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg);
        findViewById(R.id.btn_audio)
                .setOnClickListener(this);
        findViewById(R.id.btn_video).setOnClickListener(this);
        findViewById(R.id.btn_android_audio).setOnClickListener(this);
    }

    public native String showCodec();

    static {
        System.loadLibrary("xp-ffmpeg");
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_audio:
                startActivity(new Intent(FfmpegActivity.this, AudioMainActivity.class));
                break;
            case R.id.btn_video:
                FfmpegActivityPermissionsDispatcher.showVideoWithPermissionCheck(this);
                break;
            case R.id.btn_android_audio:
                startActivity(new Intent(FfmpegActivity.this, AndroidAudioActivity.class));
                break;
        }

    }

    @SuppressLint("NeedOnRequestPermissionsResult")
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        FfmpegActivityPermissionsDispatcher.onRequestPermissionsResult(this, requestCode, grantResults);
    }

    @NeedsPermission({Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE})
    void showVideo() {
        startActivity(new Intent(FfmpegActivity.this, VideoActivity.class));
    }


}
