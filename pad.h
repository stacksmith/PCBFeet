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
typedef struct sPad {
  char* Name;
  char* Number;
  char* Flags;
  double X1;
  double Y1;
  double X2;
  double Y2;
  double Thickness;   //Width of metal 
  double Clearance;   //separation of pad from other conductors (50%)
  double Mask;
} sPad;

sPad* pad_new();
void pad_delete(sPad* pad);
void pad_set(sPad* p, char*Name,char* Number, char* Flags,
  double X1,double Y1,double X2,double Y2,
  double Thickness,double Clearance, double Mask);

void pad_init();
void pad_draw(sPad*p, cairo_t* cr, sView* view);


