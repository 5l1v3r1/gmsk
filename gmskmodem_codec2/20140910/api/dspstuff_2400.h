// DSP related functions and data //
// 24000 bps version


/*
 *      Original copyright:
 *      Copyright (C) 2011 by Jonathan Naylor, G4KLX
 *
 *      Copyright (C) 2013 by Kristoff Bonne, ON1ARF
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; version 2 of the License.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 */

// These is the low-level DSP functions that make up the gmsk demodulator:
// firfilter
// modulate
// demodulate

// This code is largely based on the code from the "pcrepeatercontroller"
// project, written by Jonathan Naylor 
// More info: 
// http://groups.yahoo.com/group/pcrepeatercontroller



// Changes:
// Convert FIR-filter to integer
// Change PLL values so to make PLLINC and SMALLPLLINC match integer-boundaries
// Concert C++ to C


// Version 20111106: initial release demodulation
// Version 20111213: initial release modulation
// Version 20120105: no change
// Version 20130228: modified for API: local vars moved to session
// Version 20130314: API c2gmsk version / bitrate control + versionid codes
// Version 20130324: convert into .so shared library
// Version 20130506: add support for 2400 baud (split into two files: a_dspstuff_4800 and a_dspstuff_2400)



//////////////////////////
// 2400 BAUD VERSION
///////////////

// global data


#if _USEFLOAT == 1

float coeffs_table_modulate_2400 [] = {
// gaussfir(0.5, 2, 20)
        3.227953003617390e-014, 1.316988241100184e-013, 5.185336906425121e-013,
        1.970209678603344e-012, 7.224175781676820e-012, 2.556257202551268e-011,
        8.728933415057301e-011, 2.876459077365708e-010, 9.147356526491929e-010,
        2.807200293071396e-009, 8.313647035679894e-009, 2.376020324249415e-008,
        6.553134893509634e-008, 1.744168552591735e-007, 4.479898593205314e-007,
        1.110422626217248e-006, 2.656126831651419e-006, 6.131259576200486e-006,
        1.365812190078250e-005, 2.936112240141099e-005, 6.091085700995540e-005,
        1.219430782880170e-004, 2.355916997104800e-004, 4.392413365938098e-004,
        7.902905900637187e-004, 1.372179114501485e-003, 2.299191716915076e-003,
        3.717748055146816e-003, 5.801297154449817e-003, 8.735960373216617e-003,
        1.269511346313143e-002, 1.780339820034465e-002, 2.409403916531227e-002,
        3.146709895004599e-002, 3.965922170582210e-002, 4.823608546059376e-002,
        5.661614726353081e-002, 6.412818612763920e-002, 7.009676690120642e-002,
        7.394135519945359e-002, 7.526918477892675e-002, 7.394135519945361e-002,
        7.009676690120642e-002, 6.412818612763920e-002, 5.661614726353077e-002,
        4.823608546059376e-002, 3.965922170582214e-002, 3.146709895004599e-002,
        2.409403916531227e-002, 1.780339820034461e-002, 1.269511346313143e-002,
        8.735960373216637e-003, 5.801297154449817e-003, 3.717748055146816e-003,
        2.299191716915074e-003, 1.372179114501485e-003, 7.902905900637199e-004,
        4.392413365938098e-004, 2.355916997104800e-004, 1.219430782880168e-004,
        6.091085700995540e-005, 2.936112240141107e-005, 1.365812190078250e-005,
        6.131259576200486e-006, 2.656126831651410e-006, 1.110422626217248e-006,
        4.479898593205337e-007, 1.744168552591735e-007, 6.553134893509634e-008,
        2.376020324249398e-008, 8.313647035679894e-009, 2.807200293071416e-009,
        9.147356526491929e-010, 2.876459077365708e-010, 8.728933415057239e-011,
        2.556257202551268e-011, 7.224175781676870e-012, 1.970209678603344e-012,
        5.185336906425121e-013, 1.316988241100170e-013, 3.227953003617390e-014
}; // end table

const int firstnonzero_modulate_2400=0; // use complete table
const int lastnonzero_modulate_2400=80; // use complete table


