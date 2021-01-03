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
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hidapi.h"

#include "novinlib.h"
#include "novin-error.h"
#include "novin-op-code.h"
#include "novintools.h"

#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>	/* PEM_write_RSAPublicKey */
#include <openssl/sha.h>        /* SHA1() */

int LAST_ERR_NO = nErr_OK;
#define SET_ERR_NO(i)	LAST_ERR_NO = i;

void init (void) { /* DO NOTHING! */ }

int get_err_no (void)
{
	
	return LAST_ERR_NO;
}

/**
 * this function gets all hid devices with VID:0x03eb and PID:0x8ac7,
 * add them to tk struct, and active first device as inUse device.
 */

void get_first_device(void)
{
#define FIRST_DEV 0
	struct hid_device_info *devs, *cur_dev;
	int i=0;
	int err = nErr_OK;

	memset (tk,0,sizeof(tk));

	devs = hid_enumerate (VID, PID);
	if (!devs){
		err = nErr_DNF;
		goto out;
	}
        cur_dev = devs;
        while (cur_dev) {
		_cnv_hex_to_decimal_serial (tk[i++].serial,
					    cur_dev->serial_number);
                cur_dev = cur_dev->next;
        }

	/*
	 * at this moment I'm sure there is at least one connected device
	 * because hid enumeration with VID,PID has been succeeded .
	 */
	tk[FIRST_DEV].inUse = true;
	

out:
        hid_free_enumeration (devs);
	SET_ERR_NO (err);
}

/**
 * this function should be called after get_first_device().
 * @serial : return serial of currently activated token.
 */
void get_curr_dev_serial (unsigned char *serial)
{
	int err = nErr_OK;
	int tkNo;

	GET_ACTIVE_TOKEN (tkNo);

	if (tkNo == -nErr_DNF){
		err = nErr_DNF;
		goto out;
	}

	if (_check_dev_connected (tk[tkNo].serial) == nErr_DNF){
		err = nErr_DNF;
		goto out;
	}

	memcpy (serial, tk[tkNo].serial, 4);	

out:
	SET_ERR_NO (err);
}

/**
 * select_device(), selects currently activated device as selected device to work with.
 * @serial : serial number of currently active token which is get by get_curr_dev_serial()
 */
void select_device (const unsigned char *serial)
{
	int err = nErr_OK;
	int tkNo;

	/* get active token of when get_first_device() called */
	GET_ACTIVE_TOKEN (tkNo);
	if (tkNo == -nErr_DNF){
		err = nErr_DNF;
		goto out;
	}

	/* check weather token is still connected or not */
	if (_check_dev_connected (serial) == nErr_DNF){
		err = nErr_DNF;
		goto out;
	}
	
	tk[tkNo].selected = true;
	
out:
	SET_ERR_NO (err);
}

/**
 * this function return serial number of currently selected token. 
 * @serial : serial number of selected serial; 
 * 
 * note : this function only works after running select_device() fucntion.
 */
void get_serial (unsigned char *serial)
{
	int err = nErr_OK;
	int tkNo;

	memset (serial, 0, 4);

	GET_ACTIVE_TOKEN (tkNo);

	if (tkNo == -nErr_DNF){
		err = nErr_DNF;
		goto out;
	}

	if (_check_dev_connected (tk[tkNo].serial) == nErr_DNF){
		err = nErr_DNF;
		goto out;
	}
	
	if (_check_dev_selected (tk[tkNo].serial) == nErr_DNS ){
		err = nErr_DNS;
		goto out;
	}

	memcpy (serial, tk[tkNo].serial, 4);
		
out:
	SET_ERR_NO (err);
}

/**
 * return number of currently connected hid token (with VID:0x03eb and PID:0x8ac7)
 */
int get_device_count (void)
{
	struct hid_device_info *devs, *cur_dev;
	int total=0;
	int err = nErr_OK;

	devs = hid_enumerate (VID, PID);
	if (!devs)
		/* although no device found 
		 * but nothing wrong happend ... */
		//err = nErr_DNF;
		goto out;
	

        cur_dev = devs;
        while (cur_dev) {
		total++;
                cur_dev = cur_dev->next;
        }

out:
        hid_free_enumeration(devs);
	SET_ERR_NO (err);
	return total;
}

/**
 *  rsa_encrypt() encrypts the @flen bytes at @from using the public key 
 * (from usb token) rsa and stores the ciphertext in @to.
 *  @to must point to RSA_size(rsa) bytes of memory.
 *  
 *  On error  "nErr_INVKT" is returned;
 */
