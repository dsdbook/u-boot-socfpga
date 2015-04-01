/*
 * (C) Copyright 2000-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
// modify for S29AL032D  mode 04  , by dxzhang@ustc.edu

#include <common.h>
#if defined(CONFIG_NIOS)
#include <nios.h>
#else
#include <asm/io.h>
#endif

/* chip info:
	4M * 8 bit, 4M bytes
	offset : 0   8 * 8K
	offset : 0x10000     63 * 64K
	total : 71 sector , bottom boot sector
*/

#define SECTSZ_SMALL		(8 * 1024)
#define SECTSZ_LARGE		(64 * 1024)
#define FIRST_ADDR	( CONFIG_SYS_FLASH_BASE + 0x0AAA )
#define SECOND_ADDR	( CONFIG_SYS_FLASH_BASE + 0x0555 )

flash_info_t flash_info[CONFIG_SYS_MAX_FLASH_BANKS];

/*----------------------------------------------------------------------*/
unsigned long flash_init (void)
{
	int i;
	unsigned long addr;
	flash_info_t *fli = &flash_info[0];

	fli->size = CONFIG_SYS_FLASH_SIZE;
	fli->sector_count = CONFIG_SYS_MAX_FLASH_SECT;
	fli->flash_id = FLASH_AMD_COMP + FLASH_S29AL032D_04;

	addr = CONFIG_SYS_FLASH_BASE;
	
	for (i = 0; i < 8; ++i) {
		fli->start[i] = addr;
		addr += SECTSZ_SMALL;
		fli->protect[i] = 1;
	}
	for (i = 8; i < fli->sector_count; ++i) {
		fli->start[i] = addr;
		addr += SECTSZ_LARGE;
		fli->protect[i] = 1;
	}

	return (CONFIG_SYS_FLASH_SIZE);
}
/*--------------------------------------------------------------------*/
void flash_print_info (flash_info_t * info)
{
	int i, k;
	int erased;
	unsigned long *addr;

	printf ("  Size: %ld KB in %d Sectors\n",
		info->size >> 10, info->sector_count);
	printf ("  Sector Start Addresses:");
	for (i = 0; i < info->sector_count; ++i) {

		/* Check if whole sector is erased */
		erased = 1;
		addr = (unsigned long *) info->start[i];
//add by dxzhang
		int sector_size;
		if(i < 8) sector_size 	= SECTSZ_SMALL;
		else	sector_size 	= SECTSZ_LARGE;
//end by dxzhang

		for (k = 0; k < sector_size/sizeof(unsigned long); k++) {
			if ( readl(addr++) != (unsigned long)-1) {
				erased = 0;
				break;
			}
		}

		/* Print the info */
		if ((i % 5) == 0)
			printf ("\n   ");
		printf (" %08lX%s%s",
			info->start[i],
			erased ? " E" : "  ",
			info->protect[i] ? "RO " : "   ");
	}
	printf ("\n");
}

/*-------------------------------------------------------------------*/


int flash_erase (flash_info_t * info, int s_first, int s_last)
{
	unsigned char *addr = (unsigned char *) info->start[0];
	unsigned char *addr2;
	int prot, sect;
	ulong start;

	/* Some sanity checking */
	if ((s_first < 0) || (s_first > s_last)) {
		printf ("- no sectors to erase\n");
		return 1;
	}

	prot = 0;
	for (sect = s_first; sect <= s_last; ++sect) {
		if (info->protect[sect]) {
			prot++;
		}
	}
	if (prot) {
		printf ("- Warning: %d protected sectors will not be erased!\n",
			prot);
	} else {
		printf ("\n");
	}

	/* It's ok to erase multiple sectors provided we don't delay more
	 * than 50 usec between cmds ... at which point the erase time-out
	 * occurs. So don't go and put printf() calls in the loop ... it
	 * won't be very helpful ;-)
	 */
	for (sect = s_first; sect <= s_last; sect++) {
		if (info->protect[sect] == 0) {	/* not protected */
			addr2 = (unsigned char *) info->start[sect];
//add by dxzhang
			writeb (0xaa,FIRST_ADDR);
			flush_dcache(FIRST_ADDR,1);
			writeb (0x55,SECOND_ADDR);
			flush_dcache(SECOND_ADDR,1);
			writeb (0x80,FIRST_ADDR);
			flush_dcache(FIRST_ADDR,1);
			writeb (0xaa,FIRST_ADDR);
			flush_dcache(FIRST_ADDR,1);
			writeb (0x55,SECOND_ADDR);
			flush_dcache(SECOND_ADDR,1);
			writeb (0x30,addr2);
			flush_dcache(addr2,1);
//end by dxzhang

			/* Now just wait for 0xff & provide some user
			 * feedback while we wait.
			 */
			start = get_timer (0);
			while ( readb (addr2) != 0xff) {
				flush_dcache(addr2,1);
				udelay (1000 * 1000);
				putc ('.');
				if (get_timer (start) > CONFIG_SYS_FLASH_ERASE_TOUT) {
					printf ("timeout\n");
					return 1;
				}
			}
		}
	}
	printf ("\n");
	return 0;
}

/*-----------------------------------------------------------------------
 * Copy memory to flash, returns:
 * 0 - OK
 * 1 - write timeout
 * 2 - Flash not erased
 */

int write_buff (flash_info_t * info, uchar * src, ulong addr, ulong cnt)
{

	vu_char *cmd = (vu_char *) info->start[0];
	vu_char *dst = (vu_char *) addr;
	unsigned char b;
	ulong start;
	while (cnt) {
		/* Check for sufficient erase */
		b = *src;
		if ((readb (dst) & b) != b) {
			printf ("%02x : %02x\n", readb (dst), b);
			return (2);
		}
//add by dxzhang

		writeb (0xaa,FIRST_ADDR);
		flush_dcache(FIRST_ADDR,1);
		writeb (0x55,SECOND_ADDR);
		flush_dcache(SECOND_ADDR,1);
		writeb (0xa0,FIRST_ADDR);
		flush_dcache(FIRST_ADDR,1);
		writeb (b,dst);
		flush_dcache(dst,1);
//end by dxzhang
		/* Verify write */
		start = get_timer (0);
		while (readb (dst) != b) {
			flush_dcache(dst,1);
			if (get_timer (start) > CONFIG_SYS_FLASH_WRITE_TOUT) {
				printf ("write_buff timeout\n");
				return 1;
			}
		}
		dst++;
		src++;
		cnt--;
	}

	return (0);
}