float coeffs_table_predecimator [] = {
// generated by gnu octave fir1(12,0.5)
         7.88886896155151e-05F,  9.10376113987996e-03F, -3.05693672260121e-04F,
        -5.78542250738291e-02F,  7.59303637549332e-04F,  3.01611829936953e-01F,
         5.03901504919102e-01F,  3.01611829936953e-01F,  7.59303637549333e-04F,
        -5.78542250738291e-02F, -3.05693672260121e-04F,  9.10376113987997e-03F,
         7.88886896155151e-05F
}; 


// float coeffs_table_demodulate
// same filter as for 4800 bps modem. As we downsample the sampling-rate from 48 Khz to 24 Khz
// we can use the same filter for both filters
// that table is defined in a_dspfilter_4800.h

#else
	#if _INTMATH == 64

	int32_t coeffs_table_modulate_2400 [] = {
		0,0,0,
		0,0,0,
		0,0,1,
		6,17,51,
		140,374,962,
		2384,5703,13166,
		29330,63052,130805,
		261870,505929,943263,
		1697136,2946732,4937476,
		7983803,12458191,18760332,
		27262548,38232508,51741556,
		67575080,85167528,103586208,
		121582248,137714224,150531664,
		158787856,161639344,158787856,
		150531664,137714224,121582248,
		103586208,85167528,67575080,
		51741556,38232508,27262548,
		18760332,12458191,7983803,
		4937476,2946732,1697136,
		943263,505929,261870,
		130805,63052,29330,
		13166,5703,2384,
		962,374,140,
		51,17,6,
		1,0,0,
		0,0,0,
		0,0,0
	}; // end table
const int firstnonzero_modulate_2400=8; // element 8 = 9th element in table
const int lastnonzero_modulate_2400=72; // element 72 = 73th element in table

	int32_t coeffs_table_predecimator [] = {
	// generated by gnu octave fir1(12,0.5)
			 169412,  19550178,   -656472,
		-124241000,   1630592, 647706496,
		1082120192, 647706496,   1630592,
		-124241000,   -656472,  19550178,
			 169412
	}; 


// int32_t coeffs_table_demodulate
// same filter as for 4800 bps modem. As we downsample the sampling-rate from 48 Khz to 24 Khz
// we can use the same filter for both filters
// that table is defined in a_dspfilter_4800.h


#else
	#if _INTMATH == 3212
		// PART 1: MODULATION data: 20 bits used of 32 bits
		int32_t coeffs_table_modulate_2400 [] = {
			    0,     0,    0,      0,     0,     0,
			    0,     0,    0,      0,     0,     0,
			    0,     0,    0,      0,     1,     3,
			    7,    15,    31,    63,   123,   230,
			  414,   719,  1205,  1949,  3041,  4580,
			 6655,  9334, 12632, 16497, 20792, 25289,
			29683, 33621, 36750, 38766, 39462, 38766,
			36750, 33621, 29683, 25289, 20792, 16497,
			12632,  9334,  6655,  4580,  3041,  1949,
			 1205,   719,   414,   230,   123,    63,
			   31,    15,     7,     3,     1,     0,
			    0,     0,     0,     0,     0,     0,
			    0,     0,     0,     0,     0,     0,
			    0,     0,     0
		}; // end table

const int firstnonzero_modulate=16; // element 16 = 17th element in table
const int lastnonzero_modulate=62; // element 62 = 63th element in table

		int32_t coeffs_table_predecimator [] = {
			// generated by gnu octave fir1(12,0.5)
			    41,   4772,   -160,
			-30332,    398, 158131,
			264189, 158131,    398,
			-30332,   -160,   4772,
			    41
		}; 

// int32_t coeffs_table_demodulate [] = {
// same filter as for 4800 bps modem. As we downsample the sampling-rate from 48 Khz to 24 Khz
// we can use the same filter for both filters
// that table is defined in a_dspfilter_4800.h


#else
	#if _INTMATH == 3210

		//  MODULATION data: 22 bits used of 32 bits
		int32_t coeffs_table_modulate_2400 [] = {
			    0,       0,     0,       0,      0,      0,
			    0,       0,     0,       0,      0,      0,
			    0,       0,     0,       2,      5,     12,
			   28,     61,   127,     255,    494,    921,
			 1657,   2877,   4821,   7796,  12166,  18320,
			26623,  37336,  50528,  65991,  83171, 101158,
			118732, 134486, 147003, 155066, 157850, 155066,
			147003, 134486, 118732, 101158,  83171,  65991,
			 50528,  37336,  26623,  18320,  12166,   7796,
			  4821,   2877,   1657,    921,    494,    255,
			   127,     61,     28,     12,      5,      2,
			     0,      0,      0,      0,      0,      0,
			     0,      0,      0,      0,      0,      0,
			     0,      0,      0
		}; // end table

