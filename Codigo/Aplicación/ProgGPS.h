#ifndef _ProgGPS_H
#define _ProgGPS_H



extern int errno;

#include        <string.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <sys/time.h>
#include        <errno.h>


/*Estructura que contiene los datos de un punto */
typedef struct Point_Data_Structure
{
	double  latitude;
	double  longitude;
	char	*datetime;
	int	found;
} Point_Data;


/* Prototipos de funciones */
int stablish();
Point_Data getWGS84Pos(int gpsd);
void closeCon(int gpsd);
/*La funcion de conversion recibe Latitud y Longitud en radianes. El receptor GPS
* entrega las coordenadas en grados. Por lo tanto, es necesario hacer la conversion entre grados y radianes. */
double toRad(double deg);
/*Convierte las coordenadas en el Datum WGS84 al Datum Yacare*/ 
void convert(double lat, double lon, double *northing, double *easting);
/*Imprime en pantalla informacion de fecha y hora, latitud y longitud en Datum WGS84 y Easting y Northing en Datum Yacare. Ademas, almacena la info en la BD.*/
void getInfo();

#endif
