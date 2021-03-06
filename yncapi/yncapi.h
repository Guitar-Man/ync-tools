/* 
 * Copyright (C) 2018, Alexandre Courtois
 * 
 * This file is part of YNCAPI.
 *
 * YNCAPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * YNCAPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with YNCAPI.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef YNCAPI_H
#define YNCAPI_H

#include "types.h"
#include <string>

#define _YNCAPI_MAJOR_VERSION  0
#define _YNCAPI_MINOR_VERSION  41
#define _YNCAPI_PATCH_VERSION  2
#define _YNCAPI_PRE_RELEASE    ""

static const unsigned int YNCAPI_MAJOR_VERSION = _YNCAPI_MAJOR_VERSION;
static const unsigned int YNCAPI_MINOR_VERSION = _YNCAPI_MINOR_VERSION;
static const unsigned int YNCAPI_PATCH_VERSION = _YNCAPI_PATCH_VERSION;
static const char*        YNCAPI_PRE_RELEASE   = _YNCAPI_PRE_RELEASE;

#define NO_LIST                true

namespace yncapi
{
    bool        deviceAssign(const TDeviceList& list, unsigned int pos);
    std::string deviceInfo();
    bool        deviceProbe(const TDeviceId& id);
    void        deviceRemove();

    TReturnCode discover(TDeviceList& deviceList, unsigned int maxDevices = 7, unsigned int timeout = 1);

    const TSystemConfig& getSystemConfig();
    const TSystemPowerControl& getSystemPower();
    const TSystemPowerControl& getSystemPowerSaving();
    const TSystemBasicStatus& getBasicStatus();
    const TSystemInput& getSystemInput();
    const TSystemSound& getSystemSound();

    const TPlayerConfig& getPlayerConfig();
    const TListInfo& getListInfo(bool noList = false);
    const TPlayInfo& getPlayInfo();
    const TNetworkInfo& getNetworkInfo();

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

    string request(string& type, const string& path, const string& value = "GetParam");
}

#endif
