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

#ifndef NOVIN_ERROR_H
#define NOVIN_ERROR_H


#define nErr_OK 0		/* normal status */
#define nErr_DNF 1		/* device not found */
#define nErr_IOE 3		/* IO ERROR; send packet(to token) error */

#define nErr_IVP 12		/* wrong VID or password */

#define nErr_DIS 13		/* device disabled */
#define nErr_BLK 14		/* device blocked */
#define nErr_OTE 16		/* general error */
#define nErr_CAT 19		/* device suspended */

#define nErr_RSANS 60		/* RSA 1024 not enabled */
#define nErr_INVKT 64		/* invalid key type (public/private sould be in range {1,2} */
#define nErr_INSGN 65		/* invalid digital signature */

#define nErr_DNS 132		/* not any device selected */

/**
 * internal code status from token.
 */
#define CS_VID          0x0001	/* 0b 0000 0000 0000 0001 */
#define CS_AdminPWD     0x0002	/* 0b 0000 0000 0000 0010 */
#define CS_DevPWD       0x0004	/* 0b 0000 0000 0000 0100 */
#define CS_UserPWD      0x0008	/* 0b 0000 0000 0000 1000 */

#define CS_AESMain      0x0010	/* 0b 0000 0000 0001 0000 */
#define CS_AESUser      0x0020	/* 0b 0000 0000 0010 0000 */
#define CS_Reseved1     0x0040	/* 0b 0000 0000 0100 0000 */
#define CS_Timer        0x0080	/* 0b 0000 0000 1000 0000 */

#define CS_Disabled     0x0100	/* 0b 0000 0001 0000 0000 */
#define CS_Suspended    0x0200	/* 0b 0000 0010 0000 0000 */
#define CS_Blocked      0x0400	/* 0b 0000 0100 0000 0000 */
#define CS_RSA1024      0x0800	/* 0b 0000 1000 0000 0000 */

#define CS_RSA512       0x1000	/* 0b 0001 0000 0000 0000 */
#define CS_RSA256       0x2000	/* 0b 0010 0000 0000 0000 */
#define CS_SubVID       0x4000	/* 0b 0100 0000 0000 0000 */
#define CS_Catastrophic 0x8000  /* 0b 1000 0000 0000 0000 */

#define CS_RSAenabled   0x3800	/* 0b 0011 1000 0000 0000 */


#define IS_DISABLED(STAT) (CS_Disabled & STAT) /* nErr_DIS */
#define IS_BLOCKED(STAT) (CS_Blocked & STAT)   /* nErr_BLK */
#define IS_SUSPEND(STAT) (CS_Suspended & STAT) /* nErr_CAT */
#define RSA1024_ENABLED(STAT) (CS_RSA1024 & STAT)

#endif  /* NOVIN_ERROR_H */
