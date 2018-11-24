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
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <unistd.h>
#include <stdio.h>
static void pgm_save_xp(unsigned char *buf, int wrap, int xsize, int ysize,
                        char *filename) {
    FILE *f;
    int i;
    f = fopen(filename, "w");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}
static void decodexp(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt,
                     const char *filename) {
    char buf[1024];
    int ret;
    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error sending a packet for decoding\n");
        exit(1);
    }
    while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }
        printf("saving frame %3d\n", dec_ctx->frame_number);
        fflush(stdout);
        /* the picture is allocated by the decoder. no need to
           free it */
        snprintf(buf, sizeof(buf), "%s-%d", filename, dec_ctx->frame_number);
        pgm_save_xp(frame->data[0], frame->linesize[0],
                    frame->width, frame->height, buf);
    }
}

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
    FILE *rgbfile = fopen("/sdcard/Music/normal.rgb", "wb+");
    SwsContext *img_convert_Ctx;
    bool needDeteach = false;
    //尝试去实现jni回调


    JavaVM *g_Vm;
    (*env).GetJavaVM(&g_Vm);
    jobject bitmapcallbak = (*env).NewGlobalRef(callback);
    if ((*g_Vm).GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED) {
        g_Vm->AttachCurrentThread(&env, NULL);
        needDeteach = true;
    }
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
            av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1));
    pFrameRgba = av_frame_alloc();
    av_image_fill_arrays(pFrameRgba->data, pFrameRgba->linesize, out_buffer,
                         AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);
    img_convert_Ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                                     pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24,
                                     SWS_BICUBIC, NULL, NULL, NULL);
    int bmpname = 0;
    while (av_read_frame(pFormatCtx, packet) >= 0) {
        int ret = avcodec_send_packet(pCodecCtx, packet);
//        __android_log_print(ANDROID_LOG_DEBUG, TAG, "send packet %d", ret);
        if (ret >= 0) {
            while (avcodec_receive_frame(pCodecCtx, pFrame) >= 0) {

            }
            sws_scale(img_convert_Ctx, (const uint8_t *const *) pFrame->data, pFrame->linesize, 0,
                      pCodecCtx->height, pFrameRgba->data, pFrameRgba->linesize);
            jbyteArray data = env->NewByteArray(pFrameRgba->linesize[0]);
            env->SetByteArrayRegion(data, 0, pFrameRgba->linesize[0],
                                    reinterpret_cast<const jbyte *>(pFrameRgba->data[0]));
            (*env).CallVoidMethod(callback, jBitmapMethodId, data);
            char filename[40];
            sprintf(filename, "/sdcard/Music/pngdir/%d.bmp", bmpname);

            FILE *bmpFile = fopen(filename, "wb+");
            for (i = 0; i < pFrameRgba->height; i++)
                fwrite(pFrameRgba->data[0] + i * pFrameRgba->linesize[0], 1, pFrameRgba->width,
                       bmpFile);
//            fwrite(pFrameRgba->data[0], pCodecCtx->width * pCodecCtx->height * 3, 1, bmpFile);
            bmpname++;
        }
//        fwrite(pFrameRgba->data[0],pCodecCtx->width*pCodecCtx->height*3,1,rgbfile);
//        __android_log_print(ANDROID_LOG_DEBUG, TAG, "write file rgb %s", pFrameRgba->data[0]);
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "close h264 success");
    fclose(rgbfile);
    av_packet_unref(packet);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "free success");
    if (needDeteach) {
        g_Vm->DetachCurrentThread();
    }
    env->DeleteGlobalRef(bitmapcallbak);
}
JNIEXPORT void JNICALL
Java_com_xiaopeng_ffmpegstudy_model_VideoModel_playVideo3(JNIEnv *env, jobject instance,
                                                          jstring videoPath_, jobject callback,
                                                          jobject surface) {
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
    FILE *rgbfile = fopen("/sdcard/Music/normal.rgb", "wb+");
    SwsContext *img_convert_Ctx;
    bool needDeteach = false;
    //尝试去实现jni回调


    JavaVM *g_Vm;
    (*env).GetJavaVM(&g_Vm);
    jobject bitmapcallbak = (*env).NewGlobalRef(callback);
    if ((*g_Vm).GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED) {
        g_Vm->AttachCurrentThread(&env, NULL);
        needDeteach = true;
    }
    jclass jBitmapClass = (*env).GetObjectClass(bitmapcallbak);
    jmethodID jBitmapMethodId = (*env).GetMethodID(jBitmapClass, "callback", "([B)V");


    av_register_all(); //要使用ffmpeg就必须使用这句话
    avformat_network_init();//初始化网络
    //分配上下文
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
    av_codec_set_pkt_timebase(pCodecCtx, pFormatCtx->streams[videoindex]->time_base);
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "could not open codec");
    }

    //获取native window
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    //获取视频的高度
    int videoWidth = pCodecCtx->width;
    int videoHeight = pCodecCtx->height;
    int windowRef = ANativeWindow_setBuffersGeometry(nativeWindow, videoWidth, videoHeight,
                                                     WINDOW_FORMAT_RGBA_8888);
    if (windowRef < 0) {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "set buffer error");
    }
    ANativeWindow_Buffer window_buffer;

    //申请内存
    pFrame = av_frame_alloc();
    pFrameRgba = av_frame_alloc();
    packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, pCodecCtx->width, pCodecCtx->height,
                                            1);
    out_buffer = (uint8_t *) (av_malloc(numBytes * sizeof(uint8_t)));
    av_image_fill_arrays(pFrameRgba->data, pFrameRgba->linesize, out_buffer,
                         AV_PIX_FMT_RGBA, pCodecCtx->width, pCodecCtx->height, 1);
    img_convert_Ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                                     pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGBA,
                                     SWS_BILINEAR, NULL, NULL, NULL);
    int bmpname = 0;
    int frameFinished = -1;
    while (av_read_frame(pFormatCtx, packet) >= 0) {
        //判断是否是视频流
        if (packet->stream_index == videoindex) {
            int ret = avcodec_send_packet(pCodecCtx, packet);
            avcodec_receive_frame(pCodecCtx, pFrame);
//        __android_log_print(ANDROID_LOG_DEBUG, TAG, "send packet %d", ret);
//            if (ret >= 0) {
//                while (avcodec_receive_frame(pCodecCtx, pFrame) >= 0) {
//
//                }
            windowRef = ANativeWindow_lock(nativeWindow, &window_buffer, NULL);
            if (windowRef < 0) {
                __android_log_print(ANDROID_LOG_DEBUG, TAG, "lock window error");
            }
            sws_scale(img_convert_Ctx, (const uint8_t *const *) pFrame->data,
                      pFrame->linesize,
                      0, pCodecCtx->height, pFrameRgba->data, pFrameRgba->linesize);
//                    jbyteArray data = env->NewByteArray(pFrameRgba->linesize[0]);
//                    env->SetByteArrayRegion(data, 0, pFrameRgba->linesize[0],
//                                            reinterpret_cast<const jbyte *>(pFrameRgba->data[0]));
//                    (*env).CallVoidMethod(callback, jBitmapMethodId, data);
            // 获取stride
            uint8_t *dst = (uint8_t *) (window_buffer.bits);
            int dstStride = window_buffer.stride * 4;
            uint8_t *src = pFrameRgba->data[0];
            int srcStride = pFrameRgba->linesize[0];
            // 由于window的stride和帧的stride不同,因此需要逐行复制
            int h;
            for (h = 0; h < videoHeight; h++) {
                memcpy(dst + h * dstStride, src + h * srcStride, (size_t) srcStride);
            }

            bmpname++;
            ANativeWindow_unlockAndPost(nativeWindow);
//            }
            usleep((unsigned long) 1000 * 40 * 1);
        }
        av_packet_unref(packet);
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "close h264 success");
    fclose(rgbfile);
    av_packet_unref(packet);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "free success");
    if (needDeteach) {
        g_Vm->DetachCurrentThread();
    }
    env->DeleteGlobalRef(bitmapcallbak);
}
JNIEXPORT void JNICALL
Java_com_xiaopeng_ffmpegstudy_model_VideoModel_playVideo2(JNIEnv *env, jobject instance,
                                                          jstring videoPath_, jobject callback) {
    AVCodec *codec;
    AVCodecParserContext *parser;
    AVCodecContext *avCodecContext;
    AVFrame *avFrame;
    uint8_t inbuf[AV_INPUT_BUFFER_MIN_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    uint8_t *data;
    size_t data_size;
    AVPacket *avPacket;
    bool needDeteach = false;
    FILE *inputFile;
    JavaVM *g_Vm;
    (*env).GetJavaVM(&g_Vm);
    jobject bitmapcallbak = (*env).NewGlobalRef(callback);
    if ((*g_Vm).GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED) {
        g_Vm->AttachCurrentThread(&env, NULL);
        needDeteach = true;
    }
    jclass jBitmapClass = (*env).GetObjectClass(bitmapcallbak);
    jmethodID jBitmapMethodId = (*env).GetMethodID(jBitmapClass, "callback", "([B)V");


    av_register_all(); //要使用ffmpeg就必须使用这句话
    avformat_network_init();//初始化网络
    avPacket = av_packet_alloc();
    memset(inbuf + AV_INPUT_BUFFER_MIN_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);
    codec = avcodec_find_decoder(AV_CODEC_ID_MPEG2VIDEO);
    if (!codec) {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "can not find codec");
    }
    parser = av_parser_init(codec->id);
    if (!parser) {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "can not find parser");
    }
    avCodecContext = avcodec_alloc_context3(codec);
    if (!avCodecContext) {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "can not find avCodecContext");
    }
    int ret = avcodec_open2(avCodecContext, codec, NULL);
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "can not open codec ");
    }
    inputFile = fopen(env->GetStringUTFChars(videoPath_, NULL), "rb");
    avFrame = av_frame_alloc();
    int bmpName = 0;
    char filename[40];
    while (!feof(inputFile)) {
        data_size = fread(inbuf, 1, AV_INPUT_BUFFER_MIN_SIZE, inputFile);
        if (!data_size) {
            break;
        }
        data = inbuf;
        while (data_size > 0) {
            ret = av_parser_parse2(parser, avCodecContext, &(avPacket->data), &(avPacket->size),
                                   data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
            if (ret < 0) {
                __android_log_print(ANDROID_LOG_ERROR, TAG, "error parsing");
            }
            data += ret;
            data_size -= ret;
            sprintf(filename, "/sdcard/Music/pngdir/%d.bmp", bmpName);
            if (avPacket->size) {
                decodexp(avCodecContext, avFrame, avPacket, filename);
            }
            bmpName++;
        }
    }
    decodexp(avCodecContext, avFrame, NULL, filename);
    fclose(inputFile);
    av_parser_close(parser);
    avcodec_free_context(&avCodecContext);
    av_frame_free(&avFrame);
    av_packet_free(&avPacket);
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "free success");
    if (needDeteach) {
        g_Vm->DetachCurrentThread();
    }
    env->DeleteGlobalRef(bitmapcallbak);
}
}

