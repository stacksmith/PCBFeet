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
#include "view.h"
#include "parser.h"
#include "object.h"
/* A new direction:
 -Create an array containing all the widget pointers in the config area;
 -Create a bitmap of which widgets are active for each object type;
 -Upon selection change, AND the bitmaps of all objects selected;
 -Activate the resulting intersection.  This will allow multiple objects
   to be aligned to x or y, etc. 
 -For multiple items, name/number should not be activated.
*/


void parm_init(sParm* parm,GtkBuilder*builder){
  parm->label_frame = (GtkLabel*)gtk_builder_get_object (builder, "parm_label_frame");
  parm->label_name  = (GtkLabel*)gtk_builder_get_object (builder, "parm_label_name");
  parm->name        = (GtkEntry*)gtk_builder_get_object (builder, "parm_name");
  parm->label_number= (GtkLabel*)gtk_builder_get_object (builder, "parm_label_number");
  parm->number      = (GtkEntry*)gtk_builder_get_object (builder, "parm_number");
//
  parm->box_p1      = (GtkBox*)gtk_builder_get_object (builder, "parm_box_p1");
  parm->box_p2      = (GtkBox*)gtk_builder_get_object (builder, "parm_box_p2");
  parm->label_p1    = (GtkLabel*)gtk_builder_get_object (builder, "parm_label_p1");
  parm->p1_x        = (GtkEntry*)gtk_builder_get_object (builder, "parm_p1_x");
  parm->p1_y        = (GtkEntry*)gtk_builder_get_object (builder, "parm_p1_y");
  parm->label_p2    = (GtkLabel*)gtk_builder_get_object (builder, "parm_label_p2");
  parm->p2_x        = (GtkEntry*)gtk_builder_get_object (builder, "parm_p2_x");
  parm->p2_y        = (GtkEntry*)gtk_builder_get_object (builder, "parm_p2_y");
  parm->label_hole  = (GtkLabel*)gtk_builder_get_object (builder, "parm_label_hole");
  parm->hole        = (GtkEntry*)gtk_builder_get_object (builder, "parm_hole");
//
  parm->label_thick  = (GtkLabel*)gtk_builder_get_object (builder, "parm_label_thick");
  parm->thick        = (GtkEntry*)gtk_builder_get_object (builder, "parm_thick");
  parm->label_clear  = (GtkLabel*)gtk_builder_get_object (builder, "parm_label_clear");
  parm->clear        = (GtkEntry*)gtk_builder_get_object (builder, "parm_clear");
  parm->label_mask   = (GtkLabel*)gtk_builder_get_object (builder, "parm_label_mask");
  parm->mask         = (GtkEntry*)gtk_builder_get_object (builder, "parm_mask");
//
  parm->label_shape  = (GtkLabel*)gtk_builder_get_object (builder, "parm_label_shape");
  parm->shape        = (GtkComboBoxText*)gtk_builder_get_object (builder, "parm_shape");
  parm->label_layer  = (GtkLabel*)gtk_builder_get_object (builder, "parm_label_layer");
  parm->layer        = (GtkComboBoxText*)gtk_builder_get_object (builder, "parm_layer");
}

void parm_pin(sParm* parm,sObject* pin){
  gtk_label_set_text(parm->label_frame,"Pin:");
  //set and show name
  gtk_entry_set_text(parm->name,pin->Name->str);
  //set and show num
  gtk_entry_set_text(parm->number,pin->Number->str);
  //set and show p`
  
  gtk_entry_set_text(parm->p1_x,"1234");
  gtk_entry_set_text(parm->p1_y,"5678");
  //hide p2
  gtk_entry_set_text(parm->p1_x,"");
  gtk_entry_set_text(parm->p1_y,"");
  gtk_widget_set_sensitive((GtkWidget*)parm->box_p2,FALSE);
  gtk_widget_set_sensitive((GtkWidget*)parm->label_p2,FALSE);
  
//  gtk_widget_hide((GtkWidget*)parm->p2_x);
//  gtk_widget_hide((GtkWidget*)parm->p2_y);
  //set and show thickness, clear, mask
  gtk_entry_set_text(parm->thick,"1111");
  gtk_entry_set_text(parm->clear,"2222");
  gtk_entry_set_text(parm->mask,"3333");
  
  
}