#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
//#include "definiciones.h"

//	DIRECCIONES		//
#define 	UP  		1
#define 	DOWN		2 
#define 	LEFT		3
#define 	RIGHT		4
#define		DIRNULL		5

//	MOVIMIENTOS		//
#define 	MOVE	6
#define		TURN	7

//	ACCIONES		//
#define 	GET  	8
#define 	DROP	9
#define 	GIVE	10

//	OBJETOS		//
#define 	GOLD	11
#define 	ARROW	12
#define 	BUSH	13
#define 	FENCE	14
#define 	WALL	15
#define 	BROCH	16

//	CONTROL		//
#define		SECUENCE	17
#define		CICLIC		18
#define		CONDIC		19
#define		SI			20
#define		MIENTRAS	21

typedef struct _simbolo{
	struct _simbolo *sig;
	char nombre[8];
	int x;
	int y;
	int direccion;
	int tvar;
} simbolo;

typedef struct cosa
{
	union {
       int numero;
       simbolo *ptrSimbolo;
	} valor;
} Cosa;

void insertar(simbolo ** p_t, simbolo * s) {
	s->sig = (*p_t);
	(*p_t) = s;
}

simbolo * buscar(simbolo *t, char nombre[8], int opcion) {

   if(opcion == 0)
   {	
	
	while ((t != NULL) && (strcmp(nombre, t->nombre)))
			t = t->sig;
		return (t);
   
   }
   else
   {
   while ((t != NULL) && (t->tvar != BROCH))
			t = t->sig;
		return (t);
   }
  
	
}

int verificarPosicion(simbolo *t,int x, int y)
{
   int bOcupado = 0;
   while (t != NULL)
   {
	   if(t->x==x && t->y == y)
	   {
		   bOcupado = 1;
	   }
	   t = t->sig;
   }
   return bOcupado;
}

int eliminarCabeza(simbolo ** p_t)
{
	simbolo *aux;
	aux = *p_t;
    
    if(*p_t)
    {
    	*p_t = aux->sig;
    	free(aux);
    	
    	return 1;
    }
    return 0;
}

/////////////////////////////////////////
// Cola

typedef struct nodo
{
	int tipoAccion; 
	int nveces;
	int dir;
	int Accion; 
	struct nodo *sig;
} NODO;

typedef struct
{
	NODO *delante, *atras;

} COLA;

void CrearCola(COLA *c) {
	c->delante = c->atras = NULL;
}

int ColaStaVacia(COLA c) {
	return c.delante == NULL;
}

void Encolar(COLA *c,int action, int nVeces, int tAction, int newDireccion)
{
	NODO *Nuevo;
	Nuevo = (NODO *)malloc(sizeof(NODO));
	
	if (Nuevo == NULL)
	{
		MessageBox(NULL, L"Poca memoria !", L"Advertencia", MB_OK | MB_ICONERROR);
	}

	Nuevo->Accion = action;
	Nuevo->nveces = nVeces;
	Nuevo->tipoAccion = tAction;
	Nuevo->dir = newDireccion;
	Nuevo->sig = NULL;

	if (ColaStaVacia(*c))
	{
		c->delante = Nuevo;
		//MessageBox(NULL, L"c->delante es igual a nuevo", L"Debug", MB_OK);
	}
	else
	{
		c->atras->sig = Nuevo;
	}

	c->atras = Nuevo;

}

int Eliminar(COLA *c) {
	NODO *Muerto;

	if (ColaStaVacia(*c))
		return 0;
	Muerto = c->delante;
	c->delante = c->delante->sig;
	if (c->delante == NULL)
		c->atras = NULL;
	free(Muerto);
	return 1;
}

void VaciarCola(COLA *c) {
	if (!ColaStaVacia(*c))
		while (Eliminar(c));
}