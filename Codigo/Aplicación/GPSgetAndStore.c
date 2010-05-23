#include "ProgGPS.h"
#include "DBAccess.h"

int main(){
	  double yacEasting;
        double yacNorthing;

        Point_Data posActual;
        posActual = getWGS84Pos();
	  storePoint(posActual);
        printf("%s\n", posActual.datetime);
        printf("%f %f\n", posActual.latitude, posActual.longitude);
        double WGS84latRad = toRad(posActual.latitude);
        double WGS84longRad = toRad(posActual.longitude);
        convert(WGS84latRad, WGS84longRad, &yacNorthing, &yacEasting);
        printf("%f %f\n", yacEasting, yacNorthing);
 
}