const int firstnonzero_modulate=15; // element 15 = 16th element in table
const int lastnonzero_modulate=64; // element 64 = 65th element in table

		int32_t coeffs_table_predecimator [] = {
		// generated by gnu octave fir1(12,0.5)
			 165,   19091,    -641, -121329,
			1592,  632525, 1056758,  632525,
			1592, -121329,    -641,   19091,
			 165
		};


// int32_t coeffs_table_demodulate [] = {
// same filter as for 4800 bps modem. As we downsample the sampling-rate from 48 Khz to 24 Khz
// we can use the same filter for both filters
// that table is defined in a_dspfilter_4800.h



#endif
#endif
#endif
#endif


const int coeffs_size_modulate_2400=82;
const int buffersize_modulate_2400=1640; // coeffs_size_modulate * 20;

const int coeffs_size_predecimator_2400to4800=13;
const int buffersize_predecimator_2400to4800=260; // coeffs_size_modulate * 20;

// some defines for the PLL (part of demodulation)

#define PLLMAX 3200 
#define PLLHALF 1600
// PLL increase = PLLMAX / 10 (as we are in fact using the 4800 bps modem after predecimation)
#define PLLINC_2400 320

// INC = 32 (not used in application itself)
//#define INC 32

// SMALL PLL Increase = PLLINC / INC
#define SMALLPLLINC 10



