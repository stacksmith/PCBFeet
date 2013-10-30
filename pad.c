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
#include <malloc.h>

#include "view.h"
#include "pad.h"


sPad* pad_new(){
  sPad* pad = (sPad*)malloc(sizeof(sPad));
  return pad;
}
void pad_delete(sPad* pad){
  free(pad);
}
//TODO:this is temporary
void pad_init(sPad* p){
  pad_set(p,"PadName","1","",1000,1000,2000,1500, 500,1000,1500);
}

void pad_set(sPad* p, char*Name,char* Number, char* Flags,
  double X1,double Y1,double X2,double Y2,
  double Thickness,double Clearance, double Mask){
  p->Name   = Name;
  p->Number = Number;
  p->Flags  = Flags;
  p->X1     = X1;
  p->Y1     = Y1;
  p->X2     = X2;
  p->Y2     = Y2;
  p->Thickness = Thickness;
  p->Clearance = Clearance;
  p->Mask = Mask; 
}


void pad_draw(sPad*pad, cairo_t* cr, sView* view){
  printf("pad_draw\n");
  //manually scale here
  cairo_set_line_cap(cr,CAIRO_LINE_CAP_SQUARE);
  cairo_set_source_rgb(cr, 0,.6, 0);
  cairo_set_line_width(cr, pad->Thickness/view->scale);
  
  // convert native centimils to pixels
  cairo_move_to(cr,
    (pad->X1-view->origin.x)/view->scale,
    (pad->Y1-view->origin.y)/view->scale);
  cairo_line_to(cr,
    (pad->X2-view->origin.x)/view->scale,
    (pad->Y2-view->origin.y)/view->scale);
 
  cairo_stroke(cr);    
}


