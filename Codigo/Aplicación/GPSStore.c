#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "ProgGPS.h"
#include "DBAccess.h"

char *flagFile = "/home/flagIR";
char *errorFile = "/home/error";
int gpsd;

void sigfun(int sig);

int main(){
	gpsd = stablish();
	(void)signal(SIGINT, sigfun);
	while(1){
		printf("Entrando en while\n");
		double yacEasting;
      		double yacNorthing;
	      	Point_Data posActual;
        	posActual = getWGS84Pos(gpsd);
		
		printf("Fecha y hora en objeto: %s\n", posActual.datetime);		
		
		char *msj; 
		if(posActual.found == -1){
			msj = "El GPS no tiene lectura de posicion.\n";
		}else if(posActual.found == -2){
			msj = "El GPS no tiene datetime.\n";
		}else{
			printf("Fecha y hora en objeto: %s\n", posActual.datetime);
			msj = "OK.\n";
			int flagIR;
			flagIR = getFlagIR();
			posActual.found = flagIR;
		 	printf("Fecha y hora en objeto: %s\n", posActual.datetime);	
//			printf("Latitud: %f Longitud: %f Datetime: %s FlagIR: %d\n", posActual.latitude, posActual.longitude, posActual.datetime, posActual.found);
			printf("Fecha y hora en objeto: %s\n", posActual.datetime);
			storePoint(posActual);
		}
	
		printf("%s\n", msj);	
		FILE *er;
		er = fopen(errorFile, "w");
		if(er == NULL){
			fprintf(stderr, "Error al abrir el archivo %s: %s(%d)\n", errorFile, strerror(errno), errno);
      			exit(1);
		}else{
			fputs(msj, er);
			fclose(er);
		}

		printf("Durmiendo\n");
		sleep(5);
	}

}

void sigfun(int sig){
       close(gpsd);
	printf("Socket cerrado\n");
	(void)signal(SIGINT, SIG_DFL);
	 
}

int getFlagIR(){

	FILE *fp;
	fp = fopen(flagFile, "r+");
	int read;
	int cero = '0';
	if(fp == NULL){
		fprintf(stderr, "Error al abrir el archivo %s: %s(%d)\n", flagFile, strerror(errno), errno);
      		exit(1);
	}else{
		read = fgetc(fp);	
//		printf("%c\n", read);
		ungetc(read, fp);
		fputc(cero, fp);
		fputs("\n", fp);
		fclose(fp);
		if(read == '1'){		
			return 1;
		}else{
			return 0;
		}
		
	}

}


