%{
#include "mctypes.h"
#include "function.h"
#include "SyntaxNode.h"
#include "SyntaxTree.h"

#include <malloc.h>
#include <string.h>
#include <set>
#include <vector>

using namespace std;

//******************  用来存储程序语法树以及性质树 *******************************/
CSyntaxNode *g_syntaxTree = NULL;    //程序语法树节点指针
CSyntaxNode *g_propositionTree=NULL; //性质语法树节点指针
CSyntaxNode *function_tree=NULL;
CSyntaxNode *propositionDefineTree=NULL;
CSyntaxNode *extern_tree=NULL;
CSyntaxNode *struct_tree=NULL;       //存储结构体的定义

CSyntaxTree *g_tree=NULL;
CSyntaxTree *g_proptree=NULL; 

CMAP g_StructNameMap;
map<string, string> structName; //存储定义的结构体的名字
set<string> functionName;       //存储函数的名字
vector<string> includefile;     //存储include文件的文件名
int g_nErrors=0;

extern void yyerror(const char *msg );
extern int yylex(void);

#define YYDEBUG 1	          // Generate debug code; needed for YYERROR_VERBOSE
#define YYERROR_VERBOSE       // Give a more specific parse error message
%}

%union{
   char            *cString;   /* A character string */
   int             nInteger;   /* An intliteral value */
   float           floatval;   /* A float value */
   char            character;  /* A character */
   CSyntaxNode     *tnode;     /* Node in the syntax tree */
   NODETYPE        nodetype;
   RETURNTYPE      returntype;
}
 
%token INCLUDE
%token IF ELSE EMPTY AWAIT PROJECTION  PBEGIN PEND POINTERNULL ARRAY STRUCT DOT UNION FILEDEC
%token DEFINE MY_TRUE MY_FALSE EXIST FRAME FOR WHILE DO 
%token TRUE FALSE
%token REPEAT UNTIL DISPLAY SKIP THEN  COLON 
%token VOID BOOL CHAR SHORT INT LONG STR FLOAT DOUBLE 
%token CRITICAL_SECTION TIMECAPS        
%token OPEN_PAR CLOSE_PAR TIMES OPEN_BPAR CLOSE_BPAR OPEN_MPAR CLOSE_MPAR FUNCTION
%token INTER_OR
%token UNSIGNED SIGNED
%token SIZEOF
%token EXTERN

%left COMMA 
%left CHOP
%left OR PARALLEL
%left AND 
%right ASS_P ASS_N 
%left BOOL_OR
%left BOOL_AND
%left XOR	//按位异或		
%left ADDRESS //按位与
%left NE EQ 
%left GE LE  GT LT 
%left LST RST  //左移右移
%left ADD SUB 
%left MUL DIV MOD 
%left BNE  //按位取反
%left IMPLY IFF PROPIMPLY

%nonassoc CHOPSTAR NEXT NEGATION LENGTH REQUEST FINAL KEEP ALWAYS HALT W_NEXT /*ADDRESS*/ SOMETIMES 

%token<cString> ID STRLITERAL STRUCT_TYPE
%token<nInteger> INTLITERAL
%token<floatval> FLOATLITERAL 
%token<character> CHARLITERAL
%type<tnode> ass_statement init_list init_exp option_init_exp
%type<tnode> type_cast
%type<tnode> statement  identifier strliteral intliteral
%type<tnode> if_statement  option_else_statement while_statement do_while_statement extend_for_statement for_sta_init for_sta_condition for_sta_control
%type<tnode> ari_exp add_exp mul_exp unary_exp  basic_exp primary_exp const_exp option_exp
%type<tnode> exp bool_exp logic_and_exp  bit_or_exp bit_xor_exp bit_and_exp equlity_exp relation_exp
%type<tnode> func_formal_par inner_func_formal_par
%type<tnode> func_actual_par option_formal_par
%type<tnode> declaration option_declaration inner_option_declaration option_struct_declaration
%type<tnode> bool_par output_statement input_statement  
%type<tnode> charliteral floatliteral 
%type<tnode> struct_define_statement struct_identifier
%type<tnode> pointer
%type<nodetype>	assign_operator
%type<returntype> basic_type  windows_type
%type<tnode> program gComplexProposition complexProposition poptional_projection 
%type<tnode> leftpart_4prj  optional_leftpart_4prj
%type<tnode>for_statement optional_projection
%%
program
       :gComplexProposition  statement 
	   {
			//验证需要的语句
			g_propositionTree=$1;
			g_proptree=new CSyntaxTree(g_propositionTree);    
							
			//执行的基本语句
			g_syntaxTree=$2;
			g_tree=new CSyntaxTree(g_syntaxTree);          //2013-5-3 add by YY[fixed]       
		}
	   ;
//下面是验证条件的语句的语法
gComplexProposition
	   :PBEGIN complexProposition PEND  {$$=$2;}
	   |/*empty*/						{$$=NULL;}
	   ;

