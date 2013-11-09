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
  for(i=0;i<pw_last;i++)
    widget[i]=(GtkWidget*)gtk_builder_get_object (builder, parm_widget_name[i]);
}
/******************************************************************************
  Adjust the displayed parameters to match mask
*/
void parm_realize(sView* view,unsigned int uimask){
printf("parm_realize %X\n",uimask);
  int i;
  unsigned int bit = 1;
  for(i=0;i<pw_last;i++){
//printf("parm_realize %d %X %X\n",i,bit,uimask&bit);
    gtk_widget_set_sensitive(view->parm_widget[i],uimask & bit); //on or off...
    bit <<= 1; //next bit
  }
}