#include "types.h"
#include "xml_requests.h"
#include "xml_features.h"
#include "http_features.h"
#include "tools.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include "yncapi.h"
#include "ssdp_features.h"
#include <SFML/Network.hpp>
#include "url.hpp"

using namespace std;

#define SUCCESS         true
#define FAILURE         false

#define DEFAULT_DEVICE                  "no device"

namespace yncapi {
    string __Xml_BasicStatus__;
    string __Xml_PlayInfo__;
    string __Xml_ListInfo__;
    string __Xml_OptionInfo__;

    bool __noList_on_last_call__ = false;

    // ------ DEVICE STATIC INIT -----------------

    //string Device::id = DEFAULT_DEVICE;
    TDeviceId Device::id;
    TSystemConfig Device::SystemConfig;
    TSystemPowerControl Device::SystemPowerControl;
    TSystemBasicStatus Device::SystemBasicStatus;
    TSystemInput Device::SystemInput;
    TSystemSound Device::SystemSound;

    TPlayerConfig Device::PlayerConfig;
    TListInfo Device::ListInfo;
    TPlayInfo Device::PlayInfo;

    TNetworkInfo Device::NetworkInfo;

    // ------ DEVICE REFRESH STATIC INIT ---------

    bool DeviceRefreshNotifications::BasicStatus = true;
    bool DeviceRefreshNotifications::PlayInfo = true;
    bool DeviceRefreshNotifications::ListInfo = true;
    bool DeviceRefreshNotifications::OptionInfo = true;

    bool Device::isAssigned() {
        return Device::id.DeviceIP.size();
    }

    void Device::initialize() {
        Device::SystemBasicStatus.PowerControl = &(Device::SystemPowerControl);
        Device::SystemBasicStatus.Input = &(Device::SystemInput);
        Device::SystemBasicStatus.Sound = &(Device::SystemSound);
    }

    void Device::update() {
        getBasicStatus();
        getListInfo();
        getPlayInfo();
    }

    bool deviceProbe(const TDeviceId& id) {
        sf::TcpSocket socket;

        if(socket.connect(id.DeviceIP, 80) == sf::Socket::Status::Done) {
            return SUCCESS;
        }

        return FAILURE;
    }

    bool deviceAssign(const TDeviceList& list, unsigned int pos) {
        if(deviceProbe(list.devices[pos])) {
            Device::id = list.devices[pos];
            httpClient.setHost(Device::id.DeviceIP);

            return SUCCESS;
        }

        return FAILURE;
    }

    string deviceInfo() {
        if (Device::isAssigned())
            return Device::id.DeviceName + ":" + Device::id.DeviceIP  + Device::id.CtrlURL;
        
        return string();
    }

    string request(string type, const string& path, const string& value) {
        if(type == "put")
            type = HTTP_PUT;
        else
            if(type == "get")
                type = HTTP_GET;
            else
                return ERROR_XML_BAD_TYPE;

        return sendRequest(xmlBuildRequest(type, path, value));
    }

    TReturnCode handle(const string& response) {
        return stoi(xmlGetAttribute(response, XML_ROOTNODE, XML_ERR_ATTR));
    }

    TSystemConfig getSystemConfig() {
        string xml_0 = sendRequest(xmlBuildRequest(HTTP_GET, SYSTEM_CONFIG));

        Device::SystemConfig.ModelName         = xmlExtract(xml_0, YAMAHA_AV + SYSTEM_CONFIG + "/Model_Name");
        Device::SystemConfig.SystemID          = xmlExtract(xml_0, YAMAHA_AV + SYSTEM_CONFIG + "/System_ID");
        Device::SystemConfig.Version           = xmlExtract(xml_0, YAMAHA_AV + SYSTEM_CONFIG + "/Version");
        Device::SystemConfig.Features = slice(xmlExtract(xml_0, YAMAHA_AV + SYSTEM_CONFIG + "/Feature_Existence"), ',');

        return Device::SystemConfig;
    }

