#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "granja.h"
#include "calculo_enanito.h"

#define DERECHA 'D'
#define IZQUIERDA 'A'
#define ABAJO 'S'
#define ARRIBA 'W'

#define ZANAHORIA 'Z'
#define TOMATE 'T'
#define LECHUGA 'L'
#define BROCOLI 'B'

#define INSECTICIDA 'I'
#define FERTILIZANTE 'F'

#define SIN_INSECTICIDAS 0
#define SIN_FERTILIZANTE -1
#define INICIAL_CANASTA 0
#define UN_INSECTICIDA 1
#define DESCUENTO_FERTILIZAR 20
#define MONEDAS_POR_PISAR_ESPINAS 5
#define DISTANCIA_MANHATTAN_MINIMA_DEPOSITO 2
#define CANTIDAD_MOVIMIENTOS_PLAGA 10
#define CERO_P 0
#define CANTIDAD_MOVIMIENTOS_FERTILIZANTE 15
#define CERO_F 0
#define TIEMPO_PODRIR_PLAGA 10
#define TAMANIO 20

#define EN_CURSO 0
#define GANADO 1
#define PERDIDO -1

/*
*   La función recibe un caracter y devuelve true
* si el movimiento recibido concuerda con la convención
* propuesta, sino false.
* -> W: Si el personaje debe moverse para la arriba. 
* -> A: Si el personaje debe moverse para la izquierda.
* -> S: Si el personaje debe moverse para la abajo.
* -> D: Si el personaje debe moverse para la derecha.
* -> Z: Para sembrar zanahorias.
* -> T: Para sembrar tomates.
* -> L: Para sembrar lechugas.
* -> B: Para sembrar brocolis.
* -> F: Para usar el fertilizante.
* -> I: Para usar el insecticida.
*/
bool es_accion_valida(char movimiento){
    bool valido = false;
    if((movimiento == DERECHA) || (movimiento == IZQUIERDA) || (movimiento == ABAJO) || (movimiento == ARRIBA)){
        valido = true;
    }else if((movimiento == ZANAHORIA) || (movimiento == TOMATE) || (movimiento == LECHUGA) || (movimiento == BROCOLI)){
        valido = true;
    }else if((movimiento == INSECTICIDA) || (movimiento == FERTILIZANTE)){
        valido = true;
    }
    return valido;
}



/*precondiciones: Recibe el juego con todos los datos ya cargados.
 *postcondiciones: Le pide una tecla al usuario hasta que ingrese una correcta, ahi la guarda y devuelve por referencia
 */
void leer_movimiento(char* accion){

    printf("Ingrese la accion a realizar:\n");
    printf("Para mover el personaje: (A-Izquierda D-Derecha S-Abajo W-Arriba) \n");
    printf("Para sembrar: (Z-Zanahoria T-Tomate L-Lechuga B-Brocoli) \n");
    printf("Para usar una herramienta: (I-Insecticida F-Fertilizante)");
    scanf(" %c",accion);

    while(!es_accion_valida(*accion)){
        printf("Ingreso una accion que no es valida, ingrese una correcta.");
        printf("Para mover el personaje: (A-Izquierda D-Derecha S-Abajo W-Arriba) \n");
        printf("Para sembrar: (Z-Zanahoria T-Tomate L-Lechuga B-Brocoli) \n");
        printf("Para usar una herramienta: (I-Insecticida F-Fertilizante)");
        scanf(" %c",accion);
    }
}
/*
 * Imprime en la consola el contenido de la canasta del jugador.
 */
void imprimir_canasta(juego_t juego) {
    printf("Contenido de la canasta:\n");
    for (int i = 0; i <juego.jugador.tope_canasta; i++) {
        printf("%c ", juego.jugador.canasta[i]);
    }
    printf("\n");
}

/*
 * Imprime en la consola las iniciales de los elementos del juego.
 */
void imprimir_objetos_del_juego(juego_t juego) {
    printf("Contenido de objetos:\n");
    for (int i = 0; i <juego.tope_objetos ; i++) {
        printf("%c ", juego.objetos[i].tipo);
    }
    printf("\n");
}


/*precondiciones: Recibe el juego con todos los datos cargados correctamente
 *postcondiciones: Muestra los datos que son importantes para el usuario
 */
