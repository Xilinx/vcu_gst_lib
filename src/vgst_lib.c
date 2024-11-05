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

#include "vgst_lib.h"
#include "vgst_utils.h"

GST_DEBUG_CATEGORY (vgst_lib);
#define GST_CAT_DEFAULT vgst_lib

const gchar *
vgst_error_to_string (VGST_ERROR_LOG error_code, gint index) {
    return error_to_string (error_code, index);
}

gint
check_limitation (vgst_ip_params *ip_param, vgst_enc_params *enc_param, vgst_cmn_params *cmn_param) {
    if ((ip_param->width == MAX_WIDTH || ip_param->width == DCI_MAX_WIDTH) && ip_param->height == MAX_HEIGHT && cmn_param->num_src == 1 && cmn_param->frame_rate == MAX_SUPPORTED_FRAME_RATE) {
      /* 1-4kp60 pipeline limitations */
      if ((enc_param->b_frame > MAX_B_FRAME) || !enc_param->enable_l2Cache || (enc_param->gop_mode == LOW_DELAY_B)) {
        GST_ERROR ("1-4kp60 Pipeline limitations");
        return VGST_ERROR_4KP60_PARAM_NOT_SUPPORTED;
      }
    }
    if ((ip_param->width == MAX_WIDTH) && (ip_param->height == MAX_HEIGHT) && (cmn_param->num_src == 2) && (cmn_param->frame_rate == MAX_SUPPORTED_FRAME_RATE/2)) {
      /* 2-4kp30 pipeline limitations */
      if ((enc_param->b_frame > MAX_B_FRAME) || !enc_param->enable_l2Cache || (enc_param->gop_mode == LOW_DELAY_B)) {
        GST_ERROR ("2-4kp30 Pipeline limitations");
        return VGST_ERROR_2_4KP30_PARAM_NOT_SUPPORTED;
      }
    }
    if ((ip_param->width == MAX_WIDTH/2) && (ip_param->height == MAX_HEIGHT/2) && (cmn_param->num_src == 4) && (cmn_param->frame_rate == MAX_SUPPORTED_FRAME_RATE)) {
      /* 4-1080p60 pipeline limitations */
      if ((enc_param->b_frame > MAX_B_FRAME) || !enc_param->enable_l2Cache || (enc_param->gop_mode == LOW_DELAY_B) || \
         ((enc_param->latency_mode == SUB_FRAME_LATENCY) && (cmn_param->sink_type != STREAM))) {
        GST_ERROR ("4-1080p60 Pipeline limitations");
        return VGST_ERROR_4_1080P60_PARAM_NOT_SUPPORTED;
      }
    }
    if ((ip_param->relative_qp > MAX_RELATIVE_QP) || (ip_param->relative_qp < MIN_RELATIVE_QP)) {
      GST_ERROR ("Relative qp range limitations. It only supports range <-32 to +31>");
      return VGST_ERROR_RELATIVE_QP_RANGE_MISMATCH;
    }

    if ((ip_param->width == MAX_WIDTH/2) && (ip_param->height == MAX_HEIGHT/2) && (cmn_param->num_src == 8) && (cmn_param->frame_rate == MAX_SUPPORTED_FRAME_RATE/2)) {
      /* 8-1080p30 pipeline limitations */
      if ((enc_param->b_frame > MAX_B_FRAME) || !enc_param->enable_l2Cache || (enc_param->gop_mode == LOW_DELAY_B) || (enc_param->latency_mode == SUB_FRAME_LATENCY)) {
        GST_ERROR ("8-1080p30 Pipeline limitations");
        return VGST_ERROR_8_1080P30_PARAM_NOT_SUPPORTED;
      }
    }
    return VGST_SUCCESS;
}