complexProposition
	   :complexProposition AND complexProposition   {$$=new CSyntaxNode(AND_EXP, $1, $3, VOIDTYPE);}   
	   |complexProposition OR complexProposition    {$$=new CSyntaxNode(OR_EXP, $1, $3, VOIDTYPE);}    
	   |complexProposition IFF complexProposition   {$$=new CSyntaxNode(IFF_STA, $1, $3, BOOLTYPE);}    
	   |complexProposition PROPIMPLY complexProposition {$$=new CSyntaxNode(IMPLY_STA, $1, $3, VOIDTYPE);}    
	   |complexProposition CHOP complexProposition  
	   {
			if($1==NULL)                            $$=$3;
			else                                    $$=new CSyntaxNode(CHOP_STA, $1, $3, VOIDTYPE);       
	   }
	   |ALWAYS complexProposition					{$$=new CSyntaxNode(ALWAYS_STA, $2, VOIDTYPE);}    
	   |KEEP complexProposition					    {$$=new CSyntaxNode(KEEP_STA, $2, VOIDTYPE);}     
	   |SOMETIMES complexProposition				{$$=new CSyntaxNode(SOMETIMES_STA, $2, VOIDTYPE);}
	   |W_NEXT complexProposition					{$$=new CSyntaxNode(W_NEXT_STA, $2, VOIDTYPE);}    
	   |NEXT complexProposition			            {$$=new CSyntaxNode(NEXT_STA, $2, VOIDTYPE);}     
	   |SKIP										{$$=new CSyntaxNode(SKIP_STA, VOIDTYPE);}         
	   |MY_TRUE										{$$=new CSyntaxNode(TRUE_EXP, VOIDTYPE);}       
	   |MY_FALSE									{$$=new CSyntaxNode(FALSE_EXP, VOIDTYPE);}         
	   |LENGTH ari_exp								{$$=new CSyntaxNode(LENGTH_STA, $2, VOIDTYPE);}     
	   |EMPTY										{$$=new CSyntaxNode(EMPTY_EXP, VOIDTYPE);}         
	   |HALT complexProposition                     {$$=new CSyntaxNode(HALT_STA, $2, VOIDTYPE);}  
	   |FINAL complexProposition                    {$$=new CSyntaxNode(FINAL_STA, $2, VOIDTYPE);}   
	   |NEGATION complexProposition                 {$$=new CSyntaxNode(NEGATION_EXP, $2, VOIDTYPE);}  
	   |OPEN_BPAR complexProposition poptional_projection CLOSE_BPAR PROJECTION OPEN_BPAR complexProposition CLOSE_BPAR
	   {
			$$=new CSyntaxNode(PROJECTION_STA, $2, $3, $7, VOIDTYPE);                         
	   }
	   |ID 											{$$=new CSyntaxNode(IDENT_EXP, $1, NULL, VOIDTYPE)}     //{$$=new CSyntaxNode(IDENT_EXP, $1, 0);}  
	   |DEFINE ID COLON ari_exp             
	   {
			$$=NULL;
			propositionDefineTree=new CSyntaxNode(FUNCTION_DEFINE_STA, $2, $4, propositionDefineTree, NULL, VOIDTYPE);			
	   }
       |complexProposition  CHOPSTAR  {$$=new CSyntaxNode(CHOPSTAR_STA, $1, VOIDTYPE);}
	   |OPEN_PAR complexProposition CLOSE_PAR       {$$=$2;}
	   ;

poptional_projection
	   :COMMA complexProposition poptional_projection               
	   {
			$$ = new CSyntaxNode(PROJECTION_STA, $2, $3, VOIDTYPE);
	   }
	   |/*empty*/   {$$=NULL;}
	   ;

