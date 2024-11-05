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

#ifndef INCLUDE_VGST_LIB_H_
#define INCLUDE_VGST_LIB_H_

#include <sys/stat.h>
#include <string.h>
#include "vgst_err.h"
#include <gst/gst.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
_vgst_enc_params {
    gboolean   enable_l2Cache;
    gboolean   hlg_sdr_compatible;
    gboolean   low_bandwidth;
    gboolean   filler_data;
    gboolean   max_picture_size;
    guint      bitrate;
    guint      gop_len;
    guint      b_frame;
    guint      slice;
    guint      qp_mode;
    guint      rc_mode;
    guint      profile;
    guint      enc_type;
    guint      gop_mode;
    guint      latency_mode;
    guint      gdr_mode;
    gint       entropy_mode;
} vgst_enc_params;

typedef struct
_vgst_aud_params {
    gchar      *format;
    gboolean   enable_audio;
    guint      sampling_rate;
    guint      channel;
    gdouble    volume;
    guint      audio_in;
    guint      audio_out;
    gchar      *source_id;
    gchar      *sink_id;
} vgst_aud_params;

typedef struct
_vgst_input_param {
    gchar      *uri;
    gboolean   raw;
    gint       relative_qp;
    guint      width;
    guint      height;
    guint      src_type;
    guint      device_type;
    guint      format;
    gboolean   accelerator;
    gboolean   enable_roi;
    gboolean   enable_scd;
    gboolean   enable_llp2;
    guint      scd_type;
} vgst_ip_params;

typedef struct
_vgst_output_param {
    gchar      *file_out;
    gchar      *host_ip;
    guint      duration;
    guint      port_num;
} vgst_op_params;

typedef struct
_vgst_cmn_param {
    guint      num_src;
    guint      sink_type;
    guint      driver_type;
    guint      plane_id;
    gchar      *bus_id;
    guint      frame_rate;
} vgst_cmn_params;

typedef enum {
    STREAM,
    RECORD,
    DISPLAY,
    SPLIT_SCREEN,
} VGST_SINK_TYPE;

typedef enum {
    UNIFORM,
    ROI,
    AUTO,
} VGST_QP_MODE;

typedef enum {
    CONST_QP,
    VBR,
    CBR,
    LOW_LATENCY = 0x7F000001,
} VGST_RC_MODE;

typedef enum {
    BASIC,
    BASIC_B,
    PYRAMIDAL,
    PYRAMIDAL_B,
    ADAPTIVE,
    LOW_DELAY_P,
    LOW_DELAY_B,
} VGST_GOP_MODE;

typedef enum {
    BASELINE_PROFILE,
    MAIN_PROFILE,
    HIGH_PROFILE,
} VGST_PROFILE_MODE;

typedef enum {
    LIVE_SRC,
    FILE_SRC,
    STREAMING_SRC,
} VGST_SRC_TYPE;

typedef enum {
    AVC =1,
    HEVC,
} VGST_CODEC_TYPE;

typedef enum {
    NORMAL_LATENCY,
    SUB_FRAME_LATENCY,
} VGST_LOW_LATENCY_MODE;

typedef enum {
    EVENT_NONE,
    EVENT_EOS,
    EVENT_ERROR,
} VGST_EVENT_TYPE;

typedef enum {
    GDR_MODE_DISABLED,
    GDR_MODE_VERTICAL,
    GDR_MODE_HORIZONTAL,
} VGST_GDR_MODE;

typedef enum {
    ENTROPY_MODE_CAVLC,
    ENTROPY_MODE_CABAC,
    ENTROPY_MODE_DEFAULT =-1,
} VGST_ENTROPY_MODE;

/* This API is to initialize the library */
gint vgst_init(void);

/* This API is to initialize the options to initiate the pipeline */
gint vgst_config_options (vgst_enc_params *enc_param, vgst_ip_params *ip_param, vgst_op_params *op_param, vgst_cmn_params *cmn_param, vgst_aud_params *aud_param);

/* This API is to start the pipeline */
gint vgst_start_pipeline (void);

/* This API is interface to stop the single/multi-stream pipeline */
gint vgst_stop_pipeline (void);

/* This API is to convert error number to string */
const gchar * vgst_error_to_string (VGST_ERROR_LOG error_code, gint index);

/* This API is to get fps of the pipeline */
void vgst_get_fps (guint index, guint *fps);

/* This API is to get bitrate for file/stream-in playback */
guint vgst_get_bitrate (int index);

/* This API is to get video type for file/stream-in playback */
guint vgst_get_video_type (int index);

/* This API is to poll events */
gint vgst_poll_event (int *arg, int index);

/* This API is to un-initialize the library */
gint vgst_uninit(void);

/* This API is to get current position of the pipeline */
void vgst_get_position (guint index, gint64 *pos);

/* This API is to get duration of the file */
void vgst_get_duration (guint index, gint64 *duration);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_VGST_LIB_H_ */