gint
vgst_config_options (vgst_enc_params *enc_param, vgst_ip_params *ip_param, vgst_op_params *op_param,
                     vgst_cmn_params *cmn_param, vgst_aud_params *aud_param) {
    struct stat file_stat;
    struct vlib_config_data config;
    guint i,num_src = cmn_param->num_src;
    gint ret;

    /* initialize GStreamer */
    gst_init (NULL, NULL);

    GST_DEBUG_CATEGORY_INIT (vgst_lib, "vgst-lib", 0, "vgst lib");
    if (num_src > MAX_SRC_NUM) {
      GST_ERROR ("Source count is invalid");
      return VGST_ERROR_SRC_COUNT_INVALID;
    }
    if ((DP == cmn_param->driver_type || SDI_Tx == cmn_param->driver_type) && num_src > 1) {
      GST_ERROR ("Multi stream is not supported on DP or SDI");
      return VGST_ERROR_MULTI_STREAM_FAIL;
    }
    if ((DP == cmn_param->driver_type || SDI_Tx == cmn_param->driver_type) && cmn_param->sink_type == SPLIT_SCREEN) {
      GST_ERROR ("Split screen is not supported on DP or SDI");
      return VGST_ERROR_SPLIT_SCREEN_FAIL;
    }
    if (cmn_param->sink_type == SPLIT_SCREEN && ip_param[0].width == MAX_WIDTH && num_src > 1) {
      GST_ERROR ("Split screen is not supported for more than 1-4kp input source");
      return VGST_ERROR_SPLIT_SCREEN_NOT_SUPPORTED;
    }
    if (cmn_param->sink_type == SPLIT_SCREEN && ip_param[0].width != MAX_WIDTH && num_src > 2) {
      GST_ERROR ("Split screen is not supported for more than 2-1080p input source");
      return VGST_ERROR_SPLIT_SCREEN_NOT_SUPPORTED;
    }
    if (DP != cmn_param->driver_type && HDMI_Tx != cmn_param->driver_type && SDI_Tx != cmn_param->driver_type) {
      GST_ERROR ("Driver type is invalid");
      return VGST_ERROR_DRIVER_TYPE_MISMATCH;
    }
    for (i =0; i< num_src; i++) {
      if (!vlib_is_llp2_design() && ip_param[i].enable_llp2) {
        GST_ERROR ("LLP2 use-case is supported only in LLP2 design");
        return VGST_ERROR_LLP2_NOT_SUPPORTED;
      }
      if (vlib_is_llp2_design() && ip_param[i].enable_llp2 && RECORD == cmn_param->sink_type) {
        GST_ERROR ("LLP2 use-case is not supported with record pipeline");
        return VGST_ERROR_LLP2_NOT_SUPPORTED;
      }
      if (vlib_is_llp2_design() && ip_param[i].enable_llp2 && LIVE_SRC != ip_param[i].src_type) {
        GST_ERROR ("LLP2 use-case is supported with live source only");
        return VGST_ERROR_LLP2_NOT_SUPPORTED;
      }
      if (vlib_is_llp2_design() && ip_param[i].enable_llp2 && TRUE == ip_param[i].raw) {
        GST_ERROR ("LLP2 use-case is not supported with raw pipeline");
        return VGST_ERROR_LLP2_NOT_SUPPORTED;
      }
      if (aud_param[i].enable_audio && (AUDIO_HDMI_IN != aud_param[i].audio_in && AUDIO_SDI_IN != aud_param[i].audio_in && AUDIO_I2S_IN != aud_param[i].audio_in)) {
        GST_ERROR ("Audio in type not supported");
        return VGST_ERROR_AUDIO_IN_TYPE_NOT_SUPPORTED;
      }
      if (aud_param[i].enable_audio && (AUDIO_HDMI_OUT != aud_param[i].audio_out && AUDIO_SDI_OUT != aud_param[i].audio_out && \
          AUDIO_I2S_OUT != aud_param[i].audio_out && AUDIO_DP_OUT != aud_param[i].audio_out)) {
        GST_ERROR ("Audio out type not supported");
        return VGST_ERROR_AUDIO_OUT_TYPE_NOT_SUPPORTED;
      }
      if (LIVE_SRC != ip_param[i].src_type && FILE_SRC != ip_param[i].src_type && STREAMING_SRC != ip_param[i].src_type) {
        GST_ERROR ("Source type is not supported");
        return VGST_ERROR_SRC_TYPE_NOT_SUPPORTED;
      }
      if (ip_param[i].format != NV12 && ip_param[i].format != NV16 &&
          ip_param[i].format != XV15 && ip_param[i].format != XV20 &&
          ip_param[i].format != YU24 && ip_param[i].format != X403) {
        GST_ERROR ("Input format is not supported");
        return VGST_ERROR_FORMAT_NOT_SUPPORTED;
      }
      if ((YU24 == ip_param[i].format) || (X403 == ip_param[i].format)) {
        if ((LIVE_SRC == ip_param[i].src_type) && \
            (DISPLAY  == cmn_param->sink_type) && \
            (FALSE    == ip_param[i].raw)) {
          GST_ERROR ("Serial use-case is not supported with YUV444 format");
          return VGST_ERROR_INPUT_OPTIONS_INVALID;
        }
        if ((MAX_WIDTH  == ip_param[i].width)  &&
            (MAX_HEIGHT == ip_param[i].height) &&
            (MAX_SUPPORTED_FRAME_RATE == cmn_param->frame_rate)) {
          GST_ERROR ("4kp60 use-case is not supported with YUV444 format");
          return VGST_ERROR_INPUT_OPTIONS_INVALID;
        }
      }
      if (FILE_SRC == ip_param[i].src_type && (!ip_param[i].uri || (stat(ip_param[i].uri +strlen("file:"), &file_stat) < 0))) {
        GST_ERROR ("Input file does not exist");
        return VGST_ERROR_FILE_IO;
      }
      if (FILE_SRC == ip_param[i].src_type && num_src > 1) {
        GST_ERROR ("File playback in multi stream not supported");
        return VGST_ERROR_FILE_IN_MULTISTREAM_NOT_SUPPORTED;
      }
      if (ip_param[i].width > DCI_MAX_WIDTH || ip_param[i].height > MAX_HEIGHT) {
        GST_ERROR ("Resolution WxH not supported");
        return VGST_ERROR_RESOLUTION_NOT_SUPPORTED;
      }
      if (LIVE_SRC == ip_param[i].src_type && ip_param[i].device_type == TPG_1 \
          && (ip_param[i].width != MAX_WIDTH || ip_param[i].height != (guint)MAX_HEIGHT) \
          && (cmn_param->frame_rate != MAX_SUPPORTED_FRAME_RATE)) {
        GST_ERROR ("TPG 1080P30 resolution not supported");
        return VGST_ERROR_TPG_IN_1080P30_NOT_SUPPORTED;
      }
      if (LIVE_SRC == ip_param[i].src_type && (ip_param[i].device_type != TPG_1 && ip_param[i].device_type != HDMI_1 \
          && ip_param[i].device_type != CSI && ip_param[i].device_type != SDI && ip_param[i].device_type != HDMI_2 \
          && ip_param[i].device_type != HDMI_3 && ip_param[i].device_type != HDMI_4 && ip_param[i].device_type != HDMI_5 \
          && ip_param[i].device_type != HDMI_6 && ip_param[i].device_type != HDMI_7 && ip_param[i].device_type != CSI_2 \
          && ip_param[i].device_type != CSI_3 && ip_param[i].device_type != CSI_4)) {
        GST_ERROR ("Device type is invalid");
        return VGST_ERROR_DEVICE_TYPE_INVALID;
      }
      if ((cmn_param->sink_type  == STREAM || cmn_param->sink_type  == RECORD) && ip_param[i].raw == TRUE) {
        GST_WARNING ("Oops!! raw flag set wrong");
        ip_param[i].raw = FALSE;
      }
      if ((FILE_SRC == ip_param[i].src_type || STREAMING_SRC == ip_param[i].src_type) && (cmn_param->sink_type  == RECORD || cmn_param->sink_type  == STREAM)) {
        GST_ERROR ("For file/streaming source, sink type should be only display");
        return VGST_ERROR_INPUT_OPTIONS_INVALID;
      }
      if (aud_param[i].enable_audio) {
        if (aud_param[i].channel != MAX_AUDIO_CHANNEL) {
          GST_ERROR ("Audio channel not supported");
          return VGST_ERROR_AUDIO_CHANNEL_NOT_SUPPORTED;
        }
        if (aud_param[i].volume < MIN_AUDIO_VOLUME || aud_param[i].volume > MAX_AUDIO_VOLUME) {
          GST_ERROR ("Audio volume not supported");
          return VGST_ERROR_AUDIO_VOLUME_NOT_SUPPORTED;
        }
      }

      if (LIVE_SRC == ip_param[i].src_type && (FALSE == ip_param[i].raw)) {
        ret = check_limitation (&ip_param[i], &enc_param[i], cmn_param);
        if (ret) {
          GST_ERROR ("pipeline failed to start due to parameter limitations");
          return ret;
        }
        if (vlib_is_llp2_design() && ip_param[i].enable_llp2 && \
           (SUB_FRAME_LATENCY != enc_param[i].latency_mode || LOW_LATENCY != enc_param[i].rc_mode)) {
          GST_ERROR ("LLP2 use-case is supported with Sub_Frame latency mode and Low Latency rc mode");
          return VGST_ERROR_LLP2_NOT_SUPPORTED;
        }
        if (NORMAL_LATENCY != enc_param[i].latency_mode && SUB_FRAME_LATENCY != enc_param[i].latency_mode) {
          GST_ERROR ("latency mode not supported");
          return VGST_ERROR_LATENCY_MODE_NOT_SUPPORTED;
        }
        if ((CBR == enc_param[i].rc_mode || VBR == enc_param[i].rc_mode) && SUB_FRAME_LATENCY == enc_param[i].latency_mode) {
          GST_ERROR ("Sub_Frame latency mode is not supported in CBR/VBR rc mode");
          return VGST_ERROR_CBR_VBR_SUB_FRAME_LATENCY_NOT_SUPPORTED;
        }
        if ((GDR_MODE_DISABLED != enc_param[i].gdr_mode) && ((LOW_DELAY_P != enc_param[i].gop_mode) || \
            (SUB_FRAME_LATENCY != enc_param[i].latency_mode))) {
          GST_ERROR ("GDR mode is currently supported with LLP1/LLP2 low-delay-p use-cases only");
          return VGST_ERROR_GDR_MODE_MISMATCH;
        }
        if ((TRUE == enc_param[i].max_picture_size) && ((CBR != enc_param[i].rc_mode) && (VBR != enc_param[i].rc_mode))) {
          GST_ERROR ("Max picture size is supported with CBR/VBR rate control mode only\n");
          return VGST_ERROR_MAX_PICTURE_SIZE_NOT_SUPPORTED;
        }
        if (VBR == enc_param[i].rc_mode && cmn_param->sink_type == STREAM) {
          GST_ERROR ("VBR is not supported in streaming use case");
          return VGST_ERROR_VBR_IN_STREAMING_NOT_SUPPORTED;
        }
        if ((LOW_LATENCY == enc_param[i].rc_mode || SUB_FRAME_LATENCY == enc_param[i].latency_mode) && enc_param[i].b_frame) {
          GST_ERROR ("b-frame in low_latency or sub_frame latency mode not supported");
          return VGST_ERROR_B_FRAME_LOW_LATENCY_MODE_NOT_SUPPORTED;
        }
        if (enc_param[i].gop_mode != BASIC && LOW_DELAY_P != enc_param[i].gop_mode && LOW_DELAY_B != enc_param[i].gop_mode) {
          GST_ERROR ("gop-mode not supported");
          return VGST_ERROR_GOP_MODE_NOT_SUPPORTED;
        }
        if (enc_param[i].b_frame < MIN_B_FRAME || enc_param[i].b_frame > MAX_B_FRAME) {
          GST_ERROR ("b-frames should be in the range of 0-4");
          return VGST_ERROR_B_FRAME_RANGE_MISMATCH;
        }
        if (0 != enc_param[i].gop_len % (enc_param[i].b_frame+1)) {
          GST_ERROR ("GoP length should be multiple of b-frames+1 multiple");
          return VGST_ERROR_GOP_NOT_SUPPORTED;
        }
        if (enc_param[i].gop_len < MIN_GOP_LEN || enc_param[i].gop_len > MAX_GOP_LEN) {
          GST_ERROR ("GoP length should be 1-1000");
          return VGST_ERROR_GOP_LENGTH_RANGE_MISMATCH;
        }
        if (AVC != enc_param[i].enc_type && HEVC != enc_param[i].enc_type) {
          GST_ERROR ("Encoder name is incorrect");
          return VGST_ERROR_ENCODER_TYPE_NOT_SUPPORTED;
        }
        if ((HEVC == enc_param[i].enc_type) && (ip_param[i].width == MAX_WIDTH || ip_param[i].width == DCI_MAX_WIDTH) \
	     && (ip_param[i].height == MAX_HEIGHT) && \
            (enc_param[i].slice > MAX_H265_4KP_SLICE_CNT || enc_param[i].slice < MIN_SLICE_VALUE)) {
          GST_ERROR ("Slice range for H265 in 4kp should be : %d-%d", MIN_SLICE_VALUE, MAX_H265_4KP_SLICE_CNT);
          return VGST_ERROR_SLICE_RANGE_MISMATCH;
        }
        if ((HEVC == enc_param[i].enc_type) && (ip_param[i].width == MAX_WIDTH/2) && (ip_param[i].height == MAX_HEIGHT/2) && \
            (enc_param[i].slice > MAX_H265_1080P_SLICE_CNT || enc_param[i].slice < MIN_SLICE_VALUE)) {
          GST_ERROR ("Slice range for H265 in 1080p should be : %d-%d", MIN_SLICE_VALUE, MAX_H265_1080P_SLICE_CNT);
          return VGST_ERROR_SLICE_RANGE_MISMATCH;
        }
        if ((AVC == enc_param[i].enc_type) && (ip_param->width == MAX_WIDTH || ip_param->width == DCI_MAX_WIDTH) \
	     && (ip_param->height == MAX_HEIGHT) && \
            (enc_param[i].slice > MAX_H264_4KP_SLICE_CNT || enc_param[i].slice < MIN_SLICE_VALUE)) {
          GST_ERROR ("Slice range for H264 in 4kp should be : %d-%d", MIN_SLICE_VALUE, MAX_H264_4KP_SLICE_CNT);
          return VGST_ERROR_SLICE_RANGE_MISMATCH;
        }
        if ((AVC == enc_param[i].enc_type) && (ip_param->width == MAX_WIDTH/2) && (ip_param->height == MAX_HEIGHT/2) && \
            (enc_param[i].slice > MAX_H264_1080P_SLICE_CNT || enc_param[i].slice < MIN_SLICE_VALUE)) {
          GST_ERROR ("Slice range for H264 in 1080p should be : %d-%d", MIN_SLICE_VALUE, MAX_H264_1080P_SLICE_CNT);
          return VGST_ERROR_SLICE_RANGE_MISMATCH;
        }
        if ((enc_param[i].profile != HIGH_PROFILE) && (AVC == enc_param[i].enc_type) && (ip_param[i].format != NV12)) {
          GST_ERROR ("Only high profile supported for H264 and NV16/XV15/XV20/YU24/X403 format");
          return VGST_ERROR_PROFILE_FORMAT_NOT_SUPPORTED;
        }
        if ((enc_param[i].profile != BASELINE_PROFILE && enc_param[i].profile != MAIN_PROFILE && enc_param[i].profile != HIGH_PROFILE) &&
             (AVC == enc_param[i].enc_type)) {
          GST_ERROR ("Profile can be either baseline, main or high for H264");
          return VGST_ERROR_PROFILE_NOT_SUPPORTED;
        }
        if ((enc_param[i].profile != MAIN_PROFILE) && (HEVC == enc_param[i].enc_type)) {
          GST_ERROR ("Only main profile supported for H265");
          return VGST_ERROR_PROFILE_NOT_SUPPORTED;
        }
        if (enc_param[i].qp_mode != UNIFORM && enc_param[i].qp_mode != AUTO && enc_param[i].qp_mode != ROI) {
          GST_ERROR ("Qp mode can be either Uniform or Auto or ROI");
          return VGST_ERROR_QPMODE_NOT_SUPPORTED;
        }
        if ((enc_param[i].rc_mode != VBR && enc_param[i].rc_mode != CBR && enc_param[i].rc_mode != LOW_LATENCY)) {
          GST_ERROR ("Rate control mode can be either VBR or CBR or Low Latency");
          return VGST_ERROR_RCMODE_NOT_SUPPORTED;
        }
        if (cmn_param->sink_type == STREAM) {
          if (op_param[i].port_num < MIN_PORT_NUMBER || op_param[i].port_num > MAX_PORT_NUMBER) {
            GST_ERROR ("Port number should be in the range of [%d]-[%d]",MIN_PORT_NUMBER, MAX_PORT_NUMBER);
            return VGST_ERROR_PORT_NUM_RANGE_MISMATCH;
          }
        }
        if ((TRUE == enc_param[i].hlg_sdr_compatible) && ((SDI_Tx != cmn_param->driver_type) || (TRUE == ip_param[i].enable_llp2))) {
          GST_ERROR ("HLG is only supported with (non-llp2) SDI design");
          return VGST_ERROR_HLG_NOT_SUPPORTED;
        }
      }
      if (0 == i) {
        ret = vlib_src_reset_config();
        if (ret) {
          GST_ERROR ("vlib source reset config failure %d ",ret);
          return ret;
        }
      }

      config.fps = cmn_param->frame_rate;
      config.width_in = ip_param[i].width;
      config.height_in = ip_param[i].height;
      config.format = ip_param[i].format;
      config.enable_scd = ip_param[i].enable_scd;
      config.scd_type = SCD_NONE;
      if (LIVE_SRC == ip_param[i].src_type && (ret = vlib_src_config(ip_param[i].device_type, &config))) {
        GST_ERROR ("vlib source config failure %d ",ret);
        return ret;
      }

      ip_param[i].scd_type = config.scd_type;
      if (aud_param[i].enable_audio) {
        aud_param[i].source_id = vlib_audio_find_device_id(SND_PCM_STREAM_CAPTURE, aud_param[i].audio_in);
        if (aud_param[i].source_id == NULL) {
            GST_ERROR ("Audio source device id is not found");
            return VGST_ERROR_AUDIO_DEVICE_ID_NOT_FOUND;
	    }
        aud_param[i].sink_id = vlib_audio_find_device_id(SND_PCM_STREAM_PLAYBACK, aud_param[i].audio_out);
	    if (aud_param[i].sink_id == NULL) {
            GST_ERROR ("Audio sink device is not found");
            return VGST_ERROR_AUDIO_DEVICE_ID_NOT_FOUND;
	    }
      }
    }

    if ((DISPLAY == cmn_param->sink_type || SPLIT_SCREEN == cmn_param->sink_type) && (cmn_param->driver_type != SDI_Tx)) {
      ret = vlib_drm_find_preferred_plane(cmn_param->driver_type, ip_param->format, &cmn_param->plane_id);
      if (ret) {
        GST_ERROR ("vlib_drm_find_preferred_plane failed %d", ret);
        return ret;
      }
    }

    if ((DISPLAY == cmn_param->sink_type || SPLIT_SCREEN == cmn_param->sink_type) && (cmn_param->driver_type != SDI_Tx)) {
      cmn_param->bus_id = vlib_drm_find_bus_id(cmn_param->driver_type);
      if (cmn_param->bus_id == NULL) {
        GST_ERROR ("vlib_drm_find_bus_id failed");
        return VGST_ERROR_DRM_BUS_ID_NOT_FOUND;
      }
    }

    init_struct_params (enc_param, ip_param, op_param, cmn_param, aud_param);
    for (i =0; i< num_src; i++) {
      vgst_print_params (i);
    }
    return VGST_SUCCESS;
}

gint
vgst_start_pipeline (void) {
    VGST_ERROR_LOG ret;
    // create a pipeline
    if ((ret = vgst_create_pipeline ())) {
      GST_ERROR ("pipeline creation failed !!!");
      return ret;
    }

    // run the pipeline
    if ((ret = vgst_run_pipeline ())) {
      GST_ERROR ("pipeline start failed !!!");
      return ret;
    }
    return ret;
}

void
vgst_get_fps (guint index, guint *fps) {
    get_fps (index, fps);
}

void
vgst_get_position (guint index, gint64 *pos) {
    get_position (index, pos);
}

void
vgst_get_duration (guint index, gint64 *duration) {
    get_duration (index, duration);
}

guint
vgst_get_bitrate (int index) {
    return get_bitrate (index);
}

guint
vgst_get_video_type (int index) {
    return get_video_type (index);
}

gint
vgst_stop_pipeline () {
    /* pipeline clean up */
    GST_DEBUG ("cleaning up the pipeline");
    return stop_pipeline ();
}

gint
vgst_poll_event (int *arg, int index) {
    return poll_event (arg, index);
}

gint vgst_init(void) {
    return vlib_src_init();
}

gint vgst_uninit(void) {
    return vlib_src_uninit();
}
