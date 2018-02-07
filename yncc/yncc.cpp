#include "yncc.h"
#include "../yncapi/tools.h"
#include <vector>
#include <iostream>

#define CONTINUE        true
#define EXIT            false

#define SECTION     0
#define COMMAND     1
#define ARG_0       0
#define ARG_1       1
#define ARG_2       2

#define _1_ARGS     1
#define _2_ARGS     2
#define _3_ARGS     3

#define CMD_DEVICE              "device"
#define CMD_ASSIGN              "assign"
#define CMD_PROBE               "probe"
#define CMD_INFO                "info"
#define CMD_SYSTEM              "system"
#define CMD_SET                 "set"
#define CMD_GET                 "get"
#define CMD_CONFIG              "config"
#define CMD_STATUS              "status"
#define CMD_POWER               "power"
#define CMD_ON                  "on"
#define CMD_STANDBY             "standby"
#define CMD_EXIT                "exit"
#define CMD_INPUT               "input"
#define CMD_REQUEST             "request"
#define CMD_CD                  "cd"
#define CMD_SERVER              "server"
#define CMD_RADIO               "radio"
#define CMD_AIRPLAY             "airplay"
#define CMD_SPOTIFY             "spotify"
#define CMD_TRAY                "tray"
#define CMD_OPENCLOSE           "openclose"
#define CMD_PLAYER              "player"
#define CMD_LINE                "line"
#define CMD_CTRL                "ctrl"
#define CMD_PLAY                "play"
#define CMD_STOP                "stop"
#define CMD_PREV                "prev"
#define CMD_NEXT                "next"
#define CMD_PAUSE               "pause"
#define CMD_RETURN              "return"
#define CMD_LIST                "list"
#define CMD_JUMP                "jump"
#define CMD_DISCOVER            "discover"

#define ERROR_CMD_INVALID       "ERROR_CMD_INVALID"

#define IF_CMDARGS1P_IS(a)        if (args[0] == a)
#define IF_CMDARGS2P_IS(a,b)      if (args[0] == a && args[1] == b)
#define IF_CMDARGS3P_IS(a,b,c)    if (args[0] == a && args[1] == b && args[2] == c)
#define IF_CMD_IS(a)              if (cmd == a)

static bool CMD_REQUEST_already_called = false;

yncapi::TDeviceList list;

void yncc::handle(yncapi::TReturnCode code) {
    if(code)
        cout << "Error (code " << std::to_string(code) << ")" << endl;
}

