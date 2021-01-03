/*******************************************************
 Windows HID simplification

 Alan Ott
 Signal 11 Software

 8/22/2009

 Copyright 2009, All Rights Reserved.
 
 This contents of this file may be used by anyone
 for any reason without any conditions and may be
 used as a starting point for your own applications
 which use HIDAPI.
********************************************************/

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "lib/novinlib.h"
#include "lib/novintools.h"


#include<openssl/rsa.h>
#include<openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>	/* ERR_get_error */
#include <curses.h>		/* getch() */



#include <string.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/pem.h>


int main(int argc, char* argv[])
{
	printf("\n\t\t ********** NovinToken DLL **********\n\n");

//	unsigned char serial[4] = {91, 0, 2, 110};

	const unsigned char pwd[16] = {51, 51, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	int i;

	printf ("1.get_device_count () ...\n");
	i = get_device_count();
	printf ("\tErrNo (get_device_count): %d \n", get_err_no());
	printf ("\tNumber of devices: %d\n\n",i);

	printf ("2.get_first_device () ...\n");
	get_first_device ();
	printf ("\tErrNo (get_first_device): %d \n\n", get_err_no());
	
	unsigned char serial[]={0,0,0,0};
	printf ("3.get_curr_dev_serial (tmp) ...\n");
	get_curr_dev_serial(serial);
	printf ("\tErrNo (get_curr_dev_serial): %d\n", get_err_no());
	printf ("\tserial No: %d, %d, %d, %d\n\n", serial[0], serial[1], serial[2], serial[3]);

	printf ("4.select_device (serial) ...\n");
	select_device (serial);
	printf ("\tErrNo (select_device): %d\n\n", get_err_no());

	printf ("5.get_device_status () ...\n");
	i = get_device_status ();
	printf ("\tErrNo (get_dev_status): %d\n", get_err_no());
	printf ("\t status number : %d\n\n",i);

	char *pbuff;
	printf ("6. get_public_key () ...\n");
	pbuff = get_public_key ();
	printf ("%s",pbuff);
	free (pbuff);
	printf ("\tErrNo (get_dev_status): %d\n\n", get_err_no());

	unsigned char public[128];
	unsigned char private[128];
	unsigned char exponent[128];

	memset (public, 0, 128);
	memset (private, 0, 128);
	memset (exponent, 0, 128);

/*	printf ("_get_public_key stat: %d\n ",
		_get_public_key (public, exponent));

	printf ("_get_private_key stat: %d\n\n ",
		_get_private_key (private, exponent,pwd));
*/

	int len, pub=1, priv=2;
	unsigned  char to[1024];
	unsigned char data[5] = {190,1,1,3,1};

	printf ("7.rsa_encrypt (priv, sizeof(data), data, to, pwd) ...\n");
	printf ("\t data to encrypt: ");
	for (i=0; i<sizeof(data); i++)
		printf ("%d, ",data[i]);
	printf ("\n");
	len =  rsa_encrypt (priv,sizeof(data), data,to ,pwd);
	printf ("\tErrNo (rsa_encrypt): %d\n", get_err_no());
	printf ("\ttlen is :%d \n\n",len);
	//if (len != -1)
	//	for (i=0; i<len; i++)
	//		printf ("%d, ",to[i]);
	//printf ("\n\n");


	unsigned char data2[1024];
	printf ("8.rsa_decrypt (pub, len, to, data2, pwd) ...\n");
	int len2 =  rsa_decrypt (pub, len, to, data2 ,pwd);
	printf ("\tErrNo (rsa_decrypt): %d\n", get_err_no());
	if (len2 != -1)
	{
		printf ("\tdata : ");
		for (i=0; i<len2; i++)
			printf ("%d, ",data2[i]);
	printf ("\n\n");
	}


	unsigned char sigret[1024];
	unsigned int siglen;
	printf ("9.get_signature (NID_md5, (const unsigned char*)data, sizeof(data), sigret, &siglen, pwd) ...\n");
	get_signature (NID_md5, (const unsigned char*)data, sizeof(data), sigret, &siglen, pwd); 
	printf ("\tErrNo (get_signature): %d\n\n", get_err_no());
	


	printf ("10.verify_signature (NID_md5, data, sizeof(data), sigret, siglen) ...\n");
	verify_signature (NID_md5, data, sizeof(data), sigret, siglen);
	printf ("\tErrNo (verify_signature): %d\n\n", get_err_no());

	return 0;
}