    TSystemPowerControl getSystemPower() {
        string xml_0 = sendRequest(xmlBuildRequest(HTTP_GET, SYSTEM_POWERCONTROL_POWER));

        Device::SystemPowerControl.Power = xmlExtract(xml_0, YAMAHA_AV + SYSTEM_POWERCONTROL_POWER);

        return Device::SystemPowerControl;
    }

    TSystemPowerControl getSystemPowerSaving() {
        string xml_0 = sendRequest(xmlBuildRequest(HTTP_GET, SYSTEM_POWERCONTROL_SAVING));

        Device::SystemPowerControl.Saving = xmlExtract(xml_0, YAMAHA_AV + SYSTEM_POWERCONTROL_SAVING);

        return Device::SystemPowerControl;
    }

    TSystemBasicStatus getBasicStatus() {
        string xml_0 = sendRequest(xmlBuildRequest(HTTP_GET, SYSTEM_BASIC_STATUS));

        if(xml_0 != __Xml_BasicStatus__) {
            Device::SystemBasicStatus.PowerControl->Power = xmlExtract(xml_0, YAMAHA_AV + SYSTEM_BASIC_STATUS + "/Power_Control/Power");
            Device::SystemBasicStatus.Input->InputSel = xmlExtract(xml_0, YAMAHA_AV + SYSTEM_BASIC_STATUS + "/Input/Input_Sel");
            Device::SystemBasicStatus.Sound->PureDirectMode = xmlExtract(xml_0, YAMAHA_AV + SYSTEM_BASIC_STATUS + "/Sound/Pure_Direct/Mode");

            DeviceRefreshNotifications::BasicStatus = true;
            __Xml_BasicStatus__ = xml_0;
        }

        return Device::SystemBasicStatus;
    }

    TSystemInput getSystemInput() {
        string xml_0 = sendRequest(xmlBuildRequest(HTTP_GET, SYSTEM_INPUT_INPUTSEL));

        Device::SystemInput.InputSel = xmlExtract(xml_0, YAMAHA_AV + SYSTEM_INPUT_INPUTSEL);

        return Device::SystemInput;
    }

    TSystemSound getSystemSound() {
        string xml_0 = sendRequest(xmlBuildRequest(HTTP_GET, SYSTEM_SOUND));

        Device::SystemSound.PureDirectMode = xmlExtract(xml_0, YAMAHA_AV + SYSTEM_SOUND_PUREDIRECT_MODE);

        return Device::SystemSound;
    }

    TPlayerConfig getPlayerConfig() {
        string xml_0 = sendRequest(xmlBuildRequest(HTTP_GET, PLAYER_CONFIG));

        Device::PlayerConfig.FeatureAvailability = xmlExtract(xml_0, YAMAHA_AV + PLAYER_CONFIG + "/Feature_Availability");

        return Device::PlayerConfig;
    }

    TListInfo getListInfo(bool noList) {
        string xml_0 = sendRequest(xmlBuildRequest(HTTP_GET, PLAYER_LIST_INFO));

        if(xml_0 != __Xml_ListInfo__ || __noList_on_last_call__) {
            Device::ListInfo.MenuStatus = xmlExtract(xml_0, YAMAHA_AV + PLAYER_LIST_INFO + "/Menu_Status");
            Device::ListInfo.MenuLayer = xmlExtract(xml_0, YAMAHA_AV + PLAYER_LIST_INFO + "/Menu_Layer");
            Device::ListInfo.MenuName = xmlExtract(xml_0, YAMAHA_AV + PLAYER_LIST_INFO + "/Menu_Name");

            string lineNum;
            TLine line;

            Device::ListInfo.CursorPosition.CurrentLine = xmlExtract(xml_0, YAMAHA_AV + PLAYER_LIST_INFO + "/Cursor_Position/Current_Line");
            Device::ListInfo.CursorPosition.MaxLine     = xmlExtract(xml_0, YAMAHA_AV + PLAYER_LIST_INFO + "/Cursor_Position/Max_Line");

            Device::ListInfo.CurrentList.clear();
            __noList_on_last_call__ = false;

            if(!noList) {
                string text;

                for(unsigned int i = 1; i <= 8; ++i) {
                    lineNum = "Line_" + to_string(i);
                    text = xmlExtract(xml_0, YAMAHA_AV + PLAYER_LIST_INFO + "/Current_List/" + lineNum + "/Txt");
                    replace(text, "&amp;", "&");
                    line.Text = text;
                    line.Attr = xmlExtract(xml_0, YAMAHA_AV + PLAYER_LIST_INFO + "/Current_List/" + lineNum + "/Attribute");

                    Device::ListInfo.CurrentList.push_back(line);
                }
            } else
                __noList_on_last_call__ = true;

            DeviceRefreshNotifications::ListInfo = true;
            __Xml_ListInfo__ = xml_0;
        }

        return Device::ListInfo;
    }

