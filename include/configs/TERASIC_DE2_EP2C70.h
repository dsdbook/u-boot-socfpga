/*
 * (C) Copyright 2005, Psyent Corporation <www.psyent.com>
 * Scott McNutt <smcnutt@psyent.com>
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

/* modify by dxzhang <dxzhang@ustc.edu>, for Terasic DE2 EP2C70 Board */

#ifndef __CONFIG_H
#define __CONFIG_H

/*------------------------------------------------------------------------
 * BOARD/CPU
 *----------------------------------------------------------------------*/
#include "../../board/altera/terasic_de2_ep2c70/custom_fpga.h" /* fpga parameters */
#define CONFIG_BOARD_NAME "Terasic DE2 EP2C70 board" /* custom board name */

#define CONFIG_TERASIC_DE2_EP2C70	1	/* terasic DE2 ep2c70 board*/

#define CONFIG_BOARD_EARLY_INIT_F 1	/* enable early board-spec. init*/


/*------------------------------------------------------------------------
 * MEMORY ORGANIZATION
 *	-Monitor at top.
 *	-The heap is placed below the monitor.
 *	-Global data is placed below the heap.
 *	-The stack is placed below global data (&grows down).
 *----------------------------------------------------------------------*/
#define CONFIG_SYS_MONITOR_LEN		(256 * 1024)	/* Reserve 128k		*/
#define CONFIG_GBL_DATA_SIZE	128		/* Global data size rsvd*/
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 128*1024)

//#define CONFIG_MONITOR_BASE	TEXT_BASE
#define CONFIG_MONITOR_IS_IN_RAM
#define CONFIG_SYS_MONITOR_BASE	(CONFIG_SYS_SDRAM_BASE + \
					 CONFIG_SYS_SDRAM_SIZE - \
					 CONFIG_SYS_MONITOR_LEN)
#define CONFIG_SYS_MALLOC_BASE		(CONFIG_SYS_MONITOR_BASE - CONFIG_SYS_MALLOC_LEN)
#define CONFIG_GBL_DATA_OFFSET	(CONFIG_SYS_MALLOC_BASE - CONFIG_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP		CONFIG_GBL_DATA_OFFSET

/*------------------------------------------------------------------------
 * FLASH (S29GL064A90TFIR4) 16bit mode
 *----------------------------------------------------------------------*/

/* dxzhang : need to read flash  datasheet */
#define CONFIG_SYS_MAX_FLASH_SECT	135		/* Max # sects per bank */
#define CONFIG_SYS_MAX_FLASH_BANKS	1		/* Max # of flash banks */
#define CONFIG_SYS_FLASH_ERASE_TOUT	1024*16		/* Erase timeout (msec) */
#define CONFIG_SYS_FLASH_WRITE_TOUT	128*32		/* Write timeout (msec) */
//according to the output of Altera nios2-flash-programmer tool,
//dxzhang: use 8bit mode
#define CONFIG_SYS_FLASH_WORD_SIZE	unsigned short	/* flash word size	*/

#define CONFIG_SYS_FLASH_BASE	CFI_FLASH_BASE
#define CONFIG_SYS_FLASH_SIZE	0x00800000
/*------------------------------------------------------------------------
 * ENVIRONMENT -- Put environment in sector CONFIG_MONITOR_LEN above
 * CONFIG_RESET_ADDR, since we assume the monitor is stored at the
 * reset address, no? This will keep the environment in user region
 * of flash. NOTE: the monitor length must be multiple of sector size
 * (which is common practice).
 *----------------------------------------------------------------------*/
#define CONFIG_ENV_IS_IN_FLASH	1		/* Environment in flash */
#define CONFIG_ENV_SIZE		(64 * 1024)	/* 64 KByte (1 sector)	*/
#define CONFIG_ENV_OVERWRITE			/* Serial change Ok	*/
/* #define CONFIG_ENV_ADDR	(CONFIG_RESET_ADDR + CONFIG_MONITOR_LEN) */
/* add by dxzhang */
#define U_BOOT_BIN_MAX_SIZE	(64 * 1024 * 3)/* 64*3 K Bytes */
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + U_BOOT_BIN_MAX_SIZE)

/*
 * SERIAL
 */
#define CONFIG_ALTERA_UART
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

/*------------------------------------------------------------------------
 * EPCS Device -- wne CONFIG_NIOS_EPCSBASE is defined code/commands for
 * epcs device access is enabled. The base address is the epcs
 * _register_ base address, NOT THE ADDRESS OF THE MEMORY BLOCK.
 * The register base is currently at offset 0x600 from the memory base.
 *----------------------------------------------------------------------*/
//#define CONFIG_NIOS_EPCSBASE	na_epcs_controller	/* EPCS register base	*/

/*------------------------------------------------------------------------
 * DEBUG
 *----------------------------------------------------------------------*/
