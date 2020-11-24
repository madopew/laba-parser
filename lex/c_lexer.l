O   [0-7]
D   [0-9]
NZ  [1-9]
L   [a-zA-Z_]
A   [a-zA-Z_0-9]
H   [a-fA-F0-9]
HP  (0[xX])
E   ([Ee][+-]?{D}+)
P   ([Pp][+-]?{D}+)
FS  (f|F|l|L)
IS  (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP  (u|U|L)
SP  (u8|u|U|L)
ES  (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WS  [ \t\v\n\f]

%{

#include "../headers/tokentypes.h"

void comment(void);
void preprocessor(void);

%}

%option noyywrap
%option nounput

%%

"#"            {preprocessor();}

"/*"            {comment();}
"//".*          {/* consume // comment*/}

"break"			{  return BREAK; }
"case"			{  return CASE; }
"char"			{  return CHAR; }
"const"			{  return CONST; }
"continue"		{  return CONTINUE; }
"default"		{  return DEFAULT; }
"do"			{  return DO; }
"double"		{  return DOUBLE; }
"else"			{  return ELSE; }
"float"			{  return FLOAT; }
"for"			{  return FOR; }
"if"			{  return IF; }
"int"			{  return INT; }
"long"			{  return LONG; }
"return"		{  return RETURN; }
"short"			{  return SHORT; }
"signed"		{  return SIGNED; }
"sizeof"		{  return SIZEOF; }
"switch"		{  return SWITCH; }
"unsigned"		{  return UNSIGNED; }
"void"			{  return VOID; }
"while"			{  return WHILE; }

{L}{A}*					            {  return IDENTIFIER; }
{HP}{H}+{IS}?				        {  return I_CONSTANT; }
{NZ}{D}*{IS}?				        {  return I_CONSTANT; }
"0"{O}*{IS}?				        {  return I_CONSTANT; }
{CP}?"'"([^'\\\n]|{ES})+"'"		    {  return I_CONSTANT; }
{D}+{E}{FS}?				        {  return F_CONSTANT; }
{D}*"."{D}+{E}?{FS}?			    {  return F_CONSTANT; }
{D}+"."{E}?{FS}?			        {  return F_CONSTANT; }
{HP}{H}+{P}{FS}?			        {  return F_CONSTANT; }
{HP}{H}*"."{H}+{P}{FS}?			    {  return F_CONSTANT; }
{HP}{H}+"."{P}{FS}?			        {  return F_CONSTANT; }
({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+	{  return STRING_LITERAL; }

"+="			{  return ADD_ASSIGN; }
"-="			{  return SUB_ASSIGN; }
"*="			{  return MUL_ASSIGN; }
"/="			{  return DIV_ASSIGN; }
"%="			{  return MOD_ASSIGN; }
"&="			{  return AND_ASSIGN; }
"^="			{  return XOR_ASSIGN; }
"|="			{  return OR_ASSIGN; }
"<<="           {  return LEFT_ASSIGN; }
">>="           {  return RIGHT_ASSIGN; }
">>"			{  return RIGHT_OP; }
"<<"			{  return LEFT_OP; }
"++"			{  return INC_OP; }
"--"			{  return DEC_OP; }
"&&"			{  return AND_OP; }
"||"			{  return OR_OP; }
"<="			{  return LE_OP; }
">="			{  return GE_OP; }
"=="			{  return EQ_OP; }
"!="			{  return NE_OP; }
";"			    {  return SEMICOLON_CHAR; }
"{"		        {  return OPEN_CURLY; }
"}"		        {  return CLOSE_CURLY; }
","			    {  return COMMA_CHAR; }
":"			    {  return COLON_CHAR; }
"="			    {  return EQ_CHAR; }
"("			    {  return OPEN_BRACKET; }
")"			    {  return CLOSE_BRACKET; }
"["		        {  return OPEN_SQUARE; }
"]"     		{  return CLOSE_SQUARE; }
"."			    {  return DOT_CHAR; }
"&"			    {  return AND_CHAR; }
"!"			    {  return EXCL_CHAR; }
"~"             {  return TILDA_CHAR; }
"-"			    {  return MINUS_CHAR; }
"+"		    	{  return PLUS_CHAR; }
"*"		    	{  return MUL_CHAR; }
"/"		    	{  return DIV_CHAR; }
"%"		    	{  return MOD_CHAR; }
"<"		    	{  return LESS_CHAR; }
">"		    	{  return GREATER_CHAR; }
"^"		    	{  return XOR_CHAR; }
"|"			    {  return OR_CHAR; }

{WS}+		    { /* ignore white spaces */  }
.			    { /* discard bad chars */ }

%%

void comment(void) {
    int c;
    while ((c = input()) != EOF) {
        if (c == '*') {
            while ((c = input()) == '*')
                ;

            if (c == '/')
                return;

            if (c == EOF)
                break;
        }
    }
}

void preprocessor(void) {
    int c;
    while((c = input()) != EOF) {
        if(c == '\\') {
            c = input();
            c = input();
        }
        if(c == '\n' || c == '\r')
            return;
    }
}