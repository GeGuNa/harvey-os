/*
 * This file is part of the UCB release of Plan 9. It is subject to the license
 * terms in the LICENSE file found in the top-level directory of this
 * distribution and at http://akaros.cs.berkeley.edu/files/Plan9License. No
 * part of the UCB release of Plan 9, including this file, may be copied,
 * modified, propagated, or distributed except according to the terms contained
 * in the LICENSE file.
 */

#include	<u.h>
#include	<libc.h>
#include	<ip.h>

static	i16	endian	= 1;
static	u8 *	aendian	= (u8*)&endian;
#define	LITTLE	*aendian

u16
ptclbsum(u8 *addr, int len)
{
	u32 losum, hisum, mdsum, x;
	u32 t1, t2;

	losum = 0;
	hisum = 0;
	mdsum = 0;

	x = 0;
	if((uintptr)addr & 1) {
		if(len) {
			hisum += addr[0];
			len--;
			addr++;
		}
		x = 1;
	}
	while(len >= 16) {
		t1 = *(u16*)(addr+0);
		t2 = *(u16*)(addr+2);	mdsum += t1;
		t1 = *(u16*)(addr+4);	mdsum += t2;
		t2 = *(u16*)(addr+6);	mdsum += t1;
		t1 = *(u16*)(addr+8);	mdsum += t2;
		t2 = *(u16*)(addr+10);	mdsum += t1;
		t1 = *(u16*)(addr+12);	mdsum += t2;
		t2 = *(u16*)(addr+14);	mdsum += t1;
		mdsum += t2;
		len -= 16;
		addr += 16;
	}
	while(len >= 2) {
		mdsum += *(u16*)addr;
		len -= 2;
		addr += 2;
	}
	if(x) {
		if(len)
			losum += addr[0];
		if(LITTLE)
			losum += mdsum;
		else
			hisum += mdsum;
	} else {
		if(len)
			hisum += addr[0];
		if(LITTLE)
			hisum += mdsum;
		else
			losum += mdsum;
	}

	losum += hisum >> 8;
	losum += (hisum & 0xff) << 8;
	while((hisum = losum>>16) != 0)
		losum = hisum + (losum & 0xffff);

	return losum & 0xffff;
}
