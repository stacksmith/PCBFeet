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


void parm_init(GtkWidget* widget[],GtkBuilder*builder){
  int i;
  for(i=0;i<pw_last;i++){
    widget[i]=(GtkWidget*)gtk_builder_get_object (builder, parm_widget_name[i]);
printf(".");
  }
}

void parm_pin(sView* view,sObject* pin){
/*  gtk_label_set_text(parm->label_frame,"Pin:");
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
  
 */ 
}