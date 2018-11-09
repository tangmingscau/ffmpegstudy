package com.xiaopeng.ffmpegstudy.model;

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
}