#undef CONFIG_ROM_STUBS				/* Stubs not in ROM	*/

/*------------------------------------------------------------------------
 * TIMEBASE --
 *
 * The high res timer defaults to 1 msec. Since it includes the period
 * registers, we can slow it down to 10 msec using TMRCNT. If the default
 * period is acceptable, TMRCNT can be left undefined.
 *----------------------------------------------------------------------*/
#define CONFIG_SYS_LOW_RES_TIMER
#define CONFIG_SYS_NIOS_TMRBASE		CONFIG_SYS_TIMER_BASE
#define CONFIG_SYS_NIOS_TMRIRQ		CONFIG_SYS_TIMER_IRQ
#define CONFIG_SYS_NIOS_TMRMS		10	/* Desired period (msec)*/
#define CONFIG_SYS_NIOS_TMRCNT \
	(CONFIG_SYS_NIOS_TMRMS * (CONFIG_SYS_TIMER_FREQ / 1000) - 1)
#define CONFIG_SYS_HZ		(CONFIG_SYS_CLK_FREQ/(CONFIG_SYS_NIOS_TMRCNT + 1))

/*------------------------------------------------------------------------
 * STATUS LED -- Provides a simple blinking led. For Nios2 each board
 * must implement its own led routines -- leds are, after all,
 * board-specific, no?
 *----------------------------------------------------------------------*/
#define CONFIG_SYS_STATUS_LED		/* Enable status driver */
#define CONFIG_SYS_BOARD_SPECIFIC_LED
#define CONFIG_SYS_GPIO_LED		/* Enable GPIO LED driver */
#define CONFIG_SYS_GPIO			/* Enable GPIO driver */

#define STATUS_LED_BIT		1		/* Bit-0 on PIO		*/
#define STATUS_LED_STATE	1		/* Blinking		*/
#define STATUS_LED_PERIOD	(500/CONFIG_SYS_NIOS_TMRMS) /* Every 500 msec	*/

/*------------------------------------------------------------------------
 * ETHERNET -- The header file for the SMC91111 driver hurts my eyes ...
 * and really doesn't need any additional clutter. So I choose the lazy
 * way out to avoid changes there -- define the base address to ensure
 * cache bypass so there's no need to monkey with inx/outx macros.
 *----------------------------------------------------------------------*/

//#define CONFIG_SMC91111_BASE	0x82110300	/* Base addr (bypass)	*/
//#define CONFIG_DRIVER_SMC91111		/* Using SMC91c111	*/
//#undef  CONFIG_SMC91111_EXT_PHY		/* Internal PHY		*/
//#define CONFIG_SMC_USE_32_BIT			/* 32-bit interface	*/


#define CONFIG_DRIVER_DM9000		1
#define CONFIG_DM9000_BASE		(DM9000A_BASE|0x80000000)
#define DM9000_IO			CONFIG_DM9000_BASE
#define DM9000_DATA			(CONFIG_DM9000_BASE+4)
/* #define CONFIG_DM9000_USE_8BIT */
#define CONFIG_DM9000_USE_16BIT 
/* #define CONFIG_DM9000_USE_32BIT */
 
#define CONFIG_ETHADDR		08:00:3e:26:0a:5b
#define CONFIG_NETMASK		255.255.255.0
#define CONFIG_IPADDR		192.168.5.21
#define CONFIG_SERVERIP		192.168.5.1

// dxzhang: for net debug; should use Makefile to define -DET_DEBUG
//#define ET_DEBUG

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
//add by dxzhang
#define CONFIG_BZIP2
//end by dxzhang

#include <config_cmd_default.h>

#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_IRQ
#define CONFIG_CMD_PING
#define CONFIG_CMD_SAVES
#define CONFIG_CMD_PORTIO

#define CONFIG_CMD_RUN

//#undef CONFIG_CMD_AUTOSCRIPT
//#undef CONFIG_CMD_BOOTD
//#undef CONFIG_CMD_CONSOLE
//#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_IMLS
#undef CONFIG_CMD_ITEST
//#undef CONFIG_CMD_NFS
#undef CONFIG_CMD_SETGETDCR
#undef CONFIG_CMD_XIMG


/*------------------------------------------------------------------------
 * MISC
 *----------------------------------------------------------------------*/
#define CONFIG_SYS_LONGHELP				/* Provide extended help*/
#define CONFIG_SYS_PROMPT		"==> "		/* Command prompt	*/
#define CONFIG_SYS_CBSIZE		256		/* Console I/O buf size */
#define CONFIG_SYS_MAXARGS		16		/* Max command args	*/
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE	/* Boot arg buf size	*/
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16) /* Print buf size */
#define CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_SDRAM_BASE	/* Default load address */
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE	/* Start addr for test	*/
#define CONFIG_SYS_MEMTEST_END		CONFIG_SYS_INIT_SP - 0x00020000

#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "

#endif	/* __CONFIG_H */
