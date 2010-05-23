
#define 	PI              3.14159265358979323e0
#define 	MOLODENSKY_MAX  (89.75 * PI / 180.0) /* Polar limit */

#define		WGS84_a 	6378137.0   /* Semi-eje mayor de la elipsoide WGS84 en metros */
#define		WGS84_f 	1/298.257223563   /* Achatamiento de la elipsoide WGS84 */

/* Parametros de la elipsoide Internacional 1924 (Utilizada por el Datum yacare.) */
#define		aIN24 		6378388.000         /* Semi-eje mayor de la elipsoide Internacional 1924 en metros  */
#define		daIN24 		WGS84_a - aIN24       	/* Diferencia en los semi-ejes mayores */
#define		fIN24 		1/297.000000000     /* Achatamiento de la elipsoide Internacional 1924 */
#define		dfIN24 		WGS84_f - fIN24        /* Diferencia en los achatamientos */

//SGM dx = -154 dy = 162 dz = 46
//Geotrans dx = -155 dy = 171 dz = 37

#define		dxIN24 		-154
#define		dyIN24		162
#define		dzIN24		46


/* Parametros de la proyeccion Mercator Transversa (usada por el Datum Yacare) */
#define		originLat 	0.0
#define		centralMeridian -0.973893722
#define		falseEasting 	500000.0
#define		falseNorthing 	4002288.0
#define		scaleFactor 	1.0
#define 	MAX_LAT         ((PI * 89.99)/180.0)    /* 89.99 degrees in radians */
#define 	MAX_DELTA_LONG  ((PI * 90)/180.0)       /* 90 degrees in radians */
#define 	SPHTMD(Latitude) ((double) (TranMerc_ap * Latitude \
 - TranMerc_bp * sin(2.e0 * Latitude) + TranMerc_cp * sin(4.e0 * Latitude) \
 - TranMerc_dp * sin(6.e0 * Latitude) + TranMerc_ep * sin(8.e0 * Latitude) ) )
#define 	SPHSN(Latitude) ((double) (TranMerc_a / sqrt( 1.e0 - TranMerc_es * pow(sin(Latitude), 2))))
#define 	SPHSR(Latitude) ((double) (TranMerc_a * (1.e0 - TranMerc_es) / pow(DENOM(Latitude), 3)))
#define 	DENOM(Latitude) ((double) (sqrt(1.e0 - TranMerc_es * pow(sin(Latitude),2))))


#include        <sys/types.h>
#include        <sys/socket.h>
#include        <netinet/in.h>
#include        <arpa/inet.h>
#include        <netdb.h>
#include        <unistd.h>
#include        <sys/select.h>
#include 	<math.h>
#include  	"ProgGPS.h"

/* Ellipsoid Parameters, default to WGS 84  */
static double TranMerc_a = 6378137.0;              /* Semi-major axis of ellipsoid in meters */
static double TranMerc_f = 1 / 298.257223563;      /* Flattening of ellipsoid  */
static double TranMerc_es = 0.0066943799901413800; /* Eccentricity (0.08181919084262188000) squared */
static double TranMerc_ebs = 0.0067394967565869;   /* Second Eccentricity squared */

/* Transverse_Mercator projection Parameters */
static double TranMerc_Origin_Lat = 0.0;           /* Latitude of origin in radians */
static double TranMerc_Origin_Long = 0.0;          /* Longitude of origin in radians */
static double TranMerc_False_Northing = 0.0;       /* False northing in meters */
static double TranMerc_False_Easting = 0.0;        /* False easting in meters */
static double TranMerc_Scale_Factor = 1.0;         /* Scale factor  */

/* Isometeric to geodetic latitude parameters, default to WGS 84 */
static double TranMerc_ap = 6367449.1458008;
static double TranMerc_bp = 16038.508696861;
static double TranMerc_cp = 16.832613334334;
static double TranMerc_dp = 0.021984404273757;
static double TranMerc_ep = 3.1148371319283e-005;

/* Maximum variance for easting and northing values for WGS 84. */
static double TranMerc_Delta_Easting = 40000000.0;
static double TranMerc_Delta_Northing = 40000000.0;

/* These state variables are for optimization purposes. The only function
 * that should modify them is Set_Tranverse_Mercator_Parameters.         */

typedef struct Geodetic_Tuple_Structure
{
  double  longitude;   /* radians */
  double  latitude;    /* radians */
} Geodetic_Tuple;

