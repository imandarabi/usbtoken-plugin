/*
 * NOVINLIB - linux based library for 
 * communication with novin afzar HID devices.
 * 
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

#include <stddef.h>
#include <wchar.h>		/* wchar_t, wcslen*/
#include <string.h>		/* memcpy */
#include <stdio.h>		/* fopen */
#include <stdlib.h>		/* wcstombs */
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

#include "hidapi.h"
#include "novintools.h"
#include "novin-error.h"
#include "novin-op-code.h"

const size_t SecretIndex[64] = {1, 3, 1, 6, 2, 3, 4, 4, 5, 6, 7, 8, 9, 8, 7, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 7, 9, 1, 11, 2, 3, 4, 11, 5, 6, 7, 8, 9, 0, 1, 11, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 14};

#define JUST_FOR_INDEX_COMPATIBILITY 0
const size_t ReportLength[5] = {JUST_FOR_INDEX_COMPATIBILITY, 3, 18, 27, 39};



int _cnv_hex_to_decimal_serial (unsigned char *dest, const wchar_t *src)
{
        char ser[16];
        int tmp;
        int i;

        tmp = wcstombs (ser, src, wcslen(src));

        // change hexadecimal to decimal
	for (i=0; i<= wcslen(src); i++)
        {
                tmp = 0;

                if ('0' <= ser[i] && ser[i] <= '9'){
                        tmp = tmp * 16;
                        tmp = tmp + (ser[i] - '0');
                }
                else if ('A' <= ser[i] && ser[i] <= 'F'){
                        tmp = tmp * 16;
                        tmp = tmp + (ser[i] - 'A') + 10;
                }
                else if ('a'<= ser[i] && ser[i] <= 'f') {
                        tmp = tmp * 16;
                        tmp = tmp + (ser[i] - 'a') + 10;
                }
                else{
                        tmp = 0;
                        break;
                }
                ser[i] = tmp;
        }

        // convert tow byte decimal to one byte decimal
	for (i=0; i<4; i++)
                dest[i] = 16 * ser[2*i] + ser[2*i+1];

        return nErr_OK;
}

/**
 * checks if token since when get_first_device() is still connected or not.
 */
inline int _check_dev_connected (const unsigned char *serial)
{
	struct hid_device_info *devs, *cur_dev;
	int err = nErr_DNF;
	unsigned char tmp[4];

	devs = hid_enumerate (VID, PID);
	if (!devs)
		goto out;

        cur_dev = devs;
        while (cur_dev) {
		_cnv_hex_to_decimal_serial (tmp, cur_dev->serial_number);
		if (!memcmp(tmp, serial, 4)){
			err = nErr_OK;
			goto out;
		}
 
                cur_dev = cur_dev->next;
        }
	
out:
	hid_free_enumeration (devs);
	return err;
}

inline int _check_dev_selected (const unsigned char *serial)
{
	int i;
	int err = nErr_DNS;

	for (i=0; i<MAX_TOKEN_SUPPORT; i++)
		if ((tk[i].selected) && 
		    (!memcmp (serial, tk[i].serial, 4))){
			err = nErr_OK;
			goto out;
		}

out:
	return err;
}

