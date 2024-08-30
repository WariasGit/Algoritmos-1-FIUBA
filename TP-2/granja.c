#include<stdio.h>
#include "granja.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

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

#define EN_CURSO 0
#define GANADO 1
#define PERDIDO -1

#define MONEDAS_DE_GRUNION 150
#define MONEDAS_DE_DORMILON 200
#define MONEDAS_DE_SABIO 250
#define MONEDAS_DE_FELIZ 300

#define COSTO_ZANAHORIA 10
#define COSTO_TOMATE 5
#define COSTO_LECHUGA 20
#define COSTO_BROCOLI 15

#define VIDA_ZANAHORIA 25
#define VIDA_TOMATE 30
#define VIDA_LECHUGA 15
#define VIDA_BROCOLI 20

#define TIEMPO_COSECHA_ZANAHORIA 15 
#define TIEMPO_COSECHA_TOMATE 20
#define TIEMPO_COSECHA_LECHUGA 10
#define TIEMPO_COSECHA_BROCOLI 10

#define GANANCIA_ZANAHORIA 50
#define GANANCIA_TOMATE 30
#define GANANCIA_LECHUGA 80
#define GANANCIA_BROCOLI 70

#define LETRA_BLANCANIEVES 'S'
#define LETRA_CULTIVO_VACIO 'C'
#define LETRA_DEPOSITO 'D'
#define LETRA_ESPINAS 'E'
#define LETRA_PLAGA 'P'
#define LETRA_FERTILIZANTE 'F'

#define TAMANIO 20
#define TAMANIO_MENOS_UNO 19
#define AMPLITUD_RANGO 18
#define VALOR_MINIMO_RANGO 1
#define UNA_COORDENADA 1
#define DOS_COORDENADAS 2
#define UN_MOVIMIENTO 1
#define INDICE_CULTIVO_CENTRAL 4
#define DISTANCIA_MANHATTAN_MINIMA_HUERTA 4
#define VACIO ' '

#define SIN_PLAGAS 0
#define SIN_PLANTAS 0
#define SIN_HUERTAS 0
#define SIN_ESPINAS 0
#define SIN_DESCUENTO 0
#define SIN_INDICE_ASIGNADO -1

#define MAX_ESPINAS 5
#define MAX_INSECTICIDAS 3
#define MAX_MONEDAS 1000
#define MIN_MONEDAS 0

#define INICIAL_OBJETOS 0
#define INICIAL_MOVIMIENTOS 0

#define FILA_INICIAL_CERO 0
#define COLUMNA_INICIAL_CERO 0


/*
 * Precondiciones: Recibe una estructura del juego valida y una coordenada valida.
 * Postcondiciones: Recorre las huertas y sus cultivos. Devuelve true si la coordenada recibida coincide con la coordenada de algun cultivo. En caso contrario, false.
 */
bool se_sobrepone_con_huertas(juego_t juego, coordenada_t pos_elemento){
    bool valor = false;
    for(int i=0; i<MAX_HUERTA; i++){
        for(int j=0; j<MAX_PLANTAS; j++){
            if((juego.huertas[i].cultivos[j].posicion.fila == pos_elemento.fila) && (juego.huertas[i].cultivos[j].posicion.columna == pos_elemento.columna)){
                valor = true;
            }
        }
    }
    return valor;
}

/*
 * Precondiciones: Recibe una estructura del juego valida y una coordenada valida.
 * Postcondiciones: Recorre el vector objetos. Devuelve true si la coordenada recibida coincide con la coordenada de algun elemento. En caso contrario, false.
 */
bool se_sobrepone_con_objetos(juego_t juego, coordenada_t pos_elemento){
    bool valor = false;
    for(int i=0; i<juego.tope_objetos; i++){
        if((juego.objetos[i].posicion.fila == pos_elemento.fila) && (juego.objetos[i].posicion.columna == pos_elemento.columna)){
            valor = true;
        }
    }
    return valor;
}

/*
 * Precondiciones: Recibe una estructura del juego valida y una coordenada valida.
 * Postcondiciones: Devuelve true si la coordenada recibida coincide con la coordenada del jugador. En caso contrario, false.
 */
bool se_sobrepone_con_jugador(juego_t juego, coordenada_t pos_elemento){
    bool valor = false;
    coordenada_t pos_jugador = juego.jugador.posicion;
    if((pos_jugador.fila == pos_elemento.fila) && (pos_jugador.columna == pos_elemento.columna)){
        valor = true;
    }
    return valor;
}

/*
 * Precondiciones: Recibe una estructura del juego valida y una coordenada valida.
 * Postcondiciones: Devuelve true si la coordenada recibida coincide con la coordenada del deposito. En caso contrario, false.
 */
bool se_sobrepone_con_deposito(juego_t juego, coordenada_t pos_elemento){
    bool valor = false;
    coordenada_t pos_deposito = juego.deposito;
    if((pos_deposito.fila == pos_elemento.fila) && (pos_deposito.columna == pos_elemento.columna)){
        valor = true;
    }
    return valor;
}

/*
 * El procedimiento inicializa el contador de movimientos en cero.
 */
void inicializar_contador_movimientos(juego_t* juego){
    (*juego).movimientos = INICIAL_MOVIMIENTOS;
}

/*
 * Devolverá una coordenada aleatoria dentro del rango TAMANIOxTAMANIO.
 * No valida que dicha coordenada coincida con un algun elemento del juego.
 */
coordenada_t posicion_aleatoria_elementos(){
    coordenada_t posicion;
    
    posicion.fila=rand()%TAMANIO;
    posicion.columna=rand()%TAMANIO;

    return posicion;
}

/*
 * La función recibe dos coordenadas y devuelve la distancia manhattan entre ambas
 */
int distancia_manhattan(coordenada_t coordenada_1, coordenada_t coordenada_2){
    return (abs(coordenada_1.fila - coordenada_2.fila) + abs(coordenada_1.columna - coordenada_2.columna));
}

/*
 * La matriz matriz se carga con caracteres especiales (espacios en blanco) en cada una de sus celdas.
 */
void inicializar_matriz_vacia(char matriz[TAMANIO][TAMANIO]){
    for(int i = 0; i<TAMANIO;i++){
        for(int j = 0; j<TAMANIO; j++){
            matriz[i][j] =VACIO;
        }
    }
}

/*
 * Precondiciones: Recibe una estructura del juego valida y una coordenada valida.
 * Postcondiciones: Evalua si la coordenada recibida se sobrepone con algun elemento del juego: huertas, objetos, jugador y deposito.
 *                  Si no se sobrepone con ningun elemento del juego, devuelve true. En caso de sobreponerse, devuelve false.
 */
