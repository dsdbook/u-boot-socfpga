/*
 * (C) Copyright 2005, Psyent Corporation <www.psyent.com>
 * Scott McNutt <smcnutt@psyent.com>
 * (C) Copyright 2010, Thomas Chou <thomas@wytron.com.tw>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * BOARD/CPU
 */
#include "../board/altera/nios2-generic/3c120_fpga.h" /* fpga parameters */
#define CONFIG_BOARD_NAME "3C120" /* custom board name */
#define CONFIG_BOARD_EARLY_INIT_F	/* enable early board-spec. init */
#define CONFIG_SYS_NIOS_SYSID_BASE	CONFIG_SYS_SYSID_BASE

/*
 * SERIAL
 */
#define CONFIG_ALTERA_JTAG_UART
#if defined(CONFIG_ALTERA_JTAG_UART)
# define CONFIG_SYS_NIOS_CONSOLE	CONFIG_SYS_JTAG_UART_BASE
#else
# define CONFIG_SYS_NIOS_CONSOLE	CONFIG_SYS_UART_BASE
#endif

#define CONFIG_ALTERA_JTAG_UART_BYPASS
#define CONFIG_SYS_NIOS_FIXEDBAUD
#define CONFIG_BAUDRATE		CONFIG_SYS_UART_BAUD
#define CONFIG_SYS_BAUDRATE_TABLE	{CONFIG_BAUDRATE}
#define CONFIG_SYS_CONSOLE_INFO_QUIET	/* Suppress console info */

/*
 * TIMER
 */
#define CONFIG_SYS_NIOS_TMRBASE	CONFIG_SYS_TIMER_BASE
#define CONFIG_SYS_NIOS_TMRIRQ		CONFIG_SYS_TIMER_IRQ
#define CONFIG_SYS_HZ			1000	/* Always 1000 */
#define CONFIG_SYS_NIOS_TMRMS		10	/* Desired period (msec)*/
#define CONFIG_SYS_NIOS_TMRCNT \
	(CONFIG_SYS_NIOS_TMRMS * (CONFIG_SYS_TIMER_FREQ / 1000) - 1)

/*
 * STATUS LED
 */
#if 0
#define CONFIG_STATUS_LED		/* Enable status driver */
#define CONFIG_GPIO_LED		/* Enable GPIO LED driver */
#define CONFIG_GPIO			/* Enable GPIO driver */
#define LED_PIO_BASE USER_LED_PIO_8OUT_BASE

#define STATUS_LED_BIT			0	/* Bit-0 on GPIO */
#define STATUS_LED_STATE		1	/* Blinking */
#define STATUS_LED_PERIOD	(500 / CONFIG_SYS_NIOS_TMRMS) /* 500 msec */
#endif

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME

/*
 * Command line configuration.
 */
#include <config_cmd_default.h>
#undef CONFIG_CMD_BOOTD
#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_IMLS
#undef CONFIG_CMD_ITEST
#undef CONFIG_CMD_NFS
#undef CONFIG_CMD_SETGETDCR
#undef CONFIG_CMD_XIMG

#ifdef CONFIG_CMD_NET
# define CONFIG_NET_MULTI
# define CONFIG_CMD_DHCP
# define CONFIG_CMD_PING
#endif

/*
 * ENVIRONMENT -- Put environment in sector CONFIG_SYS_MONITOR_LEN above
 * CONFIG_SYS_RESET_ADDR, since we assume the monitor is stored at the
 * reset address, no? This will keep the environment in user region
 * of flash. NOTE: the monitor length must be multiple of sector size
 * (which is common practice).
 */
#define CONFIG_ENV_IS_IN_FLASH

#define CONFIG_ENV_SIZE		0x20000	/* 64k, 1 sector */
#define CONFIG_ENV_OVERWRITE		/* Serial change Ok	*/
/* Environment location at the second 1MB region */
#define CONFIG_ENV_ADDR		((CONFIG_SYS_RESET_ADDR + \
					0x100000) | \
					 CONFIG_SYS_FLASH_BASE)

