
#include "DBAccess.h"
#include "ProgGPS.h"


char server[40] = "localhost";
char *user = "UMixUser";
char *password = "umixpass";
char *database = "UMix";
char *DBfile = "/home/database";

void setServer(){
	FILE *fp;
	fp = fopen(DBfile, "r");
	if(fp == NULL){
		fprintf(stderr, "Error al abrir el archivo database: %s(%d)\n", strerror(errno), errno);
      		exit(1);
	}else{
		fscanf(fp, "%s", server);
		printf("%s\n", server);
		fclose(fp);
	}
}


Point_Data_Array getSurvivorPoints(){
	MYSQL *conn;
	MYSQL_RES *res;
   	MYSQL_ROW row;
	setServer();
   
   	conn = mysql_init(NULL);
   
   	/* Se conecta a la base de datos */
   	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
      		fprintf(stderr, "Error en connect: %s\n", mysql_error(conn));
      		exit(1);
   	}

   	/* Envia la sentencia SQL */
	char *query = "SELECT * FROM Puntos WHERE Encontro = 1";
	
   	if (mysql_real_query(conn, query, strlen(query))) {
      		fprintf(stderr, "Error al enviar consulta: %s\n", mysql_error(conn));
      		exit(1);
   	}

	/*Almacena el resultado. */
   	res = mysql_store_result(conn);
   

   	/* Ve cuantos resultados devolvio la consulta */
	int count;
	count = mysql_num_rows(res);

	Point_Data_Array pointsArrayStruct;
	pointsArrayStruct.length = count; 
	
	Point_Data *points;
	points = malloc(count*sizeof(Point_Data)); /* Reserva memoria para el array de puntos. */
	

	int i =0;	

	/* Almacena los resultados en el array. */

	printf("En los siguientes puntos se detectaron sobrevivientes:\n");
	while ((row = mysql_fetch_row(res)) != NULL){
		points[i].latitude = strtod(row[1], NULL);
		points[i].longitude = strtod(row[2], NULL);
		points[i].datetime = row[3];
		points[i].found = (int)strtol(row[4], NULL, 0);      		

		printf("Latitud: %s Longitud: %s Fecha y hora: %s Encontro: %s\n", row[1], row[2], row[3], row[4]);
	
		i++;
	}

	pointsArrayStruct.pointsArray = points;
	
	printf("Existen %d puntos en los que se detectaron sobrevivientes\n", i);
	



   	/* Libera la memoria utilizada para el resultado y cierra la conexion */
   	mysql_free_result(res);
   	mysql_close(conn);

	return pointsArrayStruct;
}


void storePoint(Point_Data point){
	MYSQL *conn;
	MYSQL_RES *res;
   	MYSQL_ROW row;
	setServer();
   
   	conn = mysql_init(NULL);
   
   	/* Se conecta a la base de datos */
   	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
      		fprintf(stderr, "Error en connect: %s\n", mysql_error(conn));
      		exit(1);
   	}


   	/* Envia la sentencia SQL */
	char insert[200];
	sprintf(insert, "INSERT INTO `UMix`.`Puntos` (`Identificador`, `Latitud`, `Longitud`, `Timestamp`, `Encontro`) VALUES (NULL, '%f', '%f', '%s', '%d')", point.latitude, point.longitude, point.datetime, point.found);
	printf("Fecha y hora en la consulta: %s\n", point.datetime);
	printf("%s\n", insert);
	if (mysql_real_query(conn, insert, strlen(insert))) {
      		fprintf(stderr, "Error al insertar: %s\n", mysql_error(conn));
      		exit(1);
   	}else{
              printf("Inserto\n");
	}

	/* Chequea si se inserto correctamente en la base de datos. */
	if(!mysql_affected_rows(conn)){
		printf("No se inserto correctamente\n");
		exit(1);
	}

	/* Cierra la conexion. */
   	mysql_close(conn);
}


Point_Data loadPointStruct(double latitude, double longitude, char *datetime, int found){

	Point_Data punto;
	punto.latitude = latitude;
	punto.longitude = longitude;
	punto.datetime = datetime;
	punto.found = found;

	return punto;
}

int deleteAllPoints(){
	MYSQL *conn;
	MYSQL_RES *res;
   	MYSQL_ROW row;
	setServer();
   
   	conn = mysql_init(NULL);
   
   	/* Se conecta a la base de datos */
   	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
      		fprintf(stderr, "Error en connect: %s\n", mysql_error(conn));
      		exit(1);
   	}

   	/* Envia la sentencia SQL */
	char *delete = "DELETE FROM `UMix`.`Puntos`";
	
   	if (mysql_real_query(conn, delete, strlen(delete))) {
      		fprintf(stderr, "Error al borrar: %s\n", mysql_error(conn));
      		exit(1);
   	}

	/* Obtiene la cantidad de puntos que se eliminaron. */
   	int quant;
	quant = mysql_affected_rows(conn);
	
	/* Cierra la conexion. */
   	mysql_close(conn);

	return quant;
}


void recorrerArrayPuntos(Point_Data_Array points){

	printf("La estructura contiene %d puntos\n", points.length); 
	int i = 0;
	
	while(i < points.length){
		printf("Latitud: %f\n", points.pointsArray[i].latitude);
		printf("Longitud: %f\n", points.pointsArray[i].longitude);
		printf("Fecha y hora: %s\n", points.pointsArray[i].datetime);
		printf("Encontro: %d\n", points.pointsArray[i].found);
		
		i++;
	}
	printf("Se intero %d veces\n", i);
}
