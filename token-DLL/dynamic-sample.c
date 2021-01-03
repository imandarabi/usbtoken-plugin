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
#include <dlfcn.h>
#include "lib/novinlib.h"
#include "lib/novintools.h"


#include<openssl/rsa.h>
#include<openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>	/* ERR_get_error */
//#include <curses.h>		/* getch() */



#include <string.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/pem.h>

#include <unistd.h>


int main(int argc, char* argv[])
{
	printf("\n\t\t ********** NovinToken DLL **********\n\n");

//	unsigned char serial[4] = {91, 0, 2, 110};

//	const unsigned char pwd[16] = {51, 51, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	const unsigned char pwd[16] = {110, 117, 112, 99, 101, 102, 100, 115, 45, 56, 55, 64, 117, 109, 0, 0};

	int i;
	void *lib_handle;
	char *error;

	lib_handle = dlopen("libnovin.so", RTLD_LAZY);
	if (!lib_handle){
		fprintf(stderr, "%s\n", dlerror());
		exit(1);
	}

	/* ***** get_err_no ***** */
	int (*get_err_no_p) (void);
	get_err_no_p = dlsym(lib_handle, "get_err_no");
	if ((error = dlerror()) != NULL) {  
		fprintf(stderr, "%s\n", error);
		exit(1);
	}
	
	/* ***** get_device_count ***** */
	int (*get_device_count_p) (void);

	printf ("1.get_device_count () ...\n");
	get_device_count_p = dlsym(lib_handle, "get_device_count");
	if ((error = dlerror()) != NULL) {  
		fprintf(stderr, "%s\n", error);
		exit(1);
	}

	i = (*get_device_count_p) ();
	printf ("\tErrNo (get_device_count): %d\n", (*get_err_no_p)() );
	printf ("\tNumber of devices: %d\n\n", i);

	/* ***** get_first_device ***** */
	void (*get_first_device_p) (void);

	printf ("2.get_first_device () ...\n");
	get_first_device_p = dlsym(lib_handle, "get_first_device");
	if ((error = dlerror()) != NULL) {  
		fprintf(stderr, "%s\n", error);
		exit(1);
	}
	(*get_first_device_p) ();
	printf ("\tErrNo (get_first_device): %d \n\n", (*get_err_no_p)());

	/* ***** get_curr_dev_serial ***** */
	void (*get_curr_dev_serial_p) (unsigned char*);

	unsigned char serial[]={0,0,0,0};
	printf ("3.get_curr_dev_serial (serial) ...\n");
	get_curr_dev_serial_p = dlsym(lib_handle, "get_curr_dev_serial");
	if ((error = dlerror()) != NULL) {  
		fprintf(stderr, "%s\n", error);
		exit(1);
	}
	(*get_curr_dev_serial_p) (serial);
	printf ("\tErrNo (get_curr_dev_serial): %d \n", (*get_err_no_p)());
	printf ("\tserial No: %d, %d, %d, %d\n\n", serial[0], serial[1], serial[2], serial[3]);

	/* ***** select_device ***** */
	void (*select_device_p) (unsigned char*);

	printf ("4.select_device (serial) ...\n");
	select_device_p = dlsym(lib_handle, "select_device");
	if ((error = dlerror()) != NULL) {  
		fprintf(stderr, "%s\n", error);
		exit(1);
	}
	(*select_device_p) (serial);
	printf ("\tErrNo (select_device): %d \n\n", (*get_err_no_p)());

	/* ***** get_device_status ***** */
	int (*get_device_status_p) (void);

	printf ("5.get_device_status (serial) ...\n");
	get_device_status_p = dlsym(lib_handle, "get_device_status");
	if ((error = dlerror()) != NULL) {  
		fprintf(stderr, "%s\n", error);
		exit(1);
	}
	i = (*get_device_status_p) ();
	printf ("\tErrNo (get_device_status): %d \n", (*get_err_no_p)());
	printf ("\tstatus number : %d\n\n",i);



	/* ***** get_public_key ***** */
	char *(*get_public_key_p) (void);

	char *pbuff;
	printf ("6.get_public_key () ...\n");
	get_public_key_p = dlsym(lib_handle, "get_public_key");
	if ((error = dlerror()) != NULL) {  
		fprintf(stderr, "%s\n", error);
		exit(1);
	}
	pbuff = (*get_public_key_p) ();
	printf ("%s",pbuff);
	free (pbuff);
	printf ("\tErrNo (get_public_key): %d \n\n", (*get_err_no_p)());
	usleep(1000);
/*
	unsigned char public[128];
	unsigned char private[128];
	unsigned char exponent[128];
	memset (public, 0, 128);
	memset (private, 0, 128);
	memset (exponent, 0, 128);

	printf ("_get_public_key stat: %d\n ",
		_get_public_key (public, exponent));

	printf ("_get_private_key stat: %d\n\n ",
		_get_private_key (private, exponent,pwd));
*/


	/* ***** get_private_key ***** */
	char *(*get_private_key_p) (const unsigned char*);

	char *prbuff;
	printf ("6.get_private_key () ...\n");
	get_private_key_p = dlsym(lib_handle, "get_private_key");
	if ((error = dlerror()) != NULL) {  
		fprintf(stderr, "%s\n", error);
		exit(1);
	}
	prbuff = (*get_private_key_p) (pwd);
	printf ("%s",prbuff);
	free (prbuff);
	printf ("\tErrNo (get_private_key): %d \n\n", (*get_err_no_p)());
	usleep(1000);






	/* ***** rsa_encrypt ***** */
	int (*rsa_encrypt_p) (int, int, const unsigned char*,
			      unsigned char*, const unsigned char*);
	int len, pub=1, priv=2;
	unsigned  char to[1024];
	unsigned char data[5] = {190,1,1,3,1};

	printf ("7.rsa_encrypt (priv, sizeof(data), data, to, pwd) ...\n");
	rsa_encrypt_p = dlsym(lib_handle, "rsa_encrypt");
	if ((error = dlerror()) != NULL) {  
		fprintf(stderr, "%s\n", error);
		exit(1);
	}
	printf ("\t data to encrypt: ");
	for (i=0; i<sizeof(data); i++)
		printf ("%d, ",data[i]);
	printf ("\n");
	len =  (*rsa_encrypt_p) (priv, sizeof(data), data,to ,pwd);

	printf ("\tErrNo (rsa_encrypt): %d \n", (*get_err_no_p)());
        printf ("\ttlen is :%d \n\n",len);
 

	/* ***** rsa_decrypt ***** */
	unsigned char data2[1024];
	int (*rsa_decrypt_p) (int, int, const unsigned char*,
			      unsigned char*, const unsigned char*);

	printf ("8.rsa_decrypt (pub, len, to, data2, pwd) ...\n");
	rsa_decrypt_p = dlsym(lib_handle, "rsa_decrypt");
	if ((error = dlerror()) != NULL) {  
		fprintf(stderr, "%s\n", error);
		exit(1);
	}
	int len2 = (*rsa_decrypt_p) (pub, len, to, data2 ,pwd);
	printf ("\tErrNo (rsa_decrypt): %d \n", (*get_err_no_p)());
	if (len2 != -1)
	{
		printf ("\tdata : ");
		for (i=0; i<len2; i++)
			printf ("%d, ",data2[i]);
	printf ("\n\n");
	}


	/* ***** get_signature ***** */
	int (*get_signature_p) (int, const unsigned char*, unsigned int,
				unsigned char*, unsigned int*, 
				const unsigned char*);

	unsigned char sigret[1024];
	unsigned int siglen;
	printf ("9.get_signature (NID_md5,(const unsigned char*)data, sizeof(data), sigret, &siglen, pwd) ...\n");

	get_signature_p = dlsym(lib_handle, "get_signature");
	if ((error = dlerror()) != NULL) {  
		fprintf(stderr, "%s\n", error);
		exit(1);
	}
	(*get_signature_p) (NID_md5,
			    (const unsigned char*)data, sizeof(data),
			    sigret, &siglen, pwd); 

	printf ("\tErrNo (get_signature): %d \n\n", (*get_err_no_p)());



	/* ***** verify_signature ***** */
	int (*verify_signature_p) (int, const unsigned char*, unsigned int,
				unsigned char*, unsigned int);

	printf ("10.verify_signature (NID_md5, data, sizeof(data), sigret, siglen) ...\n");

	verify_signature_p = dlsym(lib_handle, "verify_signature");
	if ((error = dlerror()) != NULL) {  
		fprintf(stderr, "%s\n", error);
		exit(1);
	}

	(*verify_signature_p) (NID_md5, data, sizeof(data), 
			       sigret, siglen);

	printf ("\tErrNo (verify_signature): %d \n\n", (*get_err_no_p)());

	return 0;
}