/*
 * MEMORY ORGANIZATION
 *	-Monitor at top of sdram.
 *	-The heap is placed below the monitor
 *	-Global data is placed below the heap.
 *	-The stack is placed below global data (&grows down).
 */
#define CONFIG_MONITOR_IS_IN_RAM
#define CONFIG_SYS_MONITOR_LEN		0x40000	/* Reserve 256k */
#define CONFIG_SYS_MONITOR_BASE		(CONFIG_SYS_SDRAM_BASE + \
					 CONFIG_SYS_SDRAM_SIZE - \
					 CONFIG_SYS_MONITOR_LEN)
#define CONFIG_SYS_GBL_DATA_SIZE	256	/* Global data size rsvd */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 0x20000)
#define CONFIG_SYS_MALLOC_BASE		(CONFIG_SYS_MONITOR_BASE - \
					 CONFIG_SYS_MALLOC_LEN)
#define CONFIG_SYS_GBL_DATA_OFFSET	(CONFIG_SYS_MALLOC_BASE - \
					 CONFIG_SYS_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP		CONFIG_SYS_GBL_DATA_OFFSET

/*
 * MISC
 */
#define CONFIG_SYS_LONGHELP		/* Provide extended help */
#define CONFIG_SYS_PROMPT		"NIOS2 # "	/* Command prompt	*/
#define CONFIG_SYS_CBSIZE		256	/* Console I/O buf size */
#define CONFIG_SYS_MAXARGS		16	/* Max command args	*/
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE /* Bootarg buf size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + \
					 16)	/* Print buf size */
#define CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_INIT_SP - 0x20000)
#define CONFIG_CMDLINE_EDITING

#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "

/* Support FDT command at console */
#define CONFIG_OF_LIBFDT
#define CONFIG_LMB

/* Added environment to ease user usage */
#define CONFIG_AUTO_COMPLETE
#define CONFIG_BOOTARGS "console=ttyJ0,115200"
#define CONFIG_EXTRA_ENV_SETTINGS \
	"autoload=no\0" \
	"bootfile=vmImage\0" \
	"fdtimage=3c120_devboard.dtb\0" \
	"fdtaddr=0xD5000000\0" \
	"loadaddr=0xD6000000\0" \
	"fdtimagesize=0x1000\0" \
	"bootfilesize=0x200000\0" \
	"cfifdtaddr=0xE0200000\0" \
	"cfibootaddr=0xE0300000\0" \
	"cfiroot=/dev/mtdblock0 rootfstype=jffs2 rw\0" \
	"cfiload=cp.b ${cfifdtaddr} ${fdtaddr} ${fdtimagesize};" \
		"cp.b ${cfibootaddr} ${loadaddr} ${bootfilesize}\0" \
	"cfiboot=setenv bootargs " CONFIG_BOOTARGS " root=${cfiroot};" \
		"bootm ${loadaddr} - ${fdtaddr}\0" \
	"nfsdhcpboot=setenv bootargs " CONFIG_BOOTARGS " root=/dev/nfs " \
		"rw nfsroot=${serverip}:${rootpath},tcp " \
		"ip=dhcp;bootm ${loadaddr} - ${fdtaddr}\0" \
	"nfsstaticboot=setenv bootargs " CONFIG_BOOTARGS " root=/dev/nfs " \
		"rw nfsroot=${serverip}:${rootpath},tcp " \
		"ip=${ipaddr}:${serverip}:${gatewayip}:${netmask};" \
		"bootm ${loadaddr} - ${fdtaddr}\0" \
	"tftpload=tftp ${loadaddr} ${bootfile};" \
		"tftp ${fdtaddr} ${fdtimage}\0" \
	"serverip=196.168.96.11\0" \
	"rootpath=/home/nios2/nfs\0" \
	"ipaddr=196.168.96.10\0" \
	"gatewayip=196.168.96.254\0" \
	"netmask=255.255.255.0\0" \
	"ethaddr=22:33:44:55:66:77\0"


#define CONFIG_CMD_RUN
/* Boot command will automatically executed */
#define CONFIG_BOOTCOMMAND 	"run cfiload; run cfiboot"
/* Delay before automatically booting the default image */
#define CONFIG_BOOTDELAY	3

#endif /* __CONFIG_H */
