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

#include "yncc-gtk-window.h"
#include <string>
#include "../yncc/yncc.h"
#include "../yncapi/xml_requests.h"
#include <algorithm>
#include <SFML/System.hpp>
#include "MACAddressUtility.h"
#include <libconfig.h++>

yncc_gtk::Yncc_Gtk_Window::Yncc_Gtk_Window() {
    yncapi::Device::initialize();

    builder = Gtk::Builder::create_from_file(GLADE_FILE);

    loadWidgets();
    initWidgets();

    if(loadDeviceList()) {
        refreshDeviceList();
        autoConnect();
    }
}

yncc_gtk::Yncc_Gtk_Window::~Yncc_Gtk_Window() {
}

Gtk::Window* yncc_gtk::Yncc_Gtk_Window::getGtkWindowPtr() {
    return window;
}

void yncc_gtk::Yncc_Gtk_Window::loadMainPanel() {
    builder->get_widget("notebook_Main",        notebook_Main);
    builder->get_widget("button_Power",         button_Power);
    builder->get_widget("button_PureDirect",    button_PureDirect);
    builder->get_widget("button_Eject",         button_Eject);

    page_Source = notebook_Main->get_nth_page(PAGE_SOURCE);
    page_List   = notebook_Main->get_nth_page(PAGE_LIST);
    page_Server = notebook_Main->get_nth_page(PAGE_SERVER);
    page_CD     = notebook_Main->get_nth_page(PAGE_CD);
    page_Radio  = notebook_Main->get_nth_page(PAGE_RADIO);
    page_Options= notebook_Main->get_nth_page(PAGE_OPTIONS);

    notebook_Main->signal_switch_page().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::page_switched));
    button_Power->signal_pressed().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Power_pressed));
    button_PureDirect->signal_pressed().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_PureDirect_clicked));
    button_Eject->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Eject_clicked));
}

void yncc_gtk::Yncc_Gtk_Window::loadDevicePanel() {
    Gtk::Button* buttonConnect;
    Gtk::Label* labelDevice;
    Gtk::CheckButton* checkRemember;
    Gtk::ListBoxRow* rowDevice;
    Gtk::CheckButton* checkAutoConnect;

    for(unsigned int i = 0; i < 7; ++i) {
        builder->get_widget("button_Connect_" + std::to_string(i), buttonConnect);
        builder->get_widget("label_Device_" + std::to_string(i), labelDevice);
        builder->get_widget("check_Remember_" + std::to_string(i), checkRemember);
        builder->get_widget("row_Device_" + std::to_string(i), rowDevice);
        builder->get_widget("check_AutoConnect_" + std::to_string(i), checkAutoConnect);

        deviceConnectButtons.push_back(buttonConnect);
        deviceLabels.push_back(labelDevice);
        deviceRememberCheckboxes.push_back(checkRemember);
        deviceRows.push_back(rowDevice);
        deviceAutoConnectCheckboxes.push_back(checkAutoConnect);

        checkAutoConnect->signal_released().connect(sigc::bind<unsigned int>(sigc::mem_fun(*this, &Yncc_Gtk_Window::check_AutoConnect_clicked), i));
        checkRemember->signal_released().connect(sigc::bind<unsigned int>(sigc::mem_fun(*this, &Yncc_Gtk_Window::check_Remember_clicked), i));
    }

    builder->get_widget("button_Discover", button_Discover);
    button_Discover->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Discover_clicked));
}

void yncc_gtk::Yncc_Gtk_Window::loadSourcePanel() {
    builder->get_widget("button_Server",        button_Server);
    builder->get_widget("button_CD",            button_CD);
    builder->get_widget("button_Radio",         button_Radio);
    builder->get_widget("button_AirPlay",       button_AirPlay);
    builder->get_widget("button_Spotify",       button_Spotify);

    button_Server->signal_pressed().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Server_pressed));
    button_CD->signal_pressed().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_CD_pressed));
    button_Radio->signal_pressed().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Radio_pressed));
}

void yncc_gtk::Yncc_Gtk_Window::loadListPanel() {
    builder->get_widget("list_CurrentList",     list_CurrentList);
    builder->get_widget("Line_0",               row_Line_0);
    builder->get_widget("Line_1",               row_Line_1);
    builder->get_widget("Line_2",               row_Line_2);
    builder->get_widget("Line_3",               row_Line_3);
    builder->get_widget("Line_4",               row_Line_4);
    builder->get_widget("Line_5",               row_Line_5);
    builder->get_widget("Line_6",               row_Line_6);
    builder->get_widget("Line_7",               row_Line_7);

    builder->get_widget("button_ListGoBack",    button_ListGoBack);
    builder->get_widget("button_ListUp",        button_ListUp);
    builder->get_widget("button_ListDown",      button_ListDown);

    Gtk::Widget* wid = nullptr;

    for(unsigned int i = 0; i < 8; ++i) {
        builder->get_widget("label_Line_" + std::to_string(i), wid);
        opt_List_Labels.push_back((Gtk::Label*)wid);
        builder->get_widget("Line_" + std::to_string(i), wid);
        opt_List_Rows.push_back((Gtk::ListBoxRow*)wid);
        builder->get_widget("img_dir_" + std::to_string(i), wid);
        opt_List_ImgDir.push_back((Gtk::Image*)wid);
        builder->get_widget("img_file_" + std::to_string(i), wid);
        opt_List_ImgFile.push_back((Gtk::Image*)wid);
    }

    builder->get_widget("level_ListProgress",       level_ListProgress);

    list_CurrentList->signal_row_activated().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::row_activated));
    button_ListGoBack->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_ListGoBack_clicked));
    button_ListUp->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_ListUp_clicked));
    button_ListDown->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_ListDown_clicked));
}

