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
/*From PCB source...
 * static FlagBitsType object_flagbits[] = {
  { PINFLAG, N ("pin"), ALL_TYPES },
  { VIAFLAG, N ("via"), ALL_TYPES },
  { FOUNDFLAG, N ("found"), ALL_TYPES },
  { HOLEFLAG, N ("hole"), PIN_TYPES },
  { RATFLAG, N ("rat"), RATLINE_TYPE },
  { PININPOLYFLAG, N ("pininpoly"), PIN_TYPES | PAD_TYPE },
  { CLEARPOLYFLAG, N ("clearpoly"), POLYGON_TYPE },
  { HIDENAMEFLAG, N ("hidename"), ELEMENT_TYPE },
  { DISPLAYNAMEFLAG, N ("showname"), ELEMENT_TYPE },
  { CLEARLINEFLAG, N ("clearline"), LINE_TYPE | ARC_TYPE | TEXT_TYPE },
  { SELECTEDFLAG, N ("selected"), ALL_TYPES },
  { ONSOLDERFLAG, N ("onsolder"), ELEMENT_TYPE | PAD_TYPE | TEXT_TYPE },
  { AUTOFLAG, N ("auto"), ALL_TYPES },
  { SQUAREFLAG, N ("square"), PIN_TYPES | PAD_TYPE },
  { RUBBERENDFLAG, N ("rubberend"), LINE_TYPE | ARC_TYPE },
  { WARNFLAG, N ("warn"), PIN_TYPES | PAD_TYPE },
  { USETHERMALFLAG, N ("usetherm"), PIN_TYPES | LINE_TYPE | ARC_TYPE },
  { OCTAGONFLAG, N ("octagon"), PIN_TYPES | PAD_TYPE },
  { DRCFLAG, N ("drc"), ALL_TYPES },
  { LOCKFLAG, N ("lock"), ALL_TYPES },
  { EDGE2FLAG, N ("edge2"), ALL_TYPES },
  { FULLPOLYFLAG, N ("fullpoly"), POLYGON_TYPE},
  { NOPASTEFLAG, N ("nopaste"), PAD_TYPE },
  { CONNECTEDFLAG, N ("connected"), ALL_TYPES }
};*/

typedef enum ePinShape {
  HOLE,
  ROUND,
  SQUARE,
  OCTAGON
} ePinShape;


typedef struct sPin {
  sVTAB vtab;
  char* Name;
  char* Number;
  int X;
  int Y;
  int Thickness;
  int Clearance;   //separation of pad from other conductors (50%)
  int Mask;
  int Hole;
  ePinShape Shape;
} sPin;

sPin* pin_new(int X,int Y,
  int Thickness,int Clearance, int Mask,
  int Hole,
  char*Name,char* Number, ePinShape Shape);
void pin_delete(sPin* pad);
void pin_draw(sPin*p, cairo_t* cr, sView* view);


