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
#include <cairo.h>
#include <gtk/gtk.h>

#include "types.h"
#include "view.h"
#include "vtab.h"
#include "pad.h"


sPad* pad_new(int X1,int Y1,int X2,int Y2,
  int Thickness,int Clearance, int Mask,
  char*Name,char* Number, char* Flags){
  
  sPad* pad = (sPad*)g_malloc(sizeof(sPad));
  pad->vtab.draw = (ptrDraw)&pad_draw;  //comply with generic vtab func 

  pad->Name   = Name;
  pad->Number = Number;
  pad->Flags  = Flags;
  pad->X1     = X1;
  pad->Y1     = Y1;
  pad->X2     = X2;
  pad->Y2     = Y2;
  pad->Thickness = Thickness;
  pad->Clearance = Clearance;
  pad->Mask = Mask; 

  return pad;
}
void pad_delete(sPad* pad){
  g_free(pad);
}


void pad_draw(sPad*pad, cairo_t* cr, sView* view){
  printf("pad_draw %p\n",pad);
  //manually scale here
  cairo_set_line_cap(cr,CAIRO_LINE_CAP_SQUARE);
  cairo_set_source_rgb(cr, 0,.6, 0);
  cairo_set_line_width(cr, pad->Thickness/view->scale);
  printf("pad_draw 1\n");
  
  // convert native centimils to pixels
  cairo_move_to(cr,
    (pad->X1-view->origin.x)/view->scale,
    (pad->Y1-view->origin.y)/view->scale);
  cairo_line_to(cr,
    (pad->X2-view->origin.x)/view->scale,
    (pad->Y2-view->origin.y)/view->scale);
 
  cairo_stroke(cr);    
}


