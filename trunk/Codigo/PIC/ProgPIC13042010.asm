;ModMovimiento.asm
;PROGRAMA PARA CONTROLAR EL MOVIMIENTO DEL CARRO
;**********************************************************
;*  Microchip Technology Incorporated
;*  PIC 16F877A
;**********************************************************


	
; Include file, change directory if needed
	include "p16f877a.inc"
	list p=16f877a
	ERRORLEVEL 1,-302


	ORG 0x00
 	goto 	Main

	ORG 0x04
	goto 	AtencionInterrupcion
	

	ORG 0x20
CONT
	ORG 0x21
PASOS
	ORG 0x22
TEMP							;Variable temporal
	ORG 0x23
TipoInt							;Variable auxiliar para grabar el tipo de interrupción
	ORG 0x24
Aux

	ORG 0x25
Main
	call 	Inicializacion
	bsf		PORTD,7			;DEBUG
	sleep
	nop
Chequeo
	call	banco0
	movlw	b'00110010'
	movwf	Aux			;Seteo Aux en 50 para que el tiempo de espera para notificar
						;a la BB sea de 2 seg aprox (HAY QUE AJUSTARLO)
	btfsc	TipoInt,0			;Me fijo si me enviaron Adelante
	goto	SubrutinaAdelante	;Si, entonces ejecuto SubrutinaAdelante
	btfsc	TipoInt,1			;No, entonces me fijo si me enviaron Atrás
	goto	SubrutinaAtras		;Si, entonces ejecuto SubrutinaAtras
	btfsc	TipoInt,2			;No, entonces me fijo si me enviaron Derecha
	goto	SubrutinaDerecha	;Si, entonces ejecuto SubrutinaDerecha
	btfsc	TipoInt,3			;No, entonces me fijo si me enviaron Izquierda
	goto	SubrutinaIzquierda	;Si, entonces ejecuto SubrutinaIzquierda
	btfsc	TipoInt,4			;No, entonces me fijo si se detectó un obstáculo en la derecha
	goto	SubrutinaObstDer	;Si, entonces ejecuto SubrutinaObstDer
	btfsc	TipoInt,5			;No, entonces me fijo si se detectó un obstáculo en la izquierda
	goto	SubrutinaObstIzq	;Si, entonces ejecuto SubrutinaObstIzq
	btfsc	TipoInt,6			;No, entonces me fijo si se detectó luz IR
	goto	SubrutinaIR		;Si, entonces ejecuto SubrutinaIR
	goto	Main				;No, entonces vuelvo a Main


;Loop			;SOLO PARA DEBUG

;	call	Adelante
;	call	Espera
;	call	Derecha
;	call	Espera
;	call	Izquierda
;	call	Espera
;	call	Detener
;	call	Espera
;	call	Atras
;	call	Espera
;	call	Detener
;	call	LiberarMotorTraccion
;	goto 	Loop

	
Inicializacion
	call	banco0
	clrf 	PORTA
	clrf 	PORTB
	clrf 	PORTC
	clrf 	PORTD
	call	banco1
	movlw	b'11111111' 
	movwf 	TRISA 		;Configuramos el puerto A como entrada (Por ahora no lo vamos a usar)
	
	movlw 	b'01110001' 	;Configuramos los bits RB0 y RB4:RB6 como entradas y el resto como salidas,
	movwf 	TRISB 			;vamos a utilizar las entradas para la interrupción para recibir directivas (RB0) y para los sensores (RB4:RB6)
							;y las salidas RB1 y RB2 para volver a poner en cero los flip-flops asociados a los sensores de obstáculos.

	movlw 	b'00000000' 
	movwf 	TRISC 		;Configuramos el puerto C como salida, lo utilizaremos para manejar los motores.
	
	movlw	b'00001111'
	movwf	TRISD		;Configuramos los bits RD0:RD3 como entrada (directivas de movimiento) y el resto como salidas (los bits RD4:RD6 se utilizan 
						;para reportarle a la placa condiciones positivas en los sensores).

	call 	banco0		;Nos posicionamos en el banco 0
	clrf 	INTCON		;Deshabilitamos todas las interrupciones
	clrf	PORTD		;Borra el puerto D
	clrf	PORTC		;Borra el puerto C
	bsf		PORTB,1		;Restea flip-flop asociado al sensor derecho
	bsf		PORTB,2		;Restea flip-flop asociado al sensor izquierdo
	clrf	PORTB		;Borra el puerto B
	clrf	PORTA		;Borra el puerto A	

	clrf	CONT		;Pongo en cero el contador
	clrf	TEMP		;Pongo en cero la variable temporal
	clrf	TipoInt		;Pongo en cero la variable para el tipo de interrupción
	movlw	b'00001010'
	movwf	PASOS		;Seteo en 7 la cantidad de pasos (Es necesario ajustarlo)
	call	banco1
	movlw 	0x06 
	movwf 	ADCON1 		;set puerto analógico como digital 
	bsf		OPTION_REG,6	;Configuramos flanco ascendente para la interrupción en RB0.	
	;bcf		OPTION_REG,7	;Habilitamos pull-ups
	
	call	banco0
	clrf	PORTB			;Limpio el puerto B
	bcf 	INTCON,RBIE		;Deshabilitamos la interrupción al cambiar RB4:RB7
	movf 	PORTB,W 		;Leemos el puerto B
	movlw	b'10011000'		;Habilitamos interrupciones globales, interrupción externa de RB0 e interrupción RB port change
	movwf	INTCON			;y limpiamos los flags
	return