//下面是可执行基本语句的语法
//SetiValue 加入语句两边有没有if的判断,便于加标签的操作
statement:
	   statement  AND statement       
	   {
	       $$=new CSyntaxNode(AND_EXP, $1, $3, VOIDTYPE);  
		   if($1->GetiValue() || $3->GetiValue()) {$$->SetiValue(1);}
	   }
	   |statement  OR  statement        {$$=new CSyntaxNode(OR_EXP, $1, $3, VOIDTYPE);  $$->SetiValue(1);}
	   |statement ARRAY statement       {$$ = new CSyntaxNode(OR_EXP, $1, $3, VOIDTYPE); $$->SetiValue(1);}
	   |statement PARALLEL  statement  
	   {
	      $$=new CSyntaxNode(PARALLEL_STA, $1, $3, VOIDTYPE);  
		  if($1->GetiValue() || $3->GetiValue()) {$$->SetiValue(1);}
	   }
	   |statement CHOP statement        
	   {
			if($1==NULL)
			{
			  $$=$3;
			}
			else
			{
			  			// 两个都是struct定义语句
			if(NULL!=$1 && $1->GetNType()==STRUCT_DEFINE_STA && NULL!=$3 && $3->GetNType()==STRUCT_DEFINE_STA)
			{
			    if($1->FindSameName($3->GetNName()) )
					{  yyerror("Structs have the same name");
					   $$=$1;
					}
				else 
				    {
					  $3->SetChild1($1);
					  $$=$3;
					}
			}
			// 第一个是struct定义语句，第二个是执行语句
			if(NULL!=$1 && $1->GetNType()==STRUCT_DEFINE_STA && NULL!=$3 && $3->GetNType()!=STRUCT_DEFINE_STA )
			{
				struct_tree=$1;
				$$=$3;
			}
			// 第一个是执行语句，第二个是struct定义语句
			if( NULL!=$1 && $1->GetNType()!=STRUCT_DEFINE_STA  && $1->GetNType()!=FUNCTION_DEFINE_STA &&  
			    NULL!=$3 && $3->GetNType()==STRUCT_DEFINE_STA)
			{
				        if(NULL==struct_tree) 
						{
							struct_tree=$3;
						}
					    else 
						{
							if(struct_tree->FindSameName($3->GetNName()))
							{  
									yyerror("Structs have the same name"); 
								    $$=$1;  
							}
							else
							{
								$3->SetChild1(struct_tree);
								struct_tree=$3;
							}
						}					
			}
			
			//两个都是函数声明语句
			if(NULL!=$1 && $1->GetNType()==FUNCTION_DEFINE_STA && NULL!=$3 && $3->GetNType()==FUNCTION_DEFINE_STA )
			{
				if($1->FindSameName($3->GetNName()) )
					{  yyerror("Functions or predicates have the same name");
					   $$=$1;
					}
				else 
				    {
					  $3->SetChild2($1);
					  $$=$3;
					}
			}
			//第一个是函数声明语句，第二个是执行语句
			if(NULL!=$1 && $1->GetNType()==FUNCTION_DEFINE_STA && NULL!=$3 && $3->GetNType()!=FUNCTION_DEFINE_STA)
			{
				function_tree=$1;
                $$=$3;
			}

			//两个都是执行语句
			if( NULL!=$1 && $1->GetNType()!=FUNCTION_DEFINE_STA &&  $1->GetNType()!=STRUCT_DEFINE_STA && 
			    NULL!=$3 && $3->GetNType()!=FUNCTION_DEFINE_STA &&  $3->GetNType()!=STRUCT_DEFINE_STA)
			{
				$$=new CSyntaxNode(CHOP_STA, $1, $3, VOIDTYPE);
				$$->SetiValue($1->GetiValue() || $3->GetiValue());
			}

			//第一个是执行语句，第二个是声明语句  
			//problem: 整个程序的第一句必须是声明语句才可以    
			if( NULL!=$1 && $1->GetNType()!=FUNCTION_DEFINE_STA   &&  $1->GetNType()!=STRUCT_DEFINE_STA &&
			    NULL!=$3 && $3->GetNType()==FUNCTION_DEFINE_STA)
				{
				        if(NULL==function_tree) 
						{
							function_tree=$3;
						}
					    else 
						{
							if(function_tree->FindSameName($3->GetNName()))
							{  
									yyerror("Functions or predicates have the same name"); 
								    $$=$1;  
							}
							else
							{
								CSyntaxNode* p = function_tree;
								while(p->GetChild2()!=NULL)
								p=p->GetChild2();
								p->SetChild2($3);
								$$=$1;
							}
						}
					
				}
			if($1!=NULL && $3!=NULL)
			{
			   if($1->GetiValue()!=2 && $3->GetiValue()!=2)
			   {
				   $$->SetiValue($1->GetiValue() || $3->GetiValue());
			   }
			   else
			   {
				   $$->SetiValue(1);
			   }			
			}
			}			
	   }
	   |INCLUDE STRLITERAL {includefile.push_back($2);$$=NULL}
	   |EXTERN  declaration
	   {  
	      CSyntaxNode* tempTree = new CSyntaxNode(EXTERN_VAR_DEC, $2, VOIDTYPE);
	      if(extern_tree!=NULL)
	      {
		     tempTree->SetChild1(extern_tree);
			 extern_tree=tempTree;
		  }
		  else
		  {
		     extern_tree=tempTree;
		  }
		  $$=NULL;
	   }
	   |EXTERN  FUNCTION ID OPEN_PAR func_formal_par CLOSE_PAR
	   {
			CSyntaxNode* tempTree = new CSyntaxNode(EXTERN_FUNC_DEC, $3, $5, VOIDTYPE);
			if(extern_tree!=NULL)
	        {
		       tempTree->SetChild1(extern_tree);
			   extern_tree=tempTree;
		    }
		    else
		    {
		       extern_tree=tempTree;
		    }
			$$=NULL;
	   }
	   |exp                             {$$=$1;}
	   |declaration                     {$$=$1;}
	   |ass_statement                   {$$=$1;}
	   |SKIP                            {$$=new CSyntaxNode(SKIP_STA, VOIDTYPE);}
       |EMPTY                           {$$=new CSyntaxNode(EMPTY_EXP, VOIDTYPE);}
	   |TRUE                            {$$=new CSyntaxNode(TRUE_EXP, VOIDTYPE);}         
	   |FALSE                           {$$=new CSyntaxNode(FALSE_EXP, VOIDTYPE);}  
	  // |RETURN exp                      {$$=new CSyntaxNode(RETURN_STA, $2, VOIDTYPE);}
	   |struct_define_statement         {$$=$1;}       
	   |input_statement                 {$$=$1;}
	   |output_statement                {$$=$1;}   
	   //|switch_statement                {$$=$1;  $$->SetiValue(1);}
	   |if_statement                    {$$=$1;  $$->SetiValue(1);}
	   |while_statement                 {$$=$1;  $$->SetiValue(1);}
	   |do_while_statement              {$$=$1;  $$->SetiValue(1);}
	   |for_statement                   {$$=$1;  $$->SetiValue(1);}
	   |extend_for_statement            {$$=$1;  $$->SetiValue(1);}//类似于C语言中的for语句
	   |NEXT OPEN_PAR statement CLOSE_PAR    {$$=new CSyntaxNode(NEXT_STA, $3, VOIDTYPE);}
	   |KEEP OPEN_PAR statement CLOSE_PAR       {$$=new CSyntaxNode(KEEP_STA, $3, VOIDTYPE);  $$->SetiValue(1);}
	   |ALWAYS OPEN_PAR statement CLOSE_PAR      {$$=new CSyntaxNode(ALWAYS_STA, $3, VOIDTYPE);  $$->SetiValue(1);}
	   |AWAIT OPEN_PAR bool_exp CLOSE_PAR  {$$=new CSyntaxNode(AWAIT_STA, $3, VOIDTYPE);  $$->SetiValue(1);}
	   |REPEAT OPEN_BPAR statement CLOSE_BPAR UNTIL OPEN_PAR bool_exp CLOSE_PAR  
	   {
	      $$=new CSyntaxNode(REPEAT_UNTIL_STA, $3, $7, VOIDTYPE);  $$->SetiValue(1);
	   }
	   |LENGTH OPEN_PAR ari_exp CLOSE_PAR   {$$=new CSyntaxNode(LENGTH_STA, $3, VOIDTYPE);  $$->SetiValue(1);}
	   |FINAL OPEN_PAR statement CLOSE_PAR   {$$=new CSyntaxNode(FINAL_STA, $3, VOIDTYPE);}	   	      
	   |FRAME OPEN_PAR exp option_exp CLOSE_PAR AND OPEN_PAR statement CLOSE_PAR
	   {
			CSyntaxNode* child0=new CSyntaxNode(OPTION_EXP, $3, $4, VOIDTYPE);
			$$=new CSyntaxNode(FRAME_STA, child0, $8, VOIDTYPE);
	   }
	   |OPEN_BPAR leftpart_4prj optional_projection CLOSE_BPAR PROJECTION OPEN_BPAR statement CLOSE_BPAR  
	   {
			$$=new CSyntaxNode(PROJECTION_STA, $2, $3, $7,VOIDTYPE);
			$$->SetiValue(1);
	   }
	   /*|OPEN_BPAR statement CLOSE_BPAR PROJECTION OPEN_BPAR statement CLOSE_BPAR
	   {
			$$=new CSyntaxNode(PROJECTION_STA, $2, NULL, $6, VOIDTYPE);
			$$->SetiValue(1);
	   }	   
	   |OPEN_BPAR leftpart_4prj CLOSE_BPAR PROJECTION OPEN_BPAR statement CLOSE_BPAR 
	   {
			$$=new CSyntaxNode(PROJECTION_STA, $2, NULL, $6, VOIDTYPE);
			$$->SetiValue(1);
	   }*/   	   
	   |EXIST identifier inner_func_formal_par COLON OPEN_BPAR statement CLOSE_BPAR
	   {
			CSyntaxNode * p;
			p=$3;
			while(p!=NULL)
			{
				p=p->GetChild0();
			}
			$$=new CSyntaxNode(EXIST_STA, $2, $3, $6,VOIDTYPE);
	   }
	   |FUNCTION ID OPEN_PAR func_formal_par CLOSE_PAR  OPEN_BPAR statement CLOSE_BPAR
	   {
			//$$=new CSyntaxNode(FUNCTION_DEFINE_STA, $2, $4, $7, NULL, VOIDTYPE);

			CSyntaxNode* tempTree = new CSyntaxNode(FUNCTION_DEFINE_STA, $2, $4, $7, NULL, VOIDTYPE);
			if(function_tree!=NULL)
	        {
		       tempTree->SetChild2(function_tree);
			   function_tree=tempTree;
		    }
		    else
		    {
		       function_tree=tempTree;
		    }
			$$=NULL;

			functionName.insert($2);
	   }
	   |OPEN_PAR statement CLOSE_PAR {$$=$2;}
	   ;