void yncc_gtk::Yncc_Gtk_Window::loadServerPanel() {
    builder->get_widget("label_ArtistServer",       label_ArtistServer);
    builder->get_widget("label_TimeElapsedServer",  label_TimeElapsedServer);
    builder->get_widget("label_TitleServer",        label_TitleServer);
    builder->get_widget("button_PlayServer",        button_PlayServer);
    builder->get_widget("button_StopServer",        button_StopServer);
    builder->get_widget("button_PauseServer",       button_PauseServer);
    builder->get_widget("button_NextServer",        button_NextServer);
    builder->get_widget("button_PreviousServer",    button_PreviousServer);
    builder->get_widget("button_ShuffleServer",     button_ShuffleServer);
    builder->get_widget("button_RepeatServer",      button_RepeatServer);
    builder->get_widget("label_ShuffleModeServer",  label_ShuffleModeServer);
    builder->get_widget("label_RepeatModeServer",   label_RepeatModeServer);

    button_PlayServer->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Play_clicked));
    button_StopServer->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Stop_clicked));
    button_PauseServer->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Pause_clicked));
    button_NextServer->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Next_clicked));
    button_PreviousServer->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Previous_clicked));
    button_ShuffleServer->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Shuffle_clicked));
    button_RepeatServer->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Repeat_clicked));
}

void yncc_gtk::Yncc_Gtk_Window::loadCDPanel() {
    builder->get_widget("label_Tracks",         label_Tracks);
    builder->get_widget("label_TimeElapsed",    label_TimeElapsed);
    builder->get_widget("button_PlayCD",        button_PlayCD);
    builder->get_widget("button_StopCD",        button_StopCD);
    builder->get_widget("button_PauseCD",       button_PauseCD);
    builder->get_widget("button_NextCD",        button_NextCD);
    builder->get_widget("button_PreviousCD",    button_PreviousCD);
    builder->get_widget("button_RewindCD",      button_RewindCD);
    builder->get_widget("button_ForwardCD",     button_ForwardCD);
    builder->get_widget("button_ShuffleCD",     button_ShuffleCD);
    builder->get_widget("button_RepeatCD",      button_RepeatCD);
    builder->get_widget("label_ShuffleModeServer",  label_ShuffleModeCD);
    builder->get_widget("label_RepeatModeServer",   label_RepeatModeCD);

    button_PlayCD->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Play_clicked));
    button_StopCD->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Stop_clicked));
    button_PauseCD->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Pause_clicked));
    button_NextCD->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Next_clicked));
    button_PreviousCD->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Previous_clicked));
    button_RewindCD->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Rewind_clicked));
    button_ForwardCD->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Forward_clicked));
    button_ShuffleCD->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Shuffle_clicked));
    button_RepeatCD->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Repeat_clicked));
}

void yncc_gtk::Yncc_Gtk_Window::loadRadioPanel() {
    builder->get_widget("label_RadioStation",       label_RadioStation);
    builder->get_widget("label_RadioSong",          label_RadioSong);
    builder->get_widget("button_Listen",            button_Listen);

    button_Listen->signal_pressed().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_Listen_pressed));
}

void yncc_gtk::Yncc_Gtk_Window::loadOptionPanel() {
    builder->get_widget("label_Model",          label_Model);
    builder->get_widget("label_SysId",          label_SysId);
    builder->get_widget("label_Version",        label_Version);
    builder->get_widget("label_Features",       label_Features);
    builder->get_widget("label_vYnccGtk",       label_vYnccGtk);
    builder->get_widget("label_vYncapi",        label_vYncapi);
    builder->get_widget("button_WebPage",       button_WepPage);
    builder->get_widget("button_Licence",       button_Licence);
    builder->get_widget("text_NetworkName",     text_NetworkName);
    builder->get_widget("button_NetworkName",   button_NetworkName);
    builder->get_widget("check_EnableMacFilter", check_EnableMACFilter);
    builder->get_widget("button_AddThisComputer", button_AddThisComputer);
    builder->get_widget("label_IP",             label_IP);
    builder->get_widget("label_Subnet",         label_Subnet);
    builder->get_widget("label_Gateway",        label_Gateway);
    builder->get_widget("label_DNS1",           label_DNS1);
    builder->get_widget("label_DNS2",           label_DNS2);
    builder->get_widget("label_MAC",            label_MAC);

    Gtk::Button* buttonValidate;
    Gtk::Button* buttonClear;
    Gtk::Entry* entryAddress;

    for(unsigned int i = 1; i < 6; ++i) {
        builder->get_widget("button_MAC_Number_" + std::to_string(i), buttonValidate);
        builder->get_widget("button_Clear_Number_" + std::to_string(i), buttonClear);
        builder->get_widget("text_MAC_Number_" + std::to_string(i), entryAddress);

        filtersValidateButtons.push_back(buttonValidate);
        filtersClearButtons.push_back(buttonClear);
        filtersAddresses.push_back(entryAddress);
        filterChanged.push_back(false);
    }

    button_NetworkName->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_NetworkName_clicked));
    button_AddThisComputer->signal_clicked().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_AddThisComputer_clicked));
    check_EnableMACFilter->signal_pressed().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::check_EnableMACFilter_clicked));
    text_NetworkName->signal_changed().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::entry_NetworkName_changed));

    for(unsigned int i = 0; i < 5; ++i) {
        filtersValidateButtons[i]->signal_clicked().connect(sigc::bind<std::string>(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_MAC_AddFilter_clicked), to_string(i + 1)));
        filtersClearButtons[i]->signal_clicked().connect(sigc::bind<std::string>(sigc::mem_fun(*this, &Yncc_Gtk_Window::button_MAC_ClearFilter_clicked), to_string(i)));
        filtersAddresses[i]->signal_changed().connect(sigc::bind<unsigned int>(sigc::mem_fun(*this, &Yncc_Gtk_Window::entry_MACAddress_changed), i));
    }

    refreshVersion();                               // refresh version numbers once here
    button_WepPage->set_label("GitHub");            // unable to set label with Glade (?)
    button_Licence->set_label("Licence");
}

