#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for fumplugin
#
#\**********************************************************/

set(PLUGIN_NAME "fumPlugin")
set(PLUGIN_PREFIX "FUM")
set(COMPANY_NAME "FUM")

# ActiveX constants:
set(FBTYPELIB_NAME fumPluginLib)
set(FBTYPELIB_DESC "fumPlugin 1.0 Type Library")
set(IFBControl_DESC "fumPlugin Control Interface")
set(FBControl_DESC "fumPlugin Control Class")
set(IFBComJavascriptObject_DESC "fumPlugin IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "fumPlugin ComJavascriptObject Class")
set(IFBComEventSource_DESC "fumPlugin IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID bad1976f-dc10-5b7a-9a45-17f7ec6ebfa7)
set(IFBControl_GUID 3787cdb7-68bb-5bca-94a2-3676a47c6bdb)
set(FBControl_GUID 79eaae70-deec-5359-8aa0-58876f48fcd6)
set(IFBComJavascriptObject_GUID b0b7be7b-e6c0-574d-bac0-417ee9a974e8)
set(FBComJavascriptObject_GUID e8c4c743-e07f-5e7b-8f1c-232923b991dc)
set(IFBComEventSource_GUID 5fc49520-f0e3-5ffd-ab0a-22c51d84331a)
if ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 851a7caf-5117-55cd-8c74-3493c27fbf57)
else ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 1363fa21-6536-59ce-814c-f996f986d14a)
endif ( FB_PLATFORM_ARCH_32 )

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "FUM.fumPlugin")
set(MOZILLA_PLUGINID "hpc.um.ac.ir/fumPlugin")

# strings
set(FBSTRING_CompanyName "Ferdowsi University of Mashhad")
set(FBSTRING_PluginDescription "Ferdowsi University of Mashhad's usb token with rfid plugin")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2013 Ferdowsi University of Mashhad")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}.dll")
set(FBSTRING_ProductName "fumplugin")
set(FBSTRING_FileExtents "")
if ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "fumplugin")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "fumplugin_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )
set(FBSTRING_MIMEType "application/x-fumplugin")


# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 0)
set(FBMAC_USE_COCOA 0)
set(FBMAC_USE_COREGRAPHICS 0)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)
