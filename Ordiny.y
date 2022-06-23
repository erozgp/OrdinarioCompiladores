%{
  #include "tabsimb.h"
  simbolo * t;
  COLA cola;
  #include <windows.h>
  #include "VGLOBAL.h"
  #include "Ordinl.h"
  simbolo * ptrSimb;
  int yyerror(const char *s);
  char variable[8];
  char cherror[64];
  TCHAR szError[60], szCad[60];
  char bus[] = " ";

%}
%union{
  Cosa myObjeto;
}
%token <myObjeto> ENTERO 
%token <myObjeto> VAR 

%token AVANZA 
%token GIRA
%token AGARRAR 
%token SOLTAR 
%token ENTREGAR

%token ARB 
%token ABJ 
%token IZQ 
%token DER

%token BROCHE 
%token GE 
%token FLECHA
%token ARBUSTO 
%token VALLA 
%token BARRERA

%type <myObjeto> direccion  
%type <myObjeto> obj 
%type <myObjeto> pasos

%left '[' ']' '('
%left ',' ')'
%left '{' '}'

%%
  programa  : encabezado cuerpo 
            ;

  encabezado  : '[' declaraciones ']'
              ;

  declaraciones : declaraciones declaracion
                | declaracion error {yyerrok;}
                | declaracion
                ;

  declaracion   : obj VAR {strcpy(variable,yytext);} '(' ENTERO ',' ENTERO ')' direccion ':'
                {
                if($2.valor.ptrSimbolo == NULL){
                      if($1.valor.numero == BROCH){
                        if(buscar(t, bus, 1) == NULL){
                            ptrSimb=(simbolo *) malloc(sizeof(simbolo));
                            strcpy(ptrSimb->nombre, variable);
                            ptrSimb->x = $5.valor.numero;
                            ptrSimb->y = $7.valor.numero;
                            ptrSimb->direccion = $9.valor.numero;
                            ptrSimb->tvar = $1.valor.numero; 
                            insertar(&t, ptrSimb);
                          }else{
                            sprintf(cherror,"ERROR => Broche ya fue creado. Linea => %d", numlineas);
                            
                          }
                      }else{
                        if(!verificarPosicion(t,$5.valor.numero, $7.valor.numero)){
                            ptrSimb=(simbolo *) malloc(sizeof(simbolo));
                            strcpy(ptrSimb->nombre, variable);
                            ptrSimb->x = $5.valor.numero;
                            ptrSimb->y = $7.valor.numero;
                            ptrSimb->direccion = DIRNULL;  
                            ptrSimb->tvar = $1.valor.numero; 
                            insertar(&t, ptrSimb);
                        }else{
                          sprintf(cherror,"ERROR => posicion ocupada. Linea => %d", numlineas);
                          
                        }
                      }
                    }else{
                      sprintf(cherror,"ERROR => variable ya existe. Linea => %d", numlineas);
                      
                    }
                }
                ;
                
  cuerpo   : '{' acciones '}'
          ;              

  obj  : BROCHE { $$.valor.numero = BROCH; }
        | GE { $$.valor.numero = GOLD; }
        | FLECHA { $$.valor.numero = ARROW;}
        | BARRERA { $$.valor.numero = WALL;}
        | ARBUSTO { $$.valor.numero = BUSH;}
        | VALLA { $$.valor.numero = FENCE;}
        ;

  direccion   : ARB { $$.valor.numero = UP;}
              | ABJ { $$.valor.numero = DOWN;}
              | IZQ { $$.valor.numero = LEFT;}
              | DER { $$.valor.numero = RIGHT; }
              ;

  acciones : acciones accion 
          | accion 
          ;

  accion  : AVANZA pasos ':' { Encolar(&cola, MOVE, $2.valor.numero, SECUENCE, DIRNULL); }
          | GIRA direccion ':' { Encolar(&cola, TURN, 0, SECUENCE, $2.valor.numero); }
          ;

  pasos  : ENTERO   {$$.valor.numero = $1.valor.numero;}
         |    {$$.valor.numero = 1;}
          ;


%%


int yyerror(const char *s ){
    printf("%s. Token: %s \n",s,yytext);
    return 0;
}
