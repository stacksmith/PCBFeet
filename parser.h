

typedef enum eTokType{
  TOK_EOF = 0,
  TOK_ELEMENT = 1,
  TOK_PIN = 2,
  TOK_PAD = 3,
  TOK_LINE = 4,
  
  TOK_BRACE_OPEN = 10,
  TOK_BRACE_CLOSE = 11,
  TOK_PAREN_OPEN = 12,
  TOK_PAREN_CLOSE = 13,
  
  TOK_NUMBER = 20,
  TOK_STRING = 21,
  
  TOK_ERROR = -1
} eTokType;

typedef union uParseData {
  GString* string;
  int number;
} uParseData;

typedef struct sParser{
  char* buffer;
  char* ptr;
  int line;
  int index;
  
  eTokType type;
  uParseData data;
  
} sParser;

sParser*  parser_new();
void      parser_init(sParser* parse);
void      parser_set(sParser* parse,char* buffer);
char      parser_get_char(sParser* parse);
char      parser_peek_char(sParser* parse);
void      parser_ws(sParser* parse);
eTokType parser_token(sParser* parse);
gboolean parser_token_dump(sParser* parse);
// helper (x) functions
gboolean parser_help_open(sParser* parse);
gboolean parser_help_close(sParser* parse);
gboolean parser_help_number(sParser* parse,int* pnum);
gboolean parser_help_string(sParser* parse,GString** pstr);
gboolean parser_help_point(sParser*parse, sPoint* point);