leftpart_4prj
		:statement				 {$$=$1;}
		|OPEN_PAR statement CLOSE_PAR XOR ari_exp
		{ 
			$$=new CSyntaxNode(PROJECTION_PLUS_STA,$2,NULL,$5,VOIDTYPE);
		}
		|OPEN_PAR leftpart_4prj  optional_leftpart_4prj CLOSE_PAR XOR ari_exp
		{
			$$=new CSyntaxNode(PROJECTION_PLUS_STA,$2,$3,$6,VOIDTYPE);
		}
		;
optional_leftpart_4prj
		:												{$$=NULL;}
		|COMMA  leftpart_4prj optional_leftpart_4prj    {$$=new CSyntaxNode(PROJECTION_STA,$2,$3,VOIDTYPE);}
		;

optional_projection
	   :                                                   {$$=NULL;}
	   |COMMA leftpart_4prj optional_projection            {$$ = new CSyntaxNode( PROJECTION_STA, $2, $3, VOIDTYPE);}								
	   |COMMA error                                        {$$=NULL;}
	   ;

struct_define_statement
       :struct_identifier OPEN_BPAR option_struct_declaration CLOSE_BPAR 
	   {
		   string structName=$1->GetNName();
           $$=new CSyntaxNode(STRUCT_DEFINE_STA, structName, $3, VOIDTYPE);
	   }
       ;
option_struct_declaration:
        declaration {$$=$1;}
       |declaration AND option_struct_declaration {$$=new CSyntaxNode(AND_EXP,$1,$3,VOIDTYPE);}
	   ;
struct_identifier
       :STRUCT ID 
	   {
			if(!g_StructNameMap.insert($2))
			{
			    yyerror("Redefinition of struct");
			}
			else
			{
			    $$=new CSyntaxNode(STRUCT_NAME, $2, STRUCT_NAME_TYPE);
			}
	   }
       |UNION  ID 
	   {
	       if(!g_StructNameMap.insert($2))
			{
			    yyerror("Redefinition of union");
			}
			else
			{
			    $$=new CSyntaxNode(STRUCT_NAME, $2, STRUCT_NAME_TYPE);
			}
	   }
	   ;

declaration
	   :basic_type  option_declaration { $$=new CSyntaxNode(DECLARE_STA, $2, $1);}   
	   |STRUCT_TYPE option_declaration { $$=new CSyntaxNode(DECLARE_STA, $1, $2, STRUCTTYPE);} 
	   |STRUCT ID option_declaration { $$=new CSyntaxNode(DECLARE_STA, $2, $3, STRUCTTYPE);}
       |UNION ID option_declaration { $$=new CSyntaxNode(DECLARE_STA, $2, $3, STRUCTTYPE); }
	   |STRUCT STRUCT_TYPE option_declaration { $$=new CSyntaxNode(DECLARE_STA, $2, $3, STRUCTTYPE);}
       |UNION STRUCT_TYPE option_declaration { $$=new CSyntaxNode(DECLARE_STA, $2, $3, STRUCTTYPE);}  
	   |VOID OPEN_PAR unary_exp CLOSE_PAR OPEN_PAR func_formal_par CLOSE_PAR
	   {
		  CSyntaxNode* child0 = new CSyntaxNode(DECLARE_STA, $3, $6, VOIDTYPE);
		  $$=new CSyntaxNode(DECLARE_STA, child0, FUNCPTYPE);
	   }
	   ;            