void mostrar_datos_jugador(juego_t juego){
    printf("\n");
    printf("Total de movimientos: %i\n", juego.movimientos);
    printf("Total de monedas: %i\n", juego.jugador.cant_monedas);
    printf("Espacios usados en canasta: %i\n", juego.jugador.tope_canasta);

    imprimir_canasta(juego);
    imprimir_objetos_del_juego(juego);
    
    if((juego.jugador.cant_insecticidas)>SIN_INSECTICIDAS){
        printf("Total de insecticidas: %i\n", juego.jugador.cant_insecticidas);
    }else{
        printf("Ya no tienes insecticidas. \n");
    }

    if((juego.jugador.tiene_fertilizante)==true){
        printf("Tiene 1 fertilizante. \n");
    }else{
        printf("No tiene fertilizante. \n");
    }
}


/*precondiciones: El juego ya tiene que haber terminado
 *postcondiciones: Mostrara el resultado
 */
void mostrar_estado_juego(juego_t juego){
    if (estado_juego(juego)==GANADO){
        printf("Felicitaciones, ha ganado el juego! \n");
    }
    else if(estado_juego(juego)==PERDIDO){
        printf("Ha perdido el juego :( \n");
    }
}


/*
 * Muestra informacion general del juego mientras transcurre la partida. Mantiene disponible en cada turno, lo relevante para continuar jugando y tomar desiciones.
 */
void mostrar_informacion_cultivos(){
    printf("\n");
    printf("\n");
    printf("Puedes cultivar T-Tomate, L-Lechuga, Z-Zanahoria o B-Brocoli. Cada una tiene un costo (en monedas $), un tiempo de vida (en movimientos M) y un beneficio diferente (en monedas $).\n");
    printf("El tomate cuesta $5, vive 30M, se puede cosechar a los 20M y su cosecha te da $30");
    printf("La lechuga cuesta $20, vive 15M, se puede cosechar a los 10M y su cosecha te da $80");
    printf("La zanahoria cuesta $10, vive 25M, se puede cosechar a los 15M y su cosecha te da $50");
    printf("El brocoli cuesta $15, vive 20M, se puede cosechar a los 10M y su cosecha te da $70");
    printf("Para cosechar debes posicionarte sobre un cultivo donde la verdura este lista, automaticamente se agrega a la canasta. La canasta puede llevar hasta 6 verduras.");
}


/* 
 * Muestra informacion genral del juego al inicio de la partida. Presenta los indicadores, las condiciones de victoria/derrota y los elementos que conforman el juego.
 */
void mostrar_informacion_general_inicial(){
    printf("\n");
    printf("\n");
    printf("Usted es el jugador, representado con la 'S', tiene que cultivar las huertas, cosechar y depositar las verduras para cambiarlas por monedas.\n");
    printf("Para no perder debes evitar quedarte sin monedas. Tendras una cantidad inicial que deberas administrar.");
    printf("Se inician 3 huertas con 9 cultivos vacios. Debes posicionarte sobre alguno de ellos para cultivar alguna verdura.\n");
    printf("Debes llevar las verduras al deposito para cambiarlas por monedas, pero tambien para vaciar la canasta y seguir cosechando.");
    printf("Los cultivos vacios se representan con 'C', y si hay verduras, se sustituye la 'C' con la inicial de la verdura (Z,T,L,B) . \n");
    printf("Pero ten cuidado! Cada 10 movimientos aparece una plaga en el terreno que podria afectar a los cultivos.\n");
    printf("Elegir correctamente que cultivar y que cosechar seran las claves para alcanzar las 1000 monedas y asi, ganar el juego.\n");
    printf("Ademas ten en cuenta que hay obstaculos que te pueden hacer perder monedas, son: Espinas 'E'. \n");
    printf("Pero puedes, simplemente, evitarlas. Desplazandote por otro camino.\n");
    printf("Inicialmente cuentas con 3 insecticidas, estos te ayudaran a deshacerte de las plagas de toda una huerta, solo si los activas sobre un cultivo de ella.\n");
    printf("Tambien, cada 15 movimientos, aparece en el terreno, un fertilizante. Puedes posicionarte encima de el para tomarlo. Te ayudara a que crescan todas las verduras de una huerta, solo si lo activas sobre un cultivo de ella.\n");
    printf("Puedes visualizar el deposito en el terreno, esta representado con la 'D'.\n");
    printf("Buena suerte! :)\n");
}





int main() {
    srand((unsigned)time(NULL));
    juego_t juego;
    char accion;
    char enanito;
    calcular_enanito(&enanito);

    inicializar_juego(&juego, enanito);
    mostrar_informacion_general_inicial();
    while(estado_juego(juego) == EN_CURSO){
        mostrar_informacion_cultivos();
        mostrar_datos_jugador(juego);
        imprimir_terreno(juego);
        leer_movimiento(&accion);
        realizar_jugada(&juego, accion);
        system("clear");
    }
    mostrar_estado_juego(juego);
    return 0;
}