    TPlayInfo getPlayInfo() {
        string xml_0 = sendRequest(xmlBuildRequest(HTTP_GET, PLAYER_PLAY_INFO));

        if(xml_0 != __Xml_PlayInfo__) {
            string artist = xmlExtract(xml_0, YAMAHA_AV + PLAYER_PLAY_INFO + "/Meta_Info/Artist");
            string album = xmlExtract(xml_0, YAMAHA_AV + PLAYER_PLAY_INFO + "/Meta_Info/Album");
            string song = xmlExtract(xml_0, YAMAHA_AV + PLAYER_PLAY_INFO + "/Meta_Info/Song");

            replace(artist, "&amp;", "&");
            replace(album, "&amp;", "&");
            replace(song, "&amp;", "&");

            Device::PlayInfo.PlaybackInfo = xmlExtract(xml_0, YAMAHA_AV + PLAYER_PLAY_INFO + "/Playback_Info");
            Device::PlayInfo.PlayMode.Repeat = xmlExtract(xml_0, YAMAHA_AV + PLAYER_PLAY_INFO + "/Play_Mode/Repeat");
            Device::PlayInfo.PlayMode.Shuffle = xmlExtract(xml_0, YAMAHA_AV + PLAYER_PLAY_INFO + "/Play_Mode/Shuffle");
            Device::PlayInfo.PlayTime = xmlExtract(xml_0, YAMAHA_AV + PLAYER_PLAY_INFO + "/Play_Time");
            Device::PlayInfo.TrackNum = xmlExtract(xml_0, YAMAHA_AV + PLAYER_PLAY_INFO + "/Track_Number");
            Device::PlayInfo.TotalTracks = xmlExtract(xml_0, YAMAHA_AV + PLAYER_PLAY_INFO + "/Total_Tracks");

            Device::PlayInfo.MetaInfo.Artist = artist;
            Device::PlayInfo.MetaInfo.Album = album;
            Device::PlayInfo.MetaInfo.Song = song;

            Device::PlayInfo.AlbumArt.URL = xmlExtract(xml_0, YAMAHA_AV + PLAYER_PLAY_INFO + "/Album_ART/URL");
            Device::PlayInfo.AlbumArt.ID = xmlExtract(xml_0, YAMAHA_AV + PLAYER_PLAY_INFO + "/Album_ART/ID");
            Device::PlayInfo.AlbumArt.Format = xmlExtract(xml_0, YAMAHA_AV + PLAYER_PLAY_INFO + "/Album_ART/Format");

            DeviceRefreshNotifications::PlayInfo = true;
            __Xml_PlayInfo__ = xml_0;
        }

        return Device::PlayInfo;
    }