option_declaration
		:identifier inner_option_declaration
		{
		   $$ = new CSyntaxNode(DEC_VAR_LIST, $1, $2, VOIDTYPE);
		}
	    |identifier OPEN_MPAR exp option_exp CLOSE_MPAR inner_option_declaration
		{
		   CSyntaxNode* child01=new CSyntaxNode(OPTION_EXP, $3, $4, VOIDTYPE );
		   CSyntaxNode* child0 = new CSyntaxNode(ARRAY_DECLARE_STA, $1, child01, LISTTYPE);
	       $$=new CSyntaxNode(DEC_VAR_LIST, child0, $6, VOIDTYPE);
		}
	    |identifier ASS_P bool_exp inner_option_declaration
		{
		   CSyntaxNode* child0 = new CSyntaxNode(ASS_EQU_EXP, $1, $3, VOIDTYPE);
		   $$=new CSyntaxNode(DEC_VAR_LIST, child0, $4, VOIDTYPE);
		}
		//S a<=={1,2,3}
		|identifier ASS_P  init_list  inner_option_declaration
		{
		   CSyntaxNode* child0 = new CSyntaxNode(ASS_EQU_EXP, $1, $3, VOIDTYPE);
		   $$=new CSyntaxNode(DEC_VAR_LIST, child0, $4, VOIDTYPE);
		}	
		// int a[]<=={1,2,3}
		|identifier ARRAY ASS_P  init_list  inner_option_declaration
		{
		   CSyntaxNode* child00 = new CSyntaxNode(ARRAY_DECLARE_STA, $1, NULL, NULL, LISTTYPE);
		   CSyntaxNode* child0 = new CSyntaxNode(ASS_EQU_EXP, child00, $4, VOIDTYPE);
		   $$=new CSyntaxNode(DEC_VAR_LIST, child0, $5, VOIDTYPE);
		}	
		// char a[]<=="hallo"
		|identifier ARRAY ASS_P strliteral inner_option_declaration
		{
		   CSyntaxNode* child00 = new CSyntaxNode(ARRAY_DECLARE_STA, $1, NULL, NULL, LISTTYPE);
		   CSyntaxNode* child0 = new CSyntaxNode(ASS_EQU_EXP, child00, $4, VOIDTYPE);
		   $$=new CSyntaxNode(DEC_VAR_LIST, child0, $5, VOIDTYPE);
		}
		// int a[3]<=={1,2,3}   char a[2,6]<=={"he","she"}
	    |identifier OPEN_MPAR exp option_exp CLOSE_MPAR ASS_P init_list  inner_option_declaration
		{
	       CSyntaxNode *child001 = new CSyntaxNode(OPTION_EXP, $3, $4, VOIDTYPE);
		   CSyntaxNode *child00 = new CSyntaxNode(ARRAY_DECLARE_STA, $1, child001, LISTTYPE);
		   CSyntaxNode *child0 = new CSyntaxNode(ASS_EQU_EXP, child00, $7, VOIDTYPE);
		   $$=new CSyntaxNode(DEC_VAR_LIST, child0, $8, VOIDTYPE);
	    } 
	    //char a[3]<=="ni";
	    |identifier OPEN_MPAR exp option_exp CLOSE_MPAR ASS_P strliteral inner_option_declaration
	    {
	       CSyntaxNode* child001 = new CSyntaxNode(OPTION_EXP, $3, $4, VOIDTYPE);
		   CSyntaxNode *child00 = new CSyntaxNode(ARRAY_DECLARE_STA, $1, child001, LISTTYPE);
		   CSyntaxNode *child0 = new CSyntaxNode(ASS_EQU_EXP, child00, $7, VOIDTYPE);
		   $$=new CSyntaxNode(DEC_VAR_LIST, child0, $8, VOIDTYPE);
	    }
	   |MUL option_declaration
	   {
	      CSyntaxNode* pChild0=new CSyntaxNode(POINTER_EXP, $2->GetChild0(), VOIDTYPE);
          $$=$2;
		  $2->SetChild0(pChild0);
		  //$$=new CSyntaxNode(DEC_VAR_LIST, pChild0, VOIDTYPE);//可能有问题
	   }
	   |OPEN_PAR MUL identifier CLOSE_PAR OPEN_MPAR exp option_exp CLOSE_MPAR inner_option_declaration
	   {
	      CSyntaxNode* child01 = new CSyntaxNode(OPTION_EXP, $6, $7, VOIDTYPE);
		  CSyntaxNode* child00 = new CSyntaxNode(POINTER_EXP, $3, VOIDTYPE);
		  CSyntaxNode* child0 = new CSyntaxNode(ARRAY_DECLARE_STA, child00, child01, LISTTYPE);
		  $$ = new CSyntaxNode(DEC_VAR_LIST, child0, $9, VOIDTYPE);
	   }
	    ;
inner_option_declaration
	   :COMMA option_declaration {$$=$2;}
	   |COMMA error   {$$=NULL;}
	   |/*empty*/  {$$=NULL;}
	   ;
init_list:     
	   OPEN_BPAR init_exp CLOSE_BPAR {$$=new CSyntaxNode(ELEMENT_LIST, $2, NULL, VOIDTYPE);}
	  ;
init_exp:
        bool_exp option_init_exp{$$=new CSyntaxNode(ELEMENT, $1, $2, VOIDTYPE);}
	   |init_list option_init_exp {$$=new CSyntaxNode(ELEMENT, $1, $2, VOIDTYPE);}
	   ;
option_init_exp:
       COMMA init_exp {$$=$2;}
	   |{$$=NULL;}
	   ;
ass_statement:
        unary_exp assign_operator exp      {$$=new CSyntaxNode($2, $1, $3, VOIDTYPE);}
	   ;
assign_operator:
        ASS_P      {$$=ASS_EQU_EXP;} 
	   |ASS_N      {$$=UNITASSIGN_EXP;}
	   ; 
exp:
    bool_exp {$$=$1;}
	;
