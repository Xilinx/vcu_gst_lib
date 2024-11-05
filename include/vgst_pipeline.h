/*********************************************************************
 * Copyright (C) 2017-2022 Xilinx, Inc.
 * Copyright (C) 2022-2023 Advanced Micro Devices, Inc.
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

#ifndef INCLUDE_VGST_PIPELINE_H_
#define INCLUDE_VGST_PIPELINE_H_

#include "vgst_utils.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* This API is to create all the elements required for single/multi-stream pipeline */
VGST_ERROR_LOG create_pipeline (vgst_ip_params *ip_param, vgst_enc_params *enc_param, vgst_playback *play_ptr, guint sink_type, gchar *uri, vgst_aud_params *aud_param);

/* This API is to parse the tag and get the bitrate value from file */
void fetch_tag (const GstTagList * list, const gchar * tag, gpointer user_data);

/* This API is to measure the current fps value */
void on_fps_measurement (GstElement *fps, gdouble fps_num, gdouble drop_rate, gdouble avg_rate, gpointer data);

/* This API is to set all the required property to start playback/capture pipeline */
void set_property (vgst_application *app, gint index);

/* This API is to link all the elements required for playback/capture pipeline */
VGST_ERROR_LOG link_elements (vgst_ip_params *ip_param, vgst_playback *play_ptr, gint sink_type, vgst_aud_params *aud_param, gchar *uri, vgst_enc_params *enc_param);

/* This API is to link all the elements required for audio-video pipeline */
VGST_ERROR_LOG link_audio_pipeline (vgst_ip_params *ip_param, vgst_playback *play_ptr, gint sink_type, vgst_enc_params *enc_param);

/* This API is to get the proper coordinates for multi-stream */
void get_coordinates (guint *x, guint *y, guint cnt, guint num_src);
/* This API is to link element pads */
void on_pad_added_dec (GstElement *element, GstPad *pad, gpointer data);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_VGST_PIPELINE_H_ */
