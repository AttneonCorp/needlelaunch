/*
* ΔLaunch
* Copyright (C) 2018  Reisyukaku
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "App.hpp"
// Not yet working function
//Result appletRequestLaunchApplication(u64 titleID, AppletStorage* s);
AppletHolder App::currentApplication;
AppletHolder App::currentApplet;
u32 App::appletSelectInd;
u32 App::gameSelectInd;
u8 App::dashLayer;

u32 App::lastAeCmd;
u32 App::lastSamsCmd;
bool App::is_active; // Application one
bool App::is_active_applet; // Applet one
AppletApplication App::app;
/*
*   Application
*/
Result App::LaunchGame(u64 tid, u128 userID) {
	Result rc = -1;
	rc = App::LaunchGameTrue(tid, userID, App::app);
	return rc;
}
Result App::LaunchGameTrue(u64 tid, u128 userID, AppletApplication app) {
    Result rc = 0;
    #ifdef __SWITCH__
    AppletStorage aStore;
    rc = appletCreateApplication(&app, tid);
    if(R_FAILED(rc)) {
        ShowError("Error launching game. The error is shown below:\n", "Error initializing handle", rc);
    }
    rc = appletUnlockForeground();
    if(R_FAILED(rc)) {
        ShowError("Error launching game. The error is shown below:\n", "Error unlocking foreground", rc);
    }
    
    struct InData{
        u32 code;
        u8 unk1;
        u8 pad[3];
        u128 id;
        u8 unk2[0x70];
    } __attribute__((packed));
    
    InData indata = {0};
    indata.code = 0xC79497CA;
    indata.unk1 = 1;
    indata.id = userID;
    
    rc = appletCreateStorage(&aStore, sizeof(InData));
    if(R_FAILED(rc)) {
        ShowError("Error launching game. The error is shown below:\n", "Error initializing arg storage", rc);
    }

    rc = appletStorageWrite(&aStore, 0, &indata, sizeof(InData));
    if(R_FAILED(rc)) {
        ShowError("Error launching game. The error is shown below:\n", "Error writing arg storage", rc);
    }
    
    rc = appletPopLaunchParameter(&aStore, AppletLaunchParameterKind_PreselectedUser);
    if(R_FAILED(rc)) {
        ShowError("Error launching game. The error is shown below:\n", "Error in function appletPopLaunchParameter", rc);
    }
    
    rc = appletHolderStart(&currentApplication);
    if(R_FAILED(rc)) {
        ShowError("Error launching game\n", "Please look up the error code for more information", rc);
    }
    
    rc = appletApplicationRequestForApplicationToGetForeground(&app);
    if(R_FAILED(rc)) {
        ShowError("Error launching game. The error is shown below:\n", "Error appRequestForApplicationToGetForeground", rc);
    }
    App::is_active = true;
    appletHolderJoin(&currentApplication);
    appletHolderClose(&currentApplication);
    appletStorageClose(&aStore);
    App::is_active = false;
    #endif
    
    return rc;
}

Result App::GetAppRecords(std::vector<NsApplicationRecord> &recs) {
    Result rc = 0;
    #ifdef __SWITCH__
    size_t size = 0;
    NsApplicationRecord rec[2000];
    rc = nsListApplicationRecord(rec, 2000, 0, &size);
    for(int i = 0; i < (int)size; i++)
        recs.push_back(rec[i]);
    #endif
    
    return rc;
}

bool App::IsVrEnabled() {
    bool b = false;
    #ifdef __SWITCH__
    appletIsVrModeEnabled(&b);
    #endif
    return b;
}

NsApplicationControlData App::GetGameControlData(u64 tid, u8 flag) {
    NsApplicationControlData buffer;
    size_t s = 0;
    #ifdef __SWITCH__
    nsGetApplicationControlData(flag, tid, &buffer, 0x20000, &s);
    #endif
    return buffer;
}

bool App::IsGamecardInserted() {
    bool b = false;
    #ifdef __SWITCH__
    FsDeviceOperator opt;
    fsOpenDeviceOperator(&opt);
    fsDeviceOperatorIsGameCardInserted(&opt, &b);
    #endif
    return b;
}

