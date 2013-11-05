#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include "types.h"
#include "parser.h"
#include <string.h>

sParser* parser_new(){
  return (sParser*)g_malloc(sizeof(sParser));
}
void parser_init(sParser* parse){
  parse->buffer = "Element[\"\" \"1x2PIN\" \"1x2PIN\" \"1x2PIN\" 27.9999mm 196.85mil -39.37mil -118.11mil 0 100 \"\"] \n\
(\r\n\
        Pin [0.0000 0.0000 81.00mil 39.37mil 84.94mil 46.00mil \"1\" \"1\" \"square,edge2\"]\n\
        Pin [100.00mil 0.0000 81.00mil 39.37mil 84.94mil 46.00mil \"2\" \"2\" \"edge2\"]\n\
        ElementLine [-49.21mil 49.21mil -49.21mil -49.21mil 10.00mil]\n\
        ElementLine [-49.21mil -49.21mil 147.64mil -49.21mil 10.00mil]\n\
        ElementLine [147.64mil -49.21mil 147.64mil 49.21mil 10.00mil]\n\
        ElementLine [147.64mil 49.21mil -49.21mil 49.21mil 10.00mil]\n\
\r)\r",0;
  parse->ptr = parse->buffer;
  parse->line = parse->index = 0;
}
/*
 \n\r\
                Pad [0.0000 0.0000 20.00mil 0 10mil 20mil 45mil \"1\" \"1\" \"square,edge2\"]\r\
*/
/*****************************************************************************/
/* get a character.  Handle all the possible end of line situations by returning
 * 0x0D.  Keep track of line and character index for error handling.
 */
char parser_get_char(sParser* parse){
  char c = *parse->ptr++;
  if(0x0D == c)
    if(0x0A == *parse->ptr)
      parse->ptr++; //handle             \n\r
  if(0x0A == c){
    c = 0x0D;
    if(0x0D == *parse->ptr)
      parse->ptr++;  //handle \r\n, return \n    
  }
  if(0x0D == c) {
    parse->index=0;
    parse->line++;
  } else
    parse->index++;
  return c;
}
/*****************************************************************************/
/* unget a character.  Simply decrement pointer.
 * Note: Don't do this for the first character!  Should work for eol.
 */
char parser_peek_char(sParser* parse){
  char c = *parse->ptr;
  if(0x0A == c)
    c = 0x0D;
  return c;
}
/*****************************************************************************/
/* skip ws.  Handle # comment to EOL
*/
void parser_ws(sParser* parse){
    char c;
    gboolean done = FALSE;
    while(!done){
      c=parser_peek_char(parse);
      switch(c){
        case ' ':
        case '\t':
        case 0x0D:
          parser_get_char(parse); //discard ws
          break;
        default:
          done=TRUE;
          break;
      }
    }
}
/*****************************************************************************
 PRIMITIVE
 Parse a quoted string
*/
eTokType parser_string(sParser* parse){
  GString* str = g_string_new("");
  parser_get_char(parse); //eat the initial "
  char c; 
  while('"' != (c=parser_get_char(parse)))
    g_string_append_c(str,c);
  parse->data.string=str;
  return parse->type=TOK_STRING;
}

/*****************************************************************************
 PRIMITIVE
 parse integer portion of the number.  Enter with parse->ptr pointing to first
 digit; exit with it pointing at the first non-digit character.
*/
double parser_integer(sParser* parse){
  double ret=0;
  while(1){
    char c=*parse->ptr;
    if(g_ascii_isdigit(c)){
      parser_get_char(parse); 
      ret = ret*10.0 + (c-'0');
    } else
      return ret;
  }
}
/*****************************************************************************
 PRIMITIVE
 parse decimal fraction of the number.  Enter with parse->ptr pointing to first
 digit; exit with it pointing at the first non-digit character.
*/
double parser_fraction(sParser* parse){
  double ret=0.0;
  double divisor=10.0;
  while(1){
    char c=*parse->ptr;
    if(g_ascii_isdigit(c)){
      parser_get_char(parse); 
      ret = ret + (c-'0')/divisor;
      divisor *= 10;
    } else
      return ret;
  }
}
eTokType parser_number(sParser* parse){ 
  //handle the sign
  gboolean negative;
  if('-'==*parse->ptr){
    parser_get_char(parse); //eat the -
    negative=TRUE;
    parser_ws(parse); //- 4 should be ok now
  } else 
    negative = FALSE;
  
  //now parse the digits
  double value = parser_integer(parse);
  if('.'==*parse->ptr){
    parser_get_char(parse); //eat .
    value += parser_fraction(parse);
  }
  //now value is good.  Handle mm and mil units
  if('m'==g_ascii_tolower(*parse->ptr)){
    if('m'==g_ascii_tolower(*(parse->ptr+1))) {
      parser_get_char(parse);parser_get_char(parse); //eat mm
      value = value*3937.007874015748;
    } else if('i'==g_ascii_tolower(*(parse->ptr+1))) {
        if('l'==g_ascii_tolower(*(parse->ptr+2))) {
          parser_get_char(parse);parser_get_char(parse);parser_get_char(parse); //eat mil
          value = value * 100.0;
        }
    }
  }
  if(negative) value =0.0-value;
  parse->data.number = value;
  return parse->type=TOK_NUMBER;
}
/*****************************************************************************
 Parse a token; fill in data and type.
 return type. 
*/