void yncc_gtk::Yncc_Gtk_Window::loadWidgets() {
    builder->get_widget("winYnccGtk", window);
    window->signal_delete_event().connect(sigc::mem_fun(*this, &Yncc_Gtk_Window::deleteWindow));

    loadMainPanel();
    loadDevicePanel();
    loadSourcePanel();
    loadListPanel();
    loadServerPanel();
    loadCDPanel();
    loadRadioPanel();
    loadOptionPanel();
}

void yncc_gtk::Yncc_Gtk_Window::initWidgets() {
    //
    // Disable AirPlay and Spotify buttons since these
    // features are not yet implemented
    //
    button_AirPlay->set_sensitive(false);
    button_Spotify->set_sensitive(false);

    //
    // Hide player's pages at startup, instead of
    // randomly choose one to show
    //
    page_Server->hide();
    page_CD->hide();
    page_Radio->hide();

    // Disable all widgets because we are not connected
    // yet to any device
    //
    disableAllWidgets();

    button_Power->set_sensitive(false);
}

void yncc_gtk::Yncc_Gtk_Window::disableAllWidgets() {
    page_Source->set_sensitive(false);
    page_List->set_sensitive(false);
    page_Options->set_sensitive(false);
    page_Server->set_sensitive(false);
    page_CD->set_sensitive(false);
    page_Radio->set_sensitive(false);
    button_PureDirect->set_sensitive(false);
    button_Eject->set_sensitive(false);
}

void yncc_gtk::Yncc_Gtk_Window::enableAllWidgets() {
    page_Source->set_sensitive();
    page_List->set_sensitive();
    page_Options->set_sensitive();
    page_Server->set_sensitive();
    page_CD->set_sensitive();
    page_Radio->set_sensitive();
    button_PureDirect->set_sensitive();
    button_Eject->set_sensitive();
}

bool yncc_gtk::Yncc_Gtk_Window::deleteWindow(GdkEventAny* event) {
    saveDeviceList();

    if(slotRefreshGui.connected())
        slotRefreshGui.disconnect();

    delete window;

    return true;
}

void yncc_gtk::Yncc_Gtk_Window::button_Eject_clicked() {
    yncapi::setTrayOpenClose();
}

void yncc_gtk::Yncc_Gtk_Window::button_Power_pressed() {
    if(button_Power->get_active()) {
        button_Power->set_active(false);
        yncapi::setSystemPowerStandby();
    } else
        yncapi::setSystemPowerOn();
}

void yncc_gtk::Yncc_Gtk_Window::button_PureDirect_clicked() {
    if(yncapi::Device::SystemSound.PureDirectMode == OFF)
        yncapi::setPureDirectModeOn();
    else
        yncapi::setPureDirectModeOff();
}

void yncc_gtk::Yncc_Gtk_Window::button_Server_pressed() {
    if(!button_Server->get_active())
        yncapi::setInputServer();

    //
    // Only one player button must be active at a time
    //
    button_Server->set_active(false);
    button_CD->set_active(false);
    button_Radio->set_active(false);
}

void yncc_gtk::Yncc_Gtk_Window::button_CD_pressed() {
    if(!button_CD->get_active())
        yncapi::setInputCd();

    //
    // Only one player button must be active at a time
    //
    button_Server->set_active(false);
    button_CD->set_active(false);
    button_Radio->set_active(false);
}

void yncc_gtk::Yncc_Gtk_Window::button_Radio_pressed() {
    if(!button_Radio->get_active())
        yncapi::setInputNetRadio();

    //
    // Only one player button must be active at a time
    //
    button_Server->set_active(false);
    button_CD->set_active(false);
    button_Radio->set_active(false);
}

void yncc_gtk::Yncc_Gtk_Window::button_AirPlay_pressed() {
    std::cerr << "Not yet implemented..." << endl;
    button_AirPlay->set_active(false);
}

void yncc_gtk::Yncc_Gtk_Window::button_Spotify_pressed() {
    std::cerr << "Not yet implemented..." << endl;
    button_Spotify->set_active(false);
}

void yncc_gtk::Yncc_Gtk_Window::button_Play_clicked() {
    yncapi::setPlayerCtrlPlay();
}

void yncc_gtk::Yncc_Gtk_Window::button_Stop_clicked() {
    yncapi::setPlayerCtrlStop();
}

void yncc_gtk::Yncc_Gtk_Window::button_Pause_clicked() {
    yncapi::setPlayerCtrlPause();
}

void yncc_gtk::Yncc_Gtk_Window::button_Next_clicked() {
    yncapi::setPlayerCtrlNext();
}

