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
 obj_pin groups functions having to do with the pin flavor of object.
*****************************************************************************/
#include <cairo.h>
#include <gtk/gtk.h>

#include "types.h"
#include "parser.h"
#include "object.h"
#include "parm.h"
#include <math.h>

/*****************************************************************************
  Parse the pin source.  called after initial parse for P1...
*/
gboolean obj_pin_parse(sObject* obj, sParser* parse){
  obj->UIMask = UIMASK_PIN;
//printf("obj_pin_parse: uimask is %X\n",UIMASK_PIN  );
  //Thickness,clearance, mask
  if(!parser_help_number(parse,&obj->Thickness)) return FALSE;
  if(!parser_help_number(parse,&obj->Clearance)) return FALSE;
  if(!parser_help_number(parse,&obj->Mask)) return FALSE;
  //Hole
  if(!parser_help_number(parse,&obj->Hole)) return FALSE;  
  //name,number
  if(!parser_help_string(parse,&obj->Name)) return FALSE;
  if(!parser_help_string(parse,&obj->Number)) return FALSE;
//Finally, the flags
//TODO: handle pin flags
  parser_token(parse);
  obj->Shape = SHAPE_ROUND;
  return TRUE;
}

/*****************************************************************************
  Draw the pin 
*/
void obj_pin_draw(sObject* obj, cairo_t* cr, sView* view){
//printf("obj_draw in:%p\n",obj);
  int rad_outer = obj->Thickness/2;
  int rad_inner = obj->Hole/2;
//  int width = (obj->Thickness - obj->Hole)/2; //pad diameter-hole diameter/2
//  int radius = (obj->Hole + width)/2;
  //Draw pin outer shape
  cairo_set_line_cap(cr,CAIRO_LINE_CAP_BUTT);
  cairo_set_source_rgb(cr, 0,.6, 0);
  cairo_set_line_width(cr, .01);
  //Draw the outer shape
  switch(obj->Shape ){
    case SHAPE_ROUND:
//printf("pin_draw round\n");
      cairo_new_sub_path(cr);
      cairo_arc(cr,
        (obj->P1.x-view->origin.x)/view->scale +.5,
        (obj->P1.y-view->origin.y)/view->scale +.5,
        rad_outer/view->scale,
        0,2 * M_PI);
      cairo_fill(cr);
      break;
    case SHAPE_SQUARE:
      cairo_rectangle(cr,
         (obj->P1.x-rad_outer-view->origin.x)/view->scale +.5,
         (obj->P1.y-rad_outer-view->origin.y)/view->scale +.5,
         obj->Thickness/view->scale +.5,obj->Thickness/view->scale +.5);                      
      cairo_fill(cr);
      break;
    case SHAPE_OCTAGON:
//printf("pin_draw octa\n");
      break;
    default:
//printf("pin_draw illegal\n");
      break;
  }
  //Draw the hole
  cairo_set_source_rgb(cr, 1,1,1);
  cairo_new_sub_path(cr);
  cairo_arc(cr,
    (obj->P1.x-view->origin.x)/view->scale +.5,
    (obj->P1.y-view->origin.y)/view->scale +.5,
    rad_inner/view->scale,
    0,2 * M_PI);
  cairo_fill(cr);
 
}  

/*****************************************************************************
  Hit test
*/
//Are we touching a pin with view's screen coordinates
gboolean obj_pin_hit_test(sObject*pin, sView* view,sPoint* screen){
  //convert to world coordinates
  sPoint at;
  at.x = screen->x * view->scale + view->origin.x;
  at.y = screen->y * view->scale + view->origin.y;
  //For now, just use rect outline
  return (
    (at.x >  (pin->P1.x - pin->Thickness/2)) &&
    (at.x <= (pin->P1.x + pin->Thickness/2)) &&
    (at.y >  (pin->P1.y - pin->Thickness/2)) &&
    (at.y <= (pin->P1.y + pin->Thickness/2)) );
}
