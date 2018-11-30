package com.xiaopeng.ffmpegstudy.audio;

import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.xiaopeng.ffmpegstudy.R;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

/**
 * @author tony
 * @date 2018/11/2
 */
public class AudioMainActivity extends Activity  {
    AudioPlayer mPlayer;
    @BindView(R.id.btn_play)
    Button mBtnPlay;
    @BindView(R.id.btn_pause)
    Button mBtnPause;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_audio);
        ButterKnife.bind(this);
        mPlayer=new AudioPlayer();
    }
    @OnClick(R.id.btn_play)
    public void play(){
        mPlayer.play("/sdcard/Music/audio/audio.mp3");
    }
}
