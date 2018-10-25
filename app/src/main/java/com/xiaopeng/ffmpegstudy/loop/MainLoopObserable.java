package com.xiaopeng.ffmpegstudy.loop;

import android.os.Binder;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.MessageQueue;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

/**
 * @author tony
 * @date 2018/9/26
 */

public class MainLoopObserable {
    public static boolean mNeedCrash=false;
    public static void hookMsgDispatchOfLooper() {
        (new Handler(Looper.getMainLooper())).post(new Runnable() {
            public void run() {
                try {
                    Looper t = Looper.getMainLooper();
                    Field fieldQueue = t.getClass().getDeclaredField("mQueue");
                    fieldQueue.setAccessible(true);
                    MessageQueue queue = (MessageQueue) fieldQueue.get(t);
                    Method methodNext = queue.getClass().getDeclaredMethod("next", new Class[0]);
                    methodNext.setAccessible(true);
                    Binder.clearCallingIdentity();
                    boolean callReflectRecycledMethod = false;
                    Method methodRecycleUnchecked = null;
                    Message msg;
                    if (Build.VERSION.SDK_INT >= 21) {
                        callReflectRecycledMethod = true;
                        msg = Message.obtain();
                        methodRecycleUnchecked = msg.getClass().getDeclaredMethod("recycleUnchecked", new Class[0]);
                        methodRecycleUnchecked.setAccessible(true);
                        msg.recycle();
                    }

                    while (true) {
                        msg = (Message) methodNext.invoke(queue, new Object[0]);
                        if (mNeedCrash){
                            throwCrash();
                        }
                        if (msg == null) {
                            return;
                        }

                        msg.getTarget().dispatchMessage(msg);
                        if (callReflectRecycledMethod) {
                            methodRecycleUnchecked.invoke(msg, new Object[0]);
                        } else {
                            msg.recycle();
                        }
                    }
                } catch (Throwable var9) {
                    var9.printStackTrace();
                }
            }
        });
    }
    public static void throwCrash(){
        throw  new IllegalArgumentException("main crash");
    }
}