int rsa_encrypt (int key_type, int flen, const unsigned char *from,
			    unsigned char *to, const unsigned char *supwd)
{ 
	unsigned char public[256];
	unsigned char private[256];
	unsigned char exponent[256];

/*	unsigned char public[128];
	unsigned char private[128];
	unsigned char exponent[128];*/

	memset (public, 0, 256);
	memset (private, 0, 256);
	memset (exponent, 0, 256);

        int err;
	int tlen=-1;
	int stat;

	err =  get_device_status ();
	if (err != nErr_OK)
		goto out;

	stat = _get_dev_status_internal ();
	if (stat < 0){
		err = stat;
		goto out; 
	}

	if (!RSA1024_ENABLED(stat))
	{
		err = nErr_RSANS;
		goto out;
	}

	RSA* rsa = RSA_new();
	if (rsa== NULL){
		err = nErr_OTE;
		goto out;
	}
	

	switch (key_type) {
	case 1:			/* encrypt with public key */
		err = _get_public_key (public, exponent);
		if (err != nErr_OK)
			goto memfree_out;

		rsa->n = BN_bin2bn (exponent, 128, NULL);
		rsa->e = BN_bin2bn (public, 128, NULL);

		if (!rsa->n || !rsa->e){
			err = nErr_OTE;
			goto memfree_out;
		}

		tlen = RSA_public_encrypt(flen, from, to, rsa,
					 RSA_PKCS1_PADDING);
		if (tlen == -1)
		  err = nErr_INVKT;
		
		break;
		
	case 2:			/* encrypt with private key */
		err = _get_public_key (public, exponent);
		if (err != nErr_OK)
			goto memfree_out;

		err = _get_private_key (private, exponent, supwd);
		if (err != nErr_OK)
			goto memfree_out;

		rsa->e = BN_bin2bn (public, 128, NULL);
		rsa->n = BN_bin2bn (exponent, 128, NULL);
		rsa->d = BN_bin2bn (private, 128, NULL);

		if (!rsa->n || !rsa->d || !rsa->e){
			err = nErr_OTE;
			goto memfree_out;
		}

		tlen = RSA_private_encrypt(flen, from, to, rsa,
					   RSA_PKCS1_PADDING);

		if (tlen == -1)
		  err = nErr_INVKT;
		
		break;
		
	default:
		err = nErr_INVKT;
		tlen = -1;
		break;
	}
	
memfree_out:
	RSA_free(rsa);

out:
	SET_ERR_NO(err);
	return tlen;
}

/**
 *  rsa_decrypt() recovers the message digest from the @flen bytes long signature at @from using the 
 *  signer's public/private key rsa. @to must point to a memory section large enough to hold 
 *  the message digest (which is smaller than RSA_size(rsa) - 11). padding is the padding mode that was used to
 *  sign the data.
 *  @key_type: 1 is used to decrypt with public key and 2 is used to decrypt with private key.
 *  on error -1 is retured otherwise length of decrypted is returned.
 */
int rsa_decrypt (int key_type, int flen, const unsigned char *from,
		 unsigned char *to, const unsigned char *supwd)
{
/*	unsigned char public[128];
	unsigned char private[128];
	unsigned char exponent[128];*/

	unsigned char public[256];
	unsigned char private[256];
	unsigned char exponent[256];

	memset (public, 0, 256);
	memset (private, 0, 256);
	memset (exponent, 0, 256);

        int err;
	int tlen=-1;
	int stat;

	err =  get_device_status ();
	if (err != nErr_OK)
		goto out;

        stat = _get_dev_status_internal ();
	if (stat < 0){
		err = stat;
		goto out; 
	}

	if (!RSA1024_ENABLED(stat))
	{
		err = nErr_RSANS;
		goto out;
	}
	
	RSA* rsa = RSA_new();
	if (rsa== NULL){
		err = nErr_OTE;
		goto out;
	}
	
	
	switch (key_type) {
	case 1:			/* decrypt with public key */
		err = _get_public_key (public, exponent);
		if (err != nErr_OK)
			goto memfree_out;
		
		rsa->n = BN_bin2bn (exponent, 128, NULL);
		rsa->e = BN_bin2bn (public, 128, NULL);
		if (!rsa->n || !rsa->e){
			err = nErr_OTE;
			goto memfree_out;
		}

		tlen = RSA_public_decrypt(flen, from, to, rsa,
					  RSA_PKCS1_PADDING);
		if (tlen == -1)
		  err = nErr_INVKT;
		
		break;
		
	case 2:			/* decrypt with private key */
		err = _get_public_key (public, exponent);
		if (err != nErr_OK)
			goto memfree_out;

		err = _get_private_key (private, exponent, supwd);
		if (err != nErr_OK)
			goto memfree_out;
		
		rsa->e = BN_bin2bn (public, 128, NULL);
		rsa->n = BN_bin2bn (exponent, 128, NULL);
		rsa->d = BN_bin2bn (private, 128, NULL);
		if (!rsa->n || !rsa->d || !rsa->e){
			err = nErr_OTE;
			goto memfree_out;
		}

		tlen = RSA_private_decrypt(flen, from, to, rsa,
					   RSA_PKCS1_PADDING);
		if (tlen == -1)
		  err = nErr_INVKT;
		
		break;
		
	default:
		err = nErr_INVKT;
		tlen = -1;
		break;
	}

memfree_out:
	RSA_free(rsa);

out:
	SET_ERR_NO(err);
	return tlen;
}