bool es_lugar_ocupado(juego_t juego, coordenada_t coordenada_elemento){
    bool valor = true;
    if(!se_sobrepone_con_huertas(juego, coordenada_elemento) && !se_sobrepone_con_objetos(juego, coordenada_elemento) && !se_sobrepone_con_jugador(juego, coordenada_elemento) && !se_sobrepone_con_deposito(juego, coordenada_elemento)){
        valor = false;
    }
    return valor;
}

/*
 * Precondiciones: recibe una coordenada valida.
 * Postcondiciones: devuelve true si la posicion corresponde a un lugar dentro del terreno. Caso contrario, devuelve false.
 */
bool esta_dentro_del_terreno(coordenada_t posicion) {
    bool valor = false;
    if (posicion.fila >= FILA_INICIAL_CERO && posicion.fila <= TAMANIO_MENOS_UNO && posicion.columna >= COLUMNA_INICIAL_CERO && posicion.columna <= TAMANIO_MENOS_UNO) {
        valor = true;
    }
    return valor;
}

/*
 * Precondiciones: Recibe un puntero a una estructura valida del juego y dos enteros; fila y columna conforman una coordenada valida. Y k es un indice que representa a cada huerta. 
 * Postcondiciones: Inicia correctamente los cultivos correspondientes a la huerta k con valores predeterminados, siempre que estos esten dentro del terreno.
 */
void iniciar_cultivo(juego_t* juego, int fila, int columna, int k){
    (*juego).huertas[k].tope_cultivos = SIN_PLANTAS;
    for(int i = fila - UNA_COORDENADA; i < fila + DOS_COORDENADAS; i ++){
        for(int j = columna - UNA_COORDENADA; j < columna + DOS_COORDENADAS; j++){
            coordenada_t coordenada_cultivo ;
            coordenada_cultivo.fila = i;
            coordenada_cultivo.columna = j;
            if(esta_dentro_del_terreno(coordenada_cultivo)){
                int indice = (*juego).huertas[k].tope_cultivos;
                (*juego).huertas[k].cultivos[indice].posicion.fila = i;
                (*juego).huertas[k].cultivos[indice].posicion.columna = j;
                (*juego).huertas[k].cultivos[indice].tipo = LETRA_CULTIVO_VACIO;
                (*juego).huertas[k].cultivos[indice].ocupado = false;
                (*juego).huertas[k].cultivos[indice].movimiento_plantado = SIN_PLANTAS;
                (*juego).huertas[k].tope_cultivos ++;
            }
        }
    }
}

/*
 * Precondiciones: coordenadas_huerta es una coordenada valida. Y k es un indice valido que representa a cada huerta.
 * Postcondiciones: La huerta k se inicia correctamente con valores predeterminados. Se inician los cultivos correspondientes.
 */
void iniciar_huerta(juego_t* juego, coordenada_t coordenadas_huerta, int k) {
    int fila = coordenadas_huerta.fila;
    int columna = coordenadas_huerta.columna;
    (*juego).huertas[k].movimientos_plagado = SIN_PLAGAS;
    (*juego).huertas[k].plagado = false;
    iniciar_cultivo(juego, fila, columna, k);
}



/*
 * Se iniciaran las 3 huertas dentro de la estructura juego, sin sobreponerse.
 * Se actualizara el terreno, para reflejar las posiciones ocupadas por las huertas y sus respectivos cultivos.
 */
void inicializar_huertas(juego_t* juego) {
    int huertas_inicializadas = SIN_HUERTAS;
    while (huertas_inicializadas < MAX_HUERTA) {
        coordenada_t coordenadas_huerta = posicion_aleatoria_elementos();
            if (huertas_inicializadas == SIN_HUERTAS ||
                (huertas_inicializadas > SIN_HUERTAS && !se_sobrepone_con_huertas(*juego, coordenadas_huerta) && distancia_manhattan(coordenadas_huerta, juego->huertas[huertas_inicializadas - 1].cultivos[INDICE_CULTIVO_CENTRAL].posicion) > DISTANCIA_MANHATTAN_MINIMA_HUERTA)) {
                iniciar_huerta(juego, coordenadas_huerta, huertas_inicializadas);
                huertas_inicializadas++;
            }
        
    }
}

/*
 * Precondiciones; juego es un puntero a una estructura valida. Las coordenadas recibidas son validas. El entero recibido representa la cantidad de espinas iniciadas hasta el momento.
 * Postcondiciones: Se carga el vector objetos con espinas y se actualiza su tope.
 */
void iniciar_espinas(juego_t* juego, coordenada_t coordenadas_espinas, int espinas_inicializadas){
    (*juego).objetos[espinas_inicializadas].posicion = coordenadas_espinas;
    (*juego).objetos[espinas_inicializadas].tipo = LETRA_ESPINAS;
    (*juego).tope_objetos++;
}

/*
 *Se inicia el tope objetos en 0. Se inician las espinas evitando que se sobrepongan con otro elemento del juego. Se agregan las espinasa a la estructura juego.
 */
void inicializar_obstaculos(juego_t* juego) {
    (*juego).tope_objetos = INICIAL_OBJETOS;
    int espinas_inicializadas = SIN_ESPINAS;
    while (espinas_inicializadas < MAX_ESPINAS) {
        coordenada_t coordenadas_espinas = posicion_aleatoria_elementos();
        
        if (!se_sobrepone_con_huertas(*juego, coordenadas_espinas) && !se_sobrepone_con_objetos(*juego, coordenadas_espinas)) {
            iniciar_espinas(juego, coordenadas_espinas, espinas_inicializadas);
            espinas_inicializadas++;
        }
    }
}

/*
 * Precondiciones: enanito es un caracter que representa la inicial de un personaje.
 * Postcondiciones: se asigna una cantidad inicial de monedas, segun la inicial  que corresponda.
 */
void iniciar_monedas(juego_t* juego, char enanito){
    switch (enanito) {
        case 'G':
            (*juego).jugador.cant_monedas = MONEDAS_DE_GRUNION;
            break;
        case 'D':
            (*juego).jugador.cant_monedas = MONEDAS_DE_DORMILON;
            break;
        case 'S':
            (*juego).jugador.cant_monedas = MONEDAS_DE_SABIO;
            break;
        case 'F':
            (*juego).jugador.cant_monedas = MONEDAS_DE_FELIZ;
            break;
    }
}

/*
 * Se inicializa la canasta vacia, es decir con caracteres nulos en sus entradas. Se establece el valor incial de su tope.
 */
void iniciar_canasta(juego_t* juego){
    for (int i = 0; i < MAX_CANASTA; i++) {
        juego->jugador.canasta[i] = ' ';
    }
    juego->jugador.tope_canasta = INICIAL_CANASTA;
}

