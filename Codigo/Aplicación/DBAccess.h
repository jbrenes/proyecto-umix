#ifndef _DBAccess_H_
#define _DBAccess_H_ 

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "ProgGPS.h"



/*Estructura que contiene un array de puntos y la cantidad de puntos en el array. */
typedef struct	Point_Data_Structure_Array
{
	Point_Data *pointsArray;
	size_t length;
} Point_Data_Array;	




/*Prototipos de funciones */
Point_Data_Array getSurvivorPoints(); /*Devuelve un array de puntos en los que se detectaron sobrevivientes. */
void storePoint(Point_Data point);	/* Almacena un punto en la base de datos. */
Point_Data loadPointStruct(double latitude, double longitude, char *datetime, int found); /* Carga la estructura Point_Data con los datos que se pasan. */
int deleteAllPoints(); /* Borra todos los puntos de la base de datos. */
void recorrerArrayPuntos(Point_Data_Array points); /*Recorre el array de puntos, imprimiendo el contenido. Este metodo es solo para debug. */

#endif
