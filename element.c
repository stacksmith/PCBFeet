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
#include "parser.h"
#include "pin.h"
#include "pad.h"
#include "line.h"
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
}

void element_hit_test(sElement*el, sView* view){
//printf("element_draw %p\n",el);
  GSList* item = el->data;
  while(item){
    sVTAB* vtab = ((sVTAB*)item->data);
    if(vtab->hit_test(vtab,view,&view->pxMouse))
      printf("HIT ITEM %p\n",item);
    item = item->next;
  }
}

/*****************************************************************************/
// parse ELEMENT
sElement* element_parse(sParser* parse){
printf("element_parse: 1\n");
  sElement* element = element_new();
  TRY
    eTokType type;
    if(!parser_help_open(parse)) THROW; 
    //Create an element
    //TODO: handle element flags.  For now, expect string
    if(parser_token(parse) != TOK_STRING) THROW;
    //description
    if(!parser_help_string(parse,&element->description)) THROW;
    //name and value are not used here (PCB sets them)
    if(parser_token(parse) != TOK_STRING) THROW;
    if(parser_token(parse) != TOK_STRING) THROW;
    //mark x,y
    if(!parser_help_point(parse,&element->markPos)) THROW;
    //text x,y
    if(!parser_help_point(parse,&element->textPos)) THROW;
    //text dir (0,1,2,3 ccw)
    if(!parser_help_number(parse,(int*)&element->textDir)) THROW;
    //text scale
    if(!parser_help_number(parse,&element->textScale)) THROW;
    //text //
    if(!parser_help_string(parse,&element->textFlags)) THROW;
    //close element
    if(!parser_help_close(parse)) THROW; 
    if(!parser_help_open(parse)) THROW; 

    // now, the innards
    gboolean done=FALSE;
    while(!done){
      type = parser_token(parse);
  printf("doc_parse_element: token %d\n",type);
      switch(type){
        case TOK_PIN: {
          sPin* pin = pin_parse(parse);
          if(!pin) THROW;
          element_add(element,pin);  
          } break;
        case TOK_PAD: {
          sPad* pad = pad_parse(parse);
          if(!pad) THROW;
          element_add(element,pad);  
          } break;
        case TOK_LINE: {
          sLine* line = line_parse(parse);
          if(!line) THROW;
          element_add(element,line);  
          } break;
          
        case TOK_BRACE_CLOSE:
        case TOK_PAREN_CLOSE: done=TRUE; break;
        default:
          THROW;
      }
    }
  CATCH
    element_delete(element);
    element = 0;
  ENDTRY   
  return element;
}