int _encrypt_packet (unsigned char *buff, const unsigned char *serial)
{
	unsigned char tmp;
	int i, j;

	tmp = serial[2] ^ serial[3];
	buff[0] = buff[0] ^ (tmp ^ 0XFF);
	
	tmp = serial[0] ^ serial[1];
	buff[1] = buff[1] ^ tmp;

	/*
	 *   in delphi source we see sth like this : j = (SecretIndex[0]-1)*2+1;
	 *   but here in c -1 is subtracted from that, this is because of serial length;
	 *   in c source each byte of serial buffer is two byte long in delphi source.
	 *   delphi ->  serial[1,2]  serial[3,4]  serial[5,6]  serial[7,8]
	 *   c      ->  serial[ 0 ]  serial[ 1 ]  serial[ 2 ]  serial[ 3 ]
	 */
	for (i=0; i<12; i++)
	{
		switch (i) {
		case 0:
			j = (SecretIndex[0]-1)*2+1 -1;
			tmp = serial[j] ^ 0XFF;
			break;
		case 1:
			j = (SecretIndex[1]-2)*2+1 -2;
			tmp = serial[j] ^ 0XFF;
			break;
		case 2:
			j = (SecretIndex[3]-3)*2+1 -4;
			tmp = serial[j] ^ 0XFF;
			break;
		case 3:
			j = (SecretIndex[7]-4)*2+1 -1;
			tmp = serial[j] ^ 0XFF;
			break;
		case 4:
			j = (SecretIndex[15]-5)*2+1 -3;
			tmp = serial[j] ^ 0XFF;
			break;
		case 5:
			j = (SecretIndex[31]-6)*2+1 -4;
			tmp = serial[j] ^ 0XFF;
			break;
		case 6:
			j = (SecretIndex[32]-7)*2+1 -1;
			tmp = serial[j] ^ 0XFF;
			break;
		case 7:
			j = (SecretIndex[33]-8)*2+1 -2;
			tmp = serial[j] ^ 0XFF;
			break;
		case 8:
			j = (SecretIndex[35]-9)*2+1 -3;
			tmp = serial[j] ^ 0XFF;
			break;
		case 9:
			j = (SecretIndex[39]-10)*2+1 -2;
			tmp = serial[j] ^ 0XFF;
			break;
		case 10:
			j = (SecretIndex[47]-11)*2+1 -1;
			tmp = serial[j] ^ 0XFF;
			break;
		case 11:
			j = (SecretIndex[63]-12)*2+1 -3;
			tmp = serial[j] ^ 0XFF;
			break;
		}
		buff [(i+1)*3+0-1] = buff[(i+1)*3+0-1] ^ buff[(i+1)*3+1-1];
		buff [(i+1)*3+1-1] = buff[(i+1)*3+1-1] ^ buff[(i+1)*3+2-1];
		buff [(i+1)*3+2-1] = buff[(i+1)*3+2-1] ^ tmp;
	}
	
	return nErr_OK;
}

int _decrypt_packet (unsigned char *buff, const unsigned char *serial)
{
	unsigned char tmp;
	int i, j;

	tmp = serial[0] ^ serial[2];
	buff[0] = buff[0] ^ (tmp ^ 0XFF);
	
	tmp = serial[1] ^ serial[3];
	buff[1] = buff[1] ^ (tmp ^ 0XFF);

	/*
	 *   in delphi source we see sth like this : j = (SecretIndex[0]-1)*2+1;
	 *   but here in c -1 is subtracted from that, this is because of serial length;
	 *   in c source each byte of serial buffer is two byte long in delphi source.
	 *   delphi ->  serial[1,2]  serial[3,4]  serial[5,6]  serial[7,8]
	 *   c      ->  serial[ 0 ]  serial[ 1 ]  serial[ 2 ]  serial[ 3 ]
	 */
	for (i=0; i<5; i++)
	{
		switch (i) {
		case 0:
			j = (SecretIndex[0]-1)*2+1 -1;
			tmp = serial[j] ^ 0XFF;
			break;
		case 1:
			j = (SecretIndex[1]-2)*2+1 -2;
			tmp = serial[j] ^ 0XFF;
			break;
		case 2:
			j = (SecretIndex[3]-3)*2+1 -4;
			tmp = serial[j] ^ 0XFF;
			break;
		case 3:
			j = (SecretIndex[7]-4)*2+1 -1;
			tmp = serial[j] ^ 0XFF;
			break;
		case 4:
			j = (SecretIndex[15]-5)*2+1 -3;
			tmp = serial[j] ^ 0XFF;
			break;
		}
		buff [(i+1)*3+2-1] = buff[(i+1)*3+2-1] ^ tmp;
		buff [(i+1)*3+1-1] = buff[(i+1)*3+1-1] ^ buff[(i+1)*3+2-1];
		buff [(i+1)*3+0-1] = buff[(i+1)*3+0-1] ^ buff[(i+1)*3+1-1];
	}
	return nErr_OK;
}

/**
 * pay attention : function returns negative as error.
 */
