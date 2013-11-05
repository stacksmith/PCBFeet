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
#include "parser.h"
#include "view.h"
#include "vtab.h"
#include "pad.h"

sPad* pad_new(){
  sPad* pad = (sPad*)g_malloc0(sizeof(sPad));
  pad->vtab.draw = (ptrDraw)&pad_draw;  //comply with generic vtab func 
  return pad;
}

/*****************************************************************************/
// parse PAD from text...
//TODO: error-protect...
sPad* pad_parse(sParser* parse){
  //create pad
  sPad* pad = pad_new();
  if(!parser_help_open(parse)) return 0; //open brace...
  //pad points
  if(!parser_help_point(parse,&pad->P1)) return 0;
  if(!parser_help_point(parse,&pad->P2)) return 0;
  //Thickness,clearance, mask
  if(!parser_help_number(parse,&pad->Thickness)) return 0;
  if(!parser_help_number(parse,&pad->Clearance)) return 0;
  if(!parser_help_number(parse,&pad->Mask)) return 0;
  //name,number
  if(!parser_help_string(parse,&pad->Name)) return 0;
  if(!parser_help_string(parse,&pad->Number)) return 0;
  //Finally, the flags
  //TODO: handle pad flags
  parser_token(parse);
  pad->Shape = PAD_ROUND;
  if(!parser_help_close(parse)) return 0; //closed brace...  
  return pad;
}

/*****************************************************************************/

sPad* pad_init(sPad* pad,int X1,int Y1,int X2,int Y2,
  int Thickness,int Clearance, int Mask,
  GString*Name,GString* Number,   ePadShape Shape){
  

  pad->Name   = Name;
  pad->Number = Number;
  pad->P1.x     = X1;
  pad->P1.y     = Y1;
  pad->P2.x     = X2;
  pad->P2.y     = Y2;
  pad->Thickness = Thickness;
  pad->Clearance = Clearance;
  pad->Mask = Mask;
  pad->Shape = Shape; 

  return pad;
}
void pad_delete(sPad* pad){
  g_free(pad);
}


void pad_draw(sPad*pad, cairo_t* cr, sView* view){
  printf("pad_draw %p\n",pad);
  //manually scale here
  if(pad->Shape==PAD_SQUARE)
    cairo_set_line_cap(cr,CAIRO_LINE_CAP_SQUARE);
  else
    cairo_set_line_cap(cr,CAIRO_LINE_CAP_ROUND);
    
  cairo_set_source_rgb(cr, 0,.6, 0);
  cairo_set_line_width(cr, pad->Thickness/view->scale);
  printf("pad_draw 1\n");
  
  // convert native centimils to pixels
  cairo_move_to(cr,
    (pad->P1.x-view->origin.x)/view->scale+.5,
    (pad->P1.y-view->origin.y)/view->scale+.5);
  cairo_line_to(cr,
    (pad->P2.x-view->origin.x)/view->scale+.5,
    (pad->P2.y-view->origin.y)/view->scale+.5);
 
  cairo_stroke(cr);    
}



