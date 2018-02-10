/* 
 * Copyright (C) 2018, Alexandre Courtois
 * 
 * This file is part of YNCC-GTK.
 *
 * YNCC-GTK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * YNCC-GTK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with YNCC-GTK.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef YNCC_GTK_WINDOW_H
#define YNCC_GTK_WINDOW_H

#include <gtkmm.h>
#include <vector>
#include <string>
#include <queue>
#include "../yncapi/yncapi.h"

#define _YNCC_GTK_MAJOR_VERSION  0
#define _YNCC_GTK_MINOR_VERSION  18
#define _YNCC_GTK_PATCH_VERSION  1
#define _YNCC_GTK_PRE_RELEASE    ""

static const unsigned int YNCC_GTK_MAJOR_VERSION = _YNCC_GTK_MAJOR_VERSION;
static const unsigned int YNCC_GTK_MINOR_VERSION = _YNCC_GTK_MINOR_VERSION;
static const unsigned int YNCC_GTK_PATCH_VERSION = _YNCC_GTK_PATCH_VERSION;
static const char*        YNCC_GTK_PRE_RELEASE   = _YNCC_GTK_PRE_RELEASE;

#define PAGE_SOURCE                 1
#define PAGE_LIST                   2
#define PAGE_SERVER                 3
#define PAGE_CD                     4
#define PAGE_RADIO                  5
#define PAGE_OPTIONS                6
#define PAGE_MAIN                   9
#define TITLE_BAR                   10

#define REFRESH_INTERVAL            1000
#define TRUNC_SIZE                  25
#define TRUNC_SIZE_LIST             62

#define REFRESH_FORCE               true
#define NO_FORCE                    false

#define CONFIG_FILE                 "/.yncc-gtk.conf"
#define GLADE_FILE                  "/usr/share/ync-tools/yncc-gtk/yncc-gtk.glade"

namespace yncc_gtk {
    class Yncc_Gtk_Window {
        private:
            Glib::RefPtr<Gtk::Builder> builder;
            Gtk::Window* window;

            // --- Main panel --------------
            Gtk::Notebook* notebook_Main;
            Gtk::Widget* page_Source;
            Gtk::Widget* page_Server;
            Gtk::Widget* page_CD;
            Gtk::Widget* page_List;
            Gtk::Widget* page_Radio;
            Gtk::Widget* page_Options;

            // --- Main buttons ---
            Gtk::ToggleButton* button_Power;
            Gtk::ToggleButton* button_PureDirect;
            Gtk::Button* button_Eject;

            // --- Page : Source -----------
            Gtk::ToggleButton* button_Server;
            Gtk::ToggleButton* button_CD;
            Gtk::ToggleButton* button_Radio;
            Gtk::ToggleButton* button_AirPlay;
            Gtk::ToggleButton* button_Spotify;

            // --- Page : List -------------
            Gtk::Button* button_ListGoBack;
            Gtk::Button* button_ListUp;
            Gtk::Button* button_ListDown;
            Gtk::ListBox* list_CurrentList;
            Gtk::ListBoxRow* row_Line_0;
            Gtk::ListBoxRow* row_Line_1;
            Gtk::ListBoxRow* row_Line_2;
            Gtk::ListBoxRow* row_Line_3;
            Gtk::ListBoxRow* row_Line_4;
            Gtk::ListBoxRow* row_Line_5;
            Gtk::ListBoxRow* row_Line_6;
            Gtk::ListBoxRow* row_Line_7;

            std::vector<Gtk::Label*> opt_List_Labels;               //
            std::vector<Gtk::ListBoxRow*> opt_List_Rows;            //  Optimizing access to each line components
            std::vector<Gtk::Image*> opt_List_ImgDir;               //  instead of using builder::get_widget to frequently
            std::vector<Gtk::Image*> opt_List_ImgFile;              //

            Gtk::LevelBar* level_ListProgress;

            // --- Page : Server -----------
            Gtk::Label* label_TitleServer;
            Gtk::Label* label_ArtistServer;
            Gtk::Label* label_TimeElapsedServer;
            Gtk::Button* button_PreviousServer;
            Gtk::Button* button_NextServer;
            Gtk::Button* button_PlayServer;
            Gtk::Button* button_StopServer;
            Gtk::Button* button_PauseServer;
            Gtk::Button* button_ShuffleServer;
            Gtk::Button* button_RepeatServer;
            Gtk::Label* label_ShuffleModeServer;
            Gtk::Label* label_RepeatModeServer;

            // --- Page : CD ---------------
            Gtk::Label* label_Tracks;
            Gtk::Label* label_TimeElapsed;
            Gtk::Button* button_PlayCD;
            Gtk::Button* button_StopCD;
            Gtk::Button* button_PauseCD;
            Gtk::Button* button_NextCD;
            Gtk::Button* button_PreviousCD;
            Gtk::Button* button_RewindCD;
            Gtk::Button* button_ForwardCD;
            Gtk::Button* button_ShuffleCD;
            Gtk::Button* button_RepeatCD;
            Gtk::Label* label_ShuffleModeCD;
            Gtk::Label* label_RepeatModeCD;

            // --- Page : Radio ------------
            Gtk::Label* label_RadioStation;
            Gtk::Label* label_RadioSong;
            Gtk::ToggleButton* button_Listen;

            // --- Page : Options ----------
            Gtk::Label* label_Model;
            Gtk::Label* label_SysId;
            Gtk::Label* label_Version;
            Gtk::Label* label_Features;
            Gtk::Label* label_vYnccGtk;
            Gtk::Label* label_vYncapi;
            Gtk::LinkButton* button_WepPage;
            Gtk::LinkButton* button_Licence;
            Gtk::Entry* text_NetworkName;
            Gtk::Button* button_NetworkName;
            Gtk::CheckButton* check_EnableMACFilter;
            Gtk::Button* button_AddThisComputer;
            Gtk::Label* label_IP;
            Gtk::Label* label_Subnet;
            Gtk::Label* label_Gateway;
            Gtk::Label* label_DNS1;
            Gtk::Label* label_DNS2;
            Gtk::Label* label_MAC;

            std::vector<Gtk::Entry*>  filtersAddresses;
            std::vector<Gtk::Button*> filtersValidateButtons;
            std::vector<Gtk::Button*> filtersClearButtons;

            // --- Page : Devices ----------
            Gtk::Button* button_Discover;
            std::vector<Gtk::Button*> deviceConnectButtons;
            std::vector<Gtk::CheckButton*> deviceRememberCheckboxes;
            std::vector<Gtk::Label*> deviceLabels;
            std::vector<Gtk::ListBoxRow*> deviceRows;
            std::vector<Gtk::CheckButton*> deviceAutoConnectCheckboxes;
            
            // --- Title Bar ---------------
            std::string titlebarSong;
            std::string titlebarTime;

            // --- Load & Init -------------
            void loadMainPanel();
            void loadDevicePanel();
            void loadSourcePanel();
            void loadListPanel();
            void loadServerPanel();
            void loadCDPanel();
            void loadRadioPanel();
            void loadOptionPanel();

            void refreshVersion();

            void loadWidgets();
            void initWidgets();

            void disableAllWidgets();
            void enableAllWidgets();

            bool deleteWindow(GdkEventAny* event);

            // --- Slots -------------------
            void button_Eject_clicked();
            void button_Power_pressed();
            void button_PureDirect_clicked();

            void button_Server_pressed();
            void button_CD_pressed();
            void button_Radio_pressed();
            void button_AirPlay_pressed();
            void button_Spotify_pressed();

            void button_Play_clicked();
            void button_Stop_clicked();
            void button_Pause_clicked();
            void button_Next_clicked();
            void button_Previous_clicked();
            void button_Rewind_clicked();
            void button_Forward_clicked();
            void button_Shuffle_clicked();
            void button_Repeat_clicked();
            void button_Listen_pressed();

            void button_ListGoBack_clicked();
            void button_ListUp_clicked();
            void button_ListDown_clicked();
            void row_activated(Gtk::ListBoxRow* row);
            void page_switched(Gtk::Widget* page, guint pageNum);

            void button_NetworkName_clicked();
            void button_MAC_AddFilter_clicked(std::string line);
            void button_MAC_ClearFilter_clicked(std::string line);
            void button_AddThisComputer_clicked();
            void check_EnableMACFilter_clicked();
            void entry_MACAddress_changed(unsigned int index);
            void entry_NetworkName_changed();

            void button_Discover_clicked();
            void check_AutoConnect_clicked(unsigned int index);
            void check_Remember_clicked(unsigned int index);

            // --- List processing ---------
            void refreshList();
            void updateListProgress();

            // --- Time processing ---------
            std::string convertPlayTime(const std::string& playTime);

            // --- Some tricks -------------
            std::string noEmpty(const std::string& str);
            void trunc(std::string& str, unsigned int truncSize = TRUNC_SIZE);
            bool firstTime = true;
            bool isValidMACAddress(const std::string& address);
            bool connected = false;
            yncapi::TDeviceList deviceList;
            void connectDevice(unsigned int pos);
            bool isConnected();
            void autoConnect();
            bool loadDeviceList();
            bool saveDeviceList();
            std::vector<bool> filterChanged;

            // --- Refresh methods ---------
            bool refreshAll(bool flagForceRefresh = NO_FORCE);
            void refreshAny(int page, bool flagForceRefresh = NO_FORCE);

            void refreshMainButtons(bool flagForceRefresh = NO_FORCE);
            void refreshPageSource(bool flagForceRefresh = NO_FORCE);
            void refreshPageList(bool flagForceRefresh = NO_FORCE);
            void refreshPageServer(bool flagForceRefresh = NO_FORCE);
            void refreshPageCD(bool flagForceRefresh = NO_FORCE);
            void refreshPageRadio(bool flagForceRefresh = NO_FORCE);
            void refreshPageOptions(bool flagForceRefresh = NO_FORCE);
            void refreshTitleBar();

            bool statusRefreshDetected();
            bool listRefreshDetected();
            bool playRefreshDetected();
            bool optionRefreshDetected();

            sigc::connection slotRefreshGui;

            void refreshListLine(int line, std::string text, std::string attr);
            void refreshShuffleRepeat(Gtk::Label* shuffleLabel, Gtk::Label* repeatLabel);
            void refreshDeviceList();

        public:
            Yncc_Gtk_Window();
            ~Yncc_Gtk_Window();

            Gtk::Window* getGtkWindowPtr();
    };
}

#endif
