package com.xiaopeng.ffmpegstudy.video;

import android.Manifest;
import android.os.Bundle;
import android.os.SystemClock;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.CompoundButton;
import android.widget.RadioButton;
import android.widget.RadioGroup;

import com.xiaopeng.ffmpegstudy.R;
import com.xiaopeng.ffmpegstudy.model.VideoModel;

import java.io.File;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnCheckedChanged;
import permissions.dispatcher.NeedsPermission;
import permissions.dispatcher.RuntimePermissions;

/**
 * @author tony
 * @date 2018/11/6
 */
public class VideoActivity extends AppCompatActivity {
    private static final String TAG = "VideoActivity";
    VideoModel mModel;
    @BindView(R.id.player_view)
    VideoView mVideoView;
    @BindView(R.id.rg_filter)
    RadioGroup rgFilter;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video);
        ButterKnife.bind(this);
        mModel = new VideoModel();
        mVideoView.setModel(mModel);
        rgFilter.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {

            }
        });
    }

    @OnCheckedChanged(R.id.rg_filter)
    public void onCheckedFilter(CompoundButton compoundButton, boolean checked) {
        switch (compoundButton.getId()) {
            case R.id.rb_class:

                break;

        }
    }
}
