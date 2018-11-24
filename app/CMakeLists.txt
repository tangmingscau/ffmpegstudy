# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )
message(STATUS "CMAKE_SOURCE_DIR" ${CMAKE_SOURCE_DIR})
set(distribution_dir ${CMAKE_SOURCE_DIR}/../distribution)
set(arm_dir ${CMAKE_SOURCE_DIR}/../distribution)
add_library( avcodec
             SHARED
             IMPORTED )
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${distribution_dir}/armeabi-v7a/libavcodec.so)
add_library( avfilter
             SHARED
             IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${distribution_dir}/armeabi-v7a/libavfilter.so)
add_library( avformat
             SHARED
             IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${distribution_dir}/armeabi-v7a/libavformat.so )
add_library( avutil
             SHARED
             IMPORTED )
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${distribution_dir}/armeabi-v7a/libavutil.so )

add_library( swresample
             SHARED
             IMPORTED )
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${distribution_dir}/armeabi-v7a/libswresample.so )
add_library( swscale
             SHARED
             IMPORTED)
set_target_properties( swscale PROPERTIES IMPORTED_LOCATION ${distribution_dir}/armeabi-v7a/libswscale.so )


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

add_library( xp-ffmpeg
             SHARED
             src/main/cpp/xp-ffmpeg.cpp
             src/main/cpp/video.cpp
              )

#include_directories(${distribution_dir}/include)

target_include_directories(xp-ffmpeg PRIVATE ${distribution_dir}/include)

target_link_libraries( xp-ffmpeg
                       -landroid #native_window
                       -ljnigraphics #bitmap
                       -lOpenSLES #openSLES
                        avcodec
                        avfilter
                        avformat
                        avutil
                        swresample
                        swscale
                        ${log-lib} )

