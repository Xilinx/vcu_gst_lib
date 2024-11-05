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

#ifndef INCLUDE_VGST_SPLIT_PIPELINE_H_
#define INCLUDE_VGST_SPLIT_PIPELINE_H_

#include "vgst_pipeline.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* This API is to link all elements required for split screen pipeline */
VGST_ERROR_LOG link_split_screen_elements (vgst_ip_params *ip_param, vgst_playback *play_ptr);

/* This API is to set all the required property for split screen pipeline */
void set_split_screen_property (vgst_application *app, gint index);

/* This API is to create all the elements required for split screen pipeline */
VGST_ERROR_LOG create_split_pipeline (vgst_ip_params *ip_param, vgst_enc_params *enc_param, vgst_playback *play_ptr);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_VGST_SPLIT_PIPELINE_H_ */
