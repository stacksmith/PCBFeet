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
#include "document.h"


gboolean doc_parse(sDocument* doc, sParser* parse){
  eTokType type;
//printf("doc_parse: %p %p\n",doc,parse);
  type=parser_token(parse);
  if(type != TOK_ELEMENT){
    fprintf(stderr,"Element expected\n");
    return FALSE;
  }
  else 
    return (doc->element = element_parse(parse))?TRUE:FALSE;
    
}

sDocument* doc_new(){
  sDocument* ret = (sDocument*)g_malloc0(sizeof(sDocument));
  return ret;
}
  
void doc_init(sDocument* doc){
  sParser* parse = parser_new();
//  parser_init(parse);
  FILE* f = fopen("test.fp","r");
  if(!f) {
    fprintf(stderr,"file not found\n");
    exit(0);
  }
  char* buf = g_malloc(0x10000);
  int len = fread(buf,1,0x10000,f);
  buf[len]=0;
  parser_set(parse,buf);
  if(!doc_parse(doc,parse)) {
    printf("doc_init: parser error %s\n",parse->ptr);
    element_delete(doc->element);
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