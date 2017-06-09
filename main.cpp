#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define CANT_FILA 8
#define CANT_COL 8
#define BOMBA -1
#define BOMBA_MARCADA -2
#define BOMBA_EXPLOTA -3
#define BOMBA_MUESTRA -4
#define MAL_MARCADA -5
#define INTERROGACION -6
//falta funcion marcar que se hizo en la clase anterior
/*
* Inicia la matriz que se mostrará al inicio sin las bombas.
*/
void inicializar(int m[CANT_FILA][CANT_COL]){
	for(int i=0; i < CANT_FILA; i++){
		for(int j=0; j < CANT_COL; j++){
			m[i][j]=INTERROGACION;
		}
	}
}
/*
* Coloca las bombas en el tablero luego de realizada la primer jugada
* del usuario, para asegurarse de que no haya una bomba alli.
*/
void poner_bombas(int m[CANT_FILA][CANT_COL],int bombas, int x, int y){
	int puestas = 0, a, b;
	srand(time(NULL));
	while(puestas < bombas){
		a = rand() % CANT_FILA;
		b = rand() % CANT_COL;
		if(m[a][b] != BOMBA){
			if(!(x == a && y == b)){
				m[a][b] = BOMBA;
				puestas++;
			}
		}
	}
}
/*
* Recorre la matriz e imprime el tablero en pantalla.
*/
void imprimir(int m[CANT_FILA][CANT_COL],int num_bombas){
	printf("##############################\n");
	printf("BUSCAMINAS\tBombas: %d\n",num_bombas);
	printf("Opciones:\n");
	printf("E: Explorar\n");
	printf("M: Marcar\n");
	printf("B: Buscar\n");
	printf("  ");
	for(int i = 0; i < CANT_COL; i++){
		printf("%c ", i + 'A');
	}
	printf("\n");
	for(int i = 0; i < CANT_COL; i++){
		printf("%c ", i + 'A');
		for(int j = 0; j < CANT_FILA; j++){
			if(m[i][j] >= 0){
				printf("%d ", m[i][j]);
			}else if(m[i][j] == BOMBA_MARCADA || m[i][j] == MAL_MARCADA){
				printf("M ");
			}else if(m[i][j] == BOMBA_EXPLOTA){
				printf("X ");
			}else if(m[i][j] == BOMBA_MUESTRA){
				printf("* ");
			}else{
				printf(". ");
				//printf("%d",m[i][j]);
			}
		}
		printf("\n");
	}
}
/*
* Cuenta la cantidad de veces que se encuentra un objeto en el tablero.
*/
int cuenta_exploradas(int m[CANT_FILA][CANT_COL]){
	int cexplorada = 0;
	for(int i = 0; i < CANT_COL; i++){
		for(int j = 0; j < CANT_FILA; j++){
			if(m[i][j] >= 0){
				cexplorada++;
			}
		}
	}
	return cexplorada;
}
int cuenta_marcadas(int m[CANT_FILA][CANT_COL]){
	int cmarcada = 0;
	for(int i = 0; i < CANT_COL; i++){
		for(int j = 0; j < CANT_FILA; j++){
			if(m[i][j] == BOMBA_MARCADA || m[i][j] == MAL_MARCADA){
				cmarcada++;
			}
		}
	}
	return cmarcada;
}
/*
* Cuenta cantidad de objetos del mismo tipo en el tablero.
*/
int cuenta_obj_tablero(int m[CANT_FILA][CANT_COL],int objeto){
	int cuenta_objeto = 0;
	for(int i = 0; i < CANT_COL; i++){
		for(int j = 0; j < CANT_FILA; j++){
			if(m[i][j] == objeto){
				cuenta_objeto++;
			}
		}
	}
	return cuenta_objeto;
}
/*
* Verifica que la coordenada ingresada sea valida y que existe un casillero.
*/
int casillaExiste(int m[CANT_FILA][CANT_COL], int x, int y){
	if(x >= 0 && x < CANT_FILA && y >= 0 && y < CANT_COL){
		return 1;
	}else{
		return 0;
	}
}
/*
* Verifica si hay un determinado objeto en una casilla.
*/
int hayObjeto(int m[CANT_FILA][CANT_COL], int x, int y, int objeto){
	if(casillaExiste(m,x,y) == 1 && m[x][y] == objeto){
		return 1;
	}else{
		return 0;
	}
}
/*
* Suma objetos adyacentes a la casilla seleccionada que sean del mismo tipo.
*/
int cuenta_obj_adyacentes(int m[CANT_FILA][CANT_COL],int x, int y, int objeto){
	int suma_objetos = 0;
	suma_objetos+=hayObjeto(m,x-1,y-1,objeto);	//a11
	suma_objetos+=hayObjeto(m,x-1,y,objeto);	//a12
	suma_objetos+=hayObjeto(m,x-1,y+1,objeto);	//a13
	suma_objetos+=hayObjeto(m,x,y-1,objeto);	//a21
	suma_objetos+=hayObjeto(m,x,y+1,objeto);	//a23
	suma_objetos+=hayObjeto(m,x+1,y-1,objeto);	//a31
	suma_objetos+=hayObjeto(m,x+1,y,objeto);	//a32
	suma_objetos+=hayObjeto(m,x+1,y+1,objeto);	//a31
	return suma_objetos;
}
/*
* Explora la casilla y verifica la cantidad de bombas
* adyacentes; si hay bomba se pierde, de lo contrario
* se le asigna el numero de bombas cercanas a ese casillero.
*/
//---->>> Añadir que no borre las marcadas incorrectas??
bool explorar(int m[CANT_FILA][CANT_COL], int x, int y){
	if(m[x][y] == BOMBA || m[x][y] == BOMBA_MARCADA || m[x][y] == BOMBA_EXPLOTA){
		m[x][y] = BOMBA_EXPLOTA;
		return false;
	}else if(m[x][y] < 0){
        int num = cuenta_obj_adyacentes(m,x,y,BOMBA) + cuenta_obj_adyacentes(m,x,y,BOMBA_MARCADA);
		m[x][y] = num;
        if(num == 0){
            if(casillaExiste(m,x-1,y-1) == 1){
                explorar(m,x-1,y-1);
            }
            if(casillaExiste(m,x+1,y+1) == 1){
                explorar(m,x+1,y+1);
            }
            if(casillaExiste(m,x,y-1) == 1){
                explorar(m,x,y-1);
            }
            if(casillaExiste(m,x-1,y) == 1){
                explorar(m,x-1,y);
            }
            if(casillaExiste(m,x,y+1) == 1){
                explorar(m,x,y+1);
            }
            if(casillaExiste(m,x+1,y) == 1){
                explorar(m,x+1,y);
            }
            if(casillaExiste(m,x+1,y-1) == 1){
                explorar(m,x+1,y-1);
            }
            if(casillaExiste(m,x-1,y+1) == 1){
                explorar(m,x-1,y+1);
            }
		}
		return true;
	}else{
		return true;
	}
}
/*
* Marca posible bomba en el tablero.
*/
void marcar(int m[CANT_FILA][CANT_COL], int x, int y){
	if(m[x][y] < 0){
		if(m[x][y] == BOMBA){
			m[x][y] = BOMBA_MARCADA;
		}else if(m[x][y] == BOMBA_MARCADA){
			m[x][y] = BOMBA;
		}else if(m[x][y] == MAL_MARCADA){
			m[x][y] = INTERROGACION;
		}else{
			m[x][y] = MAL_MARCADA;
		}
	}
}
/*
* Busca bombas en casilleros con valor 0 o minas adyacentes marcadas.
*/
bool buscar(int m[CANT_FILA][CANT_COL], int x, int y){	
	// Suma celdas adyacentes MARCADAS
	int suma_M = cuenta_obj_adyacentes(m,x,y,BOMBA_MARCADA) + cuenta_obj_adyacentes(m,x,y,MAL_MARCADA);
	// Si el numero de celdas MARCADAS adyacentes coincide con el numero de la celda, realiza la operacion buscar
	if(m[x][y] != suma_M){
		return true;
	}else{
		// Comprueba si la casilla adyacente esta MARCADA
		int a11 = hayObjeto(m,x-1,y-1,BOMBA_MARCADA) + hayObjeto(m,x-1,y-1,MAL_MARCADA);
		int a12 = hayObjeto(m,x-1,y  ,BOMBA_MARCADA) + hayObjeto(m,x-1,y  ,MAL_MARCADA);
		int a13 = hayObjeto(m,x-1,y+1,BOMBA_MARCADA) + hayObjeto(m,x-1,y+1,MAL_MARCADA);
		int a21 = hayObjeto(m,x	 ,y-1,BOMBA_MARCADA) + hayObjeto(m,x  ,y-1,MAL_MARCADA);
		int a23 = hayObjeto(m,x  ,y+1,BOMBA_MARCADA) + hayObjeto(m,x  ,y+1,MAL_MARCADA);
		int a31 = hayObjeto(m,x+1,y-1,BOMBA_MARCADA) + hayObjeto(m,x+1,y-1,MAL_MARCADA);
		int a32 = hayObjeto(m,x+1,y  ,BOMBA_MARCADA) + hayObjeto(m,x+1,y  ,MAL_MARCADA);
		int a33 = hayObjeto(m,x+1,y+1,BOMBA_MARCADA) + hayObjeto(m,x+1,y+1,MAL_MARCADA);
		// Comprueba si hay casillas adyacentes mal marcadas, en tal caso explota dicha casilla y se termina el juego.
		if(cuenta_obj_adyacentes(m,x,y,MAL_MARCADA) > 0){
			if(hayObjeto(m,x-1,y-1,MAL_MARCADA)){
				m[x-1][y-1] = BOMBA_EXPLOTA;
			}
			if(hayObjeto(m,x+1,y+1,MAL_MARCADA)){
				m[x+1][y+1] = BOMBA_EXPLOTA;
			}
			if(hayObjeto(m,x,y-1,MAL_MARCADA)){
				m[x][y-1] = BOMBA_EXPLOTA;
			}
			if(hayObjeto(m,x-1,y,MAL_MARCADA)){
				m[x-1][y] = BOMBA_EXPLOTA;
			}
			if(hayObjeto(m,x,y+1,MAL_MARCADA)){
				m[x][y+1] = BOMBA_EXPLOTA;
			}
			if(hayObjeto(m,x+1,y,MAL_MARCADA)){
				m[x+1][y] = BOMBA_EXPLOTA;
			}
			if(hayObjeto(m,x+1,y-1,MAL_MARCADA)){
				m[x+1][y-1] = BOMBA_EXPLOTA;
			}
			if(hayObjeto(m,x-1,y+1,MAL_MARCADA)){
				m[x-1][y+1] = BOMBA_EXPLOTA;
			}
			return false;
		}else{
			// Explora casillas adyacentes que no salgan del rango de la matriz y que no hayan sido marcadas.
            if(casillaExiste(m,x-1,y-1) == 1 && a11 != 1){
                explorar(m,x-1,y-1);
            }
            if(casillaExiste(m,x+1,y+1) == 1 && a33 != 1){
				explorar(m,x+1,y+1);
            }
            if(casillaExiste(m,x,y-1) == 1 && a21 != 1){
                explorar(m,x,y-1);
            }
            if(casillaExiste(m,x-1,y) == 1 && a12 != 1){
                explorar(m,x-1,y);
            }
            if(casillaExiste(m,x,y+1) == 1 && a23 != 1){
                explorar(m,x,y+1);
            }
            if(casillaExiste(m,x+1,y) == 1 && a32 != 1){
                explorar(m,x+1,y);
            }
            if(casillaExiste(m,x+1,y-1) == 1 && a31 != 1){
                explorar(m,x+1,y-1);
            }
            if(casillaExiste(m,x-1,y+1) == 1 && a13 != 1){
                explorar(m,x-1,y+1);
            }
			return true;
		}
	}
}
/*
* Si pierde, se muestra la ubicacion de las bombas en pantalla.
*/
void perdio(int m[CANT_FILA][CANT_COL],int marcadas_tablero){
	for(int i = 0; i < CANT_COL; i++){
		for(int j = 0; j < CANT_FILA; j++){
			if(m[i][j] == BOMBA){
				m[i][j] = BOMBA_MUESTRA;
			}
		}
	}
	imprimir(m,marcadas_tablero);
	printf("¡¡HAS PERDIDO LA PARTIDA!!\n");
}
main () {
	int tablero[CANT_FILA][CANT_COL];
	bool fin = false;
	bool primerajugada = true;
	int fila,columna,cant_bombas,marcadas,marcadas_tablero,aExplorar;
	char op,esp,filachar,columnachar,enter;
	// Asigna varores iniciales sin bombas en el tablero.
	inicializar(tablero);
	printf("##############################\n");
	printf("¡Bienvenido al BUSCAMINAS!\n");
	printf("Ingrese cantidad de bombas: ");
	scanf("%d%c",&cant_bombas,&enter);
	// Mientras no se ingrese un numero valido de bombas, se vuelve a preguntar.
	while(cant_bombas >= (CANT_FILA * CANT_COL) || cant_bombas <= 0){
		printf("##############################\n");
		printf("¡NUMERO INVALIDO!.\n");
		printf("Ingrese cantidad de bombas: ");
		scanf("%d%c",&cant_bombas,&enter);
	}
	aExplorar = (CANT_COL * CANT_FILA) - cant_bombas;
	do{
		// Muestra contador de bombas restantes a MARCAR (esten bien o mal marcadas)
		marcadas_tablero = cant_bombas - cuenta_obj_tablero(tablero,MAL_MARCADA) - cuenta_obj_tablero(tablero,BOMBA_MARCADA);
		imprimir(tablero,marcadas_tablero);
		printf("Realice su jugada: ");
		/*
		* Obtener datos del usuario.
		*/
		scanf("%c%c%c%c%c", &op, &esp, &filachar, &columnachar, &enter);
		fila = filachar - 'A';
		columna = columnachar - 'A';
		if (columna >= 0 && columna < CANT_COL && fila >= 0 && fila < CANT_FILA){
			switch(op){
				case 'E':{// Comprueba si es la primer jugada.
					if(primerajugada == true){
						// Vuelve a iniciar el tablero por si el usuario marco casillas antes de empezar.
						inicializar(tablero);
						// Pone las bombas por primera y unica vez omitiendo la coordenada ingresada.
						poner_bombas(tablero,cant_bombas,fila,columna);
						explorar(tablero,fila,columna);
						primerajugada = false;
						// Si la exploracion da 1 es porque exploto una bomba y se termina el juego.
					}else if(!(explorar(tablero,fila,columna))){
						perdio(tablero,marcadas_tablero);
						fin = true;	
					}
					break;
				}
				case 'M':{
					marcar(tablero,fila,columna);
					break;
				}
				case 'B':{
					if(!(buscar(tablero,fila,columna))){
						perdio(tablero,marcadas_tablero);
						fin = true;	
					}
					break;
				}
				default:{
				}
			}
		}
		// Si se marcaron todas las bombas correctamente, y se explora el tablero completo, se gana el juego.
		printf("%d", cuenta_marcadas(tablero));
		if(cuenta_marcadas(tablero) == cant_bombas && cuenta_exploradas(tablero) == aExplorar){
			imprimir(tablero,marcadas_tablero);
			printf("¡¡HAS GANADO LA PARTIDA!!\n");
			fin = true;
		}
	}while(fin == false);
}