/*
*   Applet
*/
Result App::LaunchAlbum(u8 arg, bool startupSound) {
    Result rc = 0;
    #ifdef __SWITCH__
    LibAppletArgs args;
    AppletStorage storeIn;
    
    appletCreateLibraryApplet(&currentApplet, AppletId_photoViewer, LibAppletMode_AllForeground);
    libappletArgsCreate(&args, 0);
    libappletArgsSetPlayStartupSound(&args, startupSound);
    libappletArgsPush(&args, &currentApplet);
    
    u8 stindata = arg;
    rc = appletCreateStorage(&storeIn, sizeof(stindata));
    if(R_FAILED(rc)) {
        appletStorageClose(&storeIn);
        ShowError("Error launching album\n", "Error creating storage.", rc);
    }
    
    rc = appletStorageWrite(&storeIn, 0, &stindata, sizeof(stindata));
    if(R_FAILED(rc)) {
        appletStorageClose(&storeIn);
        ShowError("Error launching album\n", "Error writing storage.", rc);
    }
    appletHolderPushInData(&currentApplet, &storeIn);
    
    App::is_active_applet = true;
    rc = appletHolderStart(&currentApplet);
    appletHolderJoin(&currentApplet);
    
    appletStorageClose(&storeIn);
    appletHolderClose(&currentApplet);
    App::is_active_applet = false;
    #endif
    
    return rc;
}

u128 App::LaunchPSelect() {
    Result rc = 0;
    u128 player = 0;
    #ifdef __SWITCH__
    AppletStorage storeIn, storeOut;
    LibAppletArgs args;
    
    rc = appletCreateLibraryApplet(&currentApplet, AppletId_playerSelect, LibAppletMode_AllForeground);
    if(R_FAILED(rc)) {
        ShowError("Error launching player select\n", "Error creating lib applet.", rc);
    }
    
    libappletArgsCreate(&args, 0);
    libappletArgsPush(&args, &currentApplet);
    
    u8 stindata[0xa0] = { 0 };
    
    rc = appletCreateStorage(&storeIn, sizeof(stindata));
    if(R_FAILED(rc)) {
        appletStorageClose(&storeIn);
        ShowError("Error launching player select\n", "Error creating storage.", rc);
    }
    
    rc = appletStorageWrite(&storeIn, 0, stindata, sizeof(stindata));
    if(R_FAILED(rc)) {
        appletStorageClose(&storeIn);
        ShowError("Error launching player select\n", "Error writing storage.", rc);
    }
    appletHolderPushInData(&currentApplet, &storeIn);
    
    App::is_active_applet = true;
    rc = appletHolderStart(&currentApplet);
    appletHolderJoin(&currentApplet);
    if(R_FAILED(rc)) {
        ShowError("Error launching player select\n", "Error starting applet.", rc);
    }
    else {
        appletHolderPopOutData(&currentApplet, &storeOut);
        u8 buf[0x18] = {0};
        appletStorageRead(&storeOut, 0, buf, 0x18);
        player = *(u128*)&buf[8];
    }
    appletStorageClose(&storeIn);
    appletStorageClose(&storeOut);
    appletHolderClose(&currentApplet);
    App::is_active_applet = false;
    
    #endif
    
    return player;
}

Result App::LaunchShop() {
    Result rc = 0;
    #ifdef __SWITCH__
    LibAppletArgs args;
    AppletStorage storeIn, storeOut;
    
    appletCreateLibraryApplet(&currentApplet, AppletId_shop, LibAppletMode_AllForeground);
    libappletArgsCreate(&args, 1);
    libappletArgsPush(&args, &currentApplet);
    
    rc = appletCreateStorage(&storeIn, 0x2000);
    if(R_FAILED(rc)) {
        ShowError("Error launching eshop", "Error initializing arg storage", rc);
    }

    char indata[0x2000] = {0};
    sprintf (indata, "eshop://%s", "success" );

    rc = appletStorageWrite(&storeIn, 0, indata, 0x2000);
    if(R_FAILED(rc)) {
        ShowError("Error launching eshop", "Error writing arg storage", rc);
    }
    appletHolderPushInData(&currentApplet, &storeIn);
    
    App::is_active_applet = true;
    rc = appletHolderStart(&currentApplet);
    appletHolderJoin(&currentApplet);
    if(R_FAILED(rc)) {
        ShowError("Error launching eshop", "Error starting applet.", rc);
    }
    else {
        appletHolderPopOutData(&currentApplet, &storeOut);
        u8 buf[0x1010] = {0};
        appletStorageRead(&storeOut, 0, buf, sizeof(buf));
        //TODO
    }
    appletStorageClose(&storeIn);
    appletStorageClose(&storeOut);
    appletHolderClose(&currentApplet);
    App::is_active_applet = false;
    #endif
    
    return rc;
}

Result App::LaunchWebsite(std::string url) {
    Result rc = 0;
    #ifdef __SWITCH__
    WebCommonConfig config;
    WebCommonReply reply;
    WebExitReason exitReason = WebExitReason_ExitButton;
    rc = webPageCreate(&config, url.c_str());
    if (R_SUCCEEDED(rc)) {
        App::is_active_applet = true;
        rc = webConfigShow(&config, &reply);
        if (R_SUCCEEDED(rc))
            rc = webReplyGetExitReason(&reply, &exitReason);
        App::is_active_applet = false;
    }
    #endif

    return rc;
}

