//
// Created by tony on 2018/11/6.
//

#include "video.h"
#include <jni.h>
#include <android/log.h>
#include <iostream>

#define TAG "tangmingjni"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
JNIEXPORT jstring JNICALL
Java_com_xiaopeng_ffmpegstudy_model_VideoModel_mp4ToH264(JNIEnv *env, jobject instance,
                                                         jstring path) {
    const char *fromPath = env->GetStringUTFChars(path, NULL);
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "path = %s", fromPath);
    const char *filePath = env->GetStringUTFChars(path, NULL);
    FILE *h264 = fopen("/sdcard/Music/normal.h264", "wb+");
    FILE *yuv = fopen("/sdcard/Music/normal.yuv", "wb+");
    AVFormatContext *pFormatCtx; //数据结构组装
    AVFrame *pFrame;   //音频原始数据
    int i, videoindex;
    AVCodecContext *pCodecCtx; //编解码
    AVCodec *pCodec; //
    AVPacket *packet; //压缩编码数据结构体
    uint8_t *out_buffer;

    av_register_all(); //要使用ffmpeg就必须使用这句话
    avformat_network_init();//初始化网络

    pFormatCtx = avformat_alloc_context();
    int openResult = avformat_open_input(&pFormatCtx, filePath, NULL, NULL);
    if (openResult != 0) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can not open video %d", openResult);
        return env->NewStringUTF("can not open video");
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG, " open video successful");
    int findInfo = avformat_find_stream_info(pFormatCtx, NULL);
    if (findInfo < 0) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can not find video info");
        return env->NewStringUTF("can not find video info");
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "find video info successful");
    videoindex = -1;
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
            break;
        }
    }
    if (videoindex == -1) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can not find a video stream");
    }
    //发现解码器
    pCodec = avcodec_find_decoder(pFormatCtx->streams[videoindex]->codecpar->codec_id);
    //申请解码器上下文
    pCodecCtx = avcodec_alloc_context3(pCodec);
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoindex]->codecpar);
    avcodec_open2(pCodecCtx, pCodec, NULL);

    //申请音频原始数据
    pFrame = av_frame_alloc();
    packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    out_buffer = (uint8_t *) av_malloc(
            av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
    av_dump_format(pFormatCtx, 0, filePath, 0); //打印相关的信息

    while (av_read_frame(pFormatCtx, packet) >= 0) {
        avcodec_send_packet(pCodecCtx, packet);
        avcodec_receive_frame(pCodecCtx, pFrame);
        fwrite(pFrame->data, 1, reinterpret_cast<size_t>(pFrame->linesize), yuv);
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "write file yuv");

    }
    fclose(h264);
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "close h264 success");
    av_packet_unref(packet);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "free success");
    return (*env).NewStringUTF("hello mp4ToH264");
}
JNIEXPORT void JNICALL
Java_com_xiaopeng_ffmpegstudy_model_VideoModel_playVideo(JNIEnv *env, jobject instance,
                                                         jstring videoPath_, jobject callback) {
    const char *fromPath = env->GetStringUTFChars(videoPath_, NULL);
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "path = %s", fromPath);
    const char *filePath = env->GetStringUTFChars(videoPath_, NULL);
    AVFormatContext *pFormatCtx; //数据结构组装
    AVFrame *pFrame, *pFrameRgba;   //音频原始数据
    int i, videoindex;
    AVCodecContext *pCodecCtx; //编解码
    AVCodec *pCodec; //
    AVPacket *packet; //压缩编码数据结构体
    uint8_t *out_buffer;
    SwsContext *img_convert_Ctx;
    //尝试去实现jni回调


    JavaVM *g_Vm;
    (*env).GetJavaVM(&g_Vm);
    jobject bitmapcallbak = (*env).NewGlobalRef(callback);
    jclass jBitmapClass = (*env).GetObjectClass(bitmapcallbak);
    jmethodID jBitmapMethodId = (*env).GetMethodID(jBitmapClass, "callback", "([B)V");


    av_register_all(); //要使用ffmpeg就必须使用这句话
    avformat_network_init();//初始化网络

    pFormatCtx = avformat_alloc_context();
    int openResult = avformat_open_input(&pFormatCtx, filePath, NULL, NULL);
    if (openResult != 0) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can not open video %d", openResult);
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG, " open video successful");
    int findInfo = avformat_find_stream_info(pFormatCtx, NULL);
    if (findInfo < 0) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can not find video info");
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "find video info successful");

    videoindex = -1;
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
            break;
        }
    }
    if (videoindex == -1) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can not find a video stream");
    }

    //发现解码器
    pCodec = avcodec_find_decoder(pFormatCtx->streams[videoindex]->codecpar->codec_id);
    //申请解码器上下文
    pCodecCtx = avcodec_alloc_context3(pCodec);
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoindex]->codecpar);
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "could not open codec");
    }

    //申请音频原始数据
    pFrame = av_frame_alloc();
    packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, pCodecCtx->width, pCodecCtx->height,
                                            1);
    out_buffer = (uint8_t *) av_malloc(
            av_image_get_buffer_size(AV_PIX_FMT_RGBA, pCodecCtx->width, pCodecCtx->height, 1));
    pFrameRgba = av_frame_alloc();
    av_image_fill_arrays(pFrameRgba->data, pFrameRgba->linesize, out_buffer,
                         AV_PIX_FMT_RGBA, pCodecCtx->width, pCodecCtx->height, 1);
   img_convert_Ctx=sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,
   pCodecCtx->width,pCodecCtx->height,AV_PIX_FMT_RGBA,SWS_BICUBIC,NULL,NULL,NULL);

    while (av_read_frame(pFormatCtx, packet) >= 0) {
        avcodec_send_packet(pCodecCtx, packet);
        avcodec_receive_frame(pCodecCtx, pFrame);
        sws_scale(img_convert_Ctx,(const uint8_t* const*)pFrame->data,pFrame->linesize,0,
                  pCodecCtx->height,pFrameRgba->data,pFrameRgba->linesize);
        jbyteArray data= env->NewByteArray(pFrameRgba->linesize[0]);
        env->SetByteArrayRegion(data, 0, pFrameRgba->linesize[0],
                                reinterpret_cast<const jbyte *>(pFrameRgba->data[0]));
        (*env).CallVoidMethod(callback, jBitmapMethodId, data);
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "write file yuv %s",pFrameRgba->data[0]);
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "close h264 success");
    av_packet_unref(packet);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "free success");
    g_Vm->DetachCurrentThread();
    env->DeleteGlobalRef(bitmapcallbak);
    bitmapcallbak=NULL;
}
}
