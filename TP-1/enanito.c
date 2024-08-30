#include <stdio.h>
#include "calculo_enanito.h"

void mostrar_enanito(char incial){
	if(incial == INICIAL_GRUNON ){
		printf("-GRUÑÓN-");
	} else if (incial== INICIAL_DORMILON){
		printf("-DORMILÓN-");
	} else if (incial== INICIAL_SABIO){
		printf("-SABIO-");
	} else {
		printf("-FELIZ-");
	}
}

int main (){
	char incial_enanito;
	calcular_enanito(&incial_enanito);
	mostrar_enanito(incial_enanito);
	return 0;
}
