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

#ifndef INCLUDE_VGST_UTILS_H_
#define INCLUDE_VGST_UTILS_H_

#include "vgst_config.h"
#include "vgst_lib.h"
#include "video.h"
#include "vlib_audio.h"
#include <gst/video/videooverlay.h>


#ifdef __cplusplus
extern "C"
{
#endif

#define MP4_MUX_TYPE        "mp4"
#define MKV_MUX_TYPE        "mkv"
#define TS_MUX_TYPE         "ts"

typedef struct
_vgst_playback {
    GstElement       *pipeline, *ip_src, *rtppay, *demux, *mux, *tee;
    GstElement       *queue, *enc_queue, *audqueue;
    GstElement       *audcapsfilter, *audcapsfilter2, *srccapsfilter, *enccapsfilter, *deccapsfilter;
    GstElement       *videoparser, *videoenc, *videodec,  *videosink, *videosink2;
    GstElement       *fpsdisplaysink, *fpsdisplaysink2;
    GstElement       *audconvert, *audresample, *audconvert2, *audresample2;
    GstElement       *alsasrc, *alsasink, *volume, *audioenc, *audiodec, *audiortppay;
    GstElement       *rtpbin, *v_rtcpsink, *v_rtcpsrc, *a_rtpsink, *a_rtcpsink, *a_rtcpsrc;
    GstElement       *bypass, *xilinxscd, *roi;
    GstVideoOverlay  *overlay, *overlay2;
    GstPad           *pad, *pad2;
    GstPad           *vidpad, *audpad;
    GMainLoop        *loop;
    gboolean         eos_flag, err_flag, stop_flag;
    gchar            *err_msg;
    guint            fps_num[MAX_SPLIT_SCREEN], file_br, video_type;
    GstClockTime     eos_time;
} vgst_playback;

typedef struct
_vgst_application {
    vgst_playback      playback[MAX_SRC_NUM];
    vgst_enc_params    *enc_params;
    vgst_ip_params     *ip_params;
    vgst_op_params     *op_params;
    vgst_cmn_params    *cmn_params;
    vgst_aud_params    *aud_params;
} vgst_application;

/* This API is interface for creating single/mult-stream pipeline */
VGST_ERROR_LOG vgst_create_pipeline ();

/* This API is to print all the parameters coming from application */
void vgst_print_params (guint index);

/* This API is to capture messages from pipeline */
gboolean bus_callback (GstBus *bus, GstMessage *msg, gpointer data);

/* This API is to initialize pipeline structure */
void init_struct_params (vgst_enc_params *enc_param, vgst_ip_params *ip_param, vgst_op_params *op_param, vgst_cmn_params *cmn_param, vgst_aud_params *aud_param);

/* This API is called when element is added to bin/sub_bin */
void on_deep_element_added (GstBin *bin, GstBin *sub_bin,
                            GstElement *element, gpointer user_data);

/* This API is required for linking src pad of decoder to sink element */
void on_pad_added (GstElement *element, GstPad *pad, gpointer data);

/* This API is to stop the single/multi-stream pipeline */
gint stop_pipeline (void);

/* This API is to run the single/multi-stream pipeline */
VGST_ERROR_LOG vgst_run_pipeline ();

/* This API is to convert error number to string */
const gchar * error_to_string (VGST_ERROR_LOG error_code, gint index);

/* This API is to get bitrate for file/stream-in playback */
guint get_bitrate (int index);

/* This API is to get video type for file/stream-in playback */
guint get_video_type (int index);

/* This API is to poll events */
gint poll_event (int *arg, int index);

/* This API is to get fps of the pipeline */
void get_fps (guint index, guint *fps);

/* This API is to get current position of the pipeline */
void get_position (guint index, gint64 *position);

/* This API is to get duration of the file */
void get_duration (guint index, gint64 *duration);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_VGST_UTILS_H_ */
