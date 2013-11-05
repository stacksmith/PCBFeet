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

typedef struct sLine {
  sVTAB vtab;
  sPoint P1;
  sPoint P2;
  int Thickness; 
} sLine;

sLine* line_new();
sLine* line_parse(sParser* parse);
sLine* line_init(sLine*line,int X1,int Y1,int X2,int Y2,
  int Thickness);
void line_delete(sLine* pad);
void line_draw(sLine*p, cairo_t* cr, sView* view);


