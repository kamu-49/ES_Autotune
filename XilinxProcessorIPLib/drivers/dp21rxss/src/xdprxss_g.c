
/*******************************************************************
* Copyright (c) 2010-2021 Xilinx, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
*******************************************************************/

#include "xparameters.h"
#include "xdprxss.h"

/*
* The configuration table for devices
*/


/*
* List of Sub-cores included from the subsystem
*/
#define XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_CLK_WIZ_PRESENT	1
#define XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_PRESENT	1
#define XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_RX_HDCP_PRESENT	1
#define XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_RX_HDCP22_PRESENT	1
#define XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_IIC_PRESENT	1
#define XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_TIMER_PRESENT	1

/*
* List of Sub-cores excluded from the subsystem
*/


XDpRxSs_Config XDpRxSs_ConfigTable[] =
{
	{
		XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DEVICE_ID,
		XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_BASEADDR,
		XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_AUDIO_ENABLE,
		XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_AUDIO_CHANNELS,
		XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_BITS_PER_COLOR,
		XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_HDCP_ENABLE,
		XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_HDCP22_ENABLE,
		XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_LANE_COUNT,
		XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_MODE,
		XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_NUM_STREAMS,
		XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_COLOR_FORMAT,
		XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_INCLUDE_AXI_IIC,
		XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_INCLUDE_CLK_WIZ,

		{
			XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_IIC_PRESENT,
			{
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_IIC_DEVICE_ID,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_IIC_BASEADDR,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_IIC_TEN_BIT_ADR,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_IIC_GPO_WIDTH
			}
		},
		{
			XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_TIMER_PRESENT,
			{
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_TIMER_DEVICE_ID,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_TIMER_BASEADDR
			}
		},
		{
			XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_CLK_WIZ_PRESENT,
			{
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_CLK_WIZ_DEVICE_ID,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_CLK_WIZ_BASEADDR
			}
		},
		{
			XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_PRESENT,
			{
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_DEVICE_ID,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_BASEADDR,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_S_AXI_ACLK,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_LANE_COUNT,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_LINK_RATE,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_MAX_BITS_PER_COLOR,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_QUAD_PIXEL_ENABLE,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_DUAL_PIXEL_ENABLE,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_YCRCB_ENABLE,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_YONLY_ENABLE,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_GT_DATAWIDTH,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_SECONDARY_SUPPORT,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_AUDIO_CHANNELS,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_MST_ENABLE,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_NUMBER_OF_MST_STREAMS,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_PROTOCOL_SELECTION,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_FLOW_DIRECTION
			}
		},
		{
			XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_RX_HDCP_PRESENT,
			{
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_RX_HDCP_DEVICE_ID,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_RX_HDCP_BASEADDR
			}
		},
		{
			XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_RX_HDCP22_PRESENT,
			{
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_RX_HDCP22_DEVICE_ID,
				XPAR_DP_RX_HIER_0_V_DP_RXSS1_0_DP_RX_HDCP22_BASEADDR
			}
		}
	}
};