/**
 * get_signature() signs the message digest @m of size @m_len using the private key rsa,
 * and gets it from token via @sUPWD password. It stoerr the signature in @sigret and 
 * the signature size in @siglen. sigret must point to RSA_size(rsa) bytes of memory.
 *
 * @type: type denotes the message digest algorithm that was used to generate m.
 * It usually is one of NID_sha1, NID_ripemd160 and NID_md5; If type is NID_md5_sha1, 
 * an SSL signature (MD5 and SHA1 message digests with PKCS #1 padding and 
 * no algorithm identifier) is created.
 */
int get_signature (int type, const unsigned char *m, unsigned int m_len,
		    unsigned char *sigret, unsigned int *siglen, 
		    const unsigned char *sUPWD)
{
	unsigned char public[128];
	unsigned char private[128];
	unsigned char exponent[128];
	unsigned char message_digest[SHA_DIGEST_LENGTH];


	int err;

	err =  get_device_status ();
	if (err != nErr_OK)
		goto out;

	err =  _get_public_key (public, exponent);
	if (err != nErr_OK)
		goto out;
	
	err = _get_private_key (private, exponent, sUPWD);
	if (err != nErr_OK)
		goto out;
	
	RSA* rsa = RSA_new();
	if (rsa== NULL){
		err = nErr_OTE;
		goto out;
	}
	
	rsa->e = BN_new();
	if (rsa->e == NULL){
		err = nErr_OTE;
		goto memfree_out;
	}
	
	rsa->n = BN_new();
	if (rsa->n == NULL){
		err = nErr_OTE;
		goto memfree_out;
	}
	
	rsa->d = BN_new();
	if (rsa->d == NULL){
		err = nErr_OTE;
		goto memfree_out;
	}
	
	rsa->e = BN_bin2bn (public, 128, NULL);
	rsa->n = BN_bin2bn (exponent, 128, NULL);
	rsa->d = BN_bin2bn (private, 128, NULL);
	if (!rsa->n || !rsa->d || !rsa->e){
		err = nErr_OTE;
		goto memfree_out;
	}

	 /* PHP openssl_sign computes a signature for the specified data by using SHA1 for hashing */
	SHA1 (m, m_len, message_digest);                                                     

	if (RSA_sign(type, message_digest, SHA_DIGEST_LENGTH, sigret, siglen, rsa) != 1){
		err=nErr_OTE;
		goto memfree_out;
	}
	
memfree_out:
	RSA_free(rsa);
	
out:
	SET_ERR_NO(err);
	return err;
}

/**
 *  verify_signature() verifies that the signature @sigbuf of size @siglen matches a given message digest @m of size @m_len.
 *  type denotes the message digest algorithm that was used to generate the signature.
 *  rsa is the signer's public key.
 *
 * @type: type denotes the message digest algorithm that was used to generate m.
 * It usually is one of NID_sha1, NID_ripemd160 and NID_md5; If type is NID_md5_sha1, 
 * an SSL signature (MD5 and SHA1 message digests with PKCS #1 padding and 
 * no algorithm identifier) is created.
 */
int verify_signature (int type, const unsigned char *m, unsigned int m_len,
		      unsigned char *sigbuf, unsigned int siglen)
{
	unsigned char public[128];
	unsigned char exponent[128];
	unsigned char message_digest[SHA_DIGEST_LENGTH];

	int err;

	err =  get_device_status ();
	if (err != nErr_OK)
		goto out;
		
	 err = _get_public_key (public, exponent);
	 if (err != nErr_OK)
		 goto out;

	 RSA *rsa = RSA_new();
	 if (rsa== NULL){
		 err = nErr_OTE;
		 goto out;
	 }

	 rsa->e = BN_bin2bn (public, 128, NULL);
	 rsa->n = BN_bin2bn (exponent, 128, NULL);
	 if (!rsa->n || !rsa->e){
		 err = nErr_OTE;
		 goto memfree_out;
	 }
	 
	 /* PHP openssl_sign computes a signature for the specified data by using SHA1 for hashing */
	SHA1 (m, m_len, message_digest);                                                     
	 if (RSA_verify(type, message_digest, SHA_DIGEST_LENGTH, sigbuf, siglen, rsa) != 1){
		 err = nErr_INSGN;
		 goto memfree_out;
	 }

memfree_out:
	RSA_free(rsa);

out:
	 SET_ERR_NO(err);
	 return err;
}

