package com.xiaopeng.ffmpegstudy;

import android.os.SystemClock;
import android.support.test.runner.AndroidJUnit4;

import com.xiaopeng.ffmpegstudy.thread.Consumer;
import com.xiaopeng.ffmpegstudy.thread.Producer;

import org.junit.Test;
import org.junit.runner.RunWith;

/**
 * @author tony
 * @date 2018/9/27
 */
@RunWith(AndroidJUnit4.class)
public class ThreadTest {
    @Test
    public void testObject() {
        Producer p = new Producer();
        p.start();
        new Consumer("Consumer1", p).start();
//        new Consumer("Consumer2", p).start();
//        new Consumer("Consumer3", p).start();
        SystemClock.sleep(100*1000);
    }
}
