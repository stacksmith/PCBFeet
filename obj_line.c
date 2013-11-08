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
/*****************************************************************************
 obj_line groups functions having to do with the pad flavor of object.
*****************************************************************************/
#include <cairo.h>
#include <gtk/gtk.h>

#include "types.h"
#include "parser.h"
#include "object.h"
/*****************************************************************************
  Parse the line source.  called after initial parse for P1...
*/
gboolean obj_line_parse(sObject* obj, sParser* parse){
  //line points
  if(!parser_help_point(parse,&obj->P2)) return FALSE;
  //Thickness
  if(!parser_help_number(parse,&obj->Thickness)) return FALSE;
  return TRUE;
}

/*****************************************************************************
  Draw the line
*/
void obj_line_draw(sObject* line, cairo_t* cr, sView* view){
  printf("line_draw %p\n",line);
  //manually scale here
  cairo_set_line_cap(cr,CAIRO_LINE_CAP_ROUND);
  cairo_set_source_rgb(cr, .3,.3,.3);
  cairo_set_line_width(cr, line->Thickness/view->scale);
  printf("line_draw 1\n");
  
  // convert native centimils to pixels
  cairo_move_to(cr,
    (line->P1.x-view->origin.x)/view->scale,
    (line->P1.y-view->origin.y)/view->scale);
  cairo_line_to(cr,
    (line->P2.x-view->origin.x)/view->scale,
    (line->P2.y-view->origin.y)/view->scale);
   
}
/*****************************************************************************
  Hit test
*/
//Are we touching a line with view's screen coordinates
gboolean obj_line_hit_test(sObject*line, sView* view,sPoint* screen){
  //convert to world coordinates
  sPoint at;
  at.x = screen->x * view->scale + view->origin.x;
  at.y = screen->y * view->scale + view->origin.y;
  //For now, just use rect outline
  return (
    (at.x >  (line->P1.x - line->Thickness/2)) &&
    (at.x <= (line->P1.x + line->Thickness/2)) &&
    (at.y >  (line->P1.y - line->Thickness/2)) &&
    (at.y <= (line->P1.y + line->Thickness/2)) );
}
