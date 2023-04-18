%option yylineno
%{
#include <iostream>
#include "parser.tab.hpp" 

extern FILE *yyin;
extern int yylex();

/* #include "Includes/Commands.hpp"
#include "Includes/Conditions.hpp"
#include "Includes/Identifiers.hpp"
#include "Includes/Expressions.hpp"
#include "Includes/Environment.hpp"
#include "Includes/Value.hpp"
#include "Includes/Nodes.hpp"*/

%}

%x COMMENT
%option noyywrap

%%

"["             {std::cout<<"Comment\n"; ;BEGIN(COMMENT);}
<COMMENT>{
    "]"         {BEGIN(INITIAL);}
    "["         { std::cout << "Error: nested comments.\n"; yyterminate();}
    <<EOF>>     { std::cout << "Error: infinite comment.\n"; yyterminate();}
    .|\n    
}

"PROCEDURE"     {return PROCEDURE;}
"VAR"           {return VAR;}
"BEGIN"         { return PBEGIN;}
"END"           { return END;}

"PROGRAM"       {return PROGRAM;}
":="            {return ASSIGN;}
"IS"            {return IS;}

"IF"            {return IF;}
"THEN"          {return THEN;}
"ELSE"          {return ELSE;}
"ENDIF"         {return ENDIF;}

"WHILE"         {return WHILE;}
"DO"            {return DO;}
"ENDWHILE"      {return ENDWHILE;}

"REPEAT"        {return REPEAT;}
"UNTIL"         {return UNTIL;}

"READ"          {return READ;}
"WRITE"         {return WRITE;}

"+"             {return '+';}
"-"             {return '-';}
"*"             {return '*';}
"/"             {return '/';}
"%"             {return '%';}

"="             {return EQ;}
"!="            {return NEQ;}
">"             {return GT;}
"<"             {return LT;}
">="            {return GEQ;}
"<="            {return LEQ;}

"("             {return '(';}
")"             {return ')';}

","             { return ',';}
";"             { return ';';}

[0-9]+          { yylval.num = std::stol(yytext); return NUM; }
[_a-z]+         { yylval.str = new std::string(yytext); return IDENTIFIER; }

\n              ;
[[:blank:]]+    ;
\[[^]]*\]		;
.               { std::cout << "Error: invalid character.\n"; yyterminate();};

%%