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
#include "pad.h"
#include "element.h"
sPad*pad;
sElement* element_new(){
  sElement* ret = (sElement*)g_malloc(sizeof(sElement));
  return ret;
}

void element_delete(sElement* el){
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
  el->textFlags = 0;
  el->data = NULL;

  pad = pad_new();
  pad_init(pad);
  element_add(el,pad);
  
}

void element_add(sElement*el,gpointer part){
  el->data = g_slist_prepend(el->data,part);
}

void element_draw(sElement*el, cairo_t* cr, sView* view){
  printf("element_draw\n");
  pad->draw(pad,cr,view);
  
 
  
}