Result App::LaunchSwkbd(char out[0xc00], std::string title, std::string placehold, std::string oktxt, std::string initTxt) {
    Result rc;
    SwkbdConfig kbd;
    char tempstr[0xc00] = {0};
    rc = swkbdCreate(&kbd, 0);
    if(R_SUCCEEDED(rc)) {
        swkbdConfigMakePresetDefault(&kbd);
        swkbdConfigSetHeaderText(&kbd, title.c_str());
        swkbdConfigSetOkButtonText(&kbd, oktxt.c_str());
        swkbdConfigSetGuideText(&kbd, placehold.c_str());
        swkbdConfigSetInitialText(&kbd, initTxt.c_str());
        //swkbdConfigSetTextCheckCallback(&kbd, validate_text);
        rc = swkbdShow(&kbd, tempstr, sizeof(tempstr));
        if(R_SUCCEEDED(rc)) {
            if(tempstr[0] != '\0')
                strcpy(out, tempstr);
        }else if(rc != (Result)0x5d59) {
            ShowError("Software Keyboard Error\n", "Unknown error, Lookup errorcode for more info", rc);
        }
    }else{
        ShowError("Software Keyboard Error\n", "Unknown error, Lookup errorcode for more info", rc);
    }
    swkbdClose(&kbd);
    return rc;
}

Result App::LaunchNetConnect() {
    Result rc = 0;
    #ifdef __SWITCH__
    appletCreateLibraryApplet(&currentApplet, AppletId_netConnect, LibAppletMode_AllForeground);
    App::is_active_applet = true;
    rc = appletHolderStart(&currentApplet);
    appletHolderJoin(&currentApplet);
    appletHolderClose(&currentApplet);
    App::is_active_applet = false;
    #endif
    
    return rc;
}

Result App::LaunchController() {
    Result rc = 0;
    #ifdef __SWITCH__
    appletCreateLibraryApplet(&currentApplet, AppletId_controller, LibAppletMode_AllForeground);
    App::is_active_applet = true;
    rc = appletHolderStart(&currentApplet);
    appletHolderJoin(&currentApplet);
    appletHolderClose(&currentApplet);
    App::is_active_applet = false;
    #endif
    
    return rc;
}

Result App::ShowError(std::string errText, std::string details, Result rc) {
    #ifdef __SWITCH__
    AppletStorage errStor;
    LibAppletArgs args;

    appletCreateLibraryApplet(&currentApplet, AppletId_error, LibAppletMode_AllForeground);
    libappletArgsCreate(&args, 1);
    libappletArgsPush(&args, &currentApplet);
    appletCreateStorage(&errStor, 0x1018);
    u8 argBuf[0x1018] = {0};
    argBuf[0] = 1;
    
    *(u64*)&argBuf[8] = (((rc & 0x1ffu) + 2000) | (((rc >> 9) & 0x1fff & 0x1fffll) << 32));
    strcpy((char*) &argBuf[24], errText.c_str());
    strcpy((char*) &argBuf[0x818], details.c_str());
    appletStorageWrite(&errStor, 0, argBuf, 0x1018);
    appletHolderPushInData(&currentApplet, &errStor);
    
    App::is_active_applet = true;
    appletHolderStart(&currentApplet);
    appletHolderJoin(&currentApplet);
    appletHolderClose(&currentApplet);
    App::is_active_applet = false;
    #endif
    
    return 0;
}

Result App::LaunchHbl() {
    Result rc = 0;
    #ifdef __SWITCH__
    appletCreateLibraryApplet(&currentApplet, AppletId_offlineWeb, LibAppletMode_AllForeground);
    App::is_active_applet = true;
    rc = appletHolderStart(&currentApplet);
    appletHolderJoin(&currentApplet);
    appletHolderClose(&currentApplet);
    App::is_active_applet = false;
    #endif
    
    return rc;
}

/*
*   Misc
*/
Result App::CommandHandler(u32 cmd) {
    switch(cmd) {
        case CMD_Home:
        {
            #ifdef __SWITCH__
            if(App::is_active) {
                appletHolderRequestExit(&currentApplication);
            }
            else if(App::is_active_applet) {
                appletHolderRequestExit(&currentApplet);
            }
            else {
                gameSelectInd = 0;
                appletSelectInd = 0;
                dashLayer = 0;
            }
            #endif
            break;
        }
        case 14:
        {
            //
            break;
        }
        case CMD_Sleepmode:
        {
            Power::Sleep();
            break;
        }
        case CMD_Shutdown:
        {
            Power::Shutdown();
            break;
        }
        case CMD_Reboot:
        {
            Power::Reboot();
            break;
        }
    }
    
    return 0;
}