AtencionInterrupcion
	call	banco0
	bcf		PORTD,7				;DEBUG
	bcf		INTCON,GIE			;Deshabilito interrupciones globales
	bcf		INTCON,RBIE			;Deshabilito interrupcion RB port change
	bcf		INTCON,INTE			;Deshabilito interrupcion externa
	clrf	PORTC
	bsf		PIR1,1
	btfsc 	INTCON,RBIF 		;La interrupción fue por un cambio en RB4:RB7?
	goto 	AtencionSensores 	;Si, entonces voy a la rutina de atención correspondiente 
	btfsc	INTCON,INTF 		;No, entonces me fijo si la interrupción fue porque la placa BB va a enviar una directiva.
	goto	RecibirDirectiva	;Si, entonces veo qué directiva me envió la placa
	bsf		INTCON,GIE			;No, entonces vuelvo a habilitar interrupciones globales,
	bsf		INTCON,RBIE			;interrupcion RB port change e
	bsf		INTCON,INTE			;interrupcion externa
	retfie						;y regreso a donde estaba antes de la interrupción


AtencionSensores
	call	banco0
	movf	PORTB,W 	;Leo el puerto B
	movwf	TEMP		;Pongo lo leído en la variable temporal
	bcf 	INTCON,RBIF ;Pongo en cero el flag correspondiente a la interrupción RB port change
	btfsc	TEMP,4		;Me fijo si fue el Sensor de obstáculos Derecho
	goto	Tipo4		;Si, entonces voy a Tipo4
	btfsc	TEMP,5		;No, entonces me fijo si fue el Sensor de obstáculos Izquierdo
	goto	Tipo5		;Si, entonces voy a Tipo5
	btfsc	TEMP,6		;No, entonces me fijo si fue el Sensor IR
	goto	Tipo6		;Si, entonces voy a Tipo6

	bsf		INTCON,RBIE	;No, entonces vuelvo a habilitar la interrupción RB port change,
	bsf		INTCON,INTE	;interrupcion externa e
	bsf		INTCON,GIE	;interrupciones globales
	retfie				;y regreso a donde estaba antes de la interrupción


Tipo4
	bsf		TipoInt,4	;Seteo el flag 4 de la variable TipoInt
	bsf		PORTB,1		;Envío Reset al flip-flop
	bcf		PORTB,1		;Apago el reset del flip-flop
	bsf		INTCON,RBIE	;Vuelvo a habilitar la interrupicón RB port change,
	bsf		INTCON,INTE ;interrupcion externa
	bsf		INTCON,GIE	;e interrupciones globales
	retfie				;y regreso a donde estaba antes de la interrupción
	
Tipo5
	bsf		TipoInt,5	;Seteo el flag 5 de la variable TipoInt
	bsf		PORTB,2		;Envío Reset al flip-flop
	bcf		PORTB,2		;Apago el reset del flip-flop
	bsf		INTCON,RBIE	;Vuelvo a habilitar la interrupicón RB port change,
	bsf		INTCON,INTE ;interrupcion externa
	bsf		INTCON,GIE	;e interrupciones globales
	retfie				;y regreso a donde estaba antes de la interrupción