/*
 * Precondiciones: enanito es un caracter que representa la inicial de un personaje.
 * Postcondiciones: Se inician los atributos del jugador correctamente en un estado predeterminado.
 */
void iniciar_personaje(juego_t* juego, coordenada_t coordenada, char enanito){
    (*juego).jugador.posicion = coordenada;
    (*juego).jugador.tiene_fertilizante = false;
    (*juego).jugador.cant_insecticidas = MAX_INSECTICIDAS;
    iniciar_monedas(juego, enanito);
    iniciar_canasta(juego);
}

/*
 * Precondiciones: juego es un puntero a una estructura valida. enanito es un caracter que representa la inicial de un personaje.
 * Postcondiciones: La posicion del jugador es aleatoria, cuidando de no sobreponerse con otros elementos del juego.
 * Se inician correctamente los atributos del personaje y se actualiza su posicion en el terreno.
 */
void inicializar_personaje(juego_t* juego, char enanito) {
    coordenada_t coordenadas_personaje = posicion_aleatoria_elementos();
    while (se_sobrepone_con_huertas(*juego, coordenadas_personaje) || se_sobrepone_con_objetos(*juego, coordenadas_personaje)) {
        coordenadas_personaje = posicion_aleatoria_elementos();
    }
    iniciar_personaje(juego, coordenadas_personaje, enanito);
}


/*
 * La posicion del depostito se carga correctamente al juego. coordenadas_deposito es una coordenada valida.
 */
void iniciar_deposito(juego_t* juego, coordenada_t coordenadas_deposito){
    (*juego).deposito.fila = coordenadas_deposito.fila;
    (*juego).deposito.columna = coordenadas_deposito.columna;
}

/*
 * Se selecciona una posicion aleatoria para el deposito y se verifica que no se sobreponga con otro elemento.
 * Se inicia la estructura deposito, con sus coordenadas.
 * Se actualiza el terreno para reflejar la posicion del deposito.
 */
void inicializar_deposito(juego_t* juego){
    coordenada_t coordenadas_deposito = posicion_aleatoria_elementos();
    while(se_sobrepone_con_huertas(*juego, coordenadas_deposito) || se_sobrepone_con_objetos(*juego, coordenadas_deposito) || se_sobrepone_con_jugador(*juego, coordenadas_deposito)){
        coordenadas_deposito = posicion_aleatoria_elementos();
    }
    iniciar_deposito(juego, coordenadas_deposito);
}


/*
 *El procedimmiento recibe la coordenada del personaje y una direccion. Actualiza la posicion de acuerdo con la direccion recibida.
 */
void mover_personaje(coordenada_t* posicion, char direccion){
    switch(direccion){
        case IZQUIERDA:
            (*posicion).columna --;
            break;
        case DERECHA:
            (*posicion).columna ++;
            break;
        case ARRIBA:
            (*posicion).fila --;
            break;
        case ABAJO:
            (*posicion).fila ++;
            break;
    }
}

/*
 * El procedimiento recibe un puntero a una estructura valida y una direccion valida.
 * Mueve al personaje y actualiza la posicion, verificando que no se salga del terreno.
 */
void realizar_movimiento(juego_t* juego, char direccion){
    coordenada_t posicion_nueva_personaje = (*juego).jugador.posicion;
    mover_personaje(&posicion_nueva_personaje, direccion);
    if(esta_dentro_del_terreno(posicion_nueva_personaje)){
        (*juego).jugador.posicion = posicion_nueva_personaje;
        (*juego).movimientos ++;
    }
}


/*
 * Precondiciones: recibe una estructura del juego valida.
 * Postcondiciones: devuelve true si el jugador está sobre un cultivo vacío, de lo contrario, devuelve false
 */
bool jugador_sobre_cultivo_vacio(juego_t juego) {
    bool valor = false;
    coordenada_t pos_jugador = juego.jugador.posicion;
    for(int i=0; i<MAX_HUERTA; i++){
        for(int j=0; j < juego.huertas[i].tope_cultivos; j++){
            if(juego.huertas[i].cultivos[j].ocupado == false){
                if((juego.huertas[i].cultivos[j].posicion.fila == pos_jugador.fila) && (juego.huertas[i].cultivos[j].posicion.columna == pos_jugador.columna)){
                    valor = true;
                }
            }
        }
    }
    return valor;
}

/*
 * Precondiciones; recibe una estructura del juego valida.
 * Postcondiciones: devuelve true si el jugador esta sobre una huerta sin importar el contenido de los cultivos, en caso contrario, devuelve false.
 */
bool jugador_sobre_huerta(juego_t juego){
    bool valor = false;
    coordenada_t pos_jugador = juego.jugador.posicion;
    for(int i=0; i<MAX_HUERTA; i++){
        for(int j=0; j < juego.huertas[i].tope_cultivos; j++){
            if((juego.huertas[i].cultivos[j].posicion.fila == pos_jugador.fila) && (juego.huertas[i].cultivos[j].posicion.columna == pos_jugador.columna)){
                valor = true;
            }
        }
    }
    return valor;
}

/*
 * Precondiciones: recibe una estructura del juego valida.
 * Postcondiciones: devuelve true si el jugador está sobre un cultivo plantado, de lo contrario, devuelve false
 */
bool jugador_sobre_cultivo_plantado(juego_t juego) {
    bool valor = false;
    coordenada_t pos_jugador = juego.jugador.posicion;
    for(int i=0; i<MAX_HUERTA; i++){
        for(int j=0; j<MAX_PLANTAS; j++){
            if(juego.huertas[i].cultivos[j].ocupado == true){
                if((juego.huertas[i].cultivos[j].posicion.fila == pos_jugador.fila) && (juego.huertas[i].cultivos[j].posicion.columna == pos_jugador.columna)){
                    valor = true;
                }
            }
        }
    }
    return valor;
}

/*
 * Precondiciones: recibe una estructura del juego valida.
 * Postcondiciones: devuelve true si dentro de la estructura del juego, del vector objetos, hay una plaga. Caso contrario, devuelve false.
 */
bool hay_plaga(juego_t juego){
    bool valor = false;
    for(int i=0; i<juego.tope_objetos; i++){
        if(juego.objetos[i].tipo == LETRA_PLAGA){
            valor = true;
        }
    }
    return valor;
}

/*
 * Precondiciones: recibe una estructura del juego valida y un indice de una huerta valida.
 * Postcondiciones: devuelve true si la huerta dada por el indice esta plagada. De lo contrario, devuelve false.
 */
bool huerto_plagado(juego_t juego, int indice_huerta){
    bool valor=false;
    if(juego.huertas[indice_huerta].plagado == true){
        valor = true;
    }
    return valor;
}

/*
 * El jugador esta parado sobre algun cultivo. 
 * Modifica los valores de *indice_huerta y *indice_cultivo para indicar la huerta y el cultivo en el que se encuentra el jugador.
 */