void yncc_gtk::Yncc_Gtk_Window::button_Previous_clicked() {
    yncapi::setPlayerCtrlPrev();
}

void yncc_gtk::Yncc_Gtk_Window::button_Rewind_clicked() {
    std::cerr << "Not yet implemented..." << endl;
}

void yncc_gtk::Yncc_Gtk_Window::button_Forward_clicked() {
    std::cerr << "Not yet implemented..." << endl;
}

void yncc_gtk::Yncc_Gtk_Window::button_Shuffle_clicked() {
    yncapi::setPlayerShuffle();
}

void yncc_gtk::Yncc_Gtk_Window::button_Repeat_clicked() {
    yncapi::setPlayerRepeat();
}

void yncc_gtk::Yncc_Gtk_Window::button_Listen_pressed() {
    if(button_Listen->get_active())
        yncapi::setPlayerCtrlStop();
}

void yncc_gtk::Yncc_Gtk_Window::button_ListGoBack_clicked() {
    yncapi::setPlayerListReturn();
}

void yncc_gtk::Yncc_Gtk_Window::button_ListUp_clicked() {
    yncapi::setPlayerJumpPrev();
}

void yncc_gtk::Yncc_Gtk_Window::button_ListDown_clicked() {
    yncapi::setPlayerJumpNext();
}

void yncc_gtk::Yncc_Gtk_Window::row_activated(Gtk::ListBoxRow* row) {
    if(row == row_Line_0) {
        yncapi::setPlayerSelection(1);
        return;
    }

    if(row == row_Line_1) {
        yncapi::setPlayerSelection(2);
        return;
    }

    if(row == row_Line_2) {
        yncapi::setPlayerSelection(3);
        return;
    }

    if(row == row_Line_3) {
        yncapi::setPlayerSelection(4);
        return;
    }

    if(row == row_Line_4) {
        yncapi::setPlayerSelection(5);
        return;
    }

    if(row == row_Line_5) {
        yncapi::setPlayerSelection(6);
        return;
    }

    if(row == row_Line_6) {
        yncapi::setPlayerSelection(7);
        return;
    }

    if(row == row_Line_7) {
        yncapi::setPlayerSelection(8);
        return;
    }
}

void yncc_gtk::Yncc_Gtk_Window::refreshList() {
    for(unsigned int i = 0 ; i < yncapi::Device::ListInfo.CurrentList.size(); ++i)
        refreshListLine(i, yncapi::Device::ListInfo.CurrentList[i].Text, yncapi::Device::ListInfo.CurrentList[i].Attr);
}

void yncc_gtk::Yncc_Gtk_Window::refreshListLine(int lineNum, std::string text, std::string attr) {
    //
    // Check if the line text is not "<empty>" and hide if true
    //
    if(text != "<empty>") {
        trunc(text, TRUNC_SIZE_LIST);

        opt_List_Labels[lineNum]->set_text(text);
        opt_List_Rows[lineNum]->set_activatable(true);
        opt_List_Rows[lineNum]->set_selectable(true);
        opt_List_Rows[lineNum]->show();

        if(attr == "Container") {
            opt_List_ImgDir[lineNum]->show();
            opt_List_ImgFile[lineNum]->hide();
        } else {
            opt_List_ImgDir[lineNum]->hide();
            opt_List_ImgFile[lineNum]->show();
        }
    } else
        opt_List_Rows[lineNum]->hide();
}

void yncc_gtk::Yncc_Gtk_Window::updateListProgress() {
    int cur = ceil(stof(yncapi::Device::ListInfo.CursorPosition.CurrentLine) / 8) - 1;
    int max = ceil(stof(yncapi::Device::ListInfo.CursorPosition.MaxLine) / 8) - 1;

    level_ListProgress->set_min_value(0);
    level_ListProgress->set_max_value(max);
    level_ListProgress->set_value(cur);
}

std::string yncc_gtk::Yncc_Gtk_Window::convertPlayTime(const std::string& playTime) {
    int timeElapsed = stoi(playTime);
    float ftimeElapsed = timeElapsed / 60;
    int minutes = ftimeElapsed;
    int seconds = timeElapsed - minutes * 60;

    if(seconds < 0)
        seconds = 0;

    string strMinutes = to_string(minutes);
    string strSeconds = to_string(seconds);

    if(minutes < 10)
        strMinutes = "0" + strMinutes;

    if(seconds < 10)
        strSeconds = "0" + strSeconds;

    return strMinutes + " : " + strSeconds;
}

std::string yncc_gtk::Yncc_Gtk_Window::noEmpty(const std::string& str) {
    if(str == "<empty>")
        return string();

    return str;
}

void yncc_gtk::Yncc_Gtk_Window::trunc(std::string& str, unsigned int truncSize) {
    if(str.size() > truncSize) {
        str.resize(truncSize - 3);
        str.append("...");
    }
}

bool yncc_gtk::Yncc_Gtk_Window::refreshAll(bool flagForceRefresh) {
    refreshAny(TITLE_BAR);
    refreshAny(PAGE_SOURCE, flagForceRefresh);
    refreshAny(PAGE_LIST, flagForceRefresh);
    refreshAny(PAGE_SERVER, flagForceRefresh);
    refreshAny(PAGE_CD, flagForceRefresh);
    refreshAny(PAGE_RADIO, flagForceRefresh);
    refreshAny(PAGE_OPTIONS, flagForceRefresh);

    return true;
}