int _send_packet (unsigned char *result, const size_t result_reportid_index,
		  unsigned char *packet, const size_t packet_reportid_index)
{
	int res, tkNo;
	hid_device *handle;

	GET_ACTIVE_TOKEN (tkNo);

	handle = hid_open(VID, PID, NULL);
	if (!handle){ 
		res = -nErr_DNF;
		goto out;
	}

	if (!tk[tkNo].selected){
		res = -nErr_DNS;
		goto out;
	}

	// WARNING: first byte of buff is REPORT ID and shouldn't be encrypted. 
	_encrypt_packet (packet+1, tk[tkNo].serial);

	res = hid_send_feature_report (handle, packet, ReportLength[packet_reportid_index]);
	if (res < 0){
		res = -nErr_IOE;
		goto out;
	}

	result[0] = result_reportid_index; 
	res = hid_get_feature_report (handle, result, ReportLength[result_reportid_index]);

	if (res < 0){ 
		res = -nErr_IOE;
		goto out;
	}

	/* WARNING: first byte of buff is REPORT ID and should not be decrypted. */ 
	_decrypt_packet (result+1, tk[tkNo].serial); 

	res = result[1];

	/* true return status of packet is 128 instead of 0 (so changed it)...
	 * why??? ask it's HW developer ... . 
	 */
	if (res == 128)
		res = nErr_OK;
	else
		res *= -1;	// retun types in this function should be less than zero.

	/* don't need ReportID_ and nErr_CODE any more... . */

	//	printf ("result_reportid_index:%d\n", result_reportid_index);
	//	printf ("ReportLength[result_reportid_index]:%d\n", ReportLength[result_reportid_index]);

	memmove (result, result+2, ReportLength[result_reportid_index] );

//	memcpy (result, result+2, ReportLength[result_reportid_index] );

out:
	hid_close (handle);
	hid_exit();        /* Free static HIDAPI objects. */
	
	return res;
}

int _get_public_key (unsigned char *public, unsigned char *exponent)
{
	int res=nErr_OK;
	int key_type, key_index;
	unsigned char packet[128], result[128];
	
	for (key_type=KT_Public; key_type<=KT_Exponent; key_type++){

		for (key_index=0; key_index<8;  key_index++){
			
			packet[0] = ReportID_GetRSAKEY;
			packet[1] = OpCode_GetRSAKEY;
			packet[2+PWD_LEN] = key_type;
			packet[3+PWD_LEN] =  key_index;
			
			res =  _send_packet (result+16*key_index, ReportID_GetLarge, packet, ReportID_GetRSAKEY);
			if (res != nErr_OK){
				/* negative should be returned as error to  
				 * Differentiate it from result.
				 */
				res *=-1;
				goto out;
			}
		}
		
		if (key_type == KT_Public)
			memcpy (public, result, 128);
		
		if (key_type == KT_Exponent)
			memcpy (exponent, result, 128);
	}
out:
	return res;
}

int _get_private_key (unsigned char *private, unsigned char *exponent, const unsigned char *pwd)
{
	int res=nErr_OK, key_type, key_index;
	unsigned char packet[128], result[128];
	
	for (key_type=KT_Private; key_type<=KT_Exponent; key_type +=2){ 
		/* +=2 is used to jump over KT_Public */

		for (key_index=0; key_index<8;  key_index++){
			
			packet[0] = ReportID_GetRSAKEY;
			packet[1] = OpCode_GetRSAKEY;
			memcpy (packet+2, pwd, PWD_LEN);
			packet[2+PWD_LEN] = key_type;
			packet[3+PWD_LEN] =  key_index;
			
			res =  _send_packet (result+ 16*key_index, ReportID_GetLarge, packet, ReportID_GetRSAKEY);
			if (res != nErr_OK){
				/* negative should be returned as error to  
				 * Differentiate it from result.
				 */
				res *=-1;
				goto out;
			}
		}
				
		if (key_type == KT_Private)
			memcpy (private, result, 128);
		
		if (key_type == KT_Exponent)
			memcpy (exponent, result, 128);
	}

out:
	return res;
}

int _get_dev_status_internal (void)
{
	int res=nErr_OK;
	unsigned char packet[128], result[128];

	packet[0] = ReportID_GetDeviceInfo;
	packet[1] = OpCode_GetDeviceInfo;
	memset (result, 0 , 128);

	res =  _send_packet (result, ReportID_GetLarge, packet, ReportID_GetDeviceInfo);
	if (res != nErr_OK){
		/* negative should be returned as error to  
		 * Differentiate it from result.
		 */
		res *=-1;
		goto out;
	}

	res = result[1]*256 + result[0];

out:
	return res;
}
