#ifndef CALCULO_ENANITO_H
#define CALCULO_ENANITO_H

#define INICIAL_GRUNON 'G'
#define INICIAL_DORMILON 'D'
#define INICIAL_SABIO 'S'
#define INICIAL_FELIZ 'F'

/*
Pre: -
Post: Se le harán preguntas al usuario y en base a sus respuestas se le asignará un enanito. El
enanito asignado será devuelto por referencia en el parámetro inicial_enanito que va a contenr
su inicial.
- G: Gruñón
- D: Dormilón
- S: Sabio
- F: Feliz
*/
 void calcular_enanito(char* inicial_enanito);

 #endif 