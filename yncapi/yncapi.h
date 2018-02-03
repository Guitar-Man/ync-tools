#ifndef YNCAPI_H
#define YNCAPI_H

#include "types.h"
#include <string>

#define _YNCAPI_MAJOR_VERSION  0
#define _YNCAPI_MINOR_VERSION  40
#define _YNCAPI_PATCH_VERSION  1
#define _YNCAPI_PRE_RELEASE    ""

static const unsigned int YNCAPI_MAJOR_VERSION = _YNCAPI_MAJOR_VERSION;
static const unsigned int YNCAPI_MINOR_VERSION = _YNCAPI_MINOR_VERSION;
static const unsigned int YNCAPI_PATCH_VERSION = _YNCAPI_PATCH_VERSION;
static const char*        YNCAPI_PRE_RELEASE   = _YNCAPI_PRE_RELEASE;

#define NO_LIST                true

namespace yncapi
{
    bool        deviceAssign(const std::string& hostName);
    std::string deviceInfo();
    bool        deviceProbe(const std::string& hostName);
    void        deviceRemove();

    TReturnCode discover(TDeviceList& deviceList, unsigned int maxDevices = 7, unsigned int timeout = 1);

    TSystemConfig getSystemConfig();
    TSystemPowerControl getSystemPower();
    TSystemPowerControl getSystemPowerSaving();
    TSystemBasicStatus getBasicStatus();
    TSystemInput getSystemInput();
    TSystemSound getSystemSound();

    TPlayerConfig getPlayerConfig();
    TListInfo getListInfo(bool noList = false);
    TPlayInfo getPlayInfo();
    TNetworkInfo getNetworkInfo();

    TReturnCode setSystemPowerOn();
    TReturnCode setSystemPowerStandby();
    TReturnCode setInputCd();
    TReturnCode setInputServer();
    TReturnCode setInputNetRadio();
    TReturnCode setInputAirplay();
    TReturnCode setInputSpotify();
    TReturnCode setTrayOpenClose();
    TReturnCode setPlayerSelection(unsigned int lineNumber);
    TReturnCode setPlayerJumpLine(unsigned int lineNumber);
    TReturnCode setPlayerJumpPrev();
    TReturnCode setPlayerJumpNext();
    TReturnCode setPlayerCtrlStop();
    TReturnCode setPlayerCtrlPlay();
    TReturnCode setPlayerCtrlPrev();
    TReturnCode setPlayerCtrlNext();
    TReturnCode setPlayerCtrlPause();
    TReturnCode setPlayerListReturn();
    TReturnCode setPlayerRepeat();
    TReturnCode setPlayerShuffle();
    TReturnCode setPureDirectModeOn();
    TReturnCode setPureDirectModeOff();
    TReturnCode setNetworkName(string name);
    TReturnCode setMacFilterOn();
    TReturnCode setMacFilterAddress(string line, string address);

    string request(string type, const string& path, const string& value = "GetParam");
}

#endif
