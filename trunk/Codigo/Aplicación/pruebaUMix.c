#include	<stdio.h>
#include 	"DBAccess.h"
#include	"ProgGPS.h"

void main(){

	double yacEasting;
	double yacNorthing;
	int flag = 1;

	Point_Data posActual;
	posActual = getWGS84Pos();
	printf("Fecha y hora: %s\n", posActual.datetime);
	printf("En WGS84: La latitud en grados es %f y la longitud en grados es %f\n", posActual.latitude, posActual.longitude);
	double WGS84latRad = toRad(posActual.latitude);
	double WGS84longRad = toRad(posActual.longitude);
	printf("En WGS84: La latitud en radianes es %f y la longitud en radianes es %f\n", WGS84latRad, WGS84longRad);
	convert(WGS84latRad, WGS84longRad, &yacNorthing, &yacEasting);
	printf("En Yacare: El easting (Coord en X) es %f y el northing (Coord en Y) es %f\n", yacEasting, yacNorthing);

	Point_Data testPoint;
	testPoint = loadPointStruct(posActual.latitude, posActual.longitude, posActual.datetime, flag);
	storePoint(testPoint);
	Point_Data_Array points;
	points = getSurvivorPoints();
	int deleted = deleteAllPoints();
	printf("Se borraron %d puntos\n", deleted);
	recorrerArrayPuntos(points);


}




