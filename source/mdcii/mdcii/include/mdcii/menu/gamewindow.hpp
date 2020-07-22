// This file is part of the MDCII Game Engine.
// Copyright (C) 2020  Armin Schlegel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef GAMEWINDOW_H_
#define GAMEWINDOW_H_

#include <iostream>
#include <memory>

#include <SDL2/SDL.h>

#include "sdlgui/imageview.h"
#include "sdlgui/screen.h"
#include "sdlgui/window.h"

#include "cod/buildings.hpp"
#include "cod/cod_parser.hpp"
#include "files/files.hpp"
#include "framebuffer/framebuffer_pal8.hpp"
#include "framebuffer/palette.hpp"
#include "gam/gam_parser.hpp"
#include "kamera.hpp"
#include "spielbildschirm.hpp"

#include "menu/fps.hpp"
#include "menu/scale.hpp"

using namespace sdlgui;

class GameWindow : public Screen
{
public:
  GameWindow(SDL_Renderer* renderer, SDL_Window* pwindow, int width, int height, const std::string& gamName, bool fullscreen);
  void Handle();

private:
  SDL_Renderer* renderer;
  int width;
  int height;
  std::string gamName;
  bool fullscreen;
  Scale* scale;
  std::shared_ptr<Buildings> buildings;
  bool running;
  std::shared_ptr<GamParser> gam;
  SDL_Surface* s8;
};
#endif