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
#include <string.h>
#include "types.h"
#include "view.h"
#include "element.h"
/*****************************************************************************/
#include "vtab.h"
#include "pad.h"
#include "pin.h"
#include "line.h"
#include "document.h"
/* some test text to be fed into the scanner */
static const gchar *test_text =
"Element[\"\" \"1x2PIN\" \"1x2PIN\" \"1x2PIN\" 27.9999mm 196.85mil -39.37mil -118.11mil 0 100 \"\"] \
(\
        Pin [0.0000 0.0000 81.00mil 39.37mil 84.94mil 46.00mil \"1\" \"1\" \"square,edge2\"]\
        Pin [100.00mil 0.0000 81.00mil 39.37mil 84.94mil 46.00mil \"2\" \"2\" \"edge2\"]\
        ElementLine [-49.21mil 49.21mil -49.21mil -49.21mil 10.00mil]\
        ElementLine [-49.21mil -49.21mil 147.64mil -49.21mil 10.00mil]\
        ElementLine [147.64mil -49.21mil 147.64mil 49.21mil 10.00mil]\
        ElementLine [147.64mil 49.21mil -49.21mil 49.21mil 10.00mil]\
\
                Pad [0.0000 0.0000 20.00mil 0 10mil 20mil 45mil \"1\" \"1\" \"square,edge2\"]\
\
";

enum {
  SYMBOL_ELEMENT = G_TOKEN_LAST + 1,
  SYMBOL_PIN = G_TOKEN_LAST + 2,
  SYMBOL_PAD = G_TOKEN_LAST + 3,
  SYMBOL_LINE = G_TOKEN_LAST + 4,
  SYMBOL_MIL = G_TOKEN_LAST + 5,
  SYMBOL_MM = G_TOKEN_LAST + 6,
};

/* symbol array */
static const struct {
  gchar *symbol_name;
  guint  symbol_token;
} symbols[] = {
  { "Element", SYMBOL_ELEMENT, },
  { "Pin", SYMBOL_PIN, },
  { "Pad", SYMBOL_PAD, },
  { "ElementLine", SYMBOL_LINE, },
  { "mil", SYMBOL_MIL, },
  { "mm", SYMBOL_MM, },

  { NULL, 0, },
}, *symbol_p = symbols;



void parse_element(GScanner* scanner){
}

sDocument* doc_new(){
  sDocument* doc = (sDocument*)g_malloc(sizeof(sDocument));
  sElement* el= element_new();
  element_init(el);
//  element_add(el,pad_new(1000,1000,2000,1500,  500,1000,1500,  "PadName","1",""));
  element_add(el,pin_new(0,0,8100,3937,8494,4600,"1","1",PIN_SQUARE));
  element_add(el,pin_new(10000,0,8100,3937,8494,4600,"2","2",PIN_ROUND));
  element_add(el,line_new(-4921, 4921,-4921,-4921,1000));
  element_add(el,line_new(-4921,-4921,14764,-4921,1000));
  element_add(el,line_new(14764,-4921,14764, 4921,1000));
  element_add(el,line_new(14764, 4921,-4921, 4921,1000));

  element_add(el,pad_new(10000,10000,20000,10000, 10000,20000,45000,"3","3",PAD_ROUND));
 // element_add(el,pin_new(4000,4000, 1000,1000,1500, 500, "Pin1","1",""));
  doc->element = el;
  return doc;
  
  
    GScanner *scanner;
  guint expected_token;
  scanner = g_scanner_new (NULL);
  scanner->config->numbers_2_int = TRUE;
  scanner->config->int_2_float = FALSE;
  while (symbol_p->symbol_name)
    {
      g_scanner_add_symbol (scanner,
                            symbol_p->symbol_name,
                            GINT_TO_POINTER (symbol_p->symbol_token));
      symbol_p++;
    }
  g_scanner_input_text (scanner, test_text, strlen (test_text));
  scanner->input_name = "test text";
  /* expect a symbol ("am") */
  GTokenType type;
  do {
  type = g_scanner_get_next_token (scanner);
  switch(type){
    case SYMBOL_ELEMENT:
      printf("ELEMENT!\n");
      break;
    case G_TOKEN_SYMBOL:
      printf("SYMBOL!:");
      switch(scanner->value.v_int){
        case SYMBOL_ELEMENT: printf("Element\n"); break;
        case SYMBOL_PIN: printf("Pin\n"); break;
        case SYMBOL_PAD: printf("Pad\n"); break;
        case SYMBOL_LINE: printf("ElementLine\n"); break;
      }
      break;
    case G_TOKEN_STRING:
      printf("STRING! %s\n",scanner->value.v_string);
      break;
    case G_TOKEN_CHAR:
      printf("CHAR! %c\n",scanner->value.v_int);
      break;
    case G_TOKEN_NONE:
      printf("NONE!\n");
      break;
    case G_TOKEN_LEFT_BRACE:
      printf("LEFTBRAC\n");
      break;
    default:
      printf("WHATEVER! %d %c\n",type,type);
    }
  } while (type !=G_TOKEN_EOF);
  
  exit(1);
}