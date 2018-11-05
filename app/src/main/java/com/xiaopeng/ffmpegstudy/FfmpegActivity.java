package com.xiaopeng.ffmpegstudy;

import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.TextView;

import com.xiaopeng.ffmpegstudy.audio.AudioMainActivity;

/**
 * @author tony
 * @date 2018/10/23
 */

public class FfmpegActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg);
        findViewById(R.id.btn_audio)
                .setOnClickListener(this);

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
        }

    }
}
