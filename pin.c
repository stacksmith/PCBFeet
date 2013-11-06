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
#include "pin.h"
#include <math.h>
gboolean pin_hit_test(sPin*pin, sView* view,sPoint* screen);

sPin* pin_new(){
  sPin* pin = (sPin*)g_malloc0(sizeof(sPin));
  pin->vtab.draw     = (ptrDraw)  &pin_draw;//comply with generic vtab func 
  pin->vtab.delete   = (ptrDelete)&pin_delete;
  pin->vtab.hit_test = (ptrHitTest)&pin_hit_test;
  return pin;
}
/*****************************************************************************/
// parse PIN from text...

sPin* pin_parse(sParser* parse){
  //create pin
  sPin* pin = pin_new();
  TRY
    if(!parser_help_open(parse)) THROW; //open brace...
    //pin points q 
    if(!parser_help_point(parse,&pin->P1)) THROW;
    //Thickness,clearance, mask
    if(!parser_help_number(parse,&pin->Thickness)) THROW;
    if(!parser_help_number(parse,&pin->Clearance)) THROW;
    if(!parser_help_number(parse,&pin->Mask)) THROW;
    //Hole
    if(!parser_help_number(parse,&pin->Hole)) THROW;  
    //name,number
    if(!parser_help_string(parse,&pin->Name)) THROW;
    if(!parser_help_string(parse,&pin->Number)) THROW;
  //Finally, the flags
  //TODO: handle pin flags
    parser_token(parse);
    pin->Shape = PIN_ROUND;
    if(!parser_help_close(parse)) THROW; //closed brace...
  CATCH
    g_free(pin);
    return 0;
    
  ENDTRY
  return pin;
}

sPin* pin_init(sPin* pin, int X,int Y,
  int Thickness,int Clearance, int Mask,
  int Hole,
  GString*Name,GString* Number,  ePinShape Shape){
  
  pin->P1.x      = X;
  pin->P1.y      = Y;
  pin->Thickness = Thickness;
  pin->Clearance = Clearance;
  pin->Mask   = Mask; 
  pin->Hole   = Hole;
  pin->Name   = Name;
  pin->Number = Number;
  pin->Shape  = Shape;
  return pin;
}
void pin_delete(sPin* pin){
printf("deleting pin\n");
  g_string_free(pin->Name,TRUE);
  g_string_free(pin->Number,TRUE);
  g_free(pin);
}
//Are we touching a pin with view's screen coordinates
gboolean pin_hit_test(sPin*pin, sView* view,sPoint* screen){
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

void pin_draw(sPin*pin, cairo_t* cr, sView* view){
printf("pin_draw in:%p\n",pin);
  int rad_outer = pin->Thickness/2;
  int rad_inner = pin->Hole/2;
//  int width = (pin->Thickness - pin->Hole)/2; //pad diameter-hole diameter/2
//  int radius = (pin->Hole + width)/2;
  //Draw pin outer shape
    cairo_set_line_cap(cr,CAIRO_LINE_CAP_BUTT);
    cairo_set_source_rgb(cr, 0,.6, 0);
    cairo_set_line_width(cr, .01);
    //Draw the outer shape
  switch(pin->Shape ){
    case PIN_ROUND:
printf("pin_draw round\n");
      cairo_new_sub_path(cr);
      cairo_arc(cr,
        (pin->P1.x-view->origin.x)/view->scale +.5,
        (pin->P1.y-view->origin.y)/view->scale +.5,
        rad_outer/view->scale,
        0,2 * M_PI);
      cairo_fill(cr);
      break;
    case PIN_SQUARE:
      cairo_rectangle(cr,
         (pin->P1.x-rad_outer-view->origin.x)/view->scale +.5,
         (pin->P1.y-rad_outer-view->origin.y)/view->scale +.5,
         pin->Thickness/view->scale +.5,pin->Thickness/view->scale +.5);                      
      cairo_fill(cr);
      break;
    case PIN_OCTAGON:
printf("pin_draw octa\n");
      break;
    default:
printf("pin_draw illegal\n");
      break;
  }
  //Draw the hole
    cairo_set_source_rgb(cr, 1,1,1);
    cairo_new_sub_path(cr);
    cairo_arc(cr,
      (pin->P1.x-view->origin.x)/view->scale +.5,
      (pin->P1.y-view->origin.y)/view->scale +.5,
      rad_inner/view->scale,
      0,2 * M_PI);
      cairo_fill(cr);
 
  
  cairo_stroke(cr);    
}

/*****************************************************************************/
// a widget containing the controls for modifying the data...
// We create a single one for each type, and hide all but the selected object's
// type.  Then we quickly update the data
sUIPin* ui_pin_new(){
  //allocate the ui structure
  sUIPin* ui = (sUIPin*)g_malloc(sizeof(sUIPin));
  //create a builder, load and get data
  //TODO: this should be done from ram, for speed
  GtkBuilder *builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "ui_pin.ui", NULL);
  ui->frame =     (GtkFrame*)gtk_builder_get_object (builder, "frame1"); 
  ui->name =      (GtkEntry*)gtk_builder_get_object (builder, "pin_name"); 
  ui->number =    (GtkEntry*)gtk_builder_get_object (builder, "pin_number"); 
  ui->p1_x =      (GtkEntry*)gtk_builder_get_object (builder, "pin_p1_x"); 
  ui->p1_y =      (GtkEntry*)gtk_builder_get_object (builder, "pin_p1_y"); 
  ui->thickness = (GtkEntry*)gtk_builder_get_object (builder, "pin_thickness"); 
  ui->clearance = (GtkEntry*)gtk_builder_get_object (builder, "pin_clearance"); 
  ui->mask =      (GtkEntry*)gtk_builder_get_object (builder, "pin_mask"); 
  ui->hole =      (GtkEntry*)gtk_builder_get_object (builder, "pin_hole"); 
  ui->shape = (GtkComboBoxText*)gtk_builder_get_object (builder, "pin_shape"); 
  return ui;
}