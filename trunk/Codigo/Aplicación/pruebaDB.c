#include	<stdio.h>
#include 	"DBAccess.h"


void main(){

	double lat = -33;
	double lon = -57.4;
	char *fechahora = "2010-02-20 21:00:00";
	int flag = 1;
	
	Point_Data testPoint;
	testPoint = loadPointStruct(lat, lon, fechahora, flag);
	storePoint(testPoint);
	Point_Data_Array points;
	points = getSurvivorPoints();
	int deleted = deleteAllPoints();
	printf("Se borraron %d puntos\n", deleted);
	recorrerArrayPuntos(points);

}


