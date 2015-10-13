
// This file is part of the MDCII Game Engine.
// Copyright (C) 2015  Benedikt Freisen
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

#include "bildspeicher.hpp"

Bildspeicher::Bildspeicher(uint32_t breite, uint32_t hoehe, uint32_t format, uint32_t farbe, uint8_t* puffer, uint32_t pufferbreite)
{
  this->breite = breite;
  this->hoehe = hoehe;
  this->format = format;
  this->farbe = farbe;
  this->pufferbreite = (pufferbreite >= breite * format) ? pufferbreite : (breite * format);
  if (puffer == NULL)
  {
    this->puffer = new uint8_t[pufferbreite * hoehe];
    puffer_freigeben = 1;
  }
  else
  {
    this->puffer = puffer;
    puffer_freigeben = 0;
  }
  //memset
}

Bildspeicher::~Bildspeicher()
{
  if (puffer_freigeben)
    delete puffer;
}

void Bildspeicher::zeichne_bsh_bild(bsh_bild_t *bild, int x, int y, int ausrichtung)
{
  // in einer Unterklasse implementiert
}

void Bildspeicher::zeichne_rechteck(int x1, int y1, int x2, int y2, uint8_t farbe)
{
  if (x1 < 0)
    x1 = 0;
  if (y1 < 0)
    y1 = 0;
  if (x2 >= breite)
    x2 = breite - 1;
  if (y2 >= hoehe)
    y2 = hoehe - 1;
  
  for (int y = y1; y <= y2; y++)
  {
    for (int x = x1; x <= x2; x++)
    {
      zeichne_pixel(x, y, farbe);
    }
  }
}

void Bildspeicher::zeichne_linie(int x1, int y1, int x2, int y2, uint8_t farbe, uint8_t muster)
{
  int xdiff = abs(x2 - x1);
  int ydiff = abs(y2 - y1);
  
  int ix = (x2 > x1) - (x2 < x1);
  int iy = (y2 > y1) - (y2 < y1);
  
  int cx = x1;
  int cy = y1;
  
  if (xdiff >= ydiff)
  {
    int s = xdiff >> 1;
    do
    {
      if ((muster = muster >> 1 | muster << 7) & 0x80)
	zeichne_pixel(cx, cy, farbe);
      cx += ix;
      s -= ydiff;
      if (s <= 0)
      {
	s += xdiff;
	cy += iy;
      }
    }
    while (cx != x2);
  }
  else
  {
    int s = ydiff >> 1;
    do
    {
      if ((muster = muster >> 1 | muster << 7) & 0x80)
	zeichne_pixel(cx, cy, farbe);
      cy += iy;
      s -= xdiff;
      if (s <= 0)
      {
	s += ydiff;
	cx += ix;
      }
    }
    while (cy != y2);
  }
  
  if ((muster = muster >> 1 | muster << 7) & 0x80)
    zeichne_pixel(x2, y2, farbe);
}

void Bildspeicher::exportiere_pnm(const char* pfadname)
{
  // in einer Unterklasse implementiert
}

void Bildspeicher::exportiere_bmp(const char* pfadname)
{
  // in einer Unterklasse implementiert
}

void Bildspeicher::bild_loeschen()
{
  // in einer Unterklasse implementiert
}
