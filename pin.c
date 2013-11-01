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
#include "pin.h"
#include <math.h>



sPin* pin_new(int X,int Y,
  int Thickness,int Clearance, int Mask,
  int Hole,
  char*Name,char* Number, eSymFlag Flags){
  
  sPin* pin = (sPin*)g_malloc(sizeof(sPin));
  pin->vtab.draw = (ptrDraw)&pin_draw;//comply with generic vtab func 
  pin->X      = X;
  pin->Y      = Y;
  pin->Thickness = Thickness;
  pin->Clearance = Clearance;
  pin->Mask   = Mask; 
  pin->Hole   = Hole;
  pin->Name   = Name;
  pin->Number = Number;
  pin->Flags  = Flags;
  return pin;
}
void pin_delete(sPin* pin){
  g_free(pin);
}


void pin_draw(sPin*pin, cairo_t* cr, sView* view){
  printf("pin_draw %p\n",pin);
  int rad_outer = pin->Thickness/2;
  int rad_inner = pin->Hole/2;
//  int width = (pin->Thickness - pin->Hole)/2; //pad diameter-hole diameter/2
//  int radius = (pin->Hole + width)/2;
  //manually scale here
  cairo_set_line_cap(cr,CAIRO_LINE_CAP_BUTT);
  cairo_set_source_rgb(cr, 0,.6, 0);
  cairo_set_line_width(cr, 1);
printf("pin_draw 1\n");
  cairo_new_sub_path(cr);
  cairo_arc(cr,
    (double)(pin->X-view->origin.x)/(double)view->scale +.5,
    (double)(pin->Y-view->origin.y)/(double)view->scale +.5,
    rad_outer/view->scale,
    0,2 * M_PI);
  cairo_fill(cr);
  cairo_stroke(cr);    
}