bool yncc::parseCmdLine(const std::string &cmdline) {
    vector<std::string> v = yncapi::slice(cmdline, ' ');
    std::string cmd;
    vector<std::string> args;
    bool noArgs;

    if(v.size()) {
        if(v.size() > 1) {
            cmd = v[SECTION];

            for(unsigned int i = 1; i < v.size(); ++i)
                args.push_back(v[i]);
        } else
            cmd = v[SECTION];

        noArgs = !args.size();

        if(cmd == CMD_EXIT && noArgs) {
            return EXIT;
        }

        IF_CMD_IS(CMD_DEVICE) {
            switch(args.size()) {
                case _1_ARGS:
                    IF_CMDARGS1P_IS(CMD_INFO) {
                        if (yncapi::Device::isAssigned())
                            cout << "Current device : " << yncapi::deviceInfo() << endl;
                        else
                            cout << "No device assigned !" << endl;
                            
                        return CONTINUE;
                    }

                    IF_CMDARGS1P_IS(CMD_DISCOVER) {
                        cout << "Searching devices..." << endl;

                        yncapi::discover(list);

                        if(list.devices.size())
                            list.show();
                        else
                            cout << "No device found." << endl;

                        return CONTINUE;
                    }

                    break;

                case _2_ARGS:
                    IF_CMDARGS1P_IS(CMD_ASSIGN) {
                        if (stoi(args[1]) < list.devices.size()) {
                            cout << "Setting device " << list.devices[stoi(args[1])].DeviceName << "... ";
                            cout.flush();

                            if(yncapi::deviceAssign(list, stoi(args[1])))
                                cout << "OK" << endl;
                            else
                                cout << "KO" << endl;

                            return CONTINUE;
                        }
                    }

                    IF_CMDARGS1P_IS(CMD_PROBE) {
                        if (stoi(args[1]) < list.devices.size()) {
                            cout << "Probing device " << list.devices[stoi(args[1])].DeviceName << "... ";
                            cout.flush();

                            if(yncapi::deviceProbe(list.devices[stoi(args[1])]))
                                cout << "OK" << endl;
                            else
                                cout << "KO" << endl;

                            return CONTINUE;
                        }
                    }

                    break;

                default:
                    break;
            }
        }

        IF_CMD_IS(CMD_SYSTEM) {
            switch(args.size()) {
                case _2_ARGS:
                    IF_CMDARGS2P_IS(CMD_GET, CMD_CONFIG) {
                        yncapi::getSystemConfig();

                        cout << "Model Name : " << yncapi::Device::SystemConfig.ModelName << endl;
                        cout << "System ID  : " << yncapi::Device::SystemConfig.SystemID << endl;
                        cout << "Version    : " << yncapi::Device::SystemConfig.Version << endl;
                        cout << "Features   : ";

                        for(unsigned int i = 0 ; i < yncapi::Device::SystemConfig.Features.size(); ++i) {
                            cout << yncapi::Device::SystemConfig.Features[i];

                            if(i != (yncapi::Device::SystemConfig.Features.size() - 1))
                                cout << ",";
                        }

                        cout << endl;

                        return CONTINUE;
                    }

                    IF_CMDARGS2P_IS(CMD_GET, CMD_POWER) {
                        yncapi::getSystemPower();

                        cout << "Power      : " << yncapi::Device::SystemPowerControl.Power << endl;

                        return CONTINUE;
                    }

                    IF_CMDARGS2P_IS(CMD_GET, CMD_STATUS) {
                        yncapi::getBasicStatus();

                        cout << "Power      : " << yncapi::Device::SystemBasicStatus.PowerControl->Power << endl;
                        cout << "Input      : " << yncapi::Device::SystemBasicStatus.Input->InputSel << endl;
                        cout << "PureDirect : " << yncapi::Device::SystemBasicStatus.Sound->PureDirectMode << endl;

                        return CONTINUE;
                    }

                    IF_CMDARGS2P_IS(CMD_GET, CMD_INPUT) {
                        yncapi::getSystemInput();

                        cout << "Input      : " << yncapi::Device::SystemInput.InputSel << endl;

                        return CONTINUE;
                    }

                    break;

                case _3_ARGS:
                    IF_CMDARGS3P_IS(CMD_SET, CMD_POWER, CMD_ON) {
                        handle(yncapi::setSystemPowerOn());
                        return CONTINUE;
                    }

                    IF_CMDARGS3P_IS(CMD_SET, CMD_POWER, CMD_STANDBY) {
                        handle(yncapi::setSystemPowerStandby());
                        return CONTINUE;
                    }

                    IF_CMDARGS3P_IS(CMD_SET, CMD_INPUT, CMD_CD) {
                        handle(yncapi::setInputCd());
                        return CONTINUE;
                    }

                    IF_CMDARGS3P_IS(CMD_SET, CMD_INPUT, CMD_SERVER) {
                        handle(yncapi::setInputServer());
                        return CONTINUE;
                    }

                    IF_CMDARGS3P_IS(CMD_SET, CMD_INPUT, CMD_RADIO) {
                        handle(yncapi::setInputNetRadio());
                        return CONTINUE;
                    }

                    IF_CMDARGS3P_IS(CMD_SET, CMD_INPUT, CMD_AIRPLAY) {
                        handle(yncapi::setInputAirplay());
                        return CONTINUE;
                    }

                    IF_CMDARGS3P_IS(CMD_SET, CMD_INPUT, CMD_SPOTIFY) {
                        handle(yncapi::setInputSpotify());
                        return CONTINUE;
                    }

                    IF_CMDARGS3P_IS(CMD_SET, CMD_TRAY, CMD_OPENCLOSE) {
                        handle(yncapi::setTrayOpenClose());
                        return CONTINUE;
                    }

                    break;

                default:
                    break;
            }
        }

        IF_CMD_IS(CMD_PLAYER) {
            switch(args.size()) {
                case _2_ARGS:
                    IF_CMDARGS2P_IS(CMD_GET, CMD_CONFIG) {
                        yncapi::getPlayerConfig();
                        cout << "Status     : " << yncapi::Device::PlayerConfig.FeatureAvailability << endl;

                        return CONTINUE;
                    }

                    IF_CMDARGS2P_IS(CMD_LIST, CMD_INFO) {
                        yncapi::getListInfo();

                        cout << "Menu Status  : " << yncapi::Device::ListInfo.MenuStatus << endl
                             << "Menu Layer   : " << yncapi::Device::ListInfo.MenuLayer << endl
                             << "Menu Name    : " << yncapi::Device::ListInfo.MenuName << endl
                             << "Current Line : " << yncapi::Device::ListInfo.CursorPosition.CurrentLine << endl
                             << "Max Line     : " << yncapi::Device::ListInfo.CursorPosition.MaxLine << endl
                             << "Current List : " << endl;

                        for(unsigned int i = 0; i < 8; ++i) {
                            if(yncapi::Device::ListInfo.CurrentList[i].Attr != "Unselectable") {
                                cout << "   "
                                     << yncapi::Device::ListInfo.CurrentList[i].Attr << " : "
                                     << to_string(i + 1) << " - "
                                     << yncapi::Device::ListInfo.CurrentList[i].Text
                                     << endl;
                            }
                        }

                        return CONTINUE;
                    }

                    IF_CMDARGS2P_IS(CMD_CTRL, CMD_INFO) {
                        yncapi::getPlayInfo();

                        cout << "Playback  : " << yncapi::Device::PlayInfo.PlaybackInfo << endl
                             << "Repeat    : " << yncapi::Device::PlayInfo.PlayMode.Repeat << endl
                             << "Shuffle   : " << yncapi::Device::PlayInfo.PlayMode.Shuffle << endl
                             << "Play Time : " << yncapi::Device::PlayInfo.PlayTime << endl
                             << "Track     : " << yncapi::Device::PlayInfo.TrackNum << endl
                             << "Total     : " << yncapi::Device::PlayInfo.TotalTracks << endl
                             << "Artist    : " << yncapi::Device::PlayInfo.MetaInfo.Artist << endl
                             << "Album     : " << yncapi::Device::PlayInfo.MetaInfo.Album << endl
                             << "Song      : " << yncapi::Device::PlayInfo.MetaInfo.Song << endl;

                        return CONTINUE;
                    }

                    IF_CMDARGS2P_IS(CMD_CTRL, CMD_PLAY) {
                        handle(yncapi::setPlayerCtrlPlay());
                        return CONTINUE;
                    }

                    IF_CMDARGS2P_IS(CMD_CTRL, CMD_STOP) {
                        handle(yncapi::setPlayerCtrlStop());
                        return CONTINUE;
                    }

                    IF_CMDARGS2P_IS(CMD_CTRL, CMD_PREV) {
                        handle(yncapi::setPlayerCtrlPrev());
                        return CONTINUE;
                    }

                    IF_CMDARGS2P_IS(CMD_CTRL, CMD_NEXT) {
                        handle(yncapi::setPlayerCtrlNext());
                        return CONTINUE;
                    }

                    IF_CMDARGS2P_IS(CMD_CTRL, CMD_PAUSE) {
                        handle(yncapi::setPlayerCtrlPause());
                        return CONTINUE;
                    }

                    IF_CMDARGS2P_IS(CMD_LIST, CMD_RETURN) {
                        handle(yncapi::setPlayerListReturn());
                        return CONTINUE;
                    }

                    break;

                case _3_ARGS :
                    IF_CMDARGS2P_IS(CMD_SET, CMD_LINE) {
                        try {
                            handle(yncapi::setPlayerSelection(stoi(args[2])));
                        } catch(invalid_argument& error) {
                            break;
                        } catch(out_of_range& error) {
                            break;
                        }

                        return CONTINUE;
                    }

                    IF_CMDARGS2P_IS(CMD_LIST, CMD_JUMP) {
                        if(args[2] == CMD_NEXT) {
                            yncapi::getListInfo(NO_LIST);
                            int cline = stoi(yncapi::Device::ListInfo.CursorPosition.CurrentLine);
                            int mline = stoi(yncapi::Device::ListInfo.CursorPosition.MaxLine);

                            if((cline + 8) < mline)
                                handle(yncapi::setPlayerJumpLine(cline + 8));

                            return CONTINUE;
                        }

                        if(args[2] == CMD_PREV) {
                            yncapi::getListInfo(NO_LIST);
                            int cline = stoi(yncapi::Device::ListInfo.CursorPosition.CurrentLine);

                            if((cline - 8) < 1)
                                handle(yncapi::setPlayerJumpLine(1));
                            else
                                handle(yncapi::setPlayerJumpLine(cline - 8));

                            return CONTINUE;
                        }

                        try {
                            handle(yncapi::setPlayerJumpLine(stoi(args[2])));
                        } catch(invalid_argument& error) {
                            break;
                        } catch(out_of_range& error) {
                            break;
                        }

                        return CONTINUE;
                    }

                    break;

                default:
                    break;
            }
        }

        IF_CMD_IS(CMD_REQUEST) {
            if(!CMD_REQUEST_already_called) {
                CMD_REQUEST_already_called = true;

                cout << endl << "!!! WARNING !!!" << endl;
                cout << "BE WARNED THAT THE 'request' COMMAND DOES NOT AFFECT" << endl;
                cout << "THE yncapi VIRTUAL DEVICE IN ANY WAY !" << endl;
                cout << "ACCORDINGLY, YOU SHOULD ONLY USE IT FOR TESTING PURPOSES." << endl;
                cout << "OTHERWISE, THE VIRTUAL DEVICE MIGHT BE IN A DIFFERENT STATE" << endl;
                cout << "THAN THE PHYSICAL DEVICE, AND yncc MIGHT HAVE STRANGE BEHAVIOURS !" << endl << endl;
            }

            if(args.size() == 2 || args.size() == 3) {
                string req;

                if(args.size() == 3)
                    req = yncapi::request(args[0], args[1], args[2]);
                else
                    req = yncapi::request(args[0], args[1]);

                for(unsigned int i = 0; i < req.size(); ++i)
                    if(req[i] == '>' && i < req.size() - 1) {
                        req.insert(i+1, "\n");
                        i++;
                    }

                cout << req << endl;

                return CONTINUE;
            }
        }

        cerr << ERROR_CMD_INVALID << endl;
    }

    return CONTINUE;
}