Tipo6
	bsf		TipoInt,6	;Seteo el flag 6 de la variable TipoInt
	bsf		INTCON,RBIE	;Vuelvo a habilitar la interrupicón RB port change,
	bsf		INTCON,INTE ;interrupcion externa
	bsf		INTCON,GIE	;e interrupciones globales
	retfie				;y regreso a donde estaba antes de la interrupción




RecibirDirectiva
	call	banco0
	movf	PORTB,W
	bcf 	INTCON,INTF ;Pongo en cero el flag correspondiente a la interrupción externa de RB0
	btfsc	PORTD,0		;Testeo si me enviaron Adelante
	goto	Tipo0		;Si, entonces voy a Tipo0
	btfsc	PORTD,1		;No, entonces testeo si me enviaron Atrás
	goto	Tipo1		;Si, entonces voy a Tipo1
	btfsc	PORTD,2		;No, entonces testeo si me enviaron Derecha
	goto	Tipo2		;Si, entonces voy a Tipo2
	btfsc	PORTD,3		;No, entonces testeo si me enviaron Izquierda
	goto	Tipo3		;Si, entonces voy a Tipo3
	bsf		INTCON,RBIE	;No, entonces vuelvo a habilitar la interrupicón RB port change,
	bsf		INTCON,INTE ;interrupcion externa
	bsf		INTCON,GIE	;e interrupciones globales
	retfie				;y regreso a donde estaba antes de la interrupción


Tipo0
	bsf		TipoInt,0	;Seteo el flag 0 de la variable TipoInt	
	bsf		INTCON,RBIE	;Vuelvo a habilitar la interrupicón RB port change,
	bsf		INTCON,INTE ;interrupcion externa
	bsf		INTCON,GIE	;e interrupciones globales
	retfie				;y regreso a donde estaba antes de la interrupción


Tipo1
	bsf		TipoInt,1	;Seteo el flag 1 de la variable TipoInt
	bsf		INTCON,RBIE	;Vuelvo a habilitar la interrupicón RB port change,
	bsf		INTCON,INTE ;interrupcion externa
	bsf		INTCON,GIE	;e interrupciones globales
	retfie				;y regreso a donde estaba antes de la interrupción


Tipo2
	bsf		TipoInt,2	;Seteo el flag 2 de la variable TipoInt
	bsf		INTCON,RBIE	;Vuelvo a habilitar la interrupicón RB port change,
	bsf		INTCON,INTE ;interrupcion externa
	bsf		INTCON,GIE	;e interrupciones globales
	retfie				;y regreso a donde estaba antes de la interrupción


Tipo3
	bsf		TipoInt,3	;Seteo el flag 3 de la variable TipoInt
	bsf		INTCON,RBIE	;Vuelvo a habilitar la interrupicón RB port change,
	bsf		INTCON,INTE ;interrupcion externa
	bsf		INTCON,GIE	;e interrupciones globales
	retfie				;y regreso a donde estaba antes de la interrupción




;******************************************************************************************************************************
;Control del motor de Continua (Tracción)
;
;Para controlas el motor de continua se utilizarán dos canales de un driver L293D.
;Se conectarán 3 pines del puerto B: RC0, RC1 y RC2 a los pines Enable1, Input1 e Input2 del L293D respectivamente.
;Cuando RC0 esté en 1 el puente 1 del L293D estará habilitado, en caso contrario dicho puente quedará deshabilitado
;por lo que el motor quedará libre.
;Estando el puente 1 del L293D habilitado: si RC1 = 1 y RC2 = 0 el motor girará en un sentido (hacia adelante)
;si RC1 = 0 y RC2 = 1 el motor girará en sentido contrario (hacia atrás) y si RC1 = RC2 el motor estará detenido.
;********************************************************************************************************************************

Adelante
	call	banco0
	bsf		PORTC,0		;Habilitamos canales del L293D (Tomamos el control del motor)
	bsf		PORTC,1		;Seteamos el bit RC1
	bcf		PORTC,2		;Ponemos en cero el bit RC2
	return
	
SubrutinaAdelante
	call	banco0
	bcf		TipoInt,0	;Limpio el flag de TipoInt
	call	Adelante
	call	Espera
	call	Detener
	call	LiberarMotorTraccion	
	goto	Chequeo	


Atras
	call	banco0
	bsf		PORTC,0		;Habilitamos canales (Tomamos el control del motor)
	bcf		PORTC,1		;Ponemos en cero el bit RC1
	bsf		PORTC,2		;Seteamos el bit RC2
	return

