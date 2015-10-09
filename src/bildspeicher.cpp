
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

#include <string.h>
#include <fstream>
#include "bildspeicher.hpp"
#include "palette.hpp"

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

void Bildspeicher::zeichne_bsh_bild_ganz(bsh_bild_t *bild, int x, int y)
{
  uint8_t ch;
  uint8_t *quelle = bild->puffer;
  uint8_t *zielzeile;
  uint8_t *ziel;
  
  if (this->format == 1)
  {
    ziel = zielzeile = this->puffer + y * this->pufferbreite + x;
    int restbreite = this->pufferbreite;
    
    while ((ch = *(quelle++)) != 0xff)
    {
      if (ch == 0xfe)
      {
	ziel = zielzeile += restbreite;
      }
      else
      {
	ziel += ch;
	
	for (ch = *(quelle++); ch > 0; ch--)
	  *(ziel++) = *(quelle++);
      }
    }
  }
  else if (this->format == 3)
  {
    ziel = zielzeile = this->puffer + y * this->pufferbreite + x * 3;
    int restbreite = this->pufferbreite;
    
    while ((ch = *(quelle++)) != 0xff)
    {
      if (ch == 0xfe)
      {
	ziel = zielzeile += restbreite;
      }
      else
      {
	ziel += ((int)ch) * 3;
	
	for (ch = *(quelle++); ch > 0; ch--)
	{
	  int index = ((int)*(quelle++)) * 3;
	  *(ziel++) = palette[index++];
	  *(ziel++) = palette[index++];
	  *(ziel++) = palette[index];
	}
      }
    }
  }
}

void Bildspeicher::zeichne_bsh_bild_partiell(bsh_bild_t *bild, int x, int y)
{
  int u = 0;
  int v = 0;
  int i = 0;
  unsigned char ch;
  
  if (this->format == 1)
  {
    uint8_t *quelle = bild->puffer;
    uint8_t *zielzeile;
    uint8_t *ziel = zielzeile = this->puffer + y * (int)this->pufferbreite + x;
    int restbreite = this->pufferbreite;
    
    if (x >= 0 && x + bild->breite < this->breite)
    {
      uint8_t *anfang = this->puffer;
      uint8_t *ende = this->puffer + this->pufferbreite * this->hoehe;
      
      while (ziel < anfang)
      {
	ch = *(quelle++);
	if (ch == 0xff)
	  return;
	if (ch == 0xfe)
	{
	  ziel = zielzeile += restbreite;
	  if (ziel >= ende)
	    return;
	}
	else
	{
	  ziel += ch;
	  
	  ch = *(quelle++);
	  quelle += ch;
	  ziel += ch;
	}
      }
      while ((ch = *(quelle++)) != 0xff)
      {
	if (ch == 0xfe)
	{
	  ziel = zielzeile += restbreite;
	  if (ziel >= ende)
	    return;
	}
	else
	{
	  ziel += ch;
	  
	  for (ch = *(quelle++); ch > 0; ch--)
	    *(ziel++) = *(quelle++);
	}
      }
    }
    else
    {
      while ((ch = *(quelle++)) != 0xff)
      {
	if (ch == 0xfe)
	{
	  ziel = zielzeile += restbreite;
	  u = 0;
	  v++;
	  if (y + v >= (int)this->hoehe)
	    return;
	}
	else
	{
	  u += ch;
	  ziel += ch;
	  
	  ch = *(quelle++);
	  if (y + v >= 0 || x + u + ch > 0 || x + u + ch < this->breite)
	  {
	    for (; ch > 0; ch--, u++, quelle++, ziel++)
	      if (x + u >= 0 && x + u < this->breite)
		*ziel = *quelle;
	  }
	  else
	  {
	    u += ch;
	    quelle += ch;
	    ziel += ch;
	    ch = 0;
	  }
	}
      }
    }
  }
  else if (this->format == 3)
  {
    while ((ch = bild->puffer[i++]) != 0xff)
    {
      if (ch == 0xfe)
      {
	u = 0;
	v++;
      }
      else
      {
	u += ch;
	
	for (ch = bild->puffer[i++]; ch > 0; ch--, u++, i++)
	{
	  if (y + v >= 0 && y + v < this->hoehe && x + u >= 0 && x + u < this->breite)
	  {
	    unsigned char a = bild->puffer[i];
	    this->puffer[(y + v) * this->pufferbreite + (x + u) * 3] = palette[a * 3];
	    this->puffer[(y + v) * this->pufferbreite + (x + u) * 3 + 1] = palette[a * 3 + 1];
	    this->puffer[(y + v) * this->pufferbreite + (x + u) * 3 + 2] = palette[a * 3 + 2];
	  }
	}
      }
    }
  }
}

void Bildspeicher::zeichne_bsh_bild(bsh_bild_t *bild, int x, int y, int ausrichtung)
{
  if (bild == NULL)
    return;
  if (ausrichtung == 1)
  {
    x -= bild->breite / 2;
    y -= bild->hoehe;
  }
  if (x >= (int)this->breite || y >= (int)this->hoehe || x + (int)bild->breite < 0 || y + (int)bild->hoehe < 0)
    return;
  if ((x < 0) || (y < 0) || (x + (int)bild->breite > (int)this->breite) || (y + (int)bild->hoehe > (int)this->hoehe))
    zeichne_bsh_bild_partiell(bild, x, y);
  else
    zeichne_bsh_bild_ganz(bild, x, y);
}

void Bildspeicher::exportiere_pnm(const char* pfadname)
{
  std::ofstream pnm;
  pnm.open(pfadname);
  if (format == 1)
    pnm << "P5\n" << breite << " " << hoehe << "\n255\n";
  else if (format == 3)
    pnm << "P6\n" << breite << " " << hoehe << "\n255\n";
  pnm.write((char*)puffer, breite * hoehe * format);
  pnm.close();
}

void Bildspeicher::exportiere_bmp(const char* pfadname)
{
  if (format != 1)
    return;
  
  struct tagBITMAPFILEHEADER
  {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
  } __attribute__((packed)) bmfh = {19778, breite * hoehe + 1078, 0, 0, 1078};
  struct tagBITMAPINFOHEADER
  {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
  } __attribute__((packed)) bmih = {40, (int32_t)breite, (int32_t)hoehe, 1, 8, 0, 0, 0, 0, 0, 0};
  
  std::ofstream bmp;
  bmp.open(pfadname);
  bmp.write((char*)&bmfh, sizeof(struct tagBITMAPFILEHEADER));
  bmp.write((char*)&bmih, sizeof(struct tagBITMAPINFOHEADER));
  
  int i;
  for (i = 0; i < 256; i++)
    bmp << palette[i * 3 + 2] << palette[i * 3 + 1] << palette[i * 3] << (char)0;
  for (i = hoehe - 1; i >= 0; i--)
    bmp.write((char*)&puffer[breite * i], breite);
  bmp.close();
}

void Bildspeicher::bild_loeschen()
{
  if (format == 1)
  {
    for (int i = 0; i < breite * hoehe; i++)
      puffer[i] = farbe;
  }
  else if (format == 3)
  {
    for (int i = 0; i < breite * hoehe * 3; i += 3)
    {
      puffer[i] = farbe;
      puffer[i + 1] = farbe >> 8;
      puffer[i + 2] = farbe >> 16;
    }
  }
}