bool_exp:
       logic_and_exp {$$=$1;}
	  |bool_exp BOOL_OR logic_and_exp {$$ = new CSyntaxNode(OR_EXP, $1, $3, BOOLTYPE);}
	  ;
logic_and_exp:
       bit_or_exp {$$= $1;}
	  |logic_and_exp BOOL_AND bit_or_exp {$$= new CSyntaxNode(AND_EXP, $1, $3, BOOLTYPE);}
	  ; 
bit_or_exp:
      bit_xor_exp {$$=$1;}
	 |bit_or_exp INTER_OR bit_xor_exp {$$=new CSyntaxNode(BOR_EXP, $1, $3, ARITHMETICTYPE);}
	 ;
bit_xor_exp:
      bit_and_exp {$$=$1;}
	 |bit_xor_exp XOR bit_and_exp {$$=new CSyntaxNode(XOR_EXP, $1, $3, ARITHMETICTYPE);}
	 ;
bit_and_exp:
        equlity_exp {$$=$1;}
	   |bit_and_exp ADDRESS equlity_exp  {$$=new CSyntaxNode(BAN_EXP, $1, $3, ARITHMETICTYPE);}
	   ;
equlity_exp:
        relation_exp {$$=$1;}
	   |equlity_exp EQ relation_exp {$$=new CSyntaxNode(EQU_EXP, $1, $3, BOOLTYPE);}
	   |equlity_exp NE relation_exp {$$=new CSyntaxNode(NE_EXP, $1, $3, BOOLTYPE);}
	   ;
relation_exp:
        ari_exp {$$=$1;}
       |relation_exp GT ari_exp     { $$=new CSyntaxNode(GT_EXP, $1, $3, BOOLTYPE);}
	   |relation_exp GE ari_exp     { $$=new CSyntaxNode(GE_EXP, $1, $3, BOOLTYPE);}
	   |relation_exp LT ari_exp     { $$=new CSyntaxNode(LT_EXP, $1, $3, BOOLTYPE);}
	   |relation_exp LE ari_exp     { $$=new CSyntaxNode(LE_EXP, $1, $3, BOOLTYPE);}
	   ;
ari_exp :
        add_exp {$$=$1;}
	    |ari_exp LST add_exp  {$$=new CSyntaxNode(LST_EXP, $1, $3, ARITHMETICTYPE);}
	    |ari_exp RST add_exp  {$$=new CSyntaxNode(RST_EXP, $1, $3, ARITHMETICTYPE);}
	   ;
add_exp:
        mul_exp {$$=$1;}
       |add_exp ADD mul_exp  {$$=new CSyntaxNode(ADD_EXP, $1, $3, ARITHMETICTYPE);}
	   |add_exp SUB mul_exp  {$$=new CSyntaxNode(SUB_EXP, $1, $3, ARITHMETICTYPE);}
	   ;
mul_exp:
       unary_exp {$$=$1;}
	  |mul_exp MUL unary_exp {$$=new CSyntaxNode(MUL_EXP, $1, $3, ARITHMETICTYPE);}
	  |mul_exp DIV unary_exp {$$=new CSyntaxNode(DIV_EXP, $1, $3, ARITHMETICTYPE);}
	  |mul_exp MOD unary_exp {$$=new CSyntaxNode(MOD_EXP, $1, $3, ARITHMETICTYPE);}
	  ;

unary_exp:
      basic_exp {$$=$1;}
	  |ADDRESS unary_exp {$$=new CSyntaxNode(ADDRESS_EXP, $2, VOIDTYPE);}
	  |MUL unary_exp {$$=new CSyntaxNode(POINTER_EXP, $2, VOIDTYPE);}
	  //|ADD unary_exp {$$=new CSyntaxNode(UNARY_ADD_EXP, $2, VOIDTYPE);}
	  |SUB unary_exp {$$=new CSyntaxNode(UNARY_SUB_EXP, $2, VOIDTYPE);}
	  |NEGATION unary_exp {$$=new CSyntaxNode(NEGATION_EXP, $2, VOIDTYPE);}
	  |BNE unary_exp {$$=new CSyntaxNode(BNE_EXP, $2, VOIDTYPE);}
	  |SIZEOF OPEN_PAR basic_type CLOSE_PAR {$$=new CSyntaxNode(SIZEOF_EXP,  $3);}
	  |SIZEOF OPEN_PAR basic_type pointer CLOSE_PAR {$$=new CSyntaxNode(SIZEOF_EXP, $4, $3);}
	  |SIZEOF OPEN_PAR STRUCT_TYPE CLOSE_PAR {$$=new CSyntaxNode(SIZEOF_EXP, $3, NULL, STRUCTTYPE);}
	  |SIZEOF OPEN_PAR STRUCT_TYPE pointer CLOSE_PAR {$$=new CSyntaxNode(SIZEOF_EXP, $3, $4, STRUCTTYPE);}
	  |SIZEOF OPEN_PAR bool_exp CLOSE_PAR {$$=new CSyntaxNode(SIZEOF_EXP, $3, VOIDTYPE);}
	  |type_cast unary_exp  {$$=new CSyntaxNode(TYPE_CAST_STA, $1, $2, VOIDTYPE);}
	  |NEXT unary_exp {$$=new CSyntaxNode(NEXT_STA, $2, VOIDTYPE);}
	  ;
