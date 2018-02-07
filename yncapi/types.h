#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <string>
#include <vector>

#define DEFAULT_DEVICE  "no device"
#define RC_SUCCESS      0

using namespace std;

namespace yncapi {
    typedef struct {
        string ModelName;
        string SystemID;
        string Version;
        vector<string> Features;
    } TSystemConfig;
    
    typedef struct {
        string Power;
        string Saving;
    } TSystemPowerControl;
    
    typedef struct {
        string InputSel;
    } TSystemInput;
    
    typedef struct {
        string PureDirectMode;
    } TSystemSound;
    
    typedef struct {
        TSystemPowerControl* PowerControl;
        TSystemInput* Input;
        TSystemSound* Sound;
    } TSystemBasicStatus;
    
    typedef struct {
        string FeatureAvailability;
    } TPlayerConfig;
    
    typedef struct {
        string Text;
        string Attr;
    } TLine;
    
    typedef struct {
        string CurrentLine;
        string MaxLine;
    } TCursorPosition;
    
    typedef struct {
        string MenuStatus = "Busy";
        string MenuLayer;
        string MenuName;
        vector<TLine> CurrentList;
        TCursorPosition CursorPosition;
    } TListInfo;
    
    typedef struct {
        string Repeat;
        string Shuffle;
    } TPlayMode;
    
    typedef struct {
        string Artist;
        string Album;
        string Song;
    } TMetaInfo;
    
    typedef struct {
        string URL;
        string ID;
        string Format;
    } TAlbumArt;
    
    typedef struct {
        string PlaybackInfo;
        TPlayMode PlayMode;
        string PlayTime;
        string TrackNum;
        string TotalTracks;
        TMetaInfo MetaInfo;
        TAlbumArt AlbumArt;
    } TPlayInfo;
    
    typedef struct {
        string NetworkName;
        string IPAddress;
        string SubnetMask;
        string Gateway;
        string DNS1;
        string DNS2;
        string MACAddress;
        vector<string> MACFilters;
        string MACFilterMode;
    } TNetworkInfo;
    
    typedef unsigned int TReturnCode;
    
    typedef struct {
        string DeviceName;
        string DeviceIP;
        string CtrlURL;
        bool Remember = false;
        bool AutoConnect = false;
    } TDeviceId;
    
    typedef struct {
        std::vector<TDeviceId> devices;
        
        void show() {
            for(unsigned int i = 0; i < devices.size(); ++i)
                std::cout << i << " - " << devices[i].DeviceName << ":" << devices[i].DeviceIP << std::endl;
        }
        
        bool exist(TDeviceId device) {
            for(unsigned int i = 0; i < devices.size(); ++i)
                if (devices[i].DeviceIP == device.DeviceIP)
                    return true;
                    
            return false;
        }
    } TDeviceList;

    struct Device {
        //static string id;
        static TDeviceId id;
        static bool isAssigned();
        static void initialize();
        static void update();
        
        static TSystemConfig SystemConfig;
        static TSystemPowerControl SystemPowerControl;
        static TSystemBasicStatus SystemBasicStatus;
        static TSystemInput SystemInput;
        static TSystemSound SystemSound;
        
        static TPlayerConfig PlayerConfig;
        static TListInfo ListInfo;
        static TPlayInfo PlayInfo;
        
        static TNetworkInfo NetworkInfo;
    };
    
    struct DeviceRefreshNotifications {
        static bool BasicStatus;
        static bool ListInfo;
        static bool PlayInfo;
        static bool OptionInfo;
    };
}

#endif