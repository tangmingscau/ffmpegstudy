//
// Created by tony on 2018/11/24.
//

#include <jni.h>
#include <android/log.h>
#include "audio.h"
#include <unistd.h>

#define TAG "tangmingjni"
#define MAX_AUDIO_FRAME_SIZE 48000*4
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
JNIEXPORT jint JNICALL
Java_com_xiaopeng_ffmpegstudy_audio_AudioPlayer_decodeAudio(JNIEnv *env, jobject instance,
                                                            jstring path_) {
    const char *path = env->GetStringUTFChars(path_, NULL);

    FILE *outfile = fopen("/sdcard/Music/audio/audio.wav", "wb");

    //注册组件
    av_register_all();
    avformat_network_init();

    AVFormatContext *avFormatContext = avformat_alloc_context();
    if (avformat_open_input(&avFormatContext, path, NULL, NULL) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can not open audio");
        return -1;
    }
    if (avformat_find_stream_info(avFormatContext, NULL) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can not find stram info");
    }
    int audio_stream_index = -1;
    for (int i = 0; i < avFormatContext->nb_streams; ++i) {
        if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            break;
        }
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "find audio stream index %d", audio_stream_index);
    //获取音频解码器
    AVCodec *avCodec = avcodec_find_decoder(
            avFormatContext->streams[audio_stream_index]->codecpar->codec_id);
    AVCodecContext *avCodecContext = avcodec_alloc_context3(avCodec);
    avcodec_parameters_to_context(avCodecContext,
                                  avFormatContext->streams[audio_stream_index]->codecpar);
    av_codec_set_pkt_timebase(avCodecContext,
                              avFormatContext->streams[audio_stream_index]->time_base);
    if (avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "can not open codec");
        return -1;
    }
    AVPacket *avPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    AVFrame *avFrame = av_frame_alloc();
    SwrContext *swrContext = swr_alloc();
    uint8_t *out_buffer = (uint8_t *) av_malloc(MAX_AUDIO_FRAME_SIZE);
    //输入采用格式
    enum AVSampleFormat in_sample_fmt = avCodecContext->sample_fmt;
    //输出采样率16bit pcm
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    //输入采样率
    int in_sample_rate = avCodecContext->sample_rate;
    //输出采用率
    int out_sample_rate = in_sample_rate;
    //声道布局
    uint64_t in_ch_layout = avCodecContext->channel_layout;
    //输出是立体声
    uint64_t out_ch_layout = AV_CH_LAYOUT_STEREO;

    swr_alloc_set_opts(swrContext, out_ch_layout, out_sample_fmt, out_sample_rate, in_ch_layout,
                       in_sample_fmt, in_sample_rate, 0, NULL);
    swr_init(swrContext);
    int out_channel_nb = av_get_channel_layout_nb_channels(out_ch_layout);
    while (av_read_frame(avFormatContext, avPacket) >= 0) {
        //解码类型是音频
        if (avPacket->stream_index == audio_stream_index) {
            int ret = avcodec_send_packet(avCodecContext, avPacket);
            if (ret < 0) {
                __android_log_print(ANDROID_LOG_ERROR, TAG, "send packet error");
            }
            ret = avcodec_receive_frame(avCodecContext, avFrame);
            if (ret < 0) {
                __android_log_print(ANDROID_LOG_ERROR, TAG, "decode frame error");
            }
            swr_convert(swrContext, &out_buffer, MAX_AUDIO_FRAME_SIZE,
                        (const uint8_t **) avFrame->data, avFrame->nb_samples);
            int out_buffer_size = av_samples_get_buffer_size(NULL,
                                                             out_channel_nb, avFrame->nb_samples,
                                                             out_sample_fmt, 1);
            fwrite(out_buffer, 1, (size_t) out_buffer_size, outfile);
        }
        av_packet_unref(avPacket);
    }
    fclose(outfile);
    av_frame_free(&avFrame);
    av_free(out_buffer);
    swr_free(&swrContext);
    avcodec_close(avCodecContext);
    avformat_close_input(&avFormatContext);
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "free success");
    env->ReleaseStringUTFChars(path_, path);
    return 1;
}
}extern "C"
JNIEXPORT jint JNICALL
Java_com_xiaopeng_ffmpegstudy_audio_AudioPlayer_playAudio(JNIEnv *env, jobject instance,
                                                          jstring path_, jobject callback) {
    const char *path = env->GetStringUTFChars(path_, 0);
    bool needDeteach = false;
    JavaVM *g_Vm;
    (*env).GetJavaVM(&g_Vm);
    jobject audioCallback = (*env).NewGlobalRef(callback);
    if ((*g_Vm).GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED) {
        g_Vm->AttachCurrentThread(&env, NULL);
        needDeteach = true;
    }
    jclass audioCallbackClass = (*env).GetObjectClass(audioCallback);
    jmethodID audioPlaybackMethodID = (*env).GetMethodID(audioCallbackClass, "audioInfoPlayback",
                                                         "(III)V");
    jmethodID audioWriteMethodID = (*env).GetMethodID(audioCallbackClass, "write", "([BI)V");
    //注册组件
    av_register_all();
    avformat_network_init();

    AVFormatContext *avFormatContext = avformat_alloc_context();
    if (avformat_open_input(&avFormatContext, path, NULL, NULL) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can not open audio");
        return -1;
    }
    if (avformat_find_stream_info(avFormatContext, NULL) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can not find stram info");
    }
    int audio_stream_index = -1;
    for (int i = 0; i < avFormatContext->nb_streams; ++i) {
        if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            break;
        }
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "find audio stream index %d", audio_stream_index);
    //获取音频解码器
    AVCodec *avCodec = avcodec_find_decoder(
            avFormatContext->streams[audio_stream_index]->codecpar->codec_id);
    AVCodecContext *avCodecContext = avcodec_alloc_context3(avCodec);
    avcodec_parameters_to_context(avCodecContext,
                                  avFormatContext->streams[audio_stream_index]->codecpar);
    av_codec_set_pkt_timebase(avCodecContext,
                              avFormatContext->streams[audio_stream_index]->time_base);
    if (avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "can not open codec");
        return -1;
    }
    AVPacket *avPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    AVFrame *avFrame = av_frame_alloc();
    SwrContext *swrContext = swr_alloc();
    uint8_t *out_buffer = (uint8_t *) av_malloc(MAX_AUDIO_FRAME_SIZE);
    //输入采用格式
    enum AVSampleFormat in_sample_fmt = avCodecContext->sample_fmt;
    //输出采样率16bit pcm
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    //输入采样率
    int in_sample_rate = avCodecContext->sample_rate;
    //输出采用率
    int out_sample_rate = in_sample_rate;
    //声道布局
    uint64_t in_ch_layout = avCodecContext->channel_layout;
    //输出是立体声
    uint64_t out_ch_layout = AV_CH_LAYOUT_STEREO;


    swr_alloc_set_opts(swrContext, out_ch_layout, out_sample_fmt, out_sample_rate, in_ch_layout,
                       in_sample_fmt, in_sample_rate, 0, NULL);
    swr_init(swrContext);
    int out_channel_nb = av_get_channel_layout_nb_channels(out_ch_layout);
    (*env).CallVoidMethod(audioCallback, audioPlaybackMethodID, out_sample_rate,
                          out_channel_nb, 2);


    while (av_read_frame(avFormatContext, avPacket) >= 0) {
        //解码类型是音频
        if (avPacket->stream_index == audio_stream_index) {
            int ret = avcodec_send_packet(avCodecContext, avPacket);
            if (ret < 0) {
                __android_log_print(ANDROID_LOG_ERROR, TAG, "send packet error %d", ret);
            }
            if (ret > 0) {
                ret = avcodec_receive_frame(avCodecContext, avFrame);
                if (ret < 0) {
                    __android_log_print(ANDROID_LOG_ERROR, TAG, "decode frame error");
                }
                swr_convert(swrContext, &out_buffer, MAX_AUDIO_FRAME_SIZE,
                            (const uint8_t **) avFrame->data, avFrame->nb_samples);
                int out_buffer_size = av_samples_get_buffer_size(NULL,
                                                                 out_channel_nb,
                                                                 avFrame->nb_samples,
                                                                 out_sample_fmt, 1);
                jbyteArray audio_sample_array = (env)->NewByteArray(out_buffer_size);
                jbyte *sample_byte_array = (env)->GetByteArrayElements(audio_sample_array, NULL);
                //拷贝缓冲数据
                memcpy(sample_byte_array, out_buffer, (size_t) out_buffer_size);
                //释放数组
                (env)->ReleaseByteArrayElements(audio_sample_array, sample_byte_array, 0);
                (*env).CallVoidMethod(audioCallback, audioWriteMethodID, audio_sample_array,
                                      out_buffer_size);
                env->DeleteLocalRef(audio_sample_array);
                usleep(1000 * 16);
            }
        }
        av_packet_unref(avPacket);
    }
    av_frame_free(&avFrame);
    av_free(out_buffer);
    swr_free(&swrContext);
    avcodec_close(avCodecContext);
    avformat_close_input(&avFormatContext);
    env->ReleaseStringUTFChars(path_, path);
    if (needDeteach) {
        g_Vm->DetachCurrentThread();
    }
    env->DeleteGlobalRef(audioCallback);
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "free success");
    return 0;
}