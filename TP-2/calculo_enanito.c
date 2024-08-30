#include <stdio.h>
#include "calculo_enanito.h"

#define LECHUGA 'A'
#define BROCOLI 'B'
#define ZANAHORIA 'C'
#define TOMATE 'D'

#define VERANO 'V'
#define INVIERNO 'I'

const int ENOJO_MINIMO = 0;
const int ENOJO_MAXIMO = 5;

const int TIEMPO_MINIMO = 40;
const int TIEMPO_MAXIMO = 120;


/*
Pre: 'verdura_favorita' debe apuntar a una variable valida.
Post: Valida la respuesta y modifica el contenido de la variable apuntada.
*/
void respuesta_verdura(char* verdura_favorita){
		scanf(" %c", verdura_favorita);
		while(!(*verdura_favorita==LECHUGA || *verdura_favorita==BROCOLI || *verdura_favorita==ZANAHORIA || *verdura_favorita==TOMATE)){
			printf("Respuesta no valida, intente nuevamente: ");
			scanf(" %c", verdura_favorita);
		}
}


/*
Pre: 'verdura' debe ser una respuestas valida a la pregunta.
	 'puntaje' debe apuntar a una variable valida.
Post: Modifica el puntaje, segun la opcion ingresada.
*/
void puntaje_verdura(char verdura, int* puntaje){
	switch(verdura){
		case LECHUGA:
			*puntaje += 0;
			break;
		case BROCOLI:
			*puntaje += 5;
			break;
		case ZANAHORIA:
			*puntaje += 10;
			break;
		case TOMATE:
			*puntaje += 15;
			break;
	}
}


/*
Pre: 'puntaje' debe apuntar a una variable valida.
Post: Interactua con el usuario para obtener su respuesta, sobre su verdura favorita.
	  Modifica el puntaje segun la respuesta del usuario.
*/
void pregunta_verdura_favorita(int* puntaje){
	printf("Cual es tu verdura favorita?");
	printf("(%c) Lechuga\n(%c) Brócoli\n(%c) Zanahoria\n(%c) Tomate\n", LECHUGA, BROCOLI, ZANAHORIA, TOMATE);
	char verdura_favorita;
	respuesta_verdura(&verdura_favorita);
	puntaje_verdura(verdura_favorita, puntaje);
}


/*
Pre: 'team_elegido' debe apuntar a una variable valida.
Post: Valida la respuesta y modifica el contenido de la variable apuntada.
*/
void respuesta_team(char* team_elegido){
	scanf(" %c", team_elegido);
	while(!(*team_elegido==VERANO || *team_elegido==INVIERNO)){
		printf("Respuesta no valida, intente nuevamente: ");
		scanf(" %c", team_elegido);
	}
}


/*
Pre: 'epoca' debe ser una respuesta valida a la pregunta.
	 'puntaje' debe apuntar a una variable valida.
Post: Modifica el puntaje, segun la respuesta del usuario.
*/
void puntaje_team(char epoca, int* puntaje){
	switch(epoca){
		case VERANO:
			*puntaje += 10;
			break;
		case INVIERNO:
			*puntaje += 0;
			break;
	}
}


/*
Pre: 'puntaje' debe apuntar a una variable valida.
Post: Interactua con el usuario para obtener su respuesta sobre su preferencia.
	  Modifica el puntaje segun la respuesta del usuario.
*/
void pregunta_team(int* puntaje){
	printf("Sos team verano o team invierno?");
	printf("(%c) Invierno\n(%c) Verano\n", INVIERNO, VERANO);
	char team_elegido;
	respuesta_team(&team_elegido);
	puntaje_team(team_elegido, puntaje);
}


/*
Pre: 'cantidad_enojo' debe apuntar a una variable valida.
Post: Valida la respuesta y modifica el contenido de la variable apuntada.
*/
void respuesta_enojo(int* cantidad_enojo){
	scanf(" %d", cantidad_enojo);
	while(!(*cantidad_enojo>=ENOJO_MINIMO && *cantidad_enojo<=ENOJO_MAXIMO)){
		printf("Respuesta no valida, intente nuevamente: ");
		scanf(" %d", cantidad_enojo);
	}
}


/*
Pre: 'enojo' debe ser un entero valido entre ENOJO_MINIMO y ENOJO_MAXIMMO.
	 'puntaje' debe apuntar a una variable
Post: Modifica el puntaje segun la respuesta ingresada.
*/
void puntaje_enojo(int enojo, int* puntaje){
	*puntaje -= enojo;
}


/*
Pre: 'puntaje' debe apuntar a una variable valida.
Post: Interactua con el usuario para obtener su respuesta sobre su nivel de enojo.
	  Modifica el puntaje segun la respuesta del usuario.
*/
void pregunta_cuanto_te_enoja(int* puntaje){
	printf("Si es team verano la pregunta es: Del 0 al 5 ¿Cuánto te enojan los mosquitos?");
	printf("Si es team invierno la pregunta es: Del 0 al 5 ¿Cuánto te enoja trabajar bajo la lluvia?");
	int cantidad_enojo;
	respuesta_enojo(&cantidad_enojo);
	puntaje_enojo(cantidad_enojo, puntaje);
}


/*
Pre: 'minutos' debe apuntar a una variable valida.
Post: Valida la respuesta y modifica el contenido de la variable apuntada.
*/
void respuesta_tiempo(float* minutos){
	scanf(" %f", minutos);
	while(!(*minutos>=TIEMPO_MINIMO && *minutos<=TIEMPO_MAXIMO)){
		printf("Respuesta no valida, intente nuevamente: ");
		scanf(" %f", minutos);
	}
}


/*
Pre: 'minutos' debe ser un flotante valido entre TIEMPO_MINIMO y TIEMPO_MAXIMO.
	 'puntaje' debe apuntar a una variable valida.
Post: Modifica el puntaje segun la respuesta ingresada.
*/
void puntaje_tiempo(float minutos, int* puntaje){
	*puntaje += (int)(minutos/8);
}


/*
Pre: 'puntaje' debe apuntar a una variable valida.
Post: Interactua con el usuario para obtener su respuesta cobre el tiempo requerido para completar una tarea.
	  Modifica el puntaje segun la respuesta del usuario.
*/
void pregunta_cuanto_tiempo_tardarias(int* puntaje){
	printf("¿Cuántos minutos te llevaría cosechar un cultivo de 10m2?");
	printf("Puede ingresar numeros con decimales, entre 40 y 120");
	float tiempo_de_cosecha;
	respuesta_tiempo(&tiempo_de_cosecha);
	puntaje_tiempo(tiempo_de_cosecha, puntaje);
}


/*
Pre: 'puntos' debe ser un entero valido.
	 'inicial_enanito' debe apuntar a una variable valida.
Post: Asigna un caracter que representa al enanito, segun su puntaje.
*/
void asignar_enano(int puntos, char* inicial_enanito){
	if (puntos < 10) {
        *inicial_enanito = 'G';
    } else if (puntos < 20) {
        *inicial_enanito = 'D';
    } else if (puntos < 30) {
        *inicial_enanito = 'S';
    } else {
        *inicial_enanito = 'F';
    }
}


void calcular_enanito (char* inicial_enanito){
	int puntaje = 0;
	pregunta_verdura_favorita(&puntaje);
	pregunta_team(&puntaje);
	pregunta_cuanto_te_enoja(&puntaje);
	pregunta_cuanto_tiempo_tardarias(&puntaje);
	asignar_enano(puntaje, inicial_enanito);
}

