/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2014 Chen-Yu Tsai <wens@csie.org>
 *
 * Placeholder wrapper to allow addressing Allwinner A23 (and later) sun8i
 * CPU based devices separately. Please do not add anything in here.
 */
#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * A23 specific configuration
 */

#ifdef CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_SUNXI
#endif

#ifdef CONFIG_MACH_SUN8I_H3
	#define CONFIG_SUNXI_USB_PHYS	4
#elif defined CONFIG_MACH_SUN8I_A83T
	#define CONFIG_SUNXI_USB_PHYS	3
#elif defined CONFIG_MACH_SUN8I_V3S
	#define CONFIG_SUNXI_USB_PHYS	1
#else
	#define CONFIG_SUNXI_USB_PHYS	2
#endif

/**
 * @brief 开启UPB，自动通过TFTP烧写uboot到SPI FLASH
 */
#define CONFIG_CMD_UPB
#define CONFIG_CMD_UPDATE

/*
 * Include common sunxi configuration where most the settings are
 */
#include <configs/sunxi-common.h>

#endif /* __CONFIG_H */
