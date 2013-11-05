/******************************************************************************
 Copyright 2013 Victor Yurkovsky

    This file is part of PCBFeet project

    PCBFeet is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PCBFeet is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PCBFeet.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
/* pad */
typedef enum ePadShape {
  PAD_ROUND,
  PAD_SQUARE,
} ePadShape;

typedef struct sPad {
  sVTAB vtab;
  GString* Name;
  GString* Number;
  sPoint P1;
  sPoint P2;
  int Thickness;   //Width of metal 
  int Clearance;   //separation of pad from other conductors (50%)
  int Mask;   //separation of pad from other conductors (50%)
  ePadShape Shape;
} sPad;

sPad* pad_new();
sPad* pad_parse(sParser* parse);
sPad* pad_init(sPad* pad,int X1,int Y1,int X2,int Y2,
  int Thickness,int Clearance, int Mask,
  GString*Name,GString* Number, ePadShape Shape);
void pad_delete(sPad* pad);
void pad_draw(sPad*p, cairo_t* cr, sView* view);