void yncc_gtk::Yncc_Gtk_Window::refreshAny(int page, bool flagForceRefresh) {
    if(isConnected())
        switch(page) {
            case PAGE_SOURCE    :
                refreshPageSource(flagForceRefresh);
                break;

            case PAGE_LIST      :
                refreshPageList(flagForceRefresh);
                break;

            case PAGE_SERVER    :
                refreshPageServer(flagForceRefresh);
                break;

            case PAGE_CD        :
                refreshPageCD(flagForceRefresh);
                break;

            case PAGE_RADIO     :
                refreshPageRadio(flagForceRefresh);
                break;

            case PAGE_OPTIONS   :
                refreshPageOptions(flagForceRefresh);
                break;

            case TITLE_BAR      :
                refreshTitleBar();
                break;

            default             :
                std::cerr << "Unknown page..." << page << endl;
        }
}

bool yncc_gtk::Yncc_Gtk_Window::statusRefreshDetected() {
    return yncapi::DeviceRefreshNotifications::BasicStatus;
}

bool yncc_gtk::Yncc_Gtk_Window::listRefreshDetected() {
    return yncapi::DeviceRefreshNotifications::ListInfo;
}

bool yncc_gtk::Yncc_Gtk_Window::playRefreshDetected() {
    return yncapi::DeviceRefreshNotifications::PlayInfo;
}

bool yncc_gtk::Yncc_Gtk_Window::optionRefreshDetected() {
    return yncapi::DeviceRefreshNotifications::OptionInfo;
}

void yncc_gtk::Yncc_Gtk_Window::refreshPageSource(bool flagForceRefresh) {
    yncapi::getBasicStatus();

    if(statusRefreshDetected() || flagForceRefresh) {
        if(yncapi::Device::SystemBasicStatus.PowerControl->Power == ON) {
            button_Power->set_active(true);
            enableAllWidgets();
        } else {
            button_Power->set_active(false);
            disableAllWidgets();
        }

        if(yncapi::Device::SystemSound.PureDirectMode == ON)
            button_PureDirect->set_active(true);
        else
            button_PureDirect->set_active(false);

        int lastPage = notebook_Main->get_current_page();

        if(yncapi::Device::SystemInput.InputSel == SERVER) {
            button_Server->set_active(true);
            page_Server->show();

            if(lastPage == PAGE_CD || lastPage == PAGE_RADIO)
                notebook_Main->set_current_page(PAGE_SERVER);
        } else {
            button_Server->set_active(false);
            page_Server->hide();
        }

        if(yncapi::Device::SystemInput.InputSel == CD) {
            button_CD->set_active(true);
            page_CD->show();

            if(lastPage == PAGE_SERVER || lastPage == PAGE_RADIO)
                notebook_Main->set_current_page(PAGE_CD);
        } else {
            button_CD->set_active(false);
            page_CD->hide();
        }

        if(yncapi::Device::SystemInput.InputSel == NETRADIO) {
            button_Radio->set_active(true);
            page_Radio->show();

            if(lastPage == PAGE_SERVER || lastPage == PAGE_CD)
                notebook_Main->set_current_page(PAGE_RADIO);
        } else {
            button_Radio->set_active(false);
            page_Radio->hide();
        }

        if(yncapi::Device::SystemInput.InputSel == AIRPLAY)
            button_AirPlay->set_active(true);
        else
            button_AirPlay->set_active(false);

        if(yncapi::Device::SystemInput.InputSel == SPOTIFY)
            button_Spotify->set_active(true);
        else
            button_Spotify->set_active(false);
    }
}

void yncc_gtk::Yncc_Gtk_Window::refreshPageList(bool flagForceRefresh) {
    if(notebook_Main->get_current_page() == PAGE_LIST || flagForceRefresh) {
        //
        // Sometimes, the network player says menu status is "busy" instead of "ready".
        // In this case, we will periodically pull list info from player, but WITHOUT
        // building the whole list (NO_LIST) to increase speed, and update the GUI while
        // waiting for "ready" status
        //
        while(yncapi::Device::ListInfo.MenuStatus == "Busy" && yncapi::Device::SystemInput.InputSel != CD) {
            while(Gtk::Main::events_pending())
                Gtk::Main::iteration();

            yncapi::getListInfo(NO_LIST);
            sf::sleep(sf::milliseconds(1));
        }

        //
        // Then we can retrieve the whole list and proceed the refresh
        //
        yncapi::getListInfo();
        
        bool done;
        
        list_CurrentList->unselect_row();
        
        for(unsigned int i = 0, done = false; i < yncapi::Device::ListInfo.CurrentList.size() && !done; ++i) {
            if (yncapi::Device::ListInfo.CurrentList[0].Attr != "Container") {
                if (yncapi::Device::PlayInfo.MetaInfo.Song.compare(0, TRUNC_SIZE_LIST, yncapi::Device::ListInfo.CurrentList[i].Text, 0, TRUNC_SIZE_LIST) == 0) {
                    list_CurrentList->select_row(*opt_List_Rows[i]);
                    done = true;
                }
            } else
                done = true;
        }

        if(listRefreshDetected() || flagForceRefresh) {
            refreshList();
            updateListProgress();

            yncapi::DeviceRefreshNotifications::ListInfo = false;
        }
    }
}

