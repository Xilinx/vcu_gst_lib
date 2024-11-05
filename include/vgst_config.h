/*********************************************************************
 * Copyright (C) 2017-2022 Xilinx, Inc.
 * Copyright (C) 2022-2024 Advanced Micro Devices, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 ********************************************************************/

#ifndef INCLUDE_VGST_CONFIG_H_
#define INCLUDE_VGST_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    VGST_V4L2_IO_MODE_AUTO,
    VGST_V4L2_IO_MODE_RW,
    VGST_V4L2_IO_MODE_MMAP,
    VGST_V4L2_IO_MODE_USERPTR,
    VGST_V4L2_IO_MODE_DMABUF_EXPORT,
    VGST_V4L2_IO_MODE_DMABUF_IMPORT,
} VGST_V4L2_IO_MODE;

typedef enum {
    VGST_ENC_IP_MODE_DEFAULT,
    VGST_ENC_IP_MODE_ZERO_COPY,
    VGST_ENC_IP_MODE_DMA_IMPORT,
    VGST_ENC_IP_MODE_DMA_EXPORT,
} VGST_ENC_IP_MODE;

typedef enum {
    VGST_ENC_CONTROL_RATE_DISABLE,
    VGST_ENC_CONTROL_RATE_VARIABLE,
    VGST_ENC_CONTROL_RATE_CONSTANT,
    VGST_ENC_CONTROL_RATE_LOW_LETENCY,
    VGST_ENC_CONTROL_RATE_DEFAULT =-1,
} VGST_ENC_CONTROL_RATE;


typedef enum {
    VGST_ENC_ENTROPY_MODE_CAVLC,
    VGST_ENC_ENTROPY_MODE_CABAC,
    VGST_ENC_ENTROPY_MODE_DEFAULT =-1,
} VGST_ENC_ENTROPY_MODE;

typedef enum {
    VGST_DEC_IP_MODE_DEFAULT,
    VGST_DEC_IP_MODE_ZERO_COPY,
} VGST_DEC_IP_MODE;

typedef enum {
    VGST_DEC_OP_MODE_DEFAULT,
    VGST_DEC_OP_MODE_DMA_EXPORT,
} VGST_DEC_OP_MODE;

#define MAX_WIDTH                    3840
#define DCI_MAX_WIDTH                4096
#define MAX_HEIGHT                   2160
#define RES_1080P_WIDTH              1920
#define RES_1080P_HEIGHT             1080
#define MAX_SUPPORTED_FRAME_RATE     60
#define MAX_FRAME_RATE_DENOM         1
#define MIN_RELATIVE_QP              -32
#define MAX_RELATIVE_QP              31
#define MIN_B_FRAME                  0
#define MAX_B_FRAME                  4
#define MIN_SLICE_VALUE              4
#define MAX_H265_4KP_SLICE_CNT       22
#define MAX_H264_4KP_SLICE_CNT       32
#define MAX_H265_1080P_SLICE_CNT     32
#define MAX_H264_1080P_SLICE_CNT     32
#define MIN_GOP_LEN                  1
#define MAX_GOP_LEN                  1000
#define MAX_SRC_NUM                  8
#define MAX_SPLIT_SCREEN             2
#define H264_ENC_NAME                "omxh264enc"
#define H265_ENC_NAME                "omxh265enc"
#define H264_DEC_NAME                "omxh264dec"
#define H265_DEC_NAME                "omxh265dec"
#define H264_PARSER_NAME             "h264parse"
#define H265_PARSER_NAME             "h265parse"
#define H264_RTP_PAYLOAD_NAME        "rtph264pay"
#define H265_RTP_PAYLOAD_NAME        "rtph265pay"
#define MPEG_TS_RTP_PAYLOAD_NAME     "rtpmp2tpay"
#define V4L2_SRC_NAME                "v4l2src"
#define FILE_SRC_NAME                "uridecodebin"
#define MPEG_TS_MUX_NAME             "mpegtsmux"
#define MKV_MUX_NAME                 "matroskamux"
#define DEFAULT_DEC_BUFFER_CNT       5
#define MIN_DEC_BUFFER_CNT           3
#define GST_MINUTE                   60
#define MAX_PORT_NUMBER              65534
#define MIN_PORT_NUMBER              1024
#define QOS_DSCP_VALUE               60
#define FPS_UPDATE_INTERVAL          1000  // 1sec
#define PKT_NUMBER_PER_BUFFER        7
#define MAX_AUDIO_CHANNEL            2
#define MIN_AUDIO_VOLUME             0.0
#define MAX_AUDIO_VOLUME             10.0
#define H265_SUPPORTED_STREAM_LEVEL  "5.2"
#define H265_SUPPORTED_STREAM_TIER   "main"
#define CPB_SIZE                     1000
#define LLP2_CPB_SIZE                500
#define UDP_BUFFER_SIZE              60000000
#define GST_CAPS_FEATURE_MEMORY_XLNX_LL "memory:XLNXLL"
#define SDI_TX_DRIVER_NAME           "xlnx"
#define LLP2_MAX_LATENESS            5000000   // 5ms
#define GDR_REFRESH_INTERVAL         240
#define LLP2_LATENCY_TIME            5000      // 5ms
#define LLP2_STREAMING_LATENCY_TIME  7000      // 7ms
#define LLP2_BUFFER_TIME             10000     // 10ms
#define LLP2_STREAMING_BUFFER_TIME   14000     // 14ms
#define LLP2_AUDIO_RTP_PAYLOAD_TYPE  97
#define LLP2_2_4KP30_MAX_LATENESS    10000000  // 10ms
#define MAX_BITRATE_MARGIN           1.2       // 20%
#define MAX_PICTURE_SIZE_MARGIN      1.1       // 10%

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_VGST_CONFIG_H_ */
