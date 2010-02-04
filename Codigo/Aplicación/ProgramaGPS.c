#define         PROTOPORT       2947

#include        <sys/types.h>
#include        <sys/socket.h>
#include        <netinet/in.h>
#include        <arpa/inet.h>
#include        <string.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <sys/time.h>
#include        <errno.h>
#include        <netdb.h>
#include        <unistd.h>
#include        <sys/select.h>

/* Prototipos de funciones */
char *getPos();
char *getLat(char *);
char *getLong(char *);

extern int errno;

main(){
	char *buf1;
	char *buf2;	
	buf1 = getPos();
	char *latitude;
	latitude = getLat(buf1);
	buf2 = getPos(); /* HAY QUE ARREGLARLO 
			*Como hago para que el valor al que apunta buf2 sea igual al valor al que apunta buf1??	
			*     *buf2 = *buf1;  <- Da error de segmentacion */	
	
	char *longitude;
	longitude = getLong(buf2);
	printf("La latitud es %s y la longitud es %s\n", latitude, longitude);
}




char *getPos(){
	float trys;  /* Variable para intentar comunicarse con GPSD Daemon durante 4 segundos. */
        trys = 5.0;
	int n, w;      /*Cantidad de caracteres leídos */
        static char buf[35];        /*Buffer para los datos del servidor */    	
	char descarte[1000];	/* Buffer para descartar los datos que no nos interesan. */
	const char *host = "127.0.0.1";
	const char *service = "2947";
	const char *protocol = "tcp";
	struct hostent *phe;
    	struct servent *pse;
	struct protoent *ppe;
	struct sockaddr_in sin;
	int gpsd, type, proto, one = 1;

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
		fprintf(stderr, "Falló connect, %s(%d)\n", strerror(errno), errno);
             	exit(1);
    }



/*Es necesario estar en modo watch para que obtenga la posicion la primera vez. */
	w = write(gpsd, "w\n", strlen("w\n"));  
	if ((ssize_t)strlen("w\n") != w) {
		(void)fprintf(stderr, "Error al enviar \"w\", %s(%d)\n", strerror(errno), errno);
		exit(1);
	}
         n = (int)read(gpsd, descarte, sizeof(descarte));
	/*printf("Se leyo %s en el primer w\n", descarte);*/

/* Salimos de modo watch para que no se confunda con la respuesta a nuestra solicitud. */
	w = write(gpsd, "w\n", strlen("w\n"));  
	if ((ssize_t)strlen("w\n") != w) {
		(void)fprintf(stderr, "Error al enviar \"w\", %s(%d)\n", strerror(errno), errno);
		exit(1);
	}
         n = (int)read(gpsd, descarte, sizeof(descarte));
	/*printf("Se leyo %s en el segundo w\n", descarte);*/

	n = (int)read(gpsd, descarte, sizeof(descarte));
	/*printf("Se descarta %s\n", descarte);*/

	while(trys >= 0.0){
                     
/* p  Returns the current position in the form "GPSD, P=%f %f"; numbers are in degrees, latitude first. */    
		w = write(gpsd, "P\n", strlen("P\n"));  
		if ((ssize_t)strlen("P\n") != w) {
			(void)fprintf(stderr, "Error al enviar comando, %s(%d)\n", strerror(errno), errno);
			exit(1);
		}
		
  
/* Recibe la salida del GPS Daemon. */
         	n = (int)read(gpsd, buf, sizeof(buf));
       		
		if (n < 0){
			fprintf(stderr, "Fallo al recibir respuesta de gpsd, %s(%d)\n", strerror(errno), errno);
                	exit(1);
	        }else if (n == 0){
        	        fprintf(stderr, "No se leyo nada\n");
        	        exit(1);
        	}else{
        	        if((strncmp(buf,"GPSD,P=?",8))==0){ 
				(void)sleep(0.4); /* Esperar 0.2 segundos antes de volver a intentar */
        	                trys = trys - 0.4;
	       	        }else break;
			
                     
        	}
	}

	close(gpsd);   /* Cierra el socket */

	if((strncmp(buf,"GPSD,P=?",8))==0){
		printf("El receptor no tiene una lectura de posicion\n");
		exit(1);
	}else{
		/*printf("%s\n", buf);	*/
		return (buf);
	}
}


char *getLat(char *buf){
	char *aux;
	char *latitude;
	char *longitude;   
	const char delimiters[] = " ,=";    

/*"GPSD, P=%f %f"*/
	/*printf("%s\n", buf);*/
	aux = strtok(buf, delimiters);
	/*printf("%s\n", aux);*/
	aux = strtok(NULL, delimiters);
	/*printf("%s\n", aux);*/
	latitude = strtok(NULL, delimiters);
	/*printf("%s\n", latitude);*/
	longitude = strtok(NULL, delimiters);
	/*printf("%s\n", longitude);*/
      
	return (latitude);
}

char *getLong(char *buf){	
	char *aux;
	char *latitude;
	char *longitude;       
	const char delimiters[] = " ,=";
	
	/*printf("%s\n", buf);*/
	aux = strtok(buf, delimiters);
	aux = strtok(NULL, delimiters);
	latitude = strtok(NULL, delimiters);
	/*printf("%s\n", latitude);*/
	longitude = strtok(NULL, delimiters);
	/*printf("%s\n", longitude);*/
      
	return(longitude);
}



