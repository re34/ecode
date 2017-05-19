#include "version.h"
#include "includes.h"

#define _STR(s) #s

#define FW_VERSION_MAJOR    1
#define FW_VERSION_MINOR    0
#define FW_VERSION_BUILD    0

#define MACRO_TO_STR(macro)     _STR(macro)


static char * fFirmwareName = "Ecode";

static char * fFirmwareVersion = MACRO_TO_STR(FW_VERSION_MAJOR)"."\
                                 MACRO_TO_STR(FW_VERSION_MINOR)"."\
                                 MACRO_TO_STR(FW_VERSION_BUILD);

static char * fAuthorName = "Arno";

static const char fLogo[]="\r\n\
    ______               __\r\n\
   / ____/________  ____/ /__\r\n\
  / __/ / ___/ __ \\/ __  / _ \\\r\n\
 / /___/ /__/ /_/ / /_/ /  __/\r\n\
/_____/\\___/\\____/\\____/\\___/\r\n\
\r\n";


static char fCompileTime[20];


char *GetFirmwareName(void)
{
    return fFirmwareName;
}

char *GetFirmwareVersion(void)
{
    return fFirmwareVersion;
}

char *GetAuthorName(void)
{
    return fAuthorName;
}

char *GetCompileTime(void)
{
    sprintf(fCompileTime, "%s %s", __DATE__,__TIME__);
    
    return fCompileTime;
}


void FirmwareInfoPrint(void)
{
    print_log("\r\n");
    print_log("---------------------------------------------");
    print_log(fLogo);
    
    print_log("Board Name: %s\r\n", TARGET_BOARD);
    print_log("Firmware Name: %s\r\n", fFirmwareName);
    print_log("Firmware version: %s\r\n", fFirmwareVersion);
    print_log("Author :%s\r\n", fAuthorName);
    print_log("Compile time: %s %s\r\n", __DATE__,__TIME__);
    print_log("---------------------------------------------");
    print_log("\r\n");
}

