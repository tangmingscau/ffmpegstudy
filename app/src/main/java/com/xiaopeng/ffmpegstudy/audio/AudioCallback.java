package com.xiaopeng.ffmpegstudy.audio;

public interface AudioCallback {
    public void audioInfoPlayback(int sampleHz, int channel, int bufferSizeInByte);

    public void write(byte[] bytes,int bufferSize);

}