void yncc_gtk::Yncc_Gtk_Window::refreshPageServer(bool flagForceRefresh) {
    if(notebook_Main->get_current_page() == PAGE_SERVER || flagForceRefresh) {
        if(playRefreshDetected() || flagForceRefresh) {
            if(yncapi::Device::PlayInfo.PlaybackInfo == "Play" || yncapi::Device::PlayInfo.PlaybackInfo == "Pause") {
                std::string artist = noEmpty(yncapi::Device::PlayInfo.MetaInfo.Artist);
                std::string album = noEmpty(yncapi::Device::PlayInfo.MetaInfo.Album);

                trunc(artist);
                trunc(album);

                label_ArtistServer->set_text(artist + " / " + album);
                label_TitleServer->set_text(titlebarSong);
                label_TimeElapsedServer->set_text(titlebarTime);
            } else {
                label_ArtistServer->set_text("");
                label_TitleServer->set_text("");
                label_TimeElapsedServer->set_text("");
            }

            refreshShuffleRepeat(label_ShuffleModeServer, label_RepeatModeServer);

            yncapi::DeviceRefreshNotifications::PlayInfo = false;
        }
    }
}

void yncc_gtk::Yncc_Gtk_Window::refreshPageCD(bool flagForceRefresh) {
    if(notebook_Main->get_current_page() == PAGE_CD || flagForceRefresh) {
        if(playRefreshDetected() || flagForceRefresh) {
            label_Tracks->set_text(yncapi::Device::PlayInfo.TrackNum + " / " + yncapi::Device::PlayInfo.TotalTracks);
            label_TimeElapsed->set_text(titlebarTime);

            refreshShuffleRepeat(label_ShuffleModeCD, label_RepeatModeCD);

            yncapi::DeviceRefreshNotifications::PlayInfo = false;
        }
    }
}

void yncc_gtk::Yncc_Gtk_Window::refreshPageRadio(bool flagForceRefresh) {
    if(notebook_Main->get_current_page() == PAGE_RADIO || flagForceRefresh) {
        if(playRefreshDetected() || flagForceRefresh) {
            std::string station = noEmpty(yncapi::Device::PlayInfo.MetaInfo.Artist);

            trunc(station);

            label_RadioStation->set_text(station);
            label_RadioSong->set_text(titlebarSong);
        }

        if(yncapi::Device::PlayInfo.PlaybackInfo == "Play")
            button_Listen->set_active(true);
        else
            button_Listen->set_active(false);

        yncapi::DeviceRefreshNotifications::PlayInfo = false;
    }
}

void yncc_gtk::Yncc_Gtk_Window::refreshVersion() {
    string str_vYnccGtk = std::to_string(YNCC_GTK_MAJOR_VERSION) + "." +
                          std::to_string(YNCC_GTK_MINOR_VERSION) + "." +
                          std::to_string(YNCC_GTK_PATCH_VERSION) +
                          std::string(YNCC_GTK_PRE_RELEASE);

    string str_vYncapi = std::to_string(YNCAPI_MAJOR_VERSION) + "." +
                         std::to_string(YNCAPI_MINOR_VERSION) + "." +
                         std::to_string(YNCAPI_PATCH_VERSION) +
                         std::string(YNCAPI_PRE_RELEASE);

    label_vYnccGtk->set_text(str_vYnccGtk);
    label_vYncapi->set_text(str_vYncapi);
}

void yncc_gtk::Yncc_Gtk_Window::refreshPageOptions(bool flagForceRefresh) {
    if(notebook_Main->get_current_page() == PAGE_OPTIONS || flagForceRefresh) {
        //
        // We will refresh some static data only on the first call,
        // as long as these parameters will not change until the next connection
        //
        if(firstTime) {
            yncapi::getSystemConfig();

            label_Model->set_text(yncapi::Device::SystemConfig.ModelName);
            label_SysId->set_text(yncapi::Device::SystemConfig.SystemID);
            label_Version->set_text(yncapi::Device::SystemConfig.Version);

            std::string strf;

            for(unsigned int i = 0; i < yncapi::Device::SystemConfig.Features.size(); ++i) {
                strf.append(yncapi::Device::SystemConfig.Features[i]);

                if(i != (yncapi::Device::SystemConfig.Features.size() - 1))
                    strf.append(",");
            }

            std::transform(strf.begin(), strf.end(), strf.begin(), ::tolower);

            label_Features->set_text(strf);

            firstTime = false;
        }

        yncapi::getNetworkInfo();

        if(optionRefreshDetected() || flagForceRefresh) {
            text_NetworkName->set_text(yncapi::Device::NetworkInfo.NetworkName);

            for(unsigned int i = 0; i < 5; ++i)
                if(!filterChanged[i])
                    filtersAddresses[i]->set_text(yncapi::Device::NetworkInfo.MACFilters[i]);

            if(yncapi::Device::NetworkInfo.MACFilterMode == "On") {
                check_EnableMACFilter->set_active(true);
                check_EnableMACFilter->set_sensitive(false);
            } else
                check_EnableMACFilter->set_active(false);

            label_IP->set_text(yncapi::Device::NetworkInfo.IPAddress);
            label_Subnet->set_text(yncapi::Device::NetworkInfo.SubnetMask);
            label_Gateway->set_text(yncapi::Device::NetworkInfo.Gateway);
            label_DNS1->set_text(yncapi::Device::NetworkInfo.DNS1);
            label_DNS2->set_text(yncapi::Device::NetworkInfo.DNS2);
            label_MAC->set_text(yncapi::Device::NetworkInfo.MACAddress);

            yncapi::DeviceRefreshNotifications::OptionInfo = false;
        }
    }
}