    TNetworkInfo getNetworkInfo() {
        string xml_0 = sendRequest(xmlBuildRequest(HTTP_GET, SYSTEM_MISC_NETWORK_NAME));
        string xml_1 = sendRequest(xmlBuildRequest(HTTP_GET, SYSTEM_MISC_MACFILTER));
        string xml_2 = sendRequest(xmlBuildRequest(HTTP_GET, SYSTEM_MISC_NETWORK_INFO));

        if(xml_0 + xml_1 + xml_2 != __Xml_OptionInfo__) {
            Device::NetworkInfo.NetworkName = xmlExtract(xml_0, YAMAHA_AV + SYSTEM_MISC_NETWORK_NAME);

            Device::NetworkInfo.MACFilterMode = xmlExtract(xml_1, YAMAHA_AV + SYSTEM_MISC_MACFILTER_MODE);

            string number;
            string line;
            Device::NetworkInfo.MACFilters.clear();

            for(unsigned int i = 1; i < 6; ++i) {
                number = "Number_" + to_string(i);
                line = xmlExtract(xml_1, YAMAHA_AV + SYSTEM_MISC_MACFILTER_ADDRESS + "/" + number);

                Device::NetworkInfo.MACFilters.push_back(line);
            }

            Device::NetworkInfo.IPAddress = xmlExtract(xml_2, YAMAHA_AV + SYSTEM_MISC_NETWORK_INFO + "/IP_Address");
            Device::NetworkInfo.SubnetMask = xmlExtract(xml_2, YAMAHA_AV + SYSTEM_MISC_NETWORK_INFO + "/Subnet_Mask");
            Device::NetworkInfo.Gateway = xmlExtract(xml_2, YAMAHA_AV + SYSTEM_MISC_NETWORK_INFO + "/Default_Gateway");
            Device::NetworkInfo.DNS1 = xmlExtract(xml_2, YAMAHA_AV + SYSTEM_MISC_NETWORK_INFO + "/DNS_Server_1");
            Device::NetworkInfo.DNS2 = xmlExtract(xml_2, YAMAHA_AV + SYSTEM_MISC_NETWORK_INFO + "/DNS_Server_2");
            Device::NetworkInfo.MACAddress = xmlExtract(xml_2, YAMAHA_AV + SYSTEM_MISC_NETWORK_INFO + "/MAC_Address");

            DeviceRefreshNotifications::OptionInfo = true;
            __Xml_OptionInfo__ = xml_0 + xml_1 + xml_2;
        }

        return Device::NetworkInfo;
    }

