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
#pragma once
#include "view.h"
/* After much deliberation I decided that the best strategy is to create a single
 * displayable object 'class' containing data usable for all possible cases:
 * pin, pad, line, curve. 
 */

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

typedef enum eObjectType {
  TYPE_PIN,
  TYPE_PAD,
  TYPE_LINE,
  TYPE_CURVE
} eObjectType;

typedef enum eObjectShape {
  SHAPE_ROUND,
  SHAPE_SQUARE,
  SHAPE_OCTAGON
} eObjectShape;

typedef struct sObject {
  eObjectType Type:2;
  eObjectShape Shape:2;
  
  GString* Name;
  GString* Number;
  sPoint P1;
  sPoint P2;
  int Thickness;
  int Clearance;   //separation of pad from other conductors (50%)
  int Mask;
  int Hole;
} sObject;

sObject* object_new();
sObject* object_parse(sParser* parse);
void     object_delete(sObject* pad);
void     object_draw(sObject*p, cairo_t* cr, sView* view);
GtkWidget* pin_create_config();


 //and the UI config panel...
typedef struct sUIPin{
  GtkFrame* frame;
  GtkEntry* name; 
  GtkEntry* number; 
  GtkEntry* p1_x; 
  GtkEntry* p1_y; 
  GtkEntry* thickness; 
  GtkEntry* clearance; 
  GtkEntry* mask; 
  GtkEntry* hole; 
  GtkComboBoxText* shape;
} sUIPin;

sUIPin* ui_pin_new();
