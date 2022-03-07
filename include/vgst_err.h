/*********************************************************************
 * Copyright (C) 2017-2022 Xilinx, Inc.
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

#ifndef INCLUDE_VGST_ERR_H_
#define INCLUDE_VGST_ERR_H_

typedef enum {
    VGST_SUCCESS,
    VGST_ERROR_FILE_IO = -1,
    VGST_ERROR_GOP_NOT_SUPPORTED = -2,
    VGST_ERROR_GOP_LENGTH_RANGE_MISMATCH = -3,
    VGST_ERROR_B_FRAME_RANGE_MISMATCH = -4,
    VGST_ERROR_PIPELINE_CREATE_FAIL = -5,
    VGST_ERROR_PIPELINE_LINKING_FAIL = -6,
    VGST_ERROR_STATE_CHANGE_FAIL = -7,
    VGST_ERROR_ENCODER_TYPE_NOT_SUPPORTED = -8,
    VGST_ERROR_SRC_TYPE_NOT_SUPPORTED = -9,
    VGST_ERROR_FORMAT_NOT_SUPPORTED = -10,
    VGST_ERROR_SRC_COUNT_INVALID = -11,
    VGST_ERROR_RESOLUTION_NOT_SUPPORTED = -12,
    VGST_ERROR_DEVICE_TYPE_INVALID = -13,
    VGST_ERROR_PIPELINE_NOT_INITIALIZED = -14,
    VGST_ERROR_SET_ENC_BUF_ENV_FAILED = -15,
    VGST_ERROR_RUN_TIME_PIPELINE_FAILED = -16,
    VGST_ERROR_SLICE_RANGE_MISMATCH = -17,
    VGST_ERROR_BITRATE_NOT_SUPPORTED = -18,
    VGST_ERROR_QPMODE_NOT_SUPPORTED = -19,
    VGST_ERROR_PROFILE_NOT_SUPPORTED = -20,
    VGST_ERROR_RCMODE_NOT_SUPPORTED = -21,
    VGST_ERROR_PORT_NUM_RANGE_MISMATCH = -22,
    VGST_ERROR_DRIVER_TYPE_MISMATCH = -23,
    VGST_ERROR_OVERLAY_CREATION_FAIL = -24,
    VGST_ERROR_MULTI_STREAM_FAIL = -25,
    VGST_ERROR_SPLIT_SCREEN_FAIL = -26,
    VGST_ERROR_APP_PTR_NULL = -27,
    VGST_ERROR_GOP_MODE_NOT_SUPPORTED = -28,
    VGST_ERROR_B_FRAME_LOW_LATENCY_MODE_NOT_SUPPORTED = -29,
    VGST_ERROR_INPUT_OPTIONS_INVALID = -30,
    VGST_ERROR_LATENCY_MODE_NOT_SUPPORTED = -31,
    VGST_ERROR_4KP60_PARAM_NOT_SUPPORTED = -32,
    VGST_ERROR_2_4KP30_PARAM_NOT_SUPPORTED = -33,
    VGST_ERROR_4_1080P60_PARAM_NOT_SUPPORTED = -34,
    VGST_ERROR_TPG_IN_1080P30_NOT_SUPPORTED = -35,
    VGST_ERROR_FILE_IN_MULTISTREAM_NOT_SUPPORTED = -36,
    VGST_ERROR_CBR_VBR_SUB_FRAME_LATENCY_NOT_SUPPORTED = -37,
    VGST_ERROR_VBR_IN_STREAMING_NOT_SUPPORTED = -38,
    VGST_ERROR_AUDIO_CHANNEL_NOT_SUPPORTED = -39,
    VGST_ERROR_AUDIO_VOLUME_NOT_SUPPORTED = -40,
    VGST_ERROR_AUDIO_IN_TYPE_NOT_SUPPORTED = -41,
    VGST_ERROR_AUDIO_OUT_TYPE_NOT_SUPPORTED = -42,
    VGST_ERROR_8_1080P30_PARAM_NOT_SUPPORTED = -43,
    VGST_ERROR_SPLIT_SCREEN_NOT_SUPPORTED = -44,
    VGST_ERROR_PROFILE_FORMAT_NOT_SUPPORTED = -45,
    VGST_ERROR_LLP2_NOT_SUPPORTED = -46,
    /* Error range -47 to -90 is assigned for VLIB */
    VGST_ERROR_DRM_BUS_ID_NOT_FOUND = -91,
    VGST_ERROR_GDR_MODE_MISMATCH = -92,
    VGST_ERROR_ENTROPY_MODE_NOT_SUPPORTED = -93,
    VGST_ERROR_MAX_PICTURE_SIZE_NOT_SUPPORTED = -94,
    VGST_ERROR_RELATIVE_QP_RANGE_MISMATCH=-95,
    VGST_ERROR_HLG_NOT_SUPPORTED = -96,
    VGST_ERROR_AUDIO_DEVICE_ID_NOT_FOUND = -97,
    VGST_ERROR_OTHER = -99,
} VGST_ERROR_LOG;
#endif /* INCLUDE_VGST_ERR_H_ */