SubrutinaAtras
	call	banco0
	bcf		TipoInt,1	;Limpio el flag de TipoInt
	call	Atras
	call	Espera
	call	Detener
	call	LiberarMotorTraccion
	goto	Chequeo

Detener
	call	banco0
	bcf		PORTC,1		;Ponemos en cero el bit RC1
	bcf		PORTC,2		;Ponemos en cero el bit RC2
	return

LiberarMotorTraccion
	call	banco0
	bcf		PORTC,0		;Deshabilitamos canales del L293B (Liberamos el motor)
	return


;**************************************************************************************************************************
;Control del motor Paso a Paso (Dirección)
;
;Para controlar la dirección del carro se utilizará un motor PaP bipolar. Para controlar este tipo de motores es necesario
;invertir las polaridades de los terminales de las bobinas en una determinada secuencia para lograr un giro en un sentido y
;en secuencia opuesta para que gire en el otro sentido.
;Se utilizará un driver L293D.
;Se conectará el pin RC3 a los pines Enable1 y Enable2 del L293D. Con el bit RC3 se habilitarán/deshabilitarán los puentes del L293D.
;Se conectará el pin RC4 al pin Input1 del L293D, el pin RC5 al pin Input2 del L293D, el pin RC6 al pin Input3 del L293D
;y el pin RC7 al pin Input4 del L293D.
;Para controlar el motor se debe tener en cuenta la siguiente tabla:
;
;Num de Pasos		RC4			RC5			RC6			RC7
;Paso 1				+Vcc		GND			+Vcc		GND
;Paso 2				+Vcc		GND			GND			+Vcc
;Paso 3 			GND			+Vcc		GND			+Vcc
;Paso 4				GND			+Vcc		+Vcc		GND

Derecha
	bsf		PORTC,3		;Tomamos el control del motor
	call	Paso1
	call	EsperaPaP
	call	Paso2
	call	EsperaPaP
	call	Paso3
	call	EsperaPaP
	call	Paso4
	call	EsperaPaP
	incf	CONT,1			;Incrementamos el contador
	movf	PASOS,0			;Escribo la cantidad de pasos en W para que la secuencia 1-2-3-4 se ejecute PASOS veces
	subwf	CONT,0			;Le resto W a CONT y lo almaceno en W
	btfss	STATUS,2		;Me fijo si la operacion dio 0
	goto 	Derecha			;Si no dio 0, CONT aún no llegó a PASOS entonces repite
	bcf		PORTC,3			;Si dio 0, CONT llegó a PASOS entonces libero el motor,
	clrf	CONT			;pongo en 0 el contador,
	call	LiberarMotorPaP	;libero el motor
	return

SubrutinaDerecha
	call	banco0
	bcf		TipoInt,2	;Limpio el flag de TipoInt	
	call 	Adelante
	call	Derecha
	call	Detener
	call	LiberarMotorTraccion
	goto	Chequeo

Izquierda
	bsf		PORTC,3			;Tomamos el control del motor
	call	Paso4
	call	EsperaPaP
	call	Paso3
	call	EsperaPaP
	call	Paso2
	call	EsperaPaP
	call	Paso1
	call	EsperaPaP
	incf	CONT,1			;Incrementamos el contador
	movf	PASOS,0			;Escribo la cantidad de pasos en W para que la secuencia 4-3-2-1 se ejecute PASOS veces
	subwf	CONT,0			;Le resto W a CONT y lo almaceno en W
	btfss	STATUS,2		;Me fijo si la operacion dio 0
	goto 	Izquierda		;Si no dio 0, CONT aún no llegó a PASOS entonces repite
	bcf		PORTC,3			;Si dio 0, CONT llegó a PASOS entonces libero el motor,
	clrf	CONT			;pongo en 0 el contador,
	call	LiberarMotorPaP	;libero el motor
	return

SubrutinaIzquierda
	call	banco0
	bcf		TipoInt,3		;Limpio el flag de TipoInt
	call 	Adelante
	call	Izquierda
	call	Detener
	call	LiberarMotorTraccion
	goto	Chequeo

LiberarMotorPaP
	call	banco0
	bcf		PORTC,3			;Deshabilitamos canales del L293B (Liberamos el motor)
	return

