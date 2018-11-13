package com.xiaopeng.ffmpegstudy.model;

import android.graphics.Bitmap;

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

    public native void playVideo(String videoPath, VideoCallback callback);

    public interface VideoCallback {
        public void callback(String result);
    }
}
