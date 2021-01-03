/**********************************************************\

  Auto-generated fumPluginAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include <string>
#include <stdio.h>

#include "fumPluginAPI.h"

///////////////////////////////////////////////////////////////////////////////
/// @fn FB::variant fumPluginAPI::echo(const FB::variant& msg)
///
/// @brief  Echos whatever is passed from Javascript.
///         Go ahead and change it. See what happens!
///////////////////////////////////////////////////////////////////////////////
FB::variant fumPluginAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo("So far, you clicked this many times: ", n++);

    // return "foobar";
    return msg;
}

///////////////////////////////////////////////////////////////////////////////
/// @fn fumPluginPtr fumPluginAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
fumPluginPtr fumPluginAPI::getPlugin()
{
    fumPluginPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

// Read/Write property testString
std::string fumPluginAPI::get_testString()
{
    return m_testString;
}

void fumPluginAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string fumPluginAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

void fumPluginAPI::testEvent()
{
  fire_test();
}

int fumPluginAPI::loadTokenDll (void)
{
  char *error;

  //  token_handle = dlopen ("libnovin.so", RTLD_NOW);
  token_handle = dlopen ("libnovin.so", RTLD_LAZY);
  if ((error = dlerror()) != NULL){
    fprintf(stderr, "%s\n", error);
  }
  if (!token_handle) 
    return nErr_DLL;

  get_err_no_p = (int(*)(void)) dlsym (token_handle, "get_err_no");
  if ((error = dlerror()) != NULL){
    fprintf(stderr, "%s\n", error);
    return nErr_DLL;
  }
  
  get_device_count_p = (int(*)(void)) dlsym (token_handle, "get_device_count");
  if ((error = dlerror()) != NULL){
    fprintf(stderr, "%s\n", error);
    return nErr_DLL;
    }

  get_first_device_p = (void(*)(void)) dlsym (token_handle, "get_first_device");
  if ((error = dlerror()) != NULL){
    fprintf(stderr, "%s\n", error);
    return nErr_DLL;
  }

  get_curr_dev_serial_p = (void(*)(unsigned char*)) dlsym (token_handle, "get_curr_dev_serial");
  if ((error = dlerror()) != NULL){
    fprintf(stderr, "%s\n", error);
    return nErr_DLL;
  }

  select_device_p = (void(*)(unsigned char*)) dlsym (token_handle, "select_device");
  if ((error = dlerror()) != NULL){
    fprintf(stderr, "%s\n", error);
    return nErr_DLL;
  }

  get_device_status_p = (int(*)(void)) dlsym (token_handle, "get_device_status");
  if ((error = dlerror()) != NULL){
    fprintf(stderr, "%s\n", error);
    return nErr_DLL;
  }

  get_public_key_p = (char*(*)(void)) dlsym (token_handle, "get_public_key");
  if ((error = dlerror()) != NULL){
    fprintf(stderr, "%s\n", error);
    return nErr_DLL;
  }

  rsa_encrypt_p = (int(*)(int, int, const unsigned char*,unsigned char*,
			  const unsigned char*)) dlsym (token_handle, "rsa_encrypt");
  if ((error = dlerror()) != NULL){
    fprintf(stderr, "%s\n", error);
    return nErr_DLL;
  }

  rsa_decrypt_p = (int (*)(int, int, const unsigned char*,unsigned char*,
			   const unsigned char*)) dlsym (token_handle, "rsa_decrypt");
  if ((error = dlerror()) != NULL){
    fprintf(stderr, "%s\n", error);
    return nErr_DLL;
  }
  
  get_signature_p=(int(*)(int, const unsigned char*,unsigned int,unsigned char*,
			  unsigned int*,const unsigned char*))dlsym(token_handle,"get_signature");
  if ((error = dlerror()) != NULL){
    fprintf(stderr, "%s\n", error);
    return nErr_DLL;
  }
  
  verify_signature_p = (int(*)(int, const unsigned char*, unsigned int,unsigned char*,
			       unsigned int)) dlsym (token_handle, "verify_signature");
  if ((error = dlerror()) != NULL){
    fprintf(stderr, "%s\n", error);
    return nErr_DLL;
  }

  return nErr_OK;
}

fumPluginAPI::~fumPluginAPI()
{
  dlclose (token_handle);
}

int fumPluginAPI::get_TkErr (void)
{
  if (dll_load_stat != nErr_OK)
    return nErr_DLL;

  return (*get_err_no_p) ();
}

void fumPluginAPI::GetToken(void)
{
  if (get_TkErr() != nErr_OK)
    return;

  (*get_first_device_p) ();
  if (get_TkErr() != nErr_OK)
    return;

  (*get_curr_dev_serial_p)(serial);
  if (get_TkErr() != nErr_OK)
    return;

  (*select_device_p)(serial);
  if (get_TkErr() != nErr_OK)
    return;
}

std::string fumPluginAPI::get_serial(void)
{
  char buff[16];
  memset (buff, 0, 16);

  sprintf(buff,"%d,%d,%d,%d",
	 serial[0], serial[1], serial[2], serial[3]);

  return string (buff);
}

int fumPluginAPI::GetDeviceCount(void)
{
  if (get_TkErr() != nErr_OK)
    return 0;

  return (*get_device_count_p) ();
}

std::string fumPluginAPI::GetPublicKey (void)
{
  if (get_TkErr() != nErr_OK)
    return string ("");

  return string ( (const char*) ((*get_public_key_p) ()) );
}

std::string fumPluginAPI::RsaEncrypt(int key_type, std::string from, std::string supwd)
{
  int flen;
  int outlen;
  int len;
  unsigned char to[1024];
  char cfrom[1024];
  char csupwd[16];
  unsigned char hex_out[1024];

  if (get_TkErr() != nErr_OK)
    return string ("");

  memset (cfrom, 0, 1024);

  flen = from.length();

  std::strcpy (cfrom, from.c_str());

  memset (csupwd, 0, 16);
  std::strncpy (csupwd, supwd.c_str(), 16);

  len = (*rsa_encrypt_p) (key_type, flen, (const unsigned char*) cfrom,
			  to, (const unsigned char*) csupwd );
  if (len == -1)
    return string ("");

  outlen = bin2hex (len, to, hex_out);
  hex_out[outlen]= 0;   // string method need null terminated string to convert.

  return string ( (const char*) hex_out);
}

std::string fumPluginAPI::RsaDecrypt (int key_type, std::string from,
				      std::string supwd)
{
  int flen;
  int len;
  int out_len;
  unsigned char to[1024];
  char cfrom[1024];
  char csupwd[16];
  unsigned char bin_out[1024];

  if (get_TkErr() != nErr_OK)
    return string ("");

  flen = from.length();

  memset (cfrom, 0 , 1024); 
  std::strcpy (cfrom, from.c_str());

  out_len = hex2bin (flen, (unsigned char*)cfrom, bin_out); 

  memset (csupwd, 0, 16);
  std::strncpy (csupwd, supwd.c_str(), 16);
  
  len = (*rsa_decrypt_p) (key_type, out_len, bin_out, to,
			  (const unsigned char*) csupwd);
  if (len == -1)
    return string ("");

  to[len] = 0;		// string method need null terminated string to convert.
  
  return string ( (const char*) to);
}

std::string fumPluginAPI::GetSignature (int type, std::string message, 
				      std::string supwd)
{
  unsigned int siglen;
  unsigned char sigret[1024];
  unsigned char hex_out[1024];
  int m_len;
  int stat;
  int outlen;
  char csupwd[16];
  
  if (get_TkErr() != nErr_OK)
    return string ("");

  memset (csupwd, 0, 16);
  m_len = message.length();

  char *m = new char[m_len];
  
  std::strcpy (m, message.c_str());

  std::strncpy (csupwd, supwd.c_str(), 16);

  stat = (*get_signature_p) (type, (const unsigned char*)m, m_len, 
  			     sigret, &siglen, (const unsigned char*)csupwd);
  if (stat != nErr_OK)
    return string("");

  outlen = bin2hex (siglen, sigret, hex_out);
  hex_out[outlen]= 0;   // string method need null terminated string to convert.

  delete[] m;
  
  return string ((const char*) hex_out );
}


int fumPluginAPI::VerifySignature (int type, string message, string signature)
{
  int stat;
  int m_len;
  int siglen;
  int out_len;

  if (get_TkErr() != nErr_OK)
    return -1; 			// documented it, in pluginmanuall.txt
  
  m_len = message.length();
  char *m = new char[m_len];
  std::strcpy (m, message.c_str());

  siglen = signature.length();
  char *sig = new char[siglen];

  std::strcpy (sig, signature.c_str());

  unsigned char bin_out[siglen/2];
  out_len = hex2bin (siglen, (unsigned char*)sig, bin_out); 

  stat = (*verify_signature_p) (type, (const unsigned char*) m, m_len,
  				(unsigned char*) bin_out, out_len);
  
  delete[] m;
  delete[] sig;
  return stat;
}

int fumPluginAPI::bin2hex (int lin, unsigned char* in, unsigned char* out)
{
  int i, j;
  char table[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
		     '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

  for (i=0,j=0 ; i<lin ; i++)
    {
      int a = (in[i] >>4 ) & 0x0F;
      int b = (in[i]     ) & 0x0F;

      out[j++] = table[a];
      out[j++] = table[b];
    }
  return j;
}

int fumPluginAPI::hex2bin (int lin, unsigned char* in, unsigned char* out)
{
  int bin_map[256];

  bin_map['0'] = 0x00;
  bin_map['1'] = 0x01;
  bin_map['2'] = 0x02;
  bin_map['3'] = 0x03;
  bin_map['4'] = 0x04;
  bin_map['5'] = 0x05;
  bin_map['6'] = 0x06;
  bin_map['7'] = 0x07;
  bin_map['8'] = 0x08;
  bin_map['9'] = 0x09;
  bin_map['a'] = 0x0a;
  bin_map['b'] = 0x0b;
  bin_map['c'] = 0x0c;
  bin_map['d'] = 0x0d;
  bin_map['e'] = 0x0e;
  bin_map['f'] = 0x0f;
  bin_map['A'] = 0x0A;
  bin_map['B'] = 0x0B;
  bin_map['C'] = 0x0C;
  bin_map['D'] = 0x0D;
  bin_map['E'] = 0x0E;
  bin_map['F'] = 0x0F;

  int i, j;
  for (i=0,j=0 ; i<lin/2 ; i++)
    {
      int a = (bin_map[in[j++]] << 4 ) & 0xF0;
      int b = (bin_map[in[j++]]      ) & 0x0F;

      out[i] = a|b;
    }
  return i;
}