void obtener_indices_huerta_cultivo(juego_t juego, int* indice_huerta, int* indice_cultivo) {
    *indice_huerta = SIN_INDICE_ASIGNADO;
    *indice_cultivo = SIN_INDICE_ASIGNADO;
    coordenada_t posicion_personaje = juego.jugador.posicion;
    for (int i = 0; i < MAX_HUERTA && *indice_huerta == SIN_INDICE_ASIGNADO; i++) {
        for (int j = 0; j < juego.huertas[i].tope_cultivos && *indice_huerta == SIN_INDICE_ASIGNADO; j++) {
            cultivo_t cultivo = (juego.huertas[i].cultivos[j]);

            if (posicion_personaje.fila == cultivo.posicion.fila && posicion_personaje.columna == cultivo.posicion.columna) {
                *indice_huerta = i;
                *indice_cultivo = j;
            }
        }
    }
}

/*
 * Precondiciones: los indices de la huerta y el cultivo deben ser validos.
 * Postcondiciones: Modifica los datos del cultivo.
 */
void modificar_cultivo(juego_t* juego, int indice_huerta, int indice_cultivo, char verdura){
    (*juego).huertas[indice_huerta].cultivos[indice_cultivo].movimiento_plantado = (*juego).movimientos;
    (*juego).huertas[indice_huerta].cultivos[indice_cultivo].tipo = verdura;
    (*juego).huertas[indice_huerta].cultivos[indice_cultivo].ocupado = true;
}

/*
 * Calcula el costo de sembrado y lo almacena en la variable costo basándose en el tipo de verdura especificado.
 */
void calcular_costo_sembrado(char verdura, int* costo){
    switch (verdura) {
        case ZANAHORIA:
            *costo = COSTO_ZANAHORIA;
            break;
        case TOMATE:
            *costo = COSTO_TOMATE;
            break;
        case LECHUGA:
            *costo = COSTO_LECHUGA;
            break;
        case BROCOLI:
            *costo = COSTO_BROCOLI;
            break;
    }
}

/*
 * Precondiciones: costo debe ser un valor valido.
 * Postcondiciones: Reduce la cantidad de monedas del jugador, segun el costo especificado.
 */
void descontar_monedas(juego_t* juego, int costo){
    (*juego).jugador.cant_monedas -= costo;
}

/*
 * Precondiciones: recibe una estructura del juego valida y la inicial de una verdura.
 * Postcondiciones: evalua la cantidad de monedas del jugador y el costo de la verdura. Si le alcanza para sembrar devuelve true, si no, false.
 */
bool jugador_tiene_monedas_minimas(juego_t juego, char verdura){
    bool valor= false;
    switch(verdura){
        case ZANAHORIA:
            if(juego.jugador.cant_monedas > COSTO_ZANAHORIA){
                valor = true;
            }
            break;
        case TOMATE:
            if(juego.jugador.cant_monedas > COSTO_TOMATE){
                valor = true;
            }
            break;
        case LECHUGA:
            if(juego.jugador.cant_monedas > COSTO_LECHUGA){
                valor = true;
            }
            break;
        case BROCOLI:
            if(juego.jugador.cant_monedas > COSTO_BROCOLI){
                valor = true;
            }
            break;
    }
    return valor;

}

/*
 * Precondiciones: El jugador debe estar posicionado sobre un cultivo vacio para poder sembrar y asi modificar los datos del juego.
 * Postcondiciones: Modifica el cultivo en el que el jugador se encuentra, actualizando su tipo y marcándolo como ocupado. Luego, descuenta las monedas necesarias del jugador en función del costo del tipo de cultivo sembrado.
 *                  Si el jugador intenta sembrar en otro lugar que no sea un huerto vacio, no se modifica nada.
 */
void sembrar_cultivo(juego_t* juego, char verdura){
    if(jugador_sobre_cultivo_vacio(*juego) && jugador_tiene_monedas_minimas(*juego, verdura)){
        int indice_huerta;
        int indice_cultivo;
        int costo;
        obtener_indices_huerta_cultivo(*juego, &indice_huerta, &indice_cultivo);
        modificar_cultivo(juego, indice_huerta, indice_cultivo, verdura);
        calcular_costo_sembrado(verdura, &costo);
        descontar_monedas(juego, costo);
    }
}

/*
 * Precondiciones: recibe la inicial de una verdura y un puntero a un entero.
 * Postcondiciones: evalua la verdura, y asigna el tiempo de cosecha de cada una.
 */
void calcular_descuento_tiempo_plantas(char verdura, int* descuento){
    switch (verdura) {
        case ZANAHORIA:
            *descuento = TIEMPO_COSECHA_ZANAHORIA;
            break;
        case TOMATE:
            *descuento = TIEMPO_COSECHA_TOMATE;
            break;
        case LECHUGA:
            *descuento = TIEMPO_COSECHA_LECHUGA;
            break;
        case BROCOLI:
            *descuento = TIEMPO_COSECHA_BROCOLI;
            break;
        default:
            *descuento = SIN_DESCUENTO;
    }
}

/*
 * Precondicioes: recibe un puntero a una estructura del juego valida. El indice_huerta debe corresponder al de una huerta valida.
 * Postcondiciones: no verifica si el jugador tiene un fertilizante. Recorre la huerta indicada, si hay huertos plantados, los fertiliza.
 */
void fertilizar(juego_t* juego, int indice_huerta){
    for (int i = 0; i < (*juego).huertas[indice_huerta].tope_cultivos; i++) {
            char verdura = (*juego).huertas[indice_huerta].cultivos[i].tipo;
            int descuento_tiempo_fertilizante;
            calcular_descuento_tiempo_plantas(verdura, &descuento_tiempo_fertilizante);
            (*juego).huertas[indice_huerta].cultivos[i].movimiento_plantado = (*juego).movimientos - (descuento_tiempo_fertilizante + UN_MOVIMIENTO);
    }
}

/*
 * Precondiciones: recibe una estructura del juego valida.
 * Postcondiciones: evalua si el jugador tiene fertilizante, devuelve true si lo tiene,si  no, devuelve false.
 */ 
bool jugador_tiene_fertilizante(juego_t juego){
    bool valor = false;
    if(juego.jugador.tiene_fertilizante == true){
        valor = true;
    }
    return valor;
}

/*
 * Precondiciones; recibe un puntero a una estructura del juego.
 * Postcondiciones: si el jugador tiene fertilizante y esta sobre una huerta, fertiliza.
 *                  si el jugador tiene fertilizante, pero no esta sobre una huerta, el fertilizante se desperdicia.
 */
