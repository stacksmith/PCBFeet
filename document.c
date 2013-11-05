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
#include <stdlib.h>
#include <cairo.h>
#include <gtk/gtk.h>
#include <string.h>
#include "types.h"
#include "parser.h"
#include "view.h"
#include "element.h"
/*****************************************************************************/
#include "vtab.h"
#include "pad.h"
#include "pin.h"
#include "line.h"
#include "document.h"

/*****************************************************************************/
// parse PAD

gboolean doc_parse_pad(sDocument* doc, sParser* parse){
  sPad* pad = pad_parse(parse);
  if(!pad) return FALSE;
  element_add(doc->element,pad);  
printf("doc_parse_pad: added  to element %p\n",doc->element);
  return TRUE;
}
/*****************************************************************************/
// parse PIN, attach to element
gboolean doc_parse_pin(sDocument* doc, sParser* parse){
printf("doc_parse_pin: 1\n");
  sPin* pin = pin_parse(parse);
  if(!pin) return FALSE;
  element_add(doc->element,pin);  
  return TRUE;
}
/*****************************************************************************/
// parse LINE
gboolean doc_parse_line(sDocument* doc, sParser* parse){
printf("doc_parse_line: 1\n");
  sLine* line = line_parse(parse);
  if(!line) return FALSE;
  element_add(doc->element,line);  
  return TRUE;
}
/*****************************************************************************/
// parse ELEMENT
gboolean doc_parse_element(sDocument* doc, sParser* parse){
printf("doc_parse_element: 1\n");
  eTokType type;
  if(!parser_help_open(parse)) return FALSE; 
  //Create an element
  sElement* element = element_new();
  doc->element = element;
  //TODO: handle element flags.  For now, expect string
  if(parser_token(parse) != TOK_STRING) return FALSE;
  //description
  if(!parser_help_string(parse,&element->description)) return FALSE;
  //name and value are not used here (PCB sets them)
  if(parser_token(parse) != TOK_STRING) return FALSE;
  if(parser_token(parse) != TOK_STRING) return FALSE;
  //mark x,y
  if(!parser_help_point(parse,&element->markPos)) return FALSE;
  //text x,y
  if(!parser_help_point(parse,&element->textPos)) return FALSE;
  //text dir (0,1,2,3 ccw)
  if(!parser_help_number(parse,(int*)&element->textDir)) return FALSE;
   //text scale
  if(!parser_help_number(parse,&element->textScale)) return FALSE;
  //text //
  if(!parser_help_string(parse,&element->textFlags)) return FALSE;
  //close element
  if(!parser_help_close(parse)) return FALSE; 
  if(!parser_help_open(parse)) return FALSE; 

  // now, the innards
  gboolean done=FALSE;
  while(!done){
    type = parser_token(parse);
printf("doc_parse_element: token %d\n",type);
    switch(type){
      case TOK_PIN: if(!doc_parse_pin(doc,parse)) return FALSE; break;
      case TOK_PAD: if(!doc_parse_pad(doc,parse)) return FALSE; break;
      case TOK_LINE: if(!doc_parse_line(doc,parse)) return FALSE; break;
      case TOK_BRACE_CLOSE:
      case TOK_PAREN_CLOSE: done=TRUE; break;
      default:
        return FALSE;
    }
  }
 return TRUE;
}

gboolean doc_parse(sDocument* doc, sParser* parse){
  eTokType type;
//printf("doc_parse: %p %p\n",doc,parse);
  type=parser_token(parse);
  if(type != TOK_ELEMENT){
    fprintf(stderr,"Element expected\n");
    return FALSE;
  }
  else 
    return doc_parse_element(doc,parse);
}

sDocument* doc_new(){
  sDocument* ret = (sDocument*)g_malloc0(sizeof(sDocument));
  return ret;
}
  
void doc_init(sDocument* doc){
  sParser* parse = parser_new();
  parser_init(parse);
  if(!doc_parse(doc,parse)) {
    printf("doc_init: parser error %s\n",parse->ptr);
    exit(0);
  }
    
/*  eTokType type;
  while(TOK_EOF != (type = parser_token(parse))){
//printf("main:(line %d char %d %p '%c')\n",parse->line,parse->index,parse->ptr,*parse->ptr);

    if(!parser_token_dump(parse))
      return 0;
// return 0;
  }
  exit(1);
*/
  
}