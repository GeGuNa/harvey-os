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
#include	<draw.h>

/*
 * Sine and Cosine of arctangents, calculated by
 *   (sin(atan(index/100.0))*1024.+0.5)
 *   (cos(atan(index/100.0))*1024.+0.5)
 * To use, get rational tangent between 0<=tan<=1, scale by 100,
 * and look up sin and cos, and use linear interpolation.  divide by 1024.
 * Maximum error is 0.0020.  Without linear interpolation, it's 0.010.
 */
static
i16 sinus[] = {
	0,	/* 0.00 */
	10,	/* 0.01 */
	20,	/* 0.02 */
	31,	/* 0.03 */
	41,	/* 0.04 */
	51,	/* 0.05 */
	61,	/* 0.06 */
	72,	/* 0.07 */
	82,	/* 0.08 */
	92,	/* 0.09 */
	102,	/* 0.10 */
	112,	/* 0.11 */
	122,	/* 0.12 */
	132,	/* 0.13 */
	142,	/* 0.14 */
	152,	/* 0.15 */
	162,	/* 0.16 */
	172,	/* 0.17 */
	181,	/* 0.18 */
	191,	/* 0.19 */
	201,	/* 0.20 */
	210,	/* 0.21 */
	220,	/* 0.22 */
	230,	/* 0.23 */
	239,	/* 0.24 */
	248,	/* 0.25 */
	258,	/* 0.26 */
	267,	/* 0.27 */
	276,	/* 0.28 */
	285,	/* 0.29 */
	294,	/* 0.30 */
	303,	/* 0.31 */
	312,	/* 0.32 */
	321,	/* 0.33 */
	330,	/* 0.34 */
	338,	/* 0.35 */
	347,	/* 0.36 */
	355,	/* 0.37 */
	364,	/* 0.38 */
	372,	/* 0.39 */
	380,	/* 0.40 */
	388,	/* 0.41 */
	397,	/* 0.42 */
	405,	/* 0.43 */
	412,	/* 0.44 */
	420,	/* 0.45 */
	428,	/* 0.46 */
	436,	/* 0.47 */
	443,	/* 0.48 */
	451,	/* 0.49 */
	458,	/* 0.50 */
	465,	/* 0.51 */
	472,	/* 0.52 */
	480,	/* 0.53 */
	487,	/* 0.54 */
	493,	/* 0.55 */
	500,	/* 0.56 */
	507,	/* 0.57 */
	514,	/* 0.58 */
	520,	/* 0.59 */
	527,	/* 0.60 */
	533,	/* 0.61 */
	540,	/* 0.62 */
	546,	/* 0.63 */
	552,	/* 0.64 */
	558,	/* 0.65 */
	564,	/* 0.66 */
	570,	/* 0.67 */
	576,	/* 0.68 */
	582,	/* 0.69 */
	587,	/* 0.70 */
	593,	/* 0.71 */
	598,	/* 0.72 */
	604,	/* 0.73 */
	609,	/* 0.74 */
	614,	/* 0.75 */
	620,	/* 0.76 */
	625,	/* 0.77 */
	630,	/* 0.78 */
	635,	/* 0.79 */
	640,	/* 0.80 */
	645,	/* 0.81 */
	649,	/* 0.82 */
	654,	/* 0.83 */
	659,	/* 0.84 */
	663,	/* 0.85 */
	668,	/* 0.86 */
	672,	/* 0.87 */
	676,	/* 0.88 */
	681,	/* 0.89 */
	685,	/* 0.90 */
	689,	/* 0.91 */
	693,	/* 0.92 */
	697,	/* 0.93 */
	701,	/* 0.94 */
	705,	/* 0.95 */
	709,	/* 0.96 */
	713,	/* 0.97 */
	717,	/* 0.98 */
	720,	/* 0.99 */
	724,	/* 1.00 */
	728,	/* 1.01 */
};

