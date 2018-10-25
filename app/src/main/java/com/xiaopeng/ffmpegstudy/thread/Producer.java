package com.xiaopeng.ffmpegstudy.thread;

import android.util.Log;

import java.util.ArrayList;
import java.util.List;

/**
 * Title: Producer.class<br>
 * Description: 消息生产者<br>
 * Create DateTime: 2016年04月17日 下午1:28 <br>
 *
 * @author podongfeng
 */
public class Producer extends Thread {

    List<Message> msgList = new ArrayList<>();

    @Override
    public void run() {
        try {
            Thread.sleep(3000);
            Message msg = new Message();
            synchronized (msgList) {
                Log.d("tangming", "start notify");
                msgList.add(msg);
                msgList.notify(); //这里只能是notify而不能是notifyAll，否则remove(0)会报java.lang.IndexOutOfBoundsException: Index: 0, Size: 0
                Log.d("tangming", "end notify");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public Message waitMsg() {
        synchronized (msgList) {
            if (msgList.size() == 0) {
                try {
                    Log.d("tangming", "start wait");
                    msgList.wait();
                    Log.d("tangming", "end wait");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            return msgList.remove(0);
        }
    }
}

