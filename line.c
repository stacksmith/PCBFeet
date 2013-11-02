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
#include "line.h"


sLine* line_new(int X1,int Y1,int X2,int Y2,
  int Thickness){
  
  sLine* line = (sLine*)g_malloc(sizeof(sLine));
  line->vtab.draw = (ptrDraw)&line_draw;  //comply with generic vtab func 

  line->X1     = X1;
  line->Y1     = Y1;
  line->X2     = X2;
  line->Y2     = Y2;
  line->Thickness = Thickness;

  return line;
}
void line_delete(sLine* line){
  g_free(line);
}


void line_draw(sLine*line, cairo_t* cr, sView* view){
  printf("line_draw %p\n",line);
  //manually scale here
  cairo_set_line_cap(cr,CAIRO_LINE_CAP_ROUND);
  cairo_set_source_rgb(cr, 0,0,0);
  cairo_set_line_width(cr, line->Thickness/view->scale);
  printf("line_draw 1\n");
  
  // convert native centimils to pixels
  cairo_move_to(cr,
    (line->X1-view->origin.x)/view->scale,
    (line->Y1-view->origin.y)/view->scale);
  cairo_line_to(cr,
    (line->X2-view->origin.x)/view->scale,
    (line->Y2-view->origin.y)/view->scale);
 
  cairo_stroke(cr);    
}

