################################################################################
# Copyright (C) 2015 - 2020 Xilinx, Inc.  All rights reserved.
# SPDX-License-Identifier: MIT
#
################################################################################

proc generate {drv_handle} {
  ::hsi::utils::define_include_file $drv_handle "xparameters.h" "XHdcp22_Cipher_Dp" \
	"NUM_INSTANCES" \
	"DEVICE_ID" \
	"C_BASEADDR" \
	"C_HIGHADDR"

  ::hsi::utils::define_config_file $drv_handle "xhdcp22_cipher_dp_g.c" "XHdcp22_Cipher_Dp" \
	"DEVICE_ID" \
	"C_BASEADDR"

  ::hsi::utils::define_canonical_xpars $drv_handle "xparameters.h" "XHdcp22_Cipher_Dp" \
	"NUM_INSTANCES" \
	"DEVICE_ID" \
	"C_BASEADDR" \
	"C_HIGHADDR"
}
