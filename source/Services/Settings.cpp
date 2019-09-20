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

#include "Settings.hpp"
#include "../ConfigParser.hpp"
SelectType Settings::gameSelType;
GameAreaType Settings::gameAreaType;
u32 Settings::currentTheme;

std::string Settings::GetFirmwareVersion() {
    SetSysFirmwareVersion firm;
    #ifdef __SWITCH__
    setsysGetFirmwareVersion(&firm);
    std::string str = std::string(firm.display_version);
    #else
    std::string str = std::string("Unknown Version Info");
    #endif
    #ifdef AMS
    std::string new_str = str + "[AMS]";
    #endif
    #ifdef REINX
    std::string new_str = str + "[RNX]";
    #endif
    #ifdef SXOS
    std::string new_str = str + "[SX]";
    #endif
    return new_str;
}

std::string Settings::GetSerialNumber() {
    #ifdef __SWITCH__
	char serial[0x20] = {0};
	setsysGetSerialNumber(serial);
    std::string str = std::string(serial);
    #else
    std::string str = std::string("XAW70000000000");
    #endif
	return str;
}

std::string Settings::GetDeviceNickname() {
    #ifdef __SWITCH__
    char nick[0x80] = {0};
    setsysGetDeviceNickname(nick);
    std::string str = std::string(nick);
    #else
    std::string str = std::string("Needletail");
    #endif
    return str;
}

u64 Settings::GetSystemLangCode() {
    Result rc = 0;
    u64 code = 0;
    #ifdef __SWITCH__
    rc = setGetSystemLanguage(&code);
    #endif
    return R_SUCCEEDED(rc) ? code : 0;
}

void Settings::SetDeviceNickname(std::string nick) {
    #ifdef __SWITCH__
    setsysSetDeviceNickname((char*)nick.c_str());
    #endif
}

u8 Settings::GetLockScreenFlag() {
    u8 flag = 0;
    #ifdef __SWITCH__
    GetLockScreenFlag();
    #endif
    return flag;
}

void Settings::SetLockScreenFlag(u8 flag) {
    #ifdef __SWITCH__
    SetLockScreenFlag(flag);
    #endif
}

u8 Settings::GetAudioVolume() {
    u8 vol = 0;
    #ifdef __SWITCH__
    GetAudioVolume();
    #endif
    return vol;
}

void Settings::SetAudioVolume(u8 vol) {
    #ifdef __SWITCH__
    SetAudioVolume(vol);
    #endif
}

std::string Settings::GetCurrentTheme() {
    std::vector<std::string> names;
    std::string path = "/themes";
    if (!std::experimental::filesystem::exists(path)) return "";
    for (const auto & entry : std::experimental::filesystem::v1::directory_iterator(path)) {
        names.push_back(entry.path().string());
	}
    return names.empty() ? "" : names[currentTheme];
}

size_t Settings::GetMemAvail() {
    size_t mem_available = 0;
    svcGetInfo(&mem_available, 6, CUR_PROCESS_HANDLE, 0);
    return mem_available;
}

size_t Settings::GetMemUsed() {
    size_t mem_used = 0;
    svcGetInfo(&mem_used, 7, CUR_PROCESS_HANDLE, 0);
    return mem_used;
}
