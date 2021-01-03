/*
 * NOVINLIB - linux based library for 
 * communication with novin afzar HID devices.
 * Windows HID simplification
 *
 * iman darabi <iman.darabi@gmail.com>
 * May 12, 2013
 *
 * 
 * 
 * This program is free software; you can redistribute it and/or         
 * modify it under the terms of the GNU General Public License           
 * as published by the Free Software Foundation; either version          
 * 2 of the License, or (at your option) any later version.              
 */

#ifndef NOVIN_OP_CODE_H
#define NOVIN_OP_CODE_H

#define VID 0x03eb
#define PID 0x8ac7

#define MAX_TOKEN_SUPPORT 8

#define PWD_LEN 16
#define PAKET_SIZE 40

struct token {
        unsigned char serial[4];
        int inUse;		/* if false so -> nErr_DNF */
	int selected;		/* if false so -> nErr_DNS */
} tk[MAX_TOKEN_SUPPORT];

#define  GET_ACTIVE_TOKEN(i)					\
        do {                                                    \
		for (i=0; i<MAX_TOKEN_SUPPORT; i++)             \
			if (tk[i].inUse)                        \
				break;                          \
		if (!tk[i].inUse)                               \
			i = -nErr_DNF;                          \
        } while (0)


#define ReportID_GetSmall 0x01 //3 byte
#define ReportID_GetLarge 0x02 //18 byte

/**
 *  set data block syntax:
 *     REPORTID_SETBLK[1]+OPCODE_SETBLK[1]+ 
 *     VID[4]+PWD[16]+ADDRESS[2]+DATA[10]+LEN[1] = 33 
 *     send: 33 / recive: 1
 */
#define REPORTID_SETBLK 0X04
#define OPCODE_SETBLK 0X41
#define VID_LEN 4


/**
 * get rsa key syntax:
 *    ReportID_GetRSAKEY[0]+OpCode_GetRSAKEY[1]+
 *    PWD[16]+KeyType[1]+KeyIndex[1]      =18 / 17
 */
#define ReportID_GetRSAKEY 3
#define OpCode_GetRSAKEY 0x38

#define KT_Private  1
#define KT_Public   2
#define KT_Exponent 3

#endif	/* NOVIN_OP_CODE_H */


/**
 * get device status syntax:
 *   ReportID_GetDeviceInfo[1] + OpCode_GetDeviceInfo[1]
 */
#define ReportID_GetDeviceInfo 1
#define OpCode_GetDeviceInfo 0x3c