eTokType parser_token(sParser* parse){
  eTokType ret;
  parser_ws(parse);
//printf("<<<%s\n>>>",parse->ptr);
  char c=parser_peek_char(parse);
  if(('-'==c)||(g_ascii_isdigit(c)))
    return parser_number(parse);
  
//printf("parser_token(line %d char %d %p '%c')\n",parse->line,parse->index,parse->ptr,*parse->ptr);
  switch(c){
    case '"': return parser_string(parse);
    case 0: return TOK_EOF;
    case '[': parser_get_char(parse); return(parse->type=TOK_BRACE_OPEN);
    case ']': parser_get_char(parse); return(parse->type=TOK_BRACE_CLOSE);
    case '(': parser_get_char(parse); return(parse->type=TOK_PAREN_OPEN);
    case ')': parser_get_char(parse); return(parse->type=TOK_PAREN_CLOSE);
  }
  if(!strncasecmp("elementline",parse->ptr,11)) { parse->ptr+=11; parse->index+=11;
    return(parse->type=TOK_LINE); }
  if(!strncasecmp("element",parse->ptr,7)) { parse->ptr+=7; parse->index+=7;
    return(parse->type=TOK_ELEMENT); }
  if(!strncasecmp("pin",parse->ptr,3)) { parse->ptr+=3; parse->index+=3;
    return(parse->type=TOK_PIN); }
  if(!strncasecmp("pad",parse->ptr,3)) { parse->ptr+=3; parse->index+=3;
    return(parse->type=TOK_PAD); }
  //Otherwise, we have an error
printf("parser_token:ERROR \n");
  return parse->type=TOK_ERROR;
}


gboolean parser_token_dump(sParser*parse){
  eTokType type=parse->type;
  uParseData data=parse->data;
  printf("(line %d char %d)type %d,",parse->line,parse->index,type);
  switch(type){
    case TOK_EOF: return FALSE;
    case TOK_ELEMENT: printf("ELEMENT "); break;
    case TOK_PIN: printf("PIN "); break;
    case TOK_PAD: printf("PAD "); break;
    case TOK_LINE: printf("ELEMENT_LINE "); break;
    case TOK_BRACE_OPEN:  printf("[ "); break;
    case TOK_BRACE_CLOSE: printf("] "); break;
    case TOK_PAREN_OPEN:  printf("( "); break;
    case TOK_PAREN_CLOSE: printf(") "); break;
    case TOK_NUMBER: printf("%d ",data.number); break;
    case TOK_STRING: printf("\"%s\" ",data.string->str); break;
    case TOK_ERROR: printf("ERROR"); return FALSE;
    default: printf("UNKNOWN\n"); return FALSE;
  }
  printf("\n");
  return TRUE;
}
    
/*****************************************************************************
******************************************************************************
*****************************************************************************/
// x Helper functions for parsing 

/*****************************************************************************/
// open - parse ( or [ and return TRUE. 
gboolean parser_help_open(sParser* parse){
  parser_token(parse);
  if((parse->type == TOK_BRACE_OPEN)||(parse->type==TOK_PAREN_OPEN))
    return TRUE;
  return FALSE; 
}
/*****************************************************************************/
// close - parse ) or ] and return TRUE
gboolean parser_help_close(sParser* parse){
  parser_token(parse);
  if((parse->type == TOK_BRACE_CLOSE)||(parse->type==TOK_PAREN_CLOSE))
    return TRUE;
  return FALSE; 
}
/*****************************************************************************/
// number - parse numeric value into the  int*
gboolean parser_help_number(sParser* parse,int*pnum){
  if(parser_token(parse) != TOK_NUMBER) return FALSE;
  *pnum = parse->data.number;
  return TRUE;
}
/*****************************************************************************/
// string - parse a string into the  GString*
gboolean parser_help_string(sParser* parse,GString** pstr){
  if(parser_token(parse) != TOK_STRING) return FALSE;
  *pstr = parse->data.string;
  return TRUE;
}
/*****************************************************************************/
// point - parse an x y pair into the  sPoint*
gboolean parser_help_point(sParser*parse, sPoint* point){
  if(parser_token(parse) != TOK_NUMBER) return FALSE;
    point->x = parse->data.number;
  if(parser_token(parse) != TOK_NUMBER) return FALSE;
    point->y = parse->data.number;
  return TRUE;
}
