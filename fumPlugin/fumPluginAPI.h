/**********************************************************\

  Auto-generated fumPluginAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "fumPlugin.h"

#include <dlfcn.h>

#ifndef H_fumPluginAPI
#define H_fumPluginAPI

#define nErr_OK 0
#define nErr_DLL 133

using namespace std;
 
class fumPluginAPI : public FB::JSAPIAuto
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn fumPluginAPI::fumPluginAPI(const fumPluginPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    fumPluginAPI(const fumPluginPtr& plugin, const FB::BrowserHostPtr& host) :
        m_plugin(plugin), m_host(host)
    {
	    
      dll_load_stat = loadTokenDll ();
      if (dll_load_stat != nErr_OK)
	      printf ("Unable to load usb token dll \n");

      
      registerMethod("echo",      make_method(this, &fumPluginAPI::echo));
      registerMethod("testEvent", make_method(this, &fumPluginAPI::testEvent));
      
      //registerMethod("GetErrNo",      make_method(this, &fumPluginAPI::GetErrNo));
      registerMethod("GetToken",      make_method(this, &fumPluginAPI::GetToken));
      
      registerMethod("GetDeviceCount",      make_method(this, &fumPluginAPI::GetDeviceCount));
      
      registerMethod("GetPublicKey",      make_method(this, &fumPluginAPI::GetPublicKey));
      
      registerMethod("RsaEncrypt",      make_method(this, &fumPluginAPI::RsaEncrypt));
      
      registerMethod("RsaDecrypt",      make_method(this, &fumPluginAPI::RsaDecrypt));
      
      registerMethod("GetSignature",      make_method(this, &fumPluginAPI::GetSignature));
      
      registerMethod("VerifySignature",      make_method(this, &fumPluginAPI::VerifySignature));
      
      
      
      

        // Read-write property
        registerProperty("testString",
                         make_property(this,
                                       &fumPluginAPI::get_testString,
                                       &fumPluginAPI::set_testString));
        
        // Read-only property
        registerProperty("version",
                         make_property(this,
                                       &fumPluginAPI::get_version));

        registerProperty("serial",
                         make_property(this,
                                       &fumPluginAPI::get_serial));
        registerProperty("TkErr",
                         make_property(this,
                                       &fumPluginAPI::get_TkErr));

    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn fumPluginAPI::~fumPluginAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~fumPluginAPI();

    fumPluginPtr getPlugin();

    // Read/Write property ${PROPERTY.ident}
    std::string get_testString();
    void set_testString(const std::string& val);

    // Read-only property ${PROPERTY.ident}
    std::string get_version();

    // Method echo
    FB::variant echo(const FB::variant& msg);
    
    // Event helpers
    FB_JSAPI_EVENT(test, 0, ());
    FB_JSAPI_EVENT(echo, 2, (const FB::variant&, const int));

    // Method test-event
    void testEvent();

/*****************************************************/

    int get_TkErr(void);
    void GetToken(void);
    int GetDeviceCount(void);
    string GetPublicKey (void);
    string RsaEncrypt (int key_type, string from,
		       string supwd);
    string RsaDecrypt (int key_type, string from,
		       string supwd);
    string GetSignature (int type, string message,
			 string supwd);
    int VerifySignature (int type, string message,
			    string signature);


    /* read only property */
    std::string get_serial(void);

/*****************************************************/
private:
    unsigned char serial[4];
    int dll_load_stat;
    unsigned char prv_to[1024];
    void *token_handle;
    int (*get_err_no_p) (void);
    int (*get_device_count_p) (void);
    void (*get_first_device_p) (void);
    void (*get_curr_dev_serial_p) (unsigned char*);
    void (*select_device_p) (unsigned char*);
    int (*get_device_status_p) (void);
    char *(*get_public_key_p) (void);

    int (*rsa_encrypt_p) (int, int, const unsigned char*,
			  unsigned char*, const unsigned char*);

    int (*rsa_decrypt_p) (int, int, const unsigned char*,
			  unsigned char*, const unsigned char*);

    int (*get_signature_p) (int, const unsigned char*, unsigned int,
			    unsigned char*, unsigned int*,
			    const unsigned char*);

    int (*verify_signature_p) (int, const unsigned char*, unsigned int,
			       unsigned char*, unsigned int);
 
    int loadTokenDll (void);
    int bin2hex (int lin, unsigned char* in, unsigned char* out);
    int hex2bin (int lin, unsigned char* in, unsigned char* out);

    fumPluginWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    std::string m_testString;
};

#endif // H_fumPluginAPI