void yncc_gtk::Yncc_Gtk_Window::refreshTitleBar() {
    yncapi::getPlayInfo();

    if(yncapi::Device::PlayInfo.PlaybackInfo == "Play" || yncapi::Device::PlayInfo.PlaybackInfo == "Pause") {
        if(button_Radio->get_active()) {
            titlebarSong = noEmpty(yncapi::Device::PlayInfo.MetaInfo.Artist);
            titlebarTime.clear();
            trunc(titlebarSong);
            window->set_title(titlebarSong);
        } else
            if(button_Server->get_active() || button_CD->get_active()) {
                titlebarSong = noEmpty(yncapi::Device::PlayInfo.MetaInfo.Song);
                titlebarTime = convertPlayTime(yncapi::Device::PlayInfo.PlayTime);
                trunc(titlebarSong);
                window->set_title(titlebarTime + " - " + titlebarSong);
            }
    } else {
        window->set_title("Yncc");
        titlebarSong.clear();
        titlebarTime.clear();
    }
}

void yncc_gtk::Yncc_Gtk_Window::refreshShuffleRepeat(Gtk::Label* shuffleLabel, Gtk::Label* repeatLabel) {
    string repeatText;

    if(yncapi::Device::PlayInfo.PlayMode.Repeat == "One")
        repeatText += gettext("One");
    else
        if(yncapi::Device::PlayInfo.PlayMode.Repeat == "All")
            repeatText += gettext("All");
        else
            repeatText += yncapi::Device::PlayInfo.PlayMode.Repeat;

    repeatLabel->set_text(repeatText);
    shuffleLabel->set_text(yncapi::Device::PlayInfo.PlayMode.Shuffle);
}

void yncc_gtk::Yncc_Gtk_Window::button_NetworkName_clicked() {
    yncapi::setNetworkName(text_NetworkName->get_text());
    button_NetworkName->set_sensitive(false);
}

void yncc_gtk::Yncc_Gtk_Window::button_MAC_AddFilter_clicked(std::string line) {
    if(isValidMACAddress(filtersAddresses[stoi(line) - 1]->get_text())) {
        yncapi::setMacFilterAddress(line, filtersAddresses[stoi(line) - 1]->get_text());
        filtersValidateButtons[stoi(line) - 1]->set_sensitive(false);
    } else
        filtersAddresses[stoi(line) - 1]->set_text(yncapi::Device::NetworkInfo.MACFilters[stoi(line) - 1]);

    filterChanged[stoi(line) - 1] = false;
}

void yncc_gtk::Yncc_Gtk_Window::button_MAC_ClearFilter_clicked(std::string line) {
    filtersAddresses[stoi(line)]->set_text("000000000000");
}

void yncc_gtk::Yncc_Gtk_Window::button_AddThisComputer_clicked() {
    unsigned char macAddr[6];
    MACAddressUtility::GetMACAddress(macAddr);

    stringstream ssMacAddr;

    for(unsigned int i = 0; i < 6; ++i)
        ssMacAddr << std::hex << (unsigned int)macAddr[i];

    string strMacAddr = ssMacAddr.str();
    std::transform(strMacAddr.begin(), strMacAddr.end(), strMacAddr.begin(), ::toupper);

    //
    // Try to push the MAC address to the first free (00000...) field, then
    // if they are all already occupied, push it to the first of the list
    //
    unsigned int i = 0;

    while(i < 5 && filtersAddresses[i]->get_text() != "000000000000" && filtersAddresses[i]->get_text() != strMacAddr)
        i++;

    if(i == 5)
        i = 0;

    filtersAddresses[i]->set_text(strMacAddr);
}

void yncc_gtk::Yncc_Gtk_Window::check_EnableMACFilter_clicked() {
    if(!check_EnableMACFilter->get_active())
        yncapi::setMacFilterOn();
}

void yncc_gtk::Yncc_Gtk_Window::entry_MACAddress_changed(unsigned int index) {
    if(filtersAddresses[index]->get_text() == yncapi::Device::NetworkInfo.MACFilters[index]) {
        filtersValidateButtons[index]->set_sensitive(false);
        filterChanged[index] = false;
    } else {
        filtersValidateButtons[index]->set_sensitive(true);
        filterChanged[index] = true;
    }
}

void yncc_gtk::Yncc_Gtk_Window::entry_NetworkName_changed() {
    if(text_NetworkName->get_text() != yncapi::Device::NetworkInfo.NetworkName)
        button_NetworkName->set_sensitive(true);
    else
        button_NetworkName->set_sensitive(false);
}

bool yncc_gtk::Yncc_Gtk_Window::isValidMACAddress(const std::string& address) {
    char c;
    unsigned int i = 0;

    while(i < address.size()) {
        c = address[i];

        if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            i++;
        else
            return false;
    }

    return true;
}

void yncc_gtk::Yncc_Gtk_Window::page_switched(Gtk::Widget* page, guint pageNum) {
    refreshAny(pageNum);
}

void yncc_gtk::Yncc_Gtk_Window::button_Discover_clicked() {
    button_Discover->set_sensitive(false);
    yncapi::discover(deviceList);
    button_Discover->set_sensitive(true);

    refreshDeviceList();
}

