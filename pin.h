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
struct sPin;
typedef void (*ptrDraw)(struct sPin*, cairo_t*,sView*);
typedef struct sVTAB {
  ptrDraw draw;
} sVTAB;

typedef struct sPin {
  sVTAB vtab;
  char* Name;
  char* Number;
  char* Flags;
  int X;
  int Y;
  int Clearance;   //separation of pad from other conductors (50%)
  int Mask;
  int Hole;
} sPad;

sPin* pin_new(int X,int Y,
  int Thickness,int Clearance, int Mask,
  int Hole,
  char*Name,char* Number, char* Flags);
void pin_delete(sPin* pad);
void pin_draw(sPin*p, cairo_t* cr, sView* view);