basic_exp:
       primary_exp  {$$=$1;}
      |basic_exp DOT identifier {$$=new CSyntaxNode(DOT_EXP, $1, $3, VOIDTYPE);}
	  |basic_exp IMPLY identifier {$$= new CSyntaxNode(ARROW_EXP, $1, $3, VOIDTYPE);}
	  |basic_exp OPEN_PAR func_actual_par CLOSE_PAR 
	  {
	     $$=new CSyntaxNode(FUNCTION_STA, $1, $3, VOIDTYPE);
		 if(functionName.find($1->GetNameNode()->GetNName())!=functionName.end())
		 {
		    //内部函数调用
		 	$$->SetiValue(1);
		 }//递归函数有并行时会有问题(没有设置为1)
		// $$->SetiValue(1);
	  }
	  |EXTERN basic_exp  {$$= new CSyntaxNode(EXT_FUNCTION_STA, $2, VOIDTYPE);}
	  |basic_exp OPEN_MPAR exp option_exp CLOSE_MPAR 
	  {
	     CSyntaxNode* child1 = new CSyntaxNode(OPTION_EXP, $3, $4, VOIDTYPE);
		 $$=new CSyntaxNode(ARRAY_EXP, $1, child1, VOIDTYPE);
	  }
	  ;

primary_exp:
         const_exp  {$$=$1;}
		|identifier {$$=$1;}
		|OPEN_PAR exp CLOSE_PAR {$$=$2;}
		|IF exp THEN exp ELSE exp {$$=new CSyntaxNode(IF_ELSE_EXP, $2, $4, $6, VOIDTYPE);}
		;
const_exp:
        intliteral                  {$$=$1;}
	   |charliteral					{$$=$1;}
	   |floatliteral			    {$$=$1;} 
	   |strliteral                  {$$=$1;}
	   |MY_TRUE                     {$$=new CSyntaxNode(TRUE_EXP, BOOLTYPE);}
	   |MY_FALSE                    {$$=new CSyntaxNode(FALSE_EXP, BOOLTYPE);}
	   |POINTERNULL                 {$$=new CSyntaxNode(NULL_EXP, VOIDTYPE)}
	   ;  
identifier
       :ID                              {$$=new CSyntaxNode(IDENT_EXP, $1, NULL, VOIDTYPE);}	   
	   |OPEN_PAR identifier CLOSE_PAR   {$$=$2;}
       ;	     
type_cast:
          OPEN_PAR basic_type  CLOSE_PAR 
		  {
		    $$=new CSyntaxNode(CAST_TYPE, $2);
		  }
		  |OPEN_PAR basic_type pointer CLOSE_PAR 
		  {
		    $$=new CSyntaxNode(CAST_TYPE, $3, $2);
		  }
		  |OPEN_PAR STRUCT_TYPE  CLOSE_PAR  
		  {
		    $$=new CSyntaxNode(CAST_TYPE, $2, NULL,  STRUCTTYPE);
		  }
		  |OPEN_PAR STRUCT_TYPE pointer CLOSE_PAR  
		  {
		    $$=new CSyntaxNode(CAST_TYPE, $2, $3, STRUCTTYPE);
		  }
		  |OPEN_PAR VOID OPEN_PAR pointer CLOSE_PAR OPEN_PAR func_formal_par CLOSE_PAR CLOSE_PAR//强制类型转换为函数指针
		  {
		    $$=new CSyntaxNode(CAST_TYPE, $4, $7, FUNCPTYPE);
		  }
		  ;
pointer:
     MUL {$$=new CSyntaxNode(POINTER_EXP, NULL, VOIDTYPE);};
	|MUL pointer {$$=new CSyntaxNode(POINTER_EXP, $2, VOIDTYPE);}
	;  
	
option_exp:
    COMMA exp option_exp {$$=new CSyntaxNode(OPTION_EXP, $2, $3, VOIDTYPE);};
   |/*empty*/ {$$=NULL;}        
   ;
basic_type:
        VOID                {$$=VOIDTYPE;}
       |BOOL                {$$=BOOLTYPE;}
       |CHAR				{$$=CHARTYPE;}
	   |SIGNED CHAR		    {$$=CHARTYPE;}
	   |UNSIGNED CHAR		{$$=UCHARTYPE;}
       |SHORT               {$$=SHORTTYPE}
	   |SIGNED SHORT        {$$=SHORTTYPE;}
	   |UNSIGNED SHORT      {$$=USHORTTYPE;}
       |INT				    {$$=INTTYPE;}
	   |SIGNED INT		    {$$=INTTYPE;}
	   |UNSIGNED INT		{$$=UINTTYPE;} 
	   |LONG                {$$=LONGTYPE}
	   |SIGNED LONG         {$$=LONGTYPE;}
	   |UNSIGNED LONG       {$$=ULONGTYPE;}
	   |LONG LONG           {$$=LLONGTYPE;}
	   |SIGNED LONG LONG    {$$=LLONGTYPE;}
	   |UNSIGNED LONG LONG  {$$=ULLONGTYPE;}
	   |FLOAT				{$$=FLOATTYPE;}
	   |DOUBLE              {$$=DOUBLETYPE;}
	   |STR				    {$$=STRTYPE;}
	   |FILEDEC    	        {$$=FILETYPE;} 
	   |windows_type        {$$=$1;}
	   ;

windows_type:
		CRITICAL_SECTION               {$$=CRITICAL_SECTION_TYPE;}
		|TIMECAPS                       {$$=TIMECAPS_TYPE;}
		;

