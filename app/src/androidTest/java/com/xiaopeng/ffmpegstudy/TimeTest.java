package com.xiaopeng.ffmpegstudy;

import android.os.SystemClock;
import android.support.test.runner.AndroidJUnit4;
import android.util.Log;

import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.Calendar;

/**
 * @author tony
 * @date 2018/10/23
 */
@RunWith(AndroidJUnit4.class)
public class TimeTest {
    @Test
    public void rememberTime() {
        long ts = System.currentTimeMillis();
        Log.e("tony","现在的时间是："+ Calendar.getInstance().getTime().toString());
        SystemClock.sleep(10*1000);
    }
}
