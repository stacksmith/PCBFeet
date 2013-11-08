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

#include "object.h"
#include "stdlib.h" //for exit

gboolean obj_pin_parse(sObject* obj, sParser* parse); 
void     obj_pin_draw(sObject* obj, cairo_t* cr, sView* view);
gboolean obj_pin_hit_test(sObject*obj, sView* view,sPoint* screen);

gboolean obj_pad_parse(sObject* obj, sParser* parse);
void     obj_pad_draw(sObject* obj, cairo_t* cr, sView* view);
gboolean obj_pad_hit_test(sObject*obj, sView* view,sPoint* screen);

gboolean obj_line_parse(sObject* obj, sParser* parse);
void     obj_line_draw(sObject* obj, cairo_t* cr, sView* view);
gboolean obj_pad_hit_test(sObject*obj, sView* view,sPoint* screen);


sObject* object_new(){
  sObject* pin = (sObject*)g_malloc0(sizeof(sObject));
  return pin;
}

/*****************************************************************************/
// parse object (inside element declaration) from text...
// 

sObject* object_parse(sParser* parse){
  sObject* obj = object_new();
  eTokType type;
  switch(type=parser_token(parse)){
    case TOK_PIN:  obj->Type = TYPE_PIN;  break;
    case TOK_PAD:  obj->Type = TYPE_PAD;  break;
    case TOK_LINE: obj->Type = TYPE_LINE; break;
    case TOK_BRACE_CLOSE:
    case TOK_PAREN_CLOSE: 
printf("object_parse:CLOSED PAREN!\n");
      return 0;
    default:
printf("object_parse: unexpected token [%d] text [%s]\n",type,parse->ptr);
      object_delete(obj);
exit(0);
  }
  TRY
    if(!parser_help_open(parse)) THROW; //open brace...
    if(!parser_help_point(parse,&obj->P1)) THROW;
    switch(obj->Type){
      case TYPE_PIN:
        if(!obj_pin_parse(obj,parse)) THROW;
        break;
      case TYPE_PAD:
        if(!obj_pad_parse(obj,parse)) THROW;
        break;
      case TYPE_LINE:
        if(!obj_line_parse(obj,parse)) THROW;
        break;
      default:printf("pin_parse: unexpected text [%s]\n",parse->ptr);
    }
    if(!parser_help_close(parse)) THROW; //closed brace...
  CATCH
    object_delete(obj);
    return 0;
  ENDTRY
  return obj;
}

void object_delete(sObject* obj){
printf("deleting object\n");
  g_string_free(obj->Name,TRUE);
  g_string_free(obj->Number,TRUE);
  g_free(obj);
}
//Are we touching a pin with view's screen coordinates
gboolean object_hit_test(sObject*obj, sView* view,sPoint* screen){
  switch(obj->Type){
    case TYPE_PIN: return obj_pin_hit_test(obj,view,screen);
    case TYPE_PAD: return obj_pad_hit_test(obj,view,screen);
    case TYPE_LINE: return obj_line_hit_test(obj,view,screen);
  }
}

void object_draw(sObject*obj, cairo_t* cr, sView* view){
  switch(obj->Type){
    case TYPE_PIN: obj_pin_draw(obj,cr,view); break;
    case TYPE_PAD: obj_pad_draw(obj,cr,view); break;
    case TYPE_LINE: obj_line_draw(obj,cr,view); break;
  }
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