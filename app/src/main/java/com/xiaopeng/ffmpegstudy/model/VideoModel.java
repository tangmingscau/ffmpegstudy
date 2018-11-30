package com.xiaopeng.ffmpegstudy.model;

import android.graphics.Bitmap;
import android.view.SurfaceView;

/**
 * @author tony
 * @date 2018/11/6
 */
public class VideoModel {
    static {
        System.loadLibrary("xp-ffmpeg");
    }

    public void mp4Toh264(String filePath) {

    }

    public native String mp4ToH264(String path);

    public native void playVideo3(String videoPath, VideoCallback callback, Object surface);
    public interface VideoCallback {
        public void callback(byte[] result);
    }
}
