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
typedef enum eTextDir{
  TEXT_HOR=0,
  TEXT_UP,
  TEXT_UD,
  TEXT_DOWN
} eTextDir;
typedef struct sElement {
  unsigned int flags;           //
  GString      description;     //textual description
  sPoint       mark_xy;         //position of mark
  sPoint       text_xy;         //position of text
  eTextDir     text_dir;
  unsigned int text_scale;
  unsigned int text_flags; 
} sElement;

sElement* element_new();
void element_delete(sElement* element);
void element_init();
void element_draw(sElement*p, cairo_t* cr, sView* view);


