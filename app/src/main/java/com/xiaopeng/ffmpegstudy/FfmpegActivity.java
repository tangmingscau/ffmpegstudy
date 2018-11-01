package com.xiaopeng.ffmpegstudy;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

/**
 * @author tony
 * @date 2018/10/23
 */

public class FfmpegActivity extends AppCompatActivity {
    private TextView mTvCodec;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg);
        mTvCodec= (TextView) findViewById(R.id.tv_codec);
        mTvCodec.setText(showCodec());
    }
    public native String showCodec();
    static {
        System.loadLibrary("xp-ffmpeg");
    }
}