void usar_fertilizante(juego_t* juego){
    if(jugador_tiene_fertilizante(*juego)  && jugador_sobre_huerta(*juego)){
            int indice_huerta;
            int indice_cultivo;
            obtener_indices_huerta_cultivo(*juego, &indice_huerta, &indice_cultivo);
            fertilizar(juego, indice_huerta);
            (*juego).jugador.tiene_fertilizante = false;
    } else{
        (*juego).jugador.tiene_fertilizante = false;
    }
}

/*
 * Precondiciones: Recibe la posicion de la plaga para eliminarla del vector objetos.
 * Postcondiciones: Elimina la plaga pegandole el siguiente elemento encima y reduciendo el tope.
 */
void eliminar_plaga_del_vector(juego_t* juego, int posicion_plaga){
    int tope = (*juego).tope_objetos;
    for(int i=posicion_plaga; i<tope-1; i++){
        (*juego).objetos[i] = (*juego).objetos[i+1];
    }
    (*juego).tope_objetos --;
}

/*
 * Precondiciones: recibe una estructura del juego valida y un puntero a un entero.
 * Postcondiciones: recorre el vector objetos en busca de una plaga y devuelve la possicion que ocupa dentro del vector.
 */
void obtener_posicion_plaga_en_objetos(juego_t juego, int* posicion_plaga){
    int tope = juego.tope_objetos;
    for(int i=0; i<tope; i++){
        if((juego.objetos[i].tipo == LETRA_PLAGA)){
            *posicion_plaga = i;
        }
    }
}

/*
 * Precondiciones: recibe un puntero a una estructura valida del juego y un indice que representa a una huerta valida.
 * Postcondiciones: fumiga la huerta, reestableciendo los valores. Descuenta un insecticida.
 */
