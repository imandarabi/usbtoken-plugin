/*
 * NOVINLIB - linux based library for                                                                                                                                         
 * communication with novin afzar HID devices.
 *
 * iman darabi <iman.darabi@gmail.com>
 * May 12, 2013
 *
 * Copyright 2009, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */


#ifndef NOVINLIB_H
#define NOVINLIB_H

#include <stdbool.h>

//#define VID 0x03eb
//#define PID 0x8ac7

void init (void);

int get_err_no (void);

void get_first_device (void);

void get_curr_dev_serial (unsigned char *serial);

void select_device (const unsigned char *serial);

void get_serial (unsigned char *serial);

int get_device_count (void);

int rsa_encrypt (int key_type, int flen, const unsigned char *from,
		 unsigned char *to, const unsigned char *supwd);

int rsa_decrypt (int key_type, int flen, const unsigned char *from,
		 unsigned char *to, const unsigned char *supwd);

int get_signature (int type, const unsigned char *m, unsigned int m_len,
		   unsigned char *sigret, unsigned int *siglen, const unsigned char *sUPWD);

int verify_signature (int type, const unsigned char *m, unsigned int m_len,
                      unsigned char *sigbuf, unsigned int siglen);

char *get_public_key (void);

char *get_private_key (const unsigned char *sUPWD);

int get_device_status (void);

#endif  /* NOVINLIB_H */