    TReturnCode setSystemPowerOn() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, SYSTEM_POWERCONTROL_POWER, ON)));
    }

    TReturnCode setSystemPowerStandby() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, SYSTEM_POWERCONTROL_POWER, STANDBY)));
    }

    TReturnCode setInputCd() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, SYSTEM_INPUT_INPUTSEL, CD)));
    }

    TReturnCode setInputServer() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, SYSTEM_INPUT_INPUTSEL, SERVER)));
    }

    TReturnCode setInputNetRadio() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, SYSTEM_INPUT_INPUTSEL, NETRADIO)));
    }

    TReturnCode setInputAirplay() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, SYSTEM_INPUT_INPUTSEL, AIRPLAY)));
    }

    TReturnCode setInputSpotify() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, SYSTEM_INPUT_INPUTSEL, SPOTIFY)));
    }

    TReturnCode setTrayOpenClose() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, SYSTEM_MISC_TRAY, OPEN_CLOSE)));
    }

    TReturnCode setPlayerSelection(unsigned int lineNumber) {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, PLAYER_LISTCONTROL_DIRECTSEL, string(LINE_) + to_string(lineNumber))));
    }

    TReturnCode setPlayerJumpLine(unsigned int lineNumber) {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, PLAYER_LISTCONTROL_JUMPLINE, to_string(lineNumber))));
    }

    TReturnCode setPlayerCtrlPlay() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, PLAYER_PLAYCONTROL_PLAYBACK, PLAY)));
    }

    TReturnCode setPlayerCtrlStop() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, PLAYER_PLAYCONTROL_PLAYBACK, STOP)));
    }

    TReturnCode setPlayerCtrlPrev() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, PLAYER_PLAYCONTROL_PLAYBACK, PREV)));
    }

    TReturnCode setPlayerCtrlNext() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, PLAYER_PLAYCONTROL_PLAYBACK, NEXT)));
    }

    TReturnCode setPlayerCtrlPause() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, PLAYER_PLAYCONTROL_PLAYBACK, PAUSE)));
    }

    TReturnCode setPlayerListReturn() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, PLAYER_LISTCONTROL_CURSOR, RETURN)));
    }

    TReturnCode setPlayerRepeat() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, PLAYER_MODE_REPEAT, TOGGLE)));
    }

    TReturnCode setPlayerShuffle() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, PLAYER_MODE_SHUFFLE, TOGGLE)));
    }

    TReturnCode setPureDirectModeOn() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, SYSTEM_SOUND_PUREDIRECT_MODE, ON)));
    }

    TReturnCode setPureDirectModeOff() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, SYSTEM_SOUND_PUREDIRECT_MODE, OFF)));
    }

    TReturnCode setNetworkName(string name) {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, SYSTEM_MISC_NETWORK_NAME, name)));
    }

    TReturnCode setMacFilterOn() {
        return handle(sendRequest(xmlBuildRequest(HTTP_PUT, SYSTEM_MISC_MACFILTER_MODE, ON)));
    }

    TReturnCode setMacFilterAddress(string line, string address) {
        int l = stoi(line);

        if(l >= 1 && l <= 5)
            if(Device::NetworkInfo.MACFilters[l - 1] != address)
                return handle(sendRequest(xmlBuildRequest(HTTP_PUT, string(SYSTEM_MISC_MACFILTER_ADDRESS) + "/" + string(NUMBER_) + line, address)));

        return RC_SUCCESS;
    }

    TReturnCode setPlayerJumpPrev() {
        return setPlayerJumpLine(stoi(Device::ListInfo.CursorPosition.CurrentLine) - 8);
    }

    TReturnCode setPlayerJumpNext() {
        return setPlayerJumpLine(stoi(Device::ListInfo.CursorPosition.CurrentLine) + 8);
    }

    TReturnCode discover(TDeviceList& deviceList, unsigned int maxDevices, unsigned int timeout) {
        sf::IpAddress multicastAddr = "239.255.255.250";
        unsigned short ssdpPort = 1900;
        std::string m_searchRequest = "M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: \"ssdp:discover\"\r\nMX: 3\r\nST: urn:schemas-upnp-org:device:MediaRenderer:1\r\n\r\n";
        sf::UdpSocket socket;
        sf::Socket::Status socketStatus;
        sf::IpAddress senderIP;
        unsigned short senderPort;
        PacketSSDP pssdp;
        sf::SocketSelector selector;

        // send SSDP M-SEARCH request
        socketStatus = socket.send(m_searchRequest.c_str(), m_searchRequest.length(), multicastAddr, ssdpPort);

        if(socketStatus != sf::Socket::Done)
            return socketStatus;

        selector.add(socket);
        
        // automatically delete the members who are not checked with "Remeber" tag
        for(unsigned int i = 0; i < deviceList.devices.size(); ++i)
            if (!deviceList.devices[i].Remember)
                deviceList.devices.erase(deviceList.devices.begin() + i);

        // only look for to fill free space on the device list (typically 7 minus saved devices)
        for(unsigned int i = 0; i < maxDevices - deviceList.devices.size(); ++i) {
            if(selector.wait(sf::seconds(timeout))) {
                socketStatus = socket.receive(pssdp, senderIP, senderPort);

                if(socketStatus == sf::Socket::Done) {
                    Url url(pssdp.getField("location"));
                    
                    // prepare the http client to receive the xml desc file
                    sf::Http l_httpClient(url.host(), stoi(url.port()));
                    sf::Http::Request httpRequest(url.path());
                    sf::Http::Response response = l_httpClient.sendRequest(httpRequest);

                    std::string xmlDesc = response.getBody();
    
                    // try to get the control URL (where to send future XML requests)
                    std::string controlURL = xmlExtract(xmlDesc, "root/yamaha:X_device/yamaha:X_serviceList/yamaha:X_service/yamaha:X_controlURL");

                    // if unable to get, then it is not a Yamaha device, ignore it
                    // else extract the device id, and add it to the list if not already in
                    if(controlURL.find(ERROR_XML_) >= controlURL.size()) {
                        std::string modelName = xmlExtract(xmlDesc, "root/device/modelName");
                        TDeviceId id;
                        id.DeviceName = modelName;
                        id.DeviceIP = url.host();
                        id.CtrlURL = controlURL;

                        if (!deviceList.exist(id))
                            deviceList.devices.push_back(id);
                    }
                }
            }
        }

        return SUCCESS;
    }
}
