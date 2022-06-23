%{
	#include "Ordiny.h"
	int numlineas = 1;
%}
%%
[0-9]+	   {
		     yylval.myObjeto.valor.numero = atoi(yytext);
		     return ENTERO;
		   }

"BROCHE"		{ return BROCHE; }
"GE"			{ return GE; }
"FLECHA"		{ return FLECHA; }
"ARBUSTO"		{ return ARBUSTO; }
"VALLA"			{ return VALLA; }
"BARRERA"		{ return BARRERA; }
"avanza"		{ return AVANZA; }
"gira"			{ return GIRA; }
"ARB"      		{ return ARB; }
"ABJ"      		{ return ABJ; }
"IZQ"      		{ return IZQ; }
"DER"      		{ return DER; }
"agarrar"	 	{ return AGARRAR; }
"soltar"	 	{ return SOLTAR; }
"entregar"		{ return ENTREGAR; }



[\ \t\b\r]  	{;}
[\n]   			{numlineas++;}

[a-zA-Z][a-zA-Z0-9]	{
	                    if(strlen(yytext)>8) yytext[8] = 0;
				        yylval.myObjeto.valor.ptrSimbolo = buscar(t,yytext, 0);   
			            return VAR;
		      		}



. 				{ return yytext[0]; }

%%
int yywrap(){
	return 1;
}