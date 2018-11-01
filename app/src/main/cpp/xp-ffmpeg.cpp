#include <jni.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
extern "C" {

JNIEXPORT jstring JNICALL
Java_com_xiaopeng_ffmpegstudy_FfmpegActivity_showCodec(JNIEnv *env, jobject instance) {

    // TODO
//    av_register_all();


    return (jstring) "1";
}
}
