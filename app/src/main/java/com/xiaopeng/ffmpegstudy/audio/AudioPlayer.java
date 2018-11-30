package com.xiaopeng.ffmpegstudy.audio;

import android.media.AudioAttributes;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.media.MediaRecorder;
import android.os.Build;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;

import static android.media.AudioFormat.CHANNEL_OUT_STEREO;
import static android.media.AudioFormat.ENCODING_PCM_16BIT;
import static android.media.AudioFormat.SAMPLE_RATE_UNSPECIFIED;
import static android.media.AudioTrack.MODE_STREAM;

/**
 * @author tony
 * @date 2018/11/27
 */
public class AudioPlayer implements AudioCallback {
    private static final String TAG = "AudioPlayer";
    AudioTrack mAudioTrack;
    Handler mHandler;

    public AudioPlayer() {
        HandlerThread handlerThread = new HandlerThread("audio_play");
        handlerThread.start();
        mHandler = new Handler(handlerThread.getLooper());

    }

    public void play(final String uri) {
        Log.d(TAG, "play " + uri);
        new Thread(new Runnable() {
            @Override
            public void run() {
                releaseInternal();
                playAudio(uri, AudioPlayer.this);
            }
        }).start();

    }

    public void pause() {

    }

    public void release() {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                releaseInternal();
            }
        });
    }

    private void releaseInternal() {
        if (mAudioTrack != null) {
            mAudioTrack.stop();
            mAudioTrack.release();
            mAudioTrack = null;
        }
    }

    public native int decodeAudio(String path);

    public native int playAudio(String path, AudioCallback callback);

    @Override
    public void audioInfoPlayback(int sampleHz, int channel, int audioFormat) {
        Log.d(TAG, "audioInfoPlayback：sampleHz=" + sampleHz + " channel=" + channel + " audioFormat=" + audioFormat);
        mAudioTrack = createAudioTrack(sampleHz, channel);
        mAudioTrack.play();
    }

    @Override
    public void write(byte[] bytes,int bufferSize) {
        Log.d(TAG, "write " + bufferSize);
        mAudioTrack.write(bytes, 0, bufferSize);
    }

    /**
     * 创建一个AudioTrack对象
     *
     * @param sampleRate 采样率
     * @param channels   声道布局
     * @return AudioTrack
     */
    public AudioTrack createAudioTrack(int sampleRate, int channels) {
        int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
        int channelConfig;
        if (channels == 1) {
            channelConfig = AudioFormat.CHANNEL_OUT_MONO;
        } else if (channels == 2) {
            channelConfig = AudioFormat.CHANNEL_OUT_STEREO;
        } else {
            channelConfig = AudioFormat.CHANNEL_OUT_STEREO;
        }

        int bufferSizeInBytes = AudioTrack.getMinBufferSize(sampleRate, channelConfig, audioFormat);

        return new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate, channelConfig, audioFormat,
                bufferSizeInBytes, AudioTrack.MODE_STREAM);
    }
}
