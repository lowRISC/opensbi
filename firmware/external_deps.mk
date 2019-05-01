#
# SPDX-License-Identifier: BSD-2-Clause
#
# Copyright (c) 2019 Western Digital Corporation or its affiliates.
#
# Authors:
#   Anup Patel <anup.patel@wdc.com>
#

$(platform_build_dir)/firmware/fw_payload.o: $(FW_PAYLOAD_PATH_FINAL)
$(platform_build_dir)/firmware/fw_payload.o: $(FW_PAYLOAD_FDT_PATH)

$(platform_build_dir)/firmware/payloads/test_head.o: $(platform_build_dir)/firmware/payloads/ariane.dtb
$(platform_build_dir)/firmware/payloads/test_head.o: $(platform_build_dir)/firmware/payloads/Image

$(platform_build_dir)/firmware/payloads/ariane.dtb: firmware/payloads/ariane.dts
	dtc -I dts $< -O dtb -o $@

$(platform_build_dir)/firmware/payloads/Image: /local/scratch/jrrk2/linux-git/arch/riscv/boot/Image
	cp -p $< $@