void yncc_gtk::Yncc_Gtk_Window::connectDevice(unsigned int pos) {
    if(yncapi::deviceAssign(deviceList, pos)) {
        connected = true;

        for(unsigned int i = 0; i < deviceList.devices.size(); ++i) {
            if(deviceList.devices[i].DeviceIP == deviceList.devices[pos].DeviceIP)
                deviceConnectButtons[i]->set_sensitive(false);
            else
                deviceConnectButtons[i]->set_sensitive(true);
        }

        firstTime = true;

        button_Power->set_sensitive(true);
        button_PureDirect->set_sensitive(true);
        button_Eject->set_sensitive(true);
        refreshAll(REFRESH_FORCE);
        slotRefreshGui = Glib::signal_timeout().connect(sigc::bind<bool>(sigc::mem_fun(*this, &Yncc_Gtk_Window::refreshAll), NO_FORCE), REFRESH_INTERVAL);

        page_Source->set_sensitive();
        page_List->set_sensitive();
        page_Options->set_sensitive();
        page_Server->set_sensitive();
        page_CD->set_sensitive();
        page_Radio->set_sensitive();
    }
}

bool yncc_gtk::Yncc_Gtk_Window::isConnected() {
    return connected;
}

void yncc_gtk::Yncc_Gtk_Window::check_AutoConnect_clicked(unsigned int index) {
    if(deviceAutoConnectCheckboxes[index]->get_active()) {
        for(unsigned int i = 0; i < 7; ++i)
            if(i != index) {
                deviceAutoConnectCheckboxes[i]->set_active(false);
                deviceList.devices[i].AutoConnect = false;
            } else
                deviceList.devices[i].AutoConnect = true;
    } else
        deviceList.devices[index].AutoConnect = false;
}

void yncc_gtk::Yncc_Gtk_Window::check_Remember_clicked(unsigned int index) {
    if(deviceRememberCheckboxes[index]->get_active()) {
        deviceAutoConnectCheckboxes[index]->set_sensitive(true);
        deviceList.devices[index].Remember = true;
    } else {
        deviceAutoConnectCheckboxes[index]->set_sensitive(false);
        deviceAutoConnectCheckboxes[index]->set_active(false);
        deviceList.devices[index].Remember = false;
        deviceList.devices[index].AutoConnect = false;
    }
}

bool yncc_gtk::Yncc_Gtk_Window::saveDeviceList() {
    libconfig::Config cfg;
    libconfig::Setting& root = cfg.getRoot();
    libconfig::Setting& devices = root.add("devices", libconfig::Setting::TypeList);

    for(unsigned int i = 0; i < deviceList.devices.size(); ++i) {
        if(deviceList.devices[i].Remember) {
            libconfig::Setting& device = devices.add(libconfig::Setting::TypeGroup);

            device.add("name", libconfig::Setting::TypeString)  = deviceList.devices[i].DeviceName;
            device.add("addr", libconfig::Setting::TypeString)  = deviceList.devices[i].DeviceIP;
            device.add("ctrl", libconfig::Setting::TypeString)  = deviceList.devices[i].CtrlURL;
            device.add("auto", libconfig::Setting::TypeBoolean) = deviceList.devices[i].AutoConnect;
        }
    }

    try {
        std::string config = getenv("HOME") + std::string(CONFIG_FILE);
        cfg.writeFile(config.c_str());
    } catch(const libconfig::FileIOException &fioex) {
        cerr << "I/O error while writing file: " << CONFIG_FILE << endl;
        return false;
    }
    
    return true;
}

bool yncc_gtk::Yncc_Gtk_Window::loadDeviceList() {
    libconfig::Config cfg;

    try {
        std::string config = getenv("HOME") + std::string(CONFIG_FILE);
        cfg.readFile(config.c_str());
    } catch(const libconfig::FileIOException &fioex) {
        std::cerr << "I/O error while reading file:" << CONFIG_FILE << std::endl;
        return(false);
    } catch(const libconfig::ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                  << " - " << pex.getError() << std::endl;
        return(false);
    }

    const libconfig::Setting& root    = cfg.getRoot();
    const libconfig::Setting& devices = root["devices"];

    for(int i = 0; i < devices.getLength(); ++i) {
        const libconfig::Setting& device = devices[i];
        yncapi::TDeviceId devId;
        std::string deviceName;
        std::string deviceAddr;
        std::string deviceCtrl;
        bool        deviceAuto;

        device.lookupValue("name", deviceName);
        device.lookupValue("addr", deviceAddr);
        device.lookupValue("ctrl", deviceCtrl);
        device.lookupValue("auto", deviceAuto);

        devId.DeviceName  = deviceName;
        devId.DeviceIP    = deviceAddr;
        devId.CtrlURL     = deviceCtrl;
        devId.Remember    = true;
        devId.AutoConnect = deviceAuto;

        deviceList.devices.push_back(devId);
    }

    return true;
}

void yncc_gtk::Yncc_Gtk_Window::refreshDeviceList() {
    for(unsigned int i = 0; i < deviceList.devices.size(); ++i) {
        deviceLabels[i]->set_text(deviceList.devices[i].DeviceName);
        deviceConnectButtons[i]->signal_clicked().connect(sigc::bind<unsigned int>(sigc::mem_fun(*this, &Yncc_Gtk_Window::connectDevice), i));
        deviceRows[i]->show();
        deviceRememberCheckboxes[i]->set_active(deviceList.devices[i].Remember);

        if(deviceRememberCheckboxes[i]->get_active())
            deviceAutoConnectCheckboxes[i]->set_sensitive();

        deviceAutoConnectCheckboxes[i]->set_active(deviceList.devices[i].AutoConnect);
    }
}

void yncc_gtk::Yncc_Gtk_Window::autoConnect() {
    for(unsigned int i = 0; i < deviceList.devices.size(); ++i)
        if(deviceList.devices[i].AutoConnect) {
            connectDevice(i);
        }
}
