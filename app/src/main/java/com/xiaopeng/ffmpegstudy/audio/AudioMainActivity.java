package com.xiaopeng.ffmpegstudy.audio;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.View;

import com.xiaopeng.ffmpegstudy.R;

/**
 * @author tony
 * @date 2018/11/2
 */
public class AudioMainActivity extends Activity implements View.OnClickListener {
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_audio);
    }

    @Override
    public void onClick(View v) {
    }
}
