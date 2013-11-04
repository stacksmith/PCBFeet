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
#include "view.h"
#include "element.h"
/*****************************************************************************/
#include "vtab.h"
#include "pad.h"
#include "pin.h"
#include "line.h"
#include "parser.h"
#include "document.h"

/*****************************************************************************/
// open brace or open paren TODO: set the multiplier...
gboolean doc_parse_open(sDocument* doc, sParser* parse){
  eTokType type = parser_token(parse);
  if((type == TOK_BRACE_OPEN)||(type==TOK_PAREN_OPEN))
    return TRUE;
  return FALSE; 
}
gboolean doc_parse_close(sDocument* doc, sParser* parse){
  eTokType type = parser_token(parse);
  if((type == TOK_BRACE_CLOSE)||(type==TOK_PAREN_CLOSE))
    return TRUE;
  return FALSE; 
}

gboolean doc_parse_number(sDocument* doc, sParser* parse,int* pnum){
  if(parser_token(parse) != TOK_NUMBER) return FALSE;
  *pnum = parse->data.number;
  return TRUE;
}
gboolean doc_parse_string(sDocument* doc, sParser* parse,GString** pstr){
  if(parser_token(parse) != TOK_STRING) return FALSE;
  *pstr = parse->data.string;
  return TRUE;
}

/*****************************************************************************/
// parse "x y " into point reference
gboolean doc_parse_point(sDocument* doc,sParser*parse, sPoint* point){
  if(parser_token(parse) != TOK_NUMBER) return FALSE;
    point->x = parse->data.number;
  if(parser_token(parse) != TOK_NUMBER) return FALSE;
    point->y = parse->data.number;
  return TRUE;
}
/*****************************************************************************/
// parse PAD

gboolean doc_parse_pad(sDocument* doc, sParser* parse){
  if(!doc_parse_open(doc,parse)) return FALSE; //open brace...
  //create pad
  sPad* pad = pad_new();
  //pad points
  if(!doc_parse_point(doc,parse,&pad->P1)) return FALSE;
  if(!doc_parse_point(doc,parse,&pad->P2)) return FALSE;
  //Thickness,clearance, mask
  if(!doc_parse_number(doc,parse,&pad->Thickness)) return FALSE;
  if(!doc_parse_number(doc,parse,&pad->Clearance)) return FALSE;
  if(!doc_parse_number(doc,parse,&pad->Mask)) return FALSE;
  //name,number
  if(!doc_parse_string(doc,parse,&pad->Name)) return FALSE;
  if(!doc_parse_string(doc,parse,&pad->Number)) return FALSE;
  //Finally, the flags
  //TODO: handle pad flags
  parser_token(parse);
  pad->Shape = PAD_ROUND;
  if(!doc_parse_close(doc,parse)) return FALSE; //closed brace...  
  element_add(doc->element,pad);  
printf("doc_parse_pad: added  to element %p\n",doc->element);
  return TRUE;
}
/*****************************************************************************/
// parse PIN, attach to element
gboolean doc_parse_pin(sDocument* doc, sParser* parse){
printf("doc_parse_pin: 1\n");
  if(!doc_parse_open(doc,parse)) return FALSE; //open brace...
  //create pin
  sPin* pin = pin_new();
  //pin points q 
  if(!doc_parse_point(doc,parse,&pin->P1)) return FALSE;
  //Thickness,clearance, mask
  if(!doc_parse_number(doc,parse,&pin->Thickness)) return FALSE;
  if(!doc_parse_number(doc,parse,&pin->Clearance)) return FALSE;
  if(!doc_parse_number(doc,parse,&pin->Mask)) return FALSE;
  //Hole
  if(!doc_parse_number(doc,parse,&pin->Hole)) return FALSE;  
  //name,number
  if(!doc_parse_string(doc,parse,&pin->Name)) return FALSE;
  if(!doc_parse_string(doc,parse,&pin->Number)) return FALSE;
  //Finally, the flags
  //TODO: handle pin flags
  parser_token(parse);
  pin->Shape = PIN_ROUND;
  if(!doc_parse_close(doc,parse)) return FALSE; //closed brace...
  element_add(doc->element,pin);  
  return TRUE;
}
/*****************************************************************************/
// parse LINE
gboolean doc_parse_line(sDocument* doc, sParser* parse){
  if(!doc_parse_open(doc,parse)) return FALSE; //open brace...
  //create pin
  sLine* line = line_new();
  //line points
  if(!doc_parse_point(doc,parse,&line->P1)) return FALSE;
  if(!doc_parse_point(doc,parse,&line->P2)) return FALSE;
  //Thickness
  if(!doc_parse_number(doc,parse,&line->Thickness)) return FALSE;
  if(!doc_parse_close(doc,parse)) return FALSE; //closed brace...  
  element_add(doc->element,line);  
printf("doc_parse_line: added line to element %p\n",doc->element);
  return TRUE;
}
/*****************************************************************************/
// parse ELEMENT
gboolean doc_parse_element(sDocument* doc, sParser* parse){
printf("doc_parse_element: 1\n");
  eTokType type;
  if(!doc_parse_open(doc,parse)) return FALSE; 
  //Create an element
  sElement* element = element_new();
  doc->element = element;
  //TODO: handle element flags.  For now, expect string
  if(parser_token(parse) != TOK_STRING) return FALSE;
  //description
  if(!doc_parse_string(doc,parse,&element->description)) return FALSE;
  //name and value are not used here (PCB sets them)
  if(parser_token(parse) != TOK_STRING) return FALSE;
  if(parser_token(parse) != TOK_STRING) return FALSE;
  //mark x,y
  if(!doc_parse_point(doc,parse,&element->markPos)) return FALSE;
  //text x,y
  if(!doc_parse_point(doc,parse,&element->textPos)) return FALSE;
  //text dir (0,1,2,3 ccw)
  if(!doc_parse_number(doc,parse,(int*)&element->textDir)) return FALSE;
   //text scale
  if(!doc_parse_number(doc,parse,&element->textScale)) return FALSE;
  //text //
  if(!doc_parse_string(doc,parse,&element->textFlags)) return FALSE;
  //close element
  if(!doc_parse_close(doc,parse)) return FALSE; 
  if(!doc_parse_open(doc,parse)) return FALSE; 

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