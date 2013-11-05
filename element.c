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
#include <stdio.h>
#include "types.h"
#include "view.h"
#include "element.h"
#include "vtab.h"
//vtable as element sees it:

sElement* element_new(){
  sElement* ret = (sElement*)g_malloc0(sizeof(sElement));
  return ret;
}

void element_delete(sElement* el){
printf("element_delete\n");
  GSList* item = el->data;
  while(item){
    sVTAB* vtab = ((sVTAB*)item->data);
    vtab->delete(vtab);
    item = item->next;
  }
  g_slist_free(el->data);
  g_free(el);
}

void element_init(sElement* el){
  el->flags=0;
  el->description=g_string_new("Test Footprint");
  el->markPos.x = 2000;
  el->markPos.y = 1500;
  el->textPos.x = 1800;
  el->textPos.y = 2200;
  el->textDir = TEXT_HOR;
  el->textScale = 100;
  el->textFlags = g_string_new("");
  el->data = NULL;
}

void element_add(sElement*el,gpointer part){
  el->data = g_slist_prepend(el->data,part);
}
void element_draw(sElement*el, cairo_t* cr, sView* view){
//printf("element_draw %p\n",el);
  GSList* item = el->data;
  while(item){
    sVTAB* drawable = ((sVTAB*)item->data);
    drawable->draw(drawable,cr,view);
    item = item->next;
  }
    
//  sPad* pad = ((sPad*)el->data->data);
//  pad->vtab.draw(pad,cr,view);
  
 
  
}
