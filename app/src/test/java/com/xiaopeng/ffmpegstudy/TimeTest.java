package com.xiaopeng.ffmpegstudy;

import android.util.Log;

import org.junit.Test;

import java.util.Calendar;

/**
 * @author tony
 * @date 2018/10/23
 */

public class TimeTest {
    @Test
    public void rememberTime() {
        long ts = System.currentTimeMillis();
        Log.d("tony","现在的时间是："+ Calendar.getInstance().getTime().toString());
    }
}