static
i16 cosinus[] = {
	1024,	/* 0.00 */
	1024,	/* 0.01 */
	1024,	/* 0.02 */
	1024,	/* 0.03 */
	1023,	/* 0.04 */
	1023,	/* 0.05 */
	1022,	/* 0.06 */
	1022,	/* 0.07 */
	1021,	/* 0.08 */
	1020,	/* 0.09 */
	1019,	/* 0.10 */
	1018,	/* 0.11 */
	1017,	/* 0.12 */
	1015,	/* 0.13 */
	1014,	/* 0.14 */
	1013,	/* 0.15 */
	1011,	/* 0.16 */
	1010,	/* 0.17 */
	1008,	/* 0.18 */
	1006,	/* 0.19 */
	1004,	/* 0.20 */
	1002,	/* 0.21 */
	1000,	/* 0.22 */
	998,	/* 0.23 */
	996,	/* 0.24 */
	993,	/* 0.25 */
	991,	/* 0.26 */
	989,	/* 0.27 */
	986,	/* 0.28 */
	983,	/* 0.29 */
	981,	/* 0.30 */
	978,	/* 0.31 */
	975,	/* 0.32 */
	972,	/* 0.33 */
	969,	/* 0.34 */
	967,	/* 0.35 */
	963,	/* 0.36 */
	960,	/* 0.37 */
	957,	/* 0.38 */
	954,	/* 0.39 */
	951,	/* 0.40 */
	947,	/* 0.41 */
	944,	/* 0.42 */
	941,	/* 0.43 */
	937,	/* 0.44 */
	934,	/* 0.45 */
	930,	/* 0.46 */
	927,	/* 0.47 */
	923,	/* 0.48 */
	920,	/* 0.49 */
	916,	/* 0.50 */
	912,	/* 0.51 */
	909,	/* 0.52 */
	905,	/* 0.53 */
	901,	/* 0.54 */
	897,	/* 0.55 */
	893,	/* 0.56 */
	890,	/* 0.57 */
	886,	/* 0.58 */
	882,	/* 0.59 */
	878,	/* 0.60 */
	874,	/* 0.61 */
	870,	/* 0.62 */
	866,	/* 0.63 */
	862,	/* 0.64 */
	859,	/* 0.65 */
	855,	/* 0.66 */
	851,	/* 0.67 */
	847,	/* 0.68 */
	843,	/* 0.69 */
	839,	/* 0.70 */
	835,	/* 0.71 */
	831,	/* 0.72 */
	827,	/* 0.73 */
	823,	/* 0.74 */
	819,	/* 0.75 */
	815,	/* 0.76 */
	811,	/* 0.77 */
	807,	/* 0.78 */
	804,	/* 0.79 */
	800,	/* 0.80 */
	796,	/* 0.81 */
	792,	/* 0.82 */
	788,	/* 0.83 */
	784,	/* 0.84 */
	780,	/* 0.85 */
	776,	/* 0.86 */
	773,	/* 0.87 */
	769,	/* 0.88 */
	765,	/* 0.89 */
	761,	/* 0.90 */
	757,	/* 0.91 */
	754,	/* 0.92 */
	750,	/* 0.93 */
	746,	/* 0.94 */
	742,	/* 0.95 */
	739,	/* 0.96 */
	735,	/* 0.97 */
	731,	/* 0.98 */
	728,	/* 0.99 */
	724,	/* 1.00 */
	720,	/* 1.01 */
};

void
icossin2(int x, int y, int *cosp, int *sinp)
{
	int sinsign, cossign, tan, tan10, rem;
	i16 *stp, *ctp;

	if(x == 0){
		if(y >= 0)
			*sinp = ICOSSCALE, *cosp = 0;
		else
			*sinp = -ICOSSCALE, *cosp = 0;
		return;
	}
	sinsign = cossign = 1;
	if(x < 0){
		cossign = -1;
		x = -x;
	}
	if(y < 0){
		sinsign = -1;
		y = -y;
	}
	if(y > x){
		tan = 1000*x/y;
		tan10 = tan/10;
		stp = &cosinus[tan10];
		ctp = &sinus[tan10];
	}else{
		tan = 1000*y/x;
		tan10 = tan/10;
		stp = &sinus[tan10];
		ctp = &cosinus[tan10];
	}
	rem = tan-(tan10*10);
	*sinp = sinsign*(stp[0]+(stp[1]-stp[0])*rem/10);
	*cosp = cossign*(ctp[0]+(ctp[1]-ctp[0])*rem/10);
}
