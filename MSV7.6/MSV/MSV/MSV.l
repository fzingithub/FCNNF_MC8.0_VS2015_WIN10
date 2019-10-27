%{
#define  _LEX_CPP_
#define isatty _isatty

#include "function.h"
#include "SyntaxNode.h"
#include "parse.cpp.h"

#include <io.h>
#include <stdio.h>

extern CMAP g_StructNameMap;
extern "C" int yywrap();
extern void error( char *cFormat, ... ); 

int  lineno=1;
bool hasOr=false;
void DeleteSingleLineComment (void);
void passStringConstant (void);
%}

%x comment

LETTER	 [a-zA-Z_]
DIV		 [._]
DIGIT    [0-9]+

OCTONARY \'[\\][0-3][0-7]?[0-7]?\'
HEXDIGIT [0][xX]([0-9]|[a-fA-F])+
IDENT    {LETTER}({LETTER}|[$]|{DIGIT})*

WSPACE   [ \t]+
RFLOAT	 [0-9]+\.[0-9]*
EXPONENT [0-9]+[\.]?[0-9]*[e][+|-]*[0-9]+
CHARNULL  \'\\0\'
ESCAPECHAR [\\][b|f|n|r|t|0|'|"|\\]
STR      \"([^\"]|{ESCAPECHAR})*\"
RCHAR     \'([^\']|{ESCAPECHAR})\'

/*
\b 退格
\f 走纸换页
\n 换行
\r 回车
\t 横向跳格 (Ctrl-I)
\' 单引号
\" 双引号
\\反斜杠
*/

%%

"/*"       BEGIN comment;
"include"  {return INCLUDE;}
"and"      {return AND;}
"or"       { hasOr=true; return OR;}
"AND"      {return BOOL_AND;}
"OR"       {return BOOL_OR;}
"skip"     {return SKIP;}
"fin"      {return FINAL;}
"empty"    {return EMPTY;}
"halt"     {return HALT;}
"await"    {return AWAIT;}
"||"       {return PARALLEL;}
"prj"      {return PROJECTION;}
"next"     {return NEXT;}
"w_next"   {return W_NEXT;}
"keep"     {return KEEP;}
"alw"      {return ALWAYS;}
"som"      {return SOMETIMES;}
":"        {return COLON;}
"local"    {return EXIST;}
"define"   {return DEFINE;}
"function" {return FUNCTION;}
"struct"   {return STRUCT;}
"union"	   {return UNION;}
"true"     {return MY_TRUE;}
"false"    {return MY_FALSE;}

"len"      {return LENGTH;}
"frame"    {return FRAME;}
"if"       {return IF;}
"then"     {return THEN;}
"else"     {return ELSE;}
"for"      {return FOR;}
"times"    {return TIMES;}
"while"    {return WHILE;}
"do"       {return DO;}
"repeat"   {return REPEAT;}
"until"    {return UNTIL;}
"display"  {return DISPLAY;}
"output"   {return DISPLAY;}
"input"    {return REQUEST;}
"request"  {return REQUEST;}
";"        {return CHOP;}
"&"        {return ADDRESS;}
"void"     {return VOID;}
"bool"     {return BOOL;}
"char"	   {return CHAR;}
"short"    {return SHORT;}
"int"      {return INT;}
"long"     {return LONG;}
"string"   {return STR;}
"float"    {return FLOAT;}
"double"   {return DOUBLE;}
"sizeof"   {return SIZEOF;}
"FILE"		{return FILEDEC;}
"extern"   {return EXTERN;}

"."        {return DOT;}
"="        {return EQ;}     //比较相等符号 “=”
"<=="      {return ASS_P;}   //赋值符号已修改为“<==”

":="       {return ASS_N;}

"<"        {return LT;}
">"        {return GT;}
"<="       {return LE;}
">="       {return GE;}
"!="       {return NE;}
"!"        {return NEGATION;}
"</"       {return PBEGIN;}
"/>"       {return PEND;}

"+"        {return ADD;}
"-"        {return SUB;}
"*"        {return MUL;}
"/"	       {return DIV;}
"%"        {return MOD;}
"^"        {return XOR;}
"<<"       {return LST;}//左移 left shift
">>"       {return RST;}//右移 right shift
"~"        {return BNE;}//按位非 bit negation
"=>"	   {return PROPIMPLY;}
"->"       {return IMPLY;}
"<->"      {return IFF;}

"("        {return OPEN_PAR;}
")"        {return CLOSE_PAR;}
"["        {return OPEN_MPAR;}
"]"        {return CLOSE_MPAR;}
"{"        {return OPEN_BPAR;}
"}"        {return CLOSE_BPAR;}
","        {return COMMA;}
"#"        {return CHOPSTAR;}


"|"			{return	INTER_OR;}


"NULL"      {return POINTERNULL;}
"[]"        {return ARRAY;}

"signed"    {return SIGNED;}
"unsigned"  {return UNSIGNED;}

"CRITICAL_SECTION" {return CRITICAL_SECTION;}
"TIMECAPS"  {return TIMECAPS;}


{IDENT}  {
			   yylval.cString=new char[strlen(yytext)+1];
			   strcpy(yylval.cString,yytext);
               
			   if(!g_StructNameMap.find(yytext))
			   {
			       return ID;
			   }
			   else
			   {
                   return STRUCT_TYPE;
			   }
		}
{DIGIT}  {    
               yylval.nInteger=atoi(yytext);
               return INTLITERAL;
		 }
{HEXDIGIT}  {    
               int decValue;//int，保存16进制对应的十进制数
			   sscanf(yytext,"%x",&decValue);//将16进制字符串转成10进制数
			   yylval.nInteger=decValue;//将10进制数赋给yytext
               return INTLITERAL;
			}
{OCTONARY} {  
               int length = strlen(yytext);
			   string str(yytext);
			   string s = str.substr(2, length-3);
               
			   int decValue=0;//int，保存8进制对应的十进制数
			   sscanf(s.data(),"%o",&decValue);//将8进制字符串转成10进制数
			   yylval.nInteger=decValue;//将10进制数赋给yytext
               return INTLITERAL;
           }
{EXPONENT} {
               char* result=NULL;
			   result=strtok(yytext, "e");
			   int iVal;
			   float fVal;
			   sscanf(result, "%f", &fVal);
			   while(result!=NULL)
			   {
			      sscanf(result, "%d", &iVal);
				  result = strtok(NULL, "e");
				}

               yylval.floatval = (float)(fVal*pow(10, iVal)); 
			   return FLOATLITERAL;
			}
{STR}    {
               passStringConstant ();
			   return STRLITERAL;
         }

{RFLOAT}	 {		
				yylval. floatval = (float)atof(yytext); 
				return FLOATLITERAL; 
		 }//Added by Jane 2012-05-21 
{RCHAR}		{//modified by yubin 2014/2/25，第一个if分支处理没有转义字符的情况，下面的分支处理有转义字符的情况
				if(yytext[1]!='\\')
				{
					yylval.character=yytext[1];
					return CHARLITERAL;
				}
				else if(yytext[2]=='n')
				{
					yylval.character='\n';
					return CHARLITERAL;
				}
				else if(yytext[2]=='b')
				{
					yylval.character='\b';
					return CHARLITERAL;
				}
				else if(yytext[2]=='f')
				{
					yylval.character='\f';
					return CHARLITERAL;
				}
				else if(yytext[2]=='r')
				{
					yylval.character='\r';
					return CHARLITERAL;
				}
				else if(yytext[2]=='t')
				{
					yylval.character='\t';
					return CHARLITERAL;
				}
				else if(yytext[2]=='\'')
				{
					yylval.character='\'';
					return CHARLITERAL;
				}
				else if(yytext[2]=='\"')
				{
					yylval.character='\"';
					return CHARLITERAL;
				}
				else if(yytext[2]=='\\')
				{
					yylval.character='\\';
					return CHARLITERAL;
				}
				else if(yytext[2]=='0')
				{
					yylval.character='\0';
					return CHARLITERAL;
				}
		}   //2013-4-22 add by YY[fixed]



"//"     {DeleteSingleLineComment();}
\n       {++lineno;}
{WSPACE} {}


<comment>.  {}
<comment>\n {++lineno;}
<comment>EOF {error("EOF in comment");}
<comment>"*/" {BEGIN 0;}

%%

void DeleteSingleLineComment(void){
    register int c;
	while((c=yyinput()) != '\n' && c!=EOF){}
	++lineno;
}

void passStringConstant (void)
{//modified by yubin 2014/2/25，处理char*字符串中的转义字符，第一个if分支处理没有转义字符的情况，下面的分支处理有转义字符的情况
	int l = ( strlen( yytext ) - 2 );
	yylval.cString = new char[l+1];
	strncpy( yylval.cString, &yytext[1], l );
	int i;
	int j=0;//j用来记录转义字符的个数
	for(i=0;i+j<l;i=i+1)
	{
		if(yylval.cString[i+j]!='\\')
		{
			yylval.cString[i]=yylval.cString[i+j];
		}
		else if(yylval.cString[i+j+1]=='n')
		{
			yylval.cString[i]='\n';
			j=j+1;
		}
		else if(yylval.cString[i+j+1]=='b')
		{
			yylval.cString[i]='\b';
			j=j+1;
		}
		else if(yylval.cString[i+j+1]=='f')
		{
			yylval.cString[i]='\f';
			j=j+1;
		}
		else if(yylval.cString[i+j+1]=='r')
		{
			yylval.cString[i]='\r';
			j=j+1;
		}
		else if(yylval.cString[i+j+1]=='t')
		{
			yylval.cString[i]='\t';
			j=j+1;
		}
		else if(yylval.cString[i+j+1]=='\'')
		{
			yylval.cString[i]='\'';
			j=j+1;
		}
		else if(yylval.cString[i+j+1]=='\"')
		{
			yylval.cString[i]='\"';
			j=j+1;
		}
		else if(yylval.cString[i+j+1]=='\\')
		{
			yylval.cString[i]='\\';
			j=j+1;
		}
		else if(yylval.cString[i+j+1]=='0')
		{
			yylval.cString[i]='\0';
			j=j+1;
		}
		else//是转义字符，但是不是上面的一些转义字符，暂时什么也不做
		{
		
		}	
	}
	yylval.cString[i] = '\0';
}