char *get_public_key (void)
{
	const char *ftmp="/tmp/public.txt";
//	unsigned char public[128]; 
//	unsigned char exponent[128];
	unsigned char public[256];
	unsigned char exponent[256];
	FILE *fp;
	size_t fz;
	int err;

	memset (public, 0, 256);
	memset (exponent, 0, 256);

	err = _get_public_key (public, exponent);
	if (err != nErr_OK)
		goto out;

	RSA *rsa = RSA_new();
	if (rsa== NULL){
		err = nErr_OTE;
		goto out;
	}
	

	rsa->e = BN_bin2bn (public, 128, NULL);
	rsa->n = BN_bin2bn (exponent, 128, NULL);
	if (!rsa->n || !rsa->e){
		err = nErr_OTE;
		goto memfree_out;
	}

	fp = fopen(ftmp, "wr");
	if(!PEM_write_RSAPublicKey(fp, rsa))
	{
		err = nErr_OTE;
		goto memfree_out;
	}
	fflush(fp);
	fclose(fp);

	fp = fopen (ftmp,"r");

	fseek(fp, 0L, SEEK_END);
	fz = ftell(fp);

	char *buff = (char *)malloc(sizeof(char)*(fz+1)); /* 1 for null */

	fseek(fp, 0L, SEEK_SET);

	if (fz != fread (buff, 1, fz, fp))
		err = nErr_OTE;
	 
	buff[fz] = 0;		/* end of line*/

	fclose (fp);
	remove (ftmp);

memfree_out:
	RSA_free(rsa);

out:
	SET_ERR_NO(err);
	return buff;
}

char *get_private_key (const unsigned char *sUPWD)
{
	const char *ftmp="/tmp/private.txt";
//	unsigned char public[128]; 
//	unsigned char exponent[128];
	unsigned char private[256];
	unsigned char exponent[256];
	FILE *fp;
	size_t fz;
	int err;

	memset (private, 0, 256);
	memset (exponent, 0, 256);

	err = _get_private_key (private, exponent, sUPWD);
	if (err != nErr_OK)
		goto out;

	RSA *rsa = RSA_new();
	if (rsa== NULL){
		err = nErr_OTE;
		goto out;
	}
	

	rsa->e = BN_bin2bn (private, 128, NULL);
	rsa->n = BN_bin2bn (exponent, 128, NULL);
	if (!rsa->n || !rsa->e){
		err = nErr_OTE;
		goto memfree_out;
	}

	fp = fopen(ftmp, "wr");
	if(!PEM_write_RSAPrivateKey(fp, rsa, NULL, 0, 0, NULL, NULL))
	{
		err = nErr_OTE;
		goto memfree_out;
	}
	fflush(fp);
	fclose(fp);

	fp = fopen (ftmp,"r");

	fseek(fp, 0L, SEEK_END);
	fz = ftell(fp);

	char *buff = (char *)malloc(sizeof(char)*(fz+1)); /* 1 for null */

	fseek(fp, 0L, SEEK_SET);

	if (fz != fread (buff, 1, fz, fp))
		err = nErr_OTE;
	 
	buff[fz] = 0;		/* end of line*/

	fclose (fp);
//	remove (ftmp);

memfree_out:
	RSA_free(rsa);

out:
	SET_ERR_NO(err);
	return buff;
}




int get_device_status (void)
{
	int tkNo;
	int err = nErr_OK;
	int internal_stat; 

	GET_ACTIVE_TOKEN (tkNo);

	if (tkNo == -nErr_DNF){
		err = nErr_DNF;
		goto out;
	}
	
	if (_check_dev_connected (tk[tkNo].serial) == nErr_DNF){
		err = nErr_DNF;
		goto out;
	}
	
	if (_check_dev_selected (tk[tkNo].serial) == nErr_DNS ){
		err = nErr_DNS;
		goto out;
	}
	
        internal_stat = _get_dev_status_internal ();
	if (internal_stat < 0){
		err = internal_stat;
		goto out;
	}

	if (IS_SUSPEND(internal_stat))
	{
		err = nErr_CAT;
		goto out;
	}

	if (IS_BLOCKED(internal_stat))
	{
		err = nErr_BLK;
		goto out;
	}

	if (IS_DISABLED(internal_stat))
	{
		err = nErr_DIS;
		goto out;
	}
	
out:
	SET_ERR_NO(err);
	return err;
}