///////////////////////////////////////
/// function firfilter for modulation
#if _USEFLOAT == 1
float firfilter_modulate_2400(struct c2gmsk_session * sessid, float val) {
#else
	#if _INTMATH == 64
	int64_t firfilter_modulate_2400(struct c2gmsk_session * sessid, int16_t val) {
	#else
	// int32_10 and int32_12
	int32_t firfilter_modulate_2400(struct c2gmsk_session * sessid, int16_t val) {
	#endif

#endif

// static vars moved to session
//static int pointer;
//static int init=1;


#if _USEFLOAT == 1
// static vars moved to session
// static float *buffer;
float retval;
float *ptr1;
float *ptr2;
static float *p_firstnonzero_modulate;
#else
// static vars moved to session
// static int16_t *buffer;
int16_t *ptr1;
int32_t *ptr2;
static int32_t *p_firstnonzero_modulate;

	#if _INTMATH == 64
		int64_t retval;
	#else
		// int32_10 and int32_12
		int32_t retval;
	#endif

#endif

int bufferloop;
int ret;

// check if sessid really points to session
ret=checksign_sess(sessid);
if (ret != C2GMSK_RET_OK) {
	return(ret);
}; // end if


if (sessid->md_filt_init) {
	sessid->md_filt_init=0;

	// allocate memory for buffer
#if _USEFLOAT == 1
	sessid->md_filt_buffer=malloc(buffersize_modulate_2400 * sizeof(float));
#else
	sessid->md_filt_buffer=malloc(buffersize_modulate_2400 * sizeof(int16_t));
#endif

	if (!sessid->md_filt_buffer) {
		// oeps. Could not create buffer
		return(C2GMSK_RET_NOMEMORY);
	}; // end if


	// init buffer: all zero for first "coeffs_size_demodulate" elements.
#if _USEFLOAT == 1
	memset(sessid->md_filt_buffer,0,coeffs_size_modulate_2400 * sizeof(float));
#else
	memset(sessid->md_filt_buffer,0,coeffs_size_modulate_2400 * sizeof(int16_t));
#endif

	// init vars

	sessid->md_filt_pointer=coeffs_size_modulate_2400;
	p_firstnonzero_modulate=&coeffs_table_modulate_2400[firstnonzero_modulate_2400];

	// END INIT
}; // end if

// main part of function starts here
sessid->md_filt_buffer[sessid->md_filt_pointer]=val;
sessid->md_filt_pointer++;


// go throu all the elements of the coeffs table
ptr1=&sessid->md_filt_buffer[sessid->md_filt_pointer-coeffs_size_modulate_2400];
ptr2=p_firstnonzero_modulate;

retval=0;
for (bufferloop=firstnonzero_modulate_2400;bufferloop<=lastnonzero_modulate_2400;bufferloop++) {
#if _USEFLOAT == 1
	retval += (*ptr1++) * (*ptr2++);
#else
#if _INTMATH == 64
	retval += ((int64_t) *ptr1++) * ((int64_t) *ptr2++);
#else
	// int32_10 adn int32_1
	retval += (*ptr1++ * *ptr2++);
#endif
#endif
}; // end for


// If pointer has moved past of end of buffer, copy last "coeffs_size_demodulate"
// elements to the beginning of the table and move pointer down
if (sessid->md_filt_pointer >= buffersize_modulate_2400) {
#if _USEFLOAT == 1
	memmove(sessid->md_filt_buffer,&sessid->md_filt_buffer[buffersize_modulate_2400-coeffs_size_modulate_2400],coeffs_size_modulate_2400*sizeof(float));
#else
	memmove(sessid->md_filt_buffer,&sessid->md_filt_buffer[buffersize_modulate_2400-coeffs_size_modulate_2400],coeffs_size_modulate_2400*sizeof(int16_t));
#endif
	sessid->md_filt_pointer=coeffs_size_modulate_2400;
}; // end if

return(retval);

}; // end function firfilter_modulate


///////////////////////////////////////
// function firfilter_predecimator_2400to4800
// floating point math: input = float, output = float
// 64 bit integer math: input = 16 bit integer, output = 64 bit integer
// 32 bit integer math: input = 16 bit integer, output = 32 bit integer
#if _USEFLOAT == 1
	float firfilter_predecimator_2400to4800(struct c2gmsk_session * sessid, float val, int usebit) {
#else
	#if _INTMATH == 64
		int64_t firfilter_predecimator_2400to4800(struct c2gmsk_session * sessid, int16_t val, int usebit) {
	#else
		// int32_20 and int32_22
		int32_t firfilter_predecimator_2400to4800(struct c2gmsk_session * sessid, int16_t val, int usebit) {
	#endif
#endif


//int64_t retval;
#if _USEFLOAT == 1
	float retval=0;
	float *ptr1;
	float *ptr2;
#else
	int16_t *ptr1;
	int32_t *ptr2;

	#if _INTMATH == 64 
		int64_t retval=0;
	#else
		// int32_20 and int32_22
		int32_t retval=0;
	#endif
#endif

int bufferloop;

if (sessid->dd_predecfilt_init) {
	sessid->dd_predecfilt_init=0;

	// allocate memory for buffer
#if _USEFLOAT == 1
	sessid->dd_predecfilt_buffer=malloc(buffersize_predecimator_2400to4800 * sizeof(float));
#else
	sessid->dd_predecfilt_buffer=malloc(buffersize_predecimator_2400to4800 * sizeof(int16_t));
#endif

	if (!sessid->dd_predecfilt_buffer) {
		fprintf(stderr,"Error: could allocate memory for databuffer firfilter predecimator!\n");
		exit(-1);
	}; // end if


	// init buffer: all zero for first "coeffs_size_predecimator" elements.
#if _USEFLOAT == 1
	memset(sessid->dd_predecfilt_buffer,0,coeffs_size_predecimator_2400to4800 * sizeof(float));
#else
	memset(sessid->dd_predecfilt_buffer,0,coeffs_size_predecimator_2400to4800 * sizeof(int16_t));
#endif

	// init vars
	sessid->dd_predecfilt_pointer=coeffs_size_predecimator_2400to4800;

	// END INIT
}; // end if


// main part of function starts here
sessid->dd_predecfilt_buffer[sessid->dd_predecfilt_pointer]=val;
sessid->dd_predecfilt_pointer++;

// if decimation used, only use value for 1 out of 2 bits, check LSB of "usebit"
if (usebit & 0x01) {
	// go throu all the elements of the coeffs table
	ptr1=&sessid->dd_predecfilt_buffer[sessid->dd_predecfilt_pointer-coeffs_size_predecimator_2400to4800];
	ptr2=coeffs_table_predecimator;

	retval=0;
	for (bufferloop=0;bufferloop<coeffs_size_predecimator_2400to4800;bufferloop++) {
		#if _USEFLOAT == 1
			retval += (*ptr1++) * (*ptr2++);
		#else
			# if _INTMATH == 64
				retval += (((int64_t) *ptr1++) * ((int64_t) *ptr2++));
			#else
				// int32_10 and int32_12
				retval += (*ptr1++ * *ptr2++);
			#endif
		#endif
	}; // end for
}; // end if (usebit?)

// If pointer has moved past of end of buffer, copy last "coeffs_size_predecimator"
// elements to the beginning of the table and move pointer down
if (sessid->dd_predecfilt_pointer >= buffersize_predecimator_2400to4800) {
#if _USEFLOAT == 1
	memmove(sessid->dd_predecfilt_buffer,&sessid->dd_predecfilt_buffer[buffersize_predecimator_2400to4800-coeffs_size_predecimator_2400to4800],coeffs_size_predecimator_2400to4800*sizeof(float));
#else
	memmove(sessid->dd_predecfilt_buffer,&sessid->dd_predecfilt_buffer[buffersize_predecimator_2400to4800-coeffs_size_predecimator_2400to4800],coeffs_size_predecimator_2400to4800*sizeof(int16_t));
#endif
	sessid->dd_predecfilt_pointer=coeffs_size_predecimator_2400to4800;
}; // end if

// if decimation used, only use value for 1 out of 2 bits, check LSB of "usebit"
if (usebit & 0x01) {
	return(retval);
} else {
	return(0);
}; // end else - if

}; // end function firfilter_predecimator_2400to4800




///////////////////////////////////////
/// function demodulate
int demodulate_2400 (struct c2gmsk_session * sessid, int16_t audioin) {

// static vars moved to "session"
// static int init=1;
// static int last;
//static int m_pll;

#if _USEFLOAT == 1
float filterret, filterret_predec;
#else
signed long long filterret, filterret_predec;
#endif

int bit;


if (sessid->dd_dem_init) {
	sessid->dd_dem_init=0;
 
	// init vars
	sessid->dd_dem_last=0;
	sessid->dd_dem_m_pll=0;
	sessid->dd_dem_bitselect=0;

// end INIT part of function
}; // end if


// main part of function starts here

// 2400 bps demodulation:
// - decimate audio by factor 2 (reduce sampling rate)
// - use 4800 bps filter


#if _USEFLOAT == 1
// always apply predecimator filter
filterret_predec=firfilter_predecimator_2400to4800(sessid, (float) audioin, sessid->dd_dem_bitselect);
#else
	#if _INTMATH == 3210
		// reduce audio to 10 bits (down from 16)
		filterret_predec=firfilter_predecimator_2400to4800(sessid, audioin >> 6, sessid->dd_dem_bitselect);
	#else
		#if _INTMATH == 3212
			// reduce audio to 12 bits (down from 16)
			filterret_predec=firfilter_predecimator_2400to4800(sessid, audioin >> 4, sessid->dd_dem_bitselect);
		#else
			// 64 bit integer math: process audio as 16 bits
			filterret_predec=firfilter_predecimator_2400to4800(sessid, audioin, sessid->dd_dem_bitselect);
		#endif
	#endif
#endif


// apply demodulator only for 1 sample out of two
sessid->dd_dem_bitselect++;

if (sessid->dd_dem_bitselect & 0x1) {
	// not 2nd bit, return
	return(-1);
}; // end if


// after decimation, apply 4800 filter (4800 bps filter!!!)
filterret=firfilter_demodulate_4800(sessid, filterret_predec);


// audio invert: (0:no), 1:receive, (2:transmit), 3=both
if (sessid->d_audioinvert & 0x01) {
	if (filterret > 0) {
		bit=0;
	} else {
		bit=1;
	}; // end else - if
} else {
	if (filterret > 0) {
		bit=1;
	} else {
		bit=0;
	}; // end else - if
}; // end else - if


// PLL loop: used to detect points where values passes over the "zero"

if (bit != sessid->dd_dem_last) {
	if (sessid->dd_dem_m_pll < PLLHALF) {
		sessid->dd_dem_m_pll += SMALLPLLINC;
	} else {
		sessid->dd_dem_m_pll -= SMALLPLLINC;
	}; // end else - if
}; // end if

sessid->dd_dem_last=bit;

sessid->dd_dem_m_pll += PLLINC_2400;


if (sessid->dd_dem_m_pll >= PLLMAX) {
	sessid->dd_dem_m_pll -= PLLMAX;

	if (bit) {
		return(0);
	} else {
		return(1);
	}; // end if
			
} else {
	return(-1);
}; // end else - if

}; // end function demodulate


