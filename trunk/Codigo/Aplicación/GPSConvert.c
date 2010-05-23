#include "ProgGPS.h"
#include "DBAccess.h"

int main(int argc, char *argv[]){
	
	if(argc == 1){
		printf("Debe ingresar la latitud y la longitud como parametros.\n");
	        exit(1);
	}else{

		double yacEasting;
      		double yacNorthing;

		double WGS84latRad = toRad(strtod(argv[1], NULL));
	      	double WGS84longRad = toRad(strtod(argv[2], NULL));
		convert(WGS84latRad, WGS84longRad, &yacNorthing, &yacEasting);
	      	printf("%f %f\n", yacEasting, yacNorthing);
	}
 
}
