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
#include "Game.hpp"

// Note that all needlelaunch homebrew must use only Game.hpp to ensure compatibility
Game::Game() : GameBase() {
	TitleId = 0;
}

Game::~Game() {
	SDL_DestroyTexture(Icon);
}

void Game::MountSaveData() {
    FsSave save = {0};
    save.titleID = TitleId;
    FsFileSystem *fs = fsdevGetDefaultFileSystem();
    fsMountSaveData(fs, 0, &save);
}
Result Game::Run() {
    Result rc = 0;
    if(!App::is_active) {
        u128 userid = Account::GetActiveAccount();
        if(userid == 0) return 0;
        rc = App::LaunchGame(TitleId, userid);
    }
    else {
        rc = appletApplicationRequestForApplicationToGetForeground(&App::app);
    }
    return rc;
}
// For future purposes or for homebrew, prehaps
Result Game::AddNewsStory(std::string title, std::string body, SDL_Texture *img, SDL_Rect pos){
	NewsMenu news_menu = NewsMenu(pos);
	news_menu.AddStory(title, body, img);
	return 0;
}