//函数形参
func_formal_par:
        basic_type  option_formal_par inner_func_formal_par { $$ = new CSyntaxNode(FORMAL_PARAMETER_EXP, $2, $3, $1);}
	   |STRUCT_TYPE option_formal_par inner_func_formal_par { $$ = new CSyntaxNode(FORMAL_PARAMETER_EXP, $1, $2, $3, STRUCTTYPE);}
	   |STRUCT ID option_formal_par inner_func_formal_par { $$ = new CSyntaxNode(FORMAL_PARAMETER_EXP, $2, $3, $4, STRUCTTYPE);}
	   |STRUCT STRUCT_TYPE option_formal_par inner_func_formal_par { $$ = new CSyntaxNode(FORMAL_PARAMETER_EXP, $2, $3, $4, STRUCTTYPE);}
	   |VOID OPEN_PAR unary_exp CLOSE_PAR OPEN_PAR func_formal_par CLOSE_PAR inner_func_formal_par //函数指针
	   {
	      CSyntaxNode* pChild0= new CSyntaxNode(FUNCP_PARAMETER_EXP, $3, $6, VOIDTYPE);
          $$=new CSyntaxNode(FORMAL_PARAMETER_EXP, pChild0, $8, FUNCPTYPE);
		}
	   |VOID OPEN_PAR unary_exp ARRAY CLOSE_PAR OPEN_PAR func_formal_par CLOSE_PAR inner_func_formal_par //函数指针
	   {
		  //数组直接识别为指针
		  CSyntaxNode* pChild00 = new CSyntaxNode(POINTER_EXP, $3, VOIDTYPE);
		  CSyntaxNode* pChild0 = new CSyntaxNode(FUNCP_PARAMETER_EXP, pChild00, $7, VOIDTYPE);
          $$=new CSyntaxNode(FORMAL_PARAMETER_EXP, pChild0, $9, FUNCPTYPE);
		}
	   |/*empty*/              {$$ = NULL;}
	   ;
option_formal_par
		:identifier { $$ = $1;}
		|basic_exp OPEN_MPAR exp option_exp CLOSE_MPAR  
		{
		   CSyntaxNode* child1 = new CSyntaxNode(OPTION_EXP, $3, $4, VOIDTYPE);
		   $$ = new CSyntaxNode(ARRAY_DECLARE_STA, $1, child1, VOIDTYPE);
		}
		|basic_exp ARRAY {$$ = new CSyntaxNode(POINTER_EXP, $1, VOIDTYPE);}
	    |MUL option_formal_par { $$ = new CSyntaxNode(POINTER_EXP, $2, VOIDTYPE);}
		|{$$=new CSyntaxNode(IDENT_EXP, "par", NULL, VOIDTYPE);}
	    ;
inner_func_formal_par
      : COMMA func_formal_par {$$=$2}
	  | {$$=NULL}
	  ;
//函数实参
func_actual_par
       :exp option_exp {$$ = new CSyntaxNode(OPTION_EXP, $1, $2, VOIDTYPE);}
	   | /* empty */                      {$$=NULL;}
	   ;

bool_par
       :OPEN_PAR exp CLOSE_PAR				   {$$=$2;}
	   |error CLOSE_PAR									   {$$=NULL;}
	   ;

strliteral: STRLITERAL                      {$$=new CSyntaxNode(STR_EXP, $1, STRTYPE);};
intliteral: INTLITERAL                      {$$=new CSyntaxNode(INTEGER_EXP, $1, INTTYPE);};
charliteral :CHARLITERAL					{$$=new CSyntaxNode(CHARLITERAL_EXP, $1, CHARTYPE);};
floatliteral :FLOATLITERAL					{$$=new CSyntaxNode(FLOATLITERAL_EXP, $1, FLOATTYPE);};

			 
while_statement
       :WHILE bool_par OPEN_BPAR statement CLOSE_BPAR	{$$=new CSyntaxNode(WHILE_DO_STA, $2, $4, VOIDTYPE);}
	   ;
do_while_statement
       :DO OPEN_BPAR statement CLOSE_BPAR WHILE bool_par {$$=new CSyntaxNode(DO_WHILE_STA, $3, $6, VOIDTYPE);}
	   ;

for_statement
       :FOR ari_exp TIMES DO OPEN_BPAR statement CLOSE_BPAR  {$$=new CSyntaxNode(FOR_TIMES_STA, $2, $6, VOIDTYPE);}
	   ;

extend_for_statement
       :FOR OPEN_PAR for_sta_init COMMA for_sta_condition COMMA for_sta_control  CLOSE_PAR  OPEN_BPAR statement CLOSE_BPAR
	   {
	     CSyntaxNode* pchild0=new CSyntaxNode(FOR_STA_CON, $3, $5, $7, VOIDTYPE); 
		 $$=new CSyntaxNode(FOR_STA, pchild0, $10, VOIDTYPE);
		 pchild0=NULL;
	   }
       ;
for_sta_init//for语句括号中的初始化语句,可以是赋值语句，也可以是声明语句,也可以不写
       :ass_statement  {$$=$1;}
	   |declaration { $$=$1;}
	   |/*empty*/{$$=NULL;}
       ;
for_sta_condition//for语句括号中的条件语句，是一个布尔表达式也可以不写
      :bool_exp {$$=$1;}
	  |/*empty*/{$$=NULL;}
	  ;
for_sta_control//for语句括号中的控制语句，是一个赋值语句也可以不写
      :ass_statement{ $$=$1;}
	  |/*empty*/{$$=NULL;}
	  ;

if_statement
        :IF exp THEN  OPEN_BPAR statement CLOSE_BPAR option_else_statement
        {
            $$ = new CSyntaxNode( IF_ELSE_STA, $2, $5, $7, VOIDTYPE);
        }
	   ;
option_else_statement
       : ELSE OPEN_BPAR statement CLOSE_BPAR    {$$=$3;}
       | /* empty */                            {$$=new CSyntaxNode(TRUE_EXP, VOIDTYPE);}//else为空时表示true
       ;
output_statement
       : DISPLAY OPEN_PAR exp option_exp CLOSE_PAR   
	   {
	      CSyntaxNode *child0=new CSyntaxNode(OPTION_EXP, $3, $4, VOIDTYPE);
	      $$=new CSyntaxNode(DISPLAY_STA, child0, UNKNOWNTYPE);
	   }
	   ;
input_statement 
		:REQUEST OPEN_PAR exp option_exp CLOSE_PAR 
		{
		   CSyntaxNode *child0=new CSyntaxNode(OPTION_EXP, $3, $4, VOIDTYPE);
		   $$=new CSyntaxNode(REQUEST_STA, $3, $4, UNKNOWNTYPE);
		}
		;
