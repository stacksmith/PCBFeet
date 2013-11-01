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
#include "pin.h"


sPad* pin_new(int X,int Y,
  int Thickness,int Clearance, int Mask,
  int Hole,
  char*Name,char* Number, char* Flags){
  
  sPad* pin = (sPad*)g_malloc(sizeof(sPad));
  pin->vtab.draw = &pin_draw;
  pin->X      = X;
  pin->Y      = Y;
  pin->Thickness = Thickness;
  pin->Clearance = Clearance;
  pin->Mask   = Mask; 
  pin->Hole   = Hole
  pin->Name   = Name;
  pin->Number = Number;
  pin->Flags  = Flags;
  return pin;
}
void pin_delete(sPad* pin){
  g_free(pin);
}


void pin_draw(sPad*pin, cairo_t* cr, sView* view){
  printf("pin_draw %p\n",pin);
  int width = (pin->Thickness - pin->Hole)/2; //pad diameter-hole diameter/2
  radius = (pin->Hole + width)/2;
  //manually scale here
  cairo_set_line_cap(cr,CAIRO_LINE_CAP_SQUARE);
  cairo_set_source_rgb(cr, 0,.6, 0);
  cairo_set_line_width(cr, width /view->scale);
  printf("pin_draw 1\n");
  
  cairo_arc(cr,
    (pin->X-view->origin.x)/view->scale,
    (pin->Y-view->origin.y)/view->scale,
    radius,
    0,360);
 
  cairo_stroke(cr);    
}