Paso1
	bsf		PORTC,4
	bcf		PORTC,5
	bsf		PORTC,6
	bcf		PORTC,7
	return

Paso2
	bsf		PORTC,4
	bcf		PORTC,5
	bcf		PORTC,6
	bsf		PORTC,7
	return

Paso3
	bcf		PORTC,4
	bsf		PORTC,5
	bcf		PORTC,6
	bsf		PORTC,7
	return	

Paso4
	bcf		PORTC,4
	bsf		PORTC,5
	bsf		PORTC,6
	bcf		PORTC,7
	return


;Rutinas de espera

Espera
	call	banco1
	movlw	0xFF
	movwf	PR2				;Escribimos 255 en PR2 para que TMR2 incremente hasta 255
	call	banco0
	clrf	TMR2			;Pongo en 0 el registro del timer 2
	movlw	b'01111011'		;Configuramos prescaler y postscaler 1:16
	movwf	T2CON
	bcf		PIR1,1			;Ponemos en cero el flag del timer2
	bsf		T2CON,2			;Encendemos el Timer 2
	call	Test			;Testeo si terminó la espera
	decf	Aux,1			;Decremento Aux
	btfss	STATUS,2		;Me fijo si Aux llegó a 0
	goto	Espera			;Si no llegò a 0 vuelvo a esperar
	return					;Si llegó a 0 regreso


EsperaPaP
	call	banco1
	movlw	b'11111111'		;El período típico de conmutación del L298 son 40us
	movwf	PR2				;Escribimos 255 en PR2 para que TMR2 incremente hasta 255
	call	banco0
	clrf	TMR2			;Pongo en 0 el registro del timer 2
	movlw	b'01111011'		;Configuramos prescaler y postscaler 1:16
	movwf	T2CON
	bcf		PIR1,1			;Ponemos en cero el flag del timer2
	bsf		T2CON,2			;Encendemos el Timer 2
	call	Test			;Testeo si terminó la espera
	return


Test
	btfss	PIR1,1			;Terminó la espera?
	goto 	Test			;Si no terminó sigo esperando
	bcf		PIR1,1			;Si terminó, limpio el flag
	bcf		T2CON,2			;Apago el timer 2
	return					;y regreso


SubrutinaObstDer
	call	banco0
	bcf		TipoInt,4		;Limpio el flag de TipoInt
	bsf		PORTD,4			;Notifico a la placa que se detectó un obstáculo del lado derecho
	call	EsperaNotific	;Ejecuto rutina de espera para que la placa lea el dato
	bcf		PORTD,4			;Limpio el bit RD4
	goto	Chequeo

SubrutinaObstIzq
	call	banco0
	bcf		TipoInt,5		;Limpio el flag de TipoInt
	bsf		PORTD,5			;Notifico a la placa que se detectó un obstáculo del lado izquierdo
	call	EsperaNotific	;Ejecuto rutina de espera para que la placa lea el dato
	bcf		PORTD,5			;Limpio el bit RD5
	goto	Chequeo

SubrutinaIR
	call	banco0
	bcf		TipoInt,6		;Limpio el flag de TipoInt
	bsf		PORTD,6			;Notifico a la placa que se detectó luz IR
	call	EsperaNotific	;Ejecuto rutina de espera para que la placa lea el dato
	bcf		PORTD,6			;Limpio el bit RD6
	goto	Chequeo

EsperaNotific				;CONFIGURAR EL TIEMPO DE ESPERA NECESARIO
	call	banco1
	movlw	0xFF
	movwf	PR2				;Escribimos 255 en PR2 para que TMR2 incremente hasta 255
	call	banco0
	clrf	TMR2			;Pongo en 0 el registro del timer 2
	movlw	b'01111011'		;Configuramos prescaler y postscaler 1:16
	movwf	T2CON
	bcf		PIR1,1			;Ponemos en cero el flag del timer2
	bsf		T2CON,2			;Encendemos el Timer 2
	call	Test			;Testeo si terminó la espera
	decf	Aux,1			;Decremento la variable Aux
	btfss	STATUS,2		;Me fijo si llegó a cero
	goto	EsperaNotific	;Si no llegó a 0 vuelvo a esperar
	return					;Si llegò a 0 regreso




banco0
	bcf 	STATUS,RP1
	bcf 	STATUS,RP0
	return

banco1
	bcf 	STATUS,RP1
	bsf 	STATUS,RP0
	return


end
