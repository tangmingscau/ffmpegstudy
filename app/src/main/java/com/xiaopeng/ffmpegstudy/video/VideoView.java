package com.xiaopeng.ffmpegstudy.video;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.PixelFormat;
import android.support.annotation.IntDef;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.xiaopeng.ffmpegstudy.model.VideoModel;
import com.xiaopeng.ffmpegstudy.util.BitmapUtil;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

/**
 * @author tony
 * @date 2018/11/12
 */
public class VideoView extends SurfaceView implements SurfaceHolder.Callback, VideoModel.VideoCallback {
    private static final String TAG = "VideoView";

    @IntDef({STATE_IDLE, STATE_BUFFERING, STATE_PLAYING, STATE_PAUSE})
    @Retention(RetentionPolicy.SOURCE)
    public @interface State {

    }

    public static final int STATE_IDLE = -1;
    public static final int STATE_BUFFERING = 1;
    public static final int STATE_PLAYING = 2;
    public static final int STATE_PAUSE = 3;

    private int mPlayState;
    VideoModel mVideoModel;

    public VideoView(Context context) {
        super(context);
        initVideoView();
    }

    public VideoView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initVideoView();
    }

    public VideoView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initVideoView();
    }

    private void initVideoView() {
        getHolder().setFormat(PixelFormat.RGB_888);
        getHolder().addCallback(this);
        mPlayState = STATE_IDLE;
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public void setModel(VideoModel model) {
        mVideoModel = model;
    }

    public void play() {
        mVideoModel.playVideo2("/sdcard/Music/normal.mp4", this);
    }

    @Override
    public void callback(byte[] result) {
        Log.d(TAG, "callback =" + new String(result));
        Bitmap bitmap = BitmapUtil.getBitmap(result);
        if (bitmap == null) {
            return;
        }
        Log.d(TAG, "callback bitmap=" + bitmap);
        Canvas canvas = getHolder().lockCanvas();
        canvas.drawBitmap(bitmap, 0, 0, null);
        getHolder().unlockCanvasAndPost(canvas);
    }

}