void fumigar(juego_t* juego, int indice_huerta){
    (*juego).huertas[indice_huerta].plagado = false;
    (*juego).huertas[indice_huerta].movimientos_plagado = SIN_PLAGAS;
    (*juego).jugador.cant_insecticidas -= UN_INSECTICIDA;
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: si el jugador tiene insecticidas, esta sobre una huerta, hay una plaga ya iniciada y el huerto esta plagado, fumiga la huerta, eliminando la plaga.
 *                  si el jugador tiene insecticidas, pero no se cumple el resto de condiciones, el insecticida se desperdicia.
 */
void usar_insecticida(juego_t* juego){
    if((*juego).jugador.cant_insecticidas > SIN_INSECTICIDAS){
        int indice_huerta;
        int indice_cultivo;
        obtener_indices_huerta_cultivo(*juego, &indice_huerta, &indice_cultivo);
        if((jugador_sobre_huerta(*juego)) && hay_plaga(*juego) && huerto_plagado(*juego, indice_huerta)){
                fumigar(juego, indice_huerta);
                int posicion_plaga;
                obtener_posicion_plaga_en_objetos((*juego), &posicion_plaga);
                eliminar_plaga_del_vector(juego, posicion_plaga);
        } else{
            (*juego).jugador.cant_insecticidas -= UN_INSECTICIDA;
        }
    }
}

/*
 * Precondiciones: recibe la inicial de una verdura valida y un puntero a un entero,
 * Postcondiciones: evalua la verdura y asigna el tiempo de cosecha.
 */
void obtener_tiempo_cosecha(char verdura, int* tiempo_cosecha){
    switch(verdura){
        case ZANAHORIA:
            *tiempo_cosecha = TIEMPO_COSECHA_ZANAHORIA;
            break; 
        case TOMATE:
            *tiempo_cosecha = TIEMPO_COSECHA_TOMATE;
            break;
        case LECHUGA:
            *tiempo_cosecha = TIEMPO_COSECHA_LECHUGA;
            break;
        case BROCOLI:
            *tiempo_cosecha = TIEMPO_COSECHA_BROCOLI;
            break;
    }
}

/*
 * Precondiciones: recibe la inicial de una verdura y un puntero a un entero.
 * Postcondiciones: evalua y asigna el tiempo de vida segun la verdura.
 */
void obtener_tiempo_vida_verdura(char verdura, int* tiempo_vida){
    switch(verdura){
        case ZANAHORIA:
            *tiempo_vida = VIDA_ZANAHORIA;
            break; 
        case TOMATE:
            *tiempo_vida = VIDA_TOMATE;
            break;
        case LECHUGA:
            *tiempo_vida = VIDA_LECHUGA;
            break;
        case BROCOLI:
            *tiempo_vida = VIDA_BROCOLI;
            break;
    }   
}

/*
 * Precondiciones: recibe una estructura del juego valida, u cultivo valido y un tiempo de cosecha.
 * Postcondiciones: evalua la diferencia entre la cantidad de movimientos totales y el de la variable movimiento plantado, que es el numero de movimientos que tenia el jugador cuando planto.
 *                  si la diferencia es igual o mayor al tiempo de cosecha, devuelve true. Caso contrario, devuelve false.
 */
bool listo_para_cosechar(juego_t juego, cultivo_t cultivo, int tiempo_cosecha, int tiempo_vida){
    int movimientos_totales = juego.movimientos;
    return((movimientos_totales - cultivo.movimiento_plantado >= tiempo_cosecha) && (movimientos_totales - cultivo.movimiento_plantado <= tiempo_vida));
}

/*
 * Precondiciones: recibe un puntero a una estructura valida y la inicial de una verdura valida.
 * Postcondiciones: agrega la verdura a la canasta del jugador y modifica su tope.
 */
void agregar_verdura_canasta(juego_t* juego, char verdura){
    int tope = (*juego).jugador.tope_canasta;
    if(tope<MAX_CANASTA){
        (*juego).jugador.canasta[tope] = verdura;
        (*juego).jugador.tope_canasta ++;
    }
}

/*
 * Precondiciones: recibe un puntero a una estructura valida del juego, un indice de una huerta valida y un indice de un cultivo valido.
 * Postcondiciones: quita la verdura del huerto, reestableciendo algunos datos del huerto.
 */
void quitar_verdura_huerto(juego_t* juego, int indice_huerta, int indice_cultivo){
    (*juego).huertas[indice_huerta].cultivos[indice_cultivo].movimiento_plantado = SIN_PLANTAS;
    (*juego).huertas[indice_huerta].cultivos[indice_cultivo].tipo = LETRA_CULTIVO_VACIO;
    (*juego).huertas[indice_huerta].cultivos[indice_cultivo].ocupado = false;
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida, un indice de una huerta valida y un indice de un cultivo valido.
 * Postcondiciones: quita las verduras del huerto y las agrega a la canasta.
 */
void recolectar(juego_t* juego, char verdura, int indice_huerta, int indice_cultivo){
    agregar_verdura_canasta(juego, verdura);
    quitar_verdura_huerto(juego, indice_huerta, indice_cultivo);

}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: obtiene los indices de la huerta y el vultivo donde se encuentra el jugador, y si la verdura en ese lugar esta lista para cosechar, la recolecta.
 */
void cosechar_verdura(juego_t* juego){
    int indice_huerta;
    int indice_cultivo;
    obtener_indices_huerta_cultivo(*juego, &indice_huerta, &indice_cultivo);
    cultivo_t cultivo = (*juego).huertas[indice_huerta].cultivos[indice_cultivo];
    char verdura = cultivo.tipo;
    int tiempo_cosecha;
    int tiempo_vida;
    obtener_tiempo_cosecha(verdura, &tiempo_cosecha);
    obtener_tiempo_vida_verdura(verdura, &tiempo_vida);
    if(listo_para_cosechar(*juego, cultivo, tiempo_cosecha, tiempo_vida)){
        recolectar(juego, verdura, indice_huerta, indice_cultivo);
    }
}

/*
 * Precondiciones: recibe una estructura del juego valida.
 * Postcondiciones: evalua la posicion del jugador, recorriendo el vector de objetos y comparando con la posicion de las espinas.
 *                  si el jugador esta parado sobre una espina, devuelve true. Si no, false.
 */
bool jugador_sobre_espinas(juego_t juego){
    bool valor = false;
    coordenada_t pos_jugador = juego.jugador.posicion;
    for(int i=0; i<juego.tope_objetos; i++){
        if(juego.objetos[i].tipo == LETRA_ESPINAS){
            if((juego.objetos[i].posicion.fila == pos_jugador.fila) && (juego.objetos[i].posicion.columna == pos_jugador.columna)){
                valor = true;
            }
        }
    }
    return valor;
}

/*
 * Precondiciones: recibe una estructura del juego valida.
 * Postcondiciones: evalua la posicion del jugador, recorriendo el vector de objetos y comparando con la posicion del fertilizante.
 *                  si el jugador esta parado sobre un fertilizante, devuelve true. Si no, false.
 */
bool jugador_sobre_fertilizante(juego_t juego){
    bool valor = false;
    coordenada_t pos_jugador = juego.jugador.posicion;
    for(int i=0; i<juego.tope_objetos; i++){
        if(juego.objetos[i].tipo == LETRA_FERTILIZANTE){
            if((juego.objetos[i].posicion.fila == pos_jugador.fila) && (juego.objetos[i].posicion.columna == pos_jugador.columna)){
                valor = true;
            }
        }
    }
    return valor;
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: agarra el fertilizante, alterando la variable booleana que indica si lo tiene o no.
 */
void agarrar_fertilizante(juego_t* juego){
    (*juego).jugador.tiene_fertilizante = true;
}

/*
 * Precondiciones: recibe una estructura del juego valida.
 * Postcondiciones: evalua la distancia del jugador, respecto al deposito. Si la distancia es menor o igual a la minima, devuelve true. Si no, false.
 */
bool jugador_cerca_deposito(juego_t juego){
    coordenada_t jugador = juego.jugador.posicion;
    coordenada_t deposito = juego.deposito;
    bool valor = false;
    int distancia = distancia_manhattan(jugador, deposito);
    if(distancia <= DISTANCIA_MANHATTAN_MINIMA_DEPOSITO){
        valor = true;
    }
    return valor;
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: recorre la canaste y umenta la cantidad de monedas del jugador, segun la verdura.
 */
void aumentar_monedas_por_verduras(juego_t* juego){
    int tope = (*juego).jugador.tope_canasta;
    for(int i=0; i<tope; i++){
        if((*juego).jugador.canasta[i]==TOMATE){
            (*juego).jugador.cant_monedas += GANANCIA_TOMATE;
        } else if((*juego).jugador.canasta[i]==LECHUGA){
            (*juego).jugador.cant_monedas += GANANCIA_LECHUGA;
        } else if((*juego).jugador.canasta[i]==ZANAHORIA){
            (*juego).jugador.cant_monedas += GANANCIA_ZANAHORIA;
        } else if((*juego).jugador.canasta[i]==BROCOLI){
            (*juego).jugador.cant_monedas += GANANCIA_BROCOLI;
        }
    }
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: recorre la canasta y le asigna un espacio vacio. Reinicia su tope.
 */
void vaciar_canasta(juego_t* juego){
    int tope = (*juego).jugador.tope_canasta;
    for(int i=0; i<tope; i++){
        (*juego).jugador.canasta[i]=' ';
    }
    (*juego).jugador.tope_canasta = INICIAL_CANASTA;
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: intercambia las verduras por monedas.
 */
void cambiar_verduras_por_monedas(juego_t* juego){
    aumentar_monedas_por_verduras(juego);
    vaciar_canasta(juego);
}

/*
 * Precondiciones: recibe una estructura del juego valida y una coordenada valida.
 * Postcondiciones: evalua la coordenada recibida, si coincide con el cultivo de alguna huerta, devuelve true. Si no, false.
 */
bool es_huerta(juego_t juego, coordenada_t coordenada){
    bool valor = false;
    for(int i=0; i<MAX_HUERTA; i++){
        for(int j=0; j<MAX_PLANTAS; j++){
            if((juego.huertas[i].cultivos[j].posicion.fila == coordenada.fila) && (juego.huertas[i].cultivos[j].posicion.columna == coordenada.columna)){
                valor = true;
            }
        }
    }
    return valor;
}


/*
 * Precondiciones: recibe una estructura del juego valida.
 * Postcondiciones: genera una coordenada aleatoria para la plaga. Siempre en un espacio vacio o sobre una huerta.
 */
coordenada_t posicion_aleatoria_plaga(juego_t juego){
    coordenada_t coordenada_plaga = posicion_aleatoria_elementos();
    while(se_sobrepone_con_objetos(juego, coordenada_plaga) && se_sobrepone_con_jugador(juego, coordenada_plaga) && se_sobrepone_con_deposito(juego, coordenada_plaga)){
        coordenada_plaga = posicion_aleatoria_elementos();
    }
    return coordenada_plaga;
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida y una coordenada valida.
 * Postcondiciones: inicia el objeto plaga, dentro del vector objetos y le asigna sus atributos.
 */
void iniciar_plaga(juego_t* juego, coordenada_t coordenada_plaga){
    (*juego).objetos[(*juego).tope_objetos].tipo = LETRA_PLAGA;
    (*juego).objetos[(*juego).tope_objetos].posicion = coordenada_plaga;
    (*juego).tope_objetos ++;
}

/*
 * Precondiciones: recibe una estructura del juego valida, una coordenada valida y un puntero a un entero.
 * Postcondiciones: recorre las huertas y los cultivos. Devuelve el indice de la huerta donde cayo la plaga.
 */
void obtener_indices_huerta(juego_t juego, coordenada_t coordenada_plaga, int* indice_huerta){
    for(int i=0; i<MAX_HUERTA; i++){
        for(int j=0; j<MAX_PLANTAS; j++){
            if((juego.huertas[i].cultivos[j].posicion.fila == coordenada_plaga.fila) && (juego.huertas[i].cultivos[j].posicion.columna == coordenada_plaga.columna)){
                *indice_huerta = i;
            }
        }
    }
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida y el indice de una huerta valida.
 * Postcondiciones: contamina la huerta, estableciendo algunos atributos.
 */
void plagar_huerta(juego_t* juego, int indice_huerta){
    (*juego).huertas[indice_huerta].movimientos_plagado = (*juego).movimientos;
    (*juego).huertas[indice_huerta].plagado = true;
}

void eliminar_plaga_de_huerta(juego_t* juego){
    for(int i=0; i < MAX_HUERTA; i++){
        if(huerto_plagado(*juego, i)){
            (*juego).huertas[i].movimientos_plagado = SIN_PLAGAS;
            (*juego).huertas[i].plagado = false;
        }
    }
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: si hay una plaga en el juego la elimina e inicializa otra.
 */
void inicializar_plaga(juego_t* juego){
    if(hay_plaga(*juego)){
        int posicion_de_plaga_en_vector;
        obtener_posicion_plaga_en_objetos((*juego), &posicion_de_plaga_en_vector);
        eliminar_plaga_del_vector(juego, posicion_de_plaga_en_vector);
        eliminar_plaga_de_huerta(juego);
    }
    coordenada_t coordenada_plaga = posicion_aleatoria_plaga(*juego);
    iniciar_plaga(juego, coordenada_plaga);
    if(es_huerta(*juego, coordenada_plaga)){
        int indice_huerta;
        obtener_indices_huerta(*juego, coordenada_plaga, &indice_huerta);
        plagar_huerta(juego, indice_huerta);
    }
}

/*
 * Precondiciones: recibe una estructura del juego valida.
 * Postcondiciones: evalua si ya pasaron la cantidad de movimientos necesarios para generar una nueva plaga. 
 *                  devuelve true si es asi, si no, false.
 */
bool hay_que_iniciar_plaga(juego_t juego){
    bool valor=false;
    int movimientos_totales = juego.movimientos;
    if(movimientos_totales%CANTIDAD_MOVIMIENTOS_PLAGA == CERO_P){
        valor=true;
    }
    return valor;
}

/*
 * Precondiciones: recibe una estructura del juego valida.
 * Postcondiciones: recorre el vector de objetos buscando un fertilizante.
 *                  devuelve true si lo encuentra, si no, false.
 */
bool hay_fertilizante(juego_t juego){
    bool valor = false;
    for(int i=0; i<juego.tope_objetos; i++){
        if(juego.objetos[i].tipo == LETRA_FERTILIZANTE){
            valor = true;
        }
    }
    return valor;
}

/*
 * Precondiciones: recibe una estructura del juego valida y un puntero a un entero.
 * Postcondiciones: recorre el vector de objetos, comparando la posicion de sus elementos con la posicion del jugador. 
 *                  si alguna posicion coincide, devuelve la ubicacion del elemento dentro del vector.
 */
void obtener_posicion_fertilizante(juego_t juego, int* posicion_fertilizante){
    coordenada_t pos_jugador = juego.jugador.posicion;
    int tope = juego.tope_objetos;
    for(int i=0; i<tope; i++){
        if((juego.objetos[i].posicion.fila == pos_jugador.fila) && (juego.objetos[i].posicion.columna == pos_jugador.columna)){
            *posicion_fertilizante = i;
        }
    }
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida y la posicion del fertilizante para eliminarlo.
 * Postcondiciones: Elimina el fertilizante pegandole el siguiente elemento encima y reduciendo el tope.
 */
void eliminar_fertilizante(juego_t* juego, int posicion_fertilizante){
    for(int i=posicion_fertilizante; i<(*juego).tope_objetos-1; i++){
        (*juego).objetos[i] = (*juego).objetos[i+1];
    }
    (*juego).tope_objetos --;
}

/*
 * Precondiciones: recibe una estructura del juego valida.
 * Postcondiciones: genera una posicion aleatoria y valida para el fertilizante.
 */
coordenada_t posicion_aleatoria_fertilizante(juego_t juego){
    coordenada_t coordenada_fertilizante = posicion_aleatoria_elementos();
    while(es_lugar_ocupado(juego, coordenada_fertilizante)){
        coordenada_fertilizante = posicion_aleatoria_elementos();
    }
    return coordenada_fertilizante;
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: inicia el objeto fertilizante con sus atributos.
 */
void iniciar_fertilizante(juego_t* juego, coordenada_t coordenada_fertilizante){
    (*juego).objetos[(*juego).tope_objetos].tipo = LETRA_FERTILIZANTE;
    (*juego).objetos[(*juego).tope_objetos].posicion = coordenada_fertilizante;
    (*juego).tope_objetos ++;
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: si todavia no se alcanzo el maximo de objetos posibles, se inicializa un fertilizante.
 */
void inicializar_fertilizante(juego_t* juego){
    coordenada_t coordenada_fertilizante = posicion_aleatoria_fertilizante(*juego);
    if((*juego).tope_objetos <MAX_OBJETOS){
        iniciar_fertilizante(juego, coordenada_fertilizante);
    }
}

/*
 * Precondiciones: recibe una estructura del juego valida.
 * Postcondiciones: evalua si ya pasaron los movimientos necesarios. Devuelve true si es asi, si no, false.
 */
bool hay_que_iniciar_fertilizante(juego_t juego){
    bool valor=false;
    int movimientos_totales = juego.movimientos;
    if(movimientos_totales%CANTIDAD_MOVIMIENTOS_FERTILIZANTE == CERO_F){
        valor=true;
    }
    return valor;
}


/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: recorre las huertas y sus cultivos, si ya se supero el tiempo de vida de las plantas, las pudre.
 */
void podrir_verduras_por_tiempo_de_vida(juego_t* juego){
    for(int i=0; i<MAX_HUERTA; i++){
        for(int j=0; j<MAX_PLANTAS; j++){
            if((*juego).huertas[i].cultivos[j].ocupado==true){
                char verdura = (*juego).huertas[i].cultivos[j].tipo;
                int tiempo_vida;
                obtener_tiempo_vida_verdura(verdura, &tiempo_vida);
                if((*juego).movimientos - (*juego).huertas[i].cultivos[j].movimiento_plantado > tiempo_vida){
                    (*juego).huertas[i].cultivos[j].movimiento_plantado = SIN_PLANTAS;
                    (*juego).huertas[i].cultivos[j].tipo = LETRA_CULTIVO_VACIO;
                    (*juego).huertas[i].cultivos[j].ocupado = false;
                }
            }
        }
    }
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: recorre las huertas y se detiene en las que estan plagadas, si ya pasaron los movimientos necesarios y no se fumigo, se pudren las verduras.
 */
void podrir_huertas_por_plaga(juego_t* juego){
    for(int i=0; i<MAX_HUERTA; i++){
        if(((*juego).huertas[i].plagado == true) && ((*juego).movimientos - (*juego).huertas[i].movimientos_plagado == TIEMPO_PODRIR_PLAGA)){
            for(int j=0; j < (*juego).huertas[i].tope_cultivos; j++){
                (*juego).huertas[i].cultivos[j].movimiento_plantado = SIN_PLANTAS;
                (*juego).huertas[i].cultivos[j].tipo = LETRA_CULTIVO_VACIO;
                (*juego).huertas[i].cultivos[j].ocupado = false;
            }
            eliminar_plaga_de_huerta(juego);
        }
    }
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: pudre las verduras.
 */
void podrir_verduras(juego_t* juego){
    podrir_verduras_por_tiempo_de_vida(juego);
    podrir_huertas_por_plaga(juego);
}

/*
 * Precondiciones: Recibe una estructura del juevo valida y la matriz del terreno.
 * Postcondiciones: Incluye la posicion del deposito en el terreno.
 */
void incluir_posicion_deposito(juego_t juego, char tablero[TAMANIO][TAMANIO]){
    tablero[juego.deposito.fila][juego.deposito.columna] = LETRA_DEPOSITO;
}

/*
 * Precondiciones: Recibe una estructura del juevo valida y la matriz del terreno.
 * Postcondiciones: Actualiza la posicion del jugador en el terreno.
 */
void actualizar_posicion_jugador_en_terreno(juego_t juego, char tablero[TAMANIO][TAMANIO]){
    tablero[juego.jugador.posicion.fila][juego.jugador.posicion.columna] = LETRA_BLANCANIEVES;
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: actualiza la posicion de las huertas en el terreno.
 */
void actualizar_huertas_en_terreno(juego_t juego, char tablero[TAMANIO][TAMANIO]){
    for(int i=0; i<MAX_HUERTA; i++){
        for(int j=0; j<juego.huertas[i].tope_cultivos; j++){
            tablero[juego.huertas[i].cultivos[j].posicion.fila][juego.huertas[i].cultivos[j].posicion.columna] = juego.huertas[i].cultivos[j].tipo;
        }
    }
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: actualiza la posicion de los objetos en el terreno.
 */
void actualizar_objetos_en_terreno(juego_t juego, char tablero[TAMANIO][TAMANIO]){
    for(int i=0; i<juego.tope_objetos; i++){
        tablero[juego.objetos[i].posicion.fila][juego.objetos[i].posicion.columna] = juego.objetos[i].tipo;
    }
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida.
 * Postcondiciones: se actualiza la posicion de todos los elementos que sufrieron cambios.
 */
void actualizar_el_tablero(juego_t juego, char tablero[TAMANIO][TAMANIO]){
    actualizar_huertas_en_terreno(juego, tablero);
    actualizar_objetos_en_terreno(juego, tablero);
    actualizar_posicion_jugador_en_terreno(juego, tablero);
    incluir_posicion_deposito(juego, tablero);
}

/*
 * Precondiciones: recibe un puntero a una estructura del juego valida y la tecla ingresada, previamente validada.
 * Postcondiciones: evalua la posicion del jugador en el terreno y realiza las acciones consecuentes.
 *                  inicia plagas, fertilizantes y pudre verduras si es necesario. Finalmente actualiza el terreno.
 */
void actualizar_juego(juego_t* juego, char tecla){
    if(jugador_sobre_cultivo_plantado(*juego)){
        cosechar_verdura(juego);
    } else if(jugador_sobre_espinas(*juego)){
        descontar_monedas(juego, MONEDAS_POR_PISAR_ESPINAS);
    } else if(jugador_sobre_fertilizante(*juego)){
        if(!jugador_tiene_fertilizante(*juego)){
            agarrar_fertilizante(juego);
            int posicion_fertilizante;
            obtener_posicion_fertilizante((*juego), &posicion_fertilizante);
            eliminar_fertilizante(juego, posicion_fertilizante);
        }
    }
    podrir_verduras(juego);
    if(jugador_cerca_deposito(*juego)){
        cambiar_verduras_por_monedas(juego);
    }
    if(hay_que_iniciar_plaga(*juego)){
        inicializar_plaga(juego);
    }
    if(hay_que_iniciar_fertilizante(*juego)){
        inicializar_fertilizante(juego);
    }
}







void inicializar_juego(juego_t* juego, char enanito){
    inicializar_huertas(juego);
    inicializar_obstaculos(juego);
    inicializar_personaje(juego, enanito);
    inicializar_deposito(juego);
    inicializar_contador_movimientos(juego);
}

void realizar_jugada(juego_t* juego, char accion){
    switch (accion) {
        case ARRIBA:
        case ABAJO:
        case IZQUIERDA:
        case DERECHA:
            realizar_movimiento(juego, accion);
            break;
        case ZANAHORIA:
        case TOMATE:
        case LECHUGA:
        case BROCOLI:
            sembrar_cultivo(juego, accion);
            break;
        case FERTILIZANTE:
            usar_fertilizante(juego);
            break;
        case INSECTICIDA:
            usar_insecticida(juego);
            break;
    }
    actualizar_juego(juego, accion); 
}

void imprimir_terreno(juego_t juego) {
    char tablero_juego[TAMANIO][TAMANIO];
    inicializar_matriz_vacia(tablero_juego);
    actualizar_el_tablero(juego, tablero_juego);
    printf("\n");
    for (int fila = FILA_INICIAL_CERO; fila < TAMANIO; fila++) {
        printf("+");
        for (int columna = COLUMNA_INICIAL_CERO; columna < TAMANIO; columna++) {
            printf("---+");
        }
        printf("\n");
        printf("|");
        for (int columna = COLUMNA_INICIAL_CERO; columna < TAMANIO; columna++) {
            printf(" %c |", tablero_juego[fila][columna]);
        }
        printf("\n");
    }
    printf("+");
    for (int columna = COLUMNA_INICIAL_CERO; columna < TAMANIO; columna++) {
        printf("---+");
    }
    printf("\n");
}



int estado_juego(juego_t juego){
    int valor = EN_CURSO;
    if(juego.jugador.cant_monedas >= MAX_MONEDAS){
        valor = GANADO;
    } else if(juego.jugador.cant_monedas <= MIN_MONEDAS){
        valor = PERDIDO;
    }
    return valor;
}
