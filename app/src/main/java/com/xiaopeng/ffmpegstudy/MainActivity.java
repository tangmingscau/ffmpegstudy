package com.xiaopeng.ffmpegstudy;

import android.os.Handler;
import android.os.Looper;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.xiaopeng.ffmpegstudy.loop.MainLoopObserable;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private static final String TAG = "MainActivity";
    Button mBt1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mBt1 = (Button) findViewById(R.id.bt_1);
        mBt1.setOnClickListener(this);
        findViewById(R.id.bt_100).setOnClickListener(this);
        findViewById(R.id.bt_click).setOnClickListener(this);
//        MainLoopObserable.hookMsgDispatchOfLooper();

    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.bt_1:
                new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        for (int i = 0; i < 1; i++) {
                            new Handler(Looper.getMainLooper()).post(new Runnable() {
                                @Override
                                public void run() {
                                    Log.d(TAG, "main run start");
                                    SystemClock.sleep(25*1000);
                                    Log.d(TAG, "main run end");
                                }
                            });
                        }
                    }
                },1000);
                break;
            case R.id.bt_100:
                new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        for (int i = 0; i < 100; i++) {
                            new Handler(Looper.getMainLooper()).post(new Runnable() {
                                @Override
                                public void run() {
                                    Log.d(TAG, "main run start");
                                    SystemClock.sleep(1*1000);
                                    Log.d(TAG, "main run end");
                                }
                            });
                        }
                    }
                },1000);

                break;
            case R.id.bt_click:
                Log.d(TAG,"click");
                break;
        }
    }
}