typedef struct Transverse_Mercator_Tuple_Structure
{
  double  easting;     /* meters */
  double  northing;    /* meters */
} Transverse_Mercator_Tuple;

int stablish(){
	const char *host = "127.0.0.1";
	const char *service = "2947";
	const char *protocol = "tcp";
	struct hostent *phe;
    	struct servent *pse;
	struct protoent *ppe;
	struct sockaddr_in sin;
	int gpsd, type, proto, one, n, w = 1;
	char descarte[1000];	/* Buffer para descartar los datos que no nos interesan. */

	memset((char *) &sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;

	if ((pse = getservbyname(service, protocol)))
		sin.sin_port = htons(ntohs((unsigned short) pse->s_port));
    	else if ((sin.sin_port = htons((unsigned short) atoi(service))) == 0){
		fprintf(stderr, "Can't get service entry, %s(%d)\n", strerror(errno), errno);
		exit(1);
	}

    	if ((phe = gethostbyname(host)))
		memcpy((char *) &sin.sin_addr, phe->h_addr, phe->h_length);
	#ifndef S_SPLINT_S
	else if ((sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE){
		fprintf(stderr, "Can't get host entry, %s(%d)\n", strerror(errno), errno);
		exit(1);
	}
	#endif /* S_SPLINT_S */

    	ppe = getprotobyname(protocol);
  	type = SOCK_STREAM;
	proto = (ppe) ? ppe->p_proto : IPPROTO_TCP;
    

    	if ((gpsd = socket(PF_INET, type, proto)) == -1){
		fprintf(stderr, "Can't create socket, %s(%d)\n", strerror(errno), errno);
		exit(1);
	}

    	if (setsockopt(gpsd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one))==-1) {
		(void)close(gpsd);
		fprintf(stderr, "Error SETSOCKOPT SO_REUSEADDR, %s(%d)\n", strerror(errno), errno);
		exit(1);
	
    	}

	


    	if (connect(gpsd, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		(void)close(gpsd);
		fprintf(stderr, "Fallo connect, %s(%d)\n", strerror(errno), errno);
             	exit(1);
    	}

	w = write(gpsd, "i\n", strlen("i\n"));  
	if ((ssize_t)strlen("i\n") != w) {
		(void)fprintf(stderr, "Error al enviar \"i\", %s(%d)\n", strerror(errno), errno);
		exit(1);
	}
	


	n = (int)read(gpsd, descarte, sizeof(descarte)-1); 

	printf("En descarte hay: %s\n", descarte);

	n = (int)read(gpsd, descarte, sizeof(descarte)-1); 

	printf("En descarte hay: %s\n", descarte);


	return gpsd;

}

Point_Data getWGS84Pos(int gpsd) {
	float trys;  /* Variable para intentar comunicarse con GPSD Daemon durante 4 segundos. */
        trys = 10.0;
	int n, w;      /*Cantidad de caracteres leídos */
        char bufP[50];        /*Buffer para los datos de posicion */
	char bufDT[50];		/* Buffer para la fecha y la hora */
	char descarte[1500];	/* Buffer para descartar los datos que no nos interesan. */
	Point_Data punto;
	punto.found = 0;


	while(trys >= 0.0){
                     
/* p  Returns the current position in the form "GPSD, P=%f %f"; numbers are in degrees, latitude first. */    
		w = write(gpsd, "p\n", strlen("p\n"));  
		if ((ssize_t)strlen("p\n") != w) {
			(void)fprintf(stderr, "Error al enviar comando, %s(%d)\n", strerror(errno), errno);
			exit(1);
		}
		
  
/* Recibe la salida del GPS Daemon. */
       	n = (int)read(gpsd, bufP, sizeof(bufP)-1);
       		
		printf("En bufP hay: %s\n", bufP);


		if (n < 0){
			fprintf(stderr, "Fallo al recibir respuesta de gpsd, %s(%d)\n", strerror(errno), errno);
                	exit(1);
        	}else{
        	        if(n == 0 || (strncmp(bufP,"GPSD,P=?",8))==0){ 
				(void)sleep(0.4); /* Esperar 0.4 segundos antes de volver a intentar */
        	                trys = trys - 0.4;
				printf("No hay fix, volviendo a intentar\n");
	       	 }else break;
                     
        	}
	}

	if((strncmp(bufP,"GPSD,P=?",8))==0){
		printf("El receptor no tiene una lectura de posicion\n");
		punto.found = -1;
	}else{
		const char delimiters[] = "GPSD ,=";    

		punto.latitude = strtod(strtok(bufP, delimiters),NULL);
		punto.longitude = strtod(strtok(NULL, delimiters),NULL);
      
	}

	
/* d Returns the UTC time in the ISO 8601 format, "D=yyyy-mm-ddThh:nmm:ss.ssZ". Digits of precision in the fractional-seconds part will vary and may be absent. Por ejemplo:  GPSD,D=2010-02-25T13:05:14.0Z */
	w = write(gpsd, "i\n", strlen("i\n"));  
	if ((ssize_t)strlen("i\n") != w) {
		(void)fprintf(stderr, "Error al enviar \"i\", %s(%d)\n", strerror(errno), errno);
		exit(1);
	}

	n = (int)read(gpsd, descarte, sizeof(descarte)-1); 

	printf("En descarte hay: %s\n", descarte);
	

	w = write(gpsd, "d\n", strlen("d\n"));  
	if ((ssize_t)strlen("d\n") != w) {
		(void)fprintf(stderr, "Error al enviar comando, %s(%d)\n", strerror(errno), errno);
		exit(1);
	}
		
  
/* Recibe la salida del GPS Daemon. */
	 n = (int)read(gpsd, bufDT, sizeof(bufDT)-1);       	
        printf("En bufDT hay: %s\n", bufDT);
		
	if (n < 0) {
		fprintf(stderr, "Fallo al recibir respuesta de gpsd, %s(%d)\n", strerror(errno), errno);
               	exit(1);
	}else if (n == 0){
                fprintf(stderr, "No se leyo nada\n");
                exit(1);
        }
	
	punto.datetime = "0000-00-00 00:00:00";	
	if((strncmp(bufDT,"GPSD,D=?",8))==0){
		printf("El receptor no tiene timestamp aun\n");
		punto.found = -2;
	}else{
		const char delimiters[] = "GPSD ,=T.:";
		char *fecha;    
		int hora;
		int horaUru;
		char *minutos;
		char *segundos;
		
		fecha = (char *)strtok(bufDT, delimiters);
		hora = strtod((char *)strtok(NULL, delimiters), NULL);
		horaUru = hora -3;
		minutos = (char *)strtok(NULL, delimiters);
		segundos = (char *)strtok(NULL, delimiters);
		char fechaHora[20];
		sprintf(fechaHora,"%s %d:%s:%s", fecha, horaUru, minutos, segundos);
		punto.datetime = fechaHora;
		printf("%s\n", punto.datetime);
      
	}

	return punto;
}

void closeCon(int gpsd){

	close(gpsd);   /* Cierra el socket */
}



/* A continuacion se encuentra el codigo necesario para convertir las coordenadas entre el Datum Yacare
*  y el Datum utilizado por GoogleEarth (WGS84).
*  Se adapto codigo obtenido de Geotrans, un producto de la agencia nacional de inteligencia geoespacial (NGA)
*  y del centro de investigacion y desarrollo del ejercito de los Estados Unidos, cuya licencia permite su
*  reutilizacion. */




double toRad(double deg){
	double rad;	
	rad = deg*2*PI/360;
 	while(rad >= PI){
		rad -= 2*PI;
	}
	while(rad < -PI){
		rad += 2*PI;
	}
	return rad;
}

void Convert_Geodetic_To_Transverse_Mercator (double Latitude,
                                              double Longitude,
                                              double *Easting,
                                              double *Northing)

{      /* BEGIN Convert_Geodetic_To_Transverse_Mercator */

  /*
   * The function Convert_Geodetic_To_Transverse_Mercator converts geodetic
   * (latitude and longitude) coordinates to Transverse Mercator projection
   * (easting and northing) coordinates, according to the current ellipsoid
   * and Transverse Mercator projection coordinates. 
   *
   *    Latitude      : Latitude in radians                         (input)
   *    Longitude     : Longitude in radians                        (input)
   *    Easting       : Easting/X in meters                         (output)
   *    Northing      : Northing/Y in meters                        (output)
   */

	double c;       /* Cosine of latitude                          */
  	double c2;
  	double c3;
  	double c5;
  	double c7;
  	double dlam;    /* Delta longitude - Difference in Longitude       */
  	double eta;     /* constant - TranMerc_ebs *c *c                   */
  	double eta2;
  	double eta3;
  	double eta4;
  	double s;       /* Sine of latitude                        */
  	double sn;      /* Radius of curvature in the prime vertical       */
  	double t;       /* Tangent of latitude                             */
  	double tan2;
  	double tan3;
  	double tan4;
  	double tan5;
  	double tan6;
  	double t1;      /* Term in coordinate conversion formula - GP to Y */
  	double t2;      /* Term in coordinate conversion formula - GP to Y */
  	double t3;      /* Term in coordinate conversion formula - GP to Y */
  	double t4;      /* Term in coordinate conversion formula - GP to Y */
  	double t5;      /* Term in coordinate conversion formula - GP to Y */
  	double t6;      /* Term in coordinate conversion formula - GP to Y */
  	double t7;      /* Term in coordinate conversion formula - GP to Y */
  	double t8;      /* Term in coordinate conversion formula - GP to Y */
  	double t9;      /* Term in coordinate conversion formula - GP to Y */
  	double tmd;     /* True Meridional distance                        */
  	double tmdo;    /* True Meridional distance for latitude of origin */
  	double temp_Origin;
  	double temp_Long;

   	 /* 
   	  *  Delta Longitude
   	  */
    	dlam = Longitude - TranMerc_Origin_Long;

   	if (dlam > PI)
      		dlam -= (2 * PI);
    	if (dlam < -PI)
      		dlam += (2 * PI);
    	if (fabs(dlam) < 2.e-10)
      		dlam = 0.0;

    	s = sin(Latitude);
    	c = cos(Latitude);
    	c2 = c * c;
    	c3 = c2 * c;
    	c5 = c3 * c2;
    	c7 = c5 * c2;
    	t = tan (Latitude);
    	tan2 = t * t;
    	tan3 = tan2 * t;
    	tan4 = tan3 * t;
    	tan5 = tan4 * t;
    	tan6 = tan5 * t;
    	eta = TranMerc_ebs * c2;
    	eta2 = eta * eta;
    	eta3 = eta2 * eta;
    	eta4 = eta3 * eta;

    	/* radius of curvature in prime vertical */
    	sn = SPHSN(Latitude);

    	/* True Meridianal Distances */
    	tmd = SPHTMD(Latitude);

    	/*  Origin  */
    	tmdo = SPHTMD(TranMerc_Origin_Lat);

   	 /* northing */
    	t1 = (tmd - tmdo) * TranMerc_Scale_Factor;
    	t2 = sn * s * c * TranMerc_Scale_Factor/ 2.e0;
    	t3 = sn * s * c3 * TranMerc_Scale_Factor * (5.e0 - tan2 + 9.e0 * eta 
                                                + 4.e0 * eta2) /24.e0; 

    	t4 = sn * s * c5 * TranMerc_Scale_Factor * (61.e0 - 58.e0 * tan2
                                                + tan4 + 270.e0 * eta - 330.e0 * tan2 * eta + 445.e0 * eta2
                                                + 324.e0 * eta3 -680.e0 * tan2 * eta2 + 88.e0 * eta4 
                                                -600.e0 * tan2 * eta3 - 192.e0 * tan2 * eta4) / 720.e0;

    	t5 = sn * s * c7 * TranMerc_Scale_Factor * (1385.e0 - 3111.e0 * 
                                                tan2 + 543.e0 * tan4 - tan6) / 40320.e0;

    	*Northing = TranMerc_False_Northing + t1 + pow(dlam,2.e0) * t2
                + pow(dlam,4.e0) * t3 + pow(dlam,6.e0) * t4
                + pow(dlam,8.e0) * t5; 

    	/* Easting */
    	t6 = sn * c * TranMerc_Scale_Factor;
    	t7 = sn * c3 * TranMerc_Scale_Factor * (1.e0 - tan2 + eta ) /6.e0;
    	t8 = sn * c5 * TranMerc_Scale_Factor * (5.e0 - 18.e0 * tan2 + tan4
                                            + 14.e0 * eta - 58.e0 * tan2 * eta + 13.e0 * eta2 + 4.e0 * eta3 
                                            - 64.e0 * tan2 * eta2 - 24.e0 * tan2 * eta3 )/ 120.e0;
    	t9 = sn * c7 * TranMerc_Scale_Factor * ( 61.e0 - 479.e0 * tan2
                                             + 179.e0 * tan4 - tan6 ) /5040.e0;

    	*Easting = TranMerc_False_Easting + dlam * t6 + pow(dlam,3.e0) * t7 
               + pow(dlam,5.e0) * t8 + pow(dlam,7.e0) * t9;

} /* END OF Convert_Geodetic_To_Transverse_Mercator */


void Set_Transverse_Mercator_Parameters(double a,
                                        double f,
                                        double Origin_Latitude,
                                        double Central_Meridian,
                                        double False_Easting,
                                        double False_Northing,
                                        double Scale_Factor)

{ /* BEGIN Set_Tranverse_Mercator_Parameters */
  /*
   * The function Set_Tranverse_Mercator_Parameters receives the ellipsoid
   * parameters and Tranverse Mercator projection parameters as inputs, and
   * sets the corresponding state variables.
   *
   *    a                 : Semi-major axis of ellipsoid, in meters    (input)
   *    f                 : Flattening of ellipsoid						         (input)
   *    Origin_Latitude   : Latitude in radians at the origin of the   (input)
   *                         projection
   *    Central_Meridian  : Longitude in radians at the center of the  (input)
   *                         projection
   *    False_Easting     : Easting/X at the center of the projection  (input)
   *    False_Northing    : Northing/Y at the center of the projection (input)
   *    Scale_Factor      : Projection scale factor                    (input) 
   */

	double tn;        /* True Meridianal distance constant  */
  	double tn2;
  	double tn3;
  	double tn4;
  	double tn5;
  	double dummy_northing;
  	double TranMerc_b; /* Semi-minor axis of ellipsoid, in meters */
  	double inv_f = 1 / f;

  	TranMerc_a = a;
  	TranMerc_f = f;
  	TranMerc_Origin_Lat = Origin_Latitude;
    	TranMerc_Origin_Long = Central_Meridian;
    	TranMerc_False_Northing = False_Northing;
    	TranMerc_False_Easting = False_Easting; 
    	TranMerc_Scale_Factor = Scale_Factor;

    	/* Eccentricity Squared */
   	TranMerc_es = 2 * TranMerc_f - TranMerc_f * TranMerc_f;
    	/* Second Eccentricity Squared */
    	TranMerc_ebs = (1 / (1 - TranMerc_es)) - 1;

    	TranMerc_b = TranMerc_a * (1 - TranMerc_f);    
    	/*True meridianal constants  */
    	tn = (TranMerc_a - TranMerc_b) / (TranMerc_a + TranMerc_b);
    	tn2 = tn * tn;
    	tn3 = tn2 * tn;
    	tn4 = tn3 * tn;
    	tn5 = tn4 * tn;

    	TranMerc_ap = TranMerc_a * (1.e0 - tn + 5.e0 * (tn2 - tn3)/4.e0
                                + 81.e0 * (tn4 - tn5)/64.e0 );
    	TranMerc_bp = 3.e0 * TranMerc_a * (tn - tn2 + 7.e0 * (tn3 - tn4)
                                       /8.e0 + 55.e0 * tn5/64.e0 )/2.e0;
    	TranMerc_cp = 15.e0 * TranMerc_a * (tn2 - tn3 + 3.e0 * (tn4 - tn5 )/4.e0) /16.0;
    	TranMerc_dp = 35.e0 * TranMerc_a * (tn3 - tn4 + 11.e0 * tn5 / 16.e0) / 48.e0;
    	TranMerc_ep = 315.e0 * TranMerc_a * (tn4 - tn5) / 512.e0;
    	Convert_Geodetic_To_Transverse_Mercator(MAX_LAT,
                                            MAX_DELTA_LONG + Central_Meridian,
                                            &TranMerc_Delta_Easting,
                                            &TranMerc_Delta_Northing);
    	Convert_Geodetic_To_Transverse_Mercator(0,
                                            MAX_DELTA_LONG + Central_Meridian,
                                            &TranMerc_Delta_Easting,
                                            &dummy_northing);
    	TranMerc_Delta_Northing++;
    	TranMerc_Delta_Easting++;


}  /* END of Set_Transverse_Mercator_Parameters  */


/* La funcion convert pasa las coordenadas entregadas por el receptor GPS (en el Datum WGS84)
* a coordenadas en el Datum Yacare. */

void convert(double lat, double lon, double *northing, double *easting)
{
  	
	Set_Transverse_Mercator_Parameters(aIN24, fIN24, originLat, centralMeridian, falseEasting, falseNorthing, scaleFactor);
                                                       
	Transverse_Mercator_Tuple coord;
        
	Convert_Geodetic_To_Transverse_Mercator(lat, lon, &(coord.easting), &(coord.northing));

	*northing = coord.northing;
	*easting = coord.easting;	
   

}



