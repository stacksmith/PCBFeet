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
 obj_pad groups functions having to do with the pad flavor of object.
*****************************************************************************/
#include <cairo.h>
#include <gtk/gtk.h>

#include "types.h"
#include "parser.h"
#include "object.h"
#include "parm.h"
/*****************************************************************************
  Parse the pad source.  called after initial parse for P1...
*/
gboolean obj_pad_parse(sObject* obj, sParser* parse){
  obj->UIMask = UIMASK_PAD;
  //pad points
  if(!parser_help_point(parse,&obj->P2)) return FALSE;
  //Thickness,clearance, mask
  if(!parser_help_number(parse,&obj->Thickness)) return FALSE;
  if(!parser_help_number(parse,&obj->Clearance)) return FALSE;
  if(!parser_help_number(parse,&obj->Mask)) return FALSE;
  //name,number
  if(!parser_help_string(parse,&obj->Name)) return FALSE;
  if(!parser_help_string(parse,&obj->Number)) return FALSE;
  //Finally, the flags
  //TODO: handle pad flags
  parser_token(parse);
  obj->Shape = SHAPE_ROUND;
  return TRUE;
}
/*****************************************************************************
  Draw pad
*/
void obj_pad_draw(sObject*pad, cairo_t* cr, sView* view){
  printf("pad_draw %p\n",pad);
  //manually scale here
  if(pad->Shape==SHAPE_SQUARE)
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

}
/*****************************************************************************
  Hit test
*/
//Are we touching a pad with view's screen coordinates
gboolean obj_pad_hit_test(sObject*pad, sView* view,sPoint* screen){
  //convert to world coordinates
  sPoint at;
  at.x = screen->x * view->scale + view->origin.x;
  at.y = screen->y * view->scale + view->origin.y;
  //For now, just use rect outline
  return (
    (at.x >  (pad->P1.x - pad->Thickness/2)) &&
    (at.x <= (pad->P1.x + pad->Thickness/2)) &&
    (at.y >  (pad->P1.y - pad->Thickness/2)) &&
    (at.y <= (pad->P1.y + pad->Thickness/2)) );
}
