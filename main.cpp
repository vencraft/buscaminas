#include <stdlib.h>
#include <stdio.h>
#define BOMBA_MUESTRA -6
#define BOMBA_EXPLOTA -5
#define BOMBA -1
#define MARCADA -2
#define MAL_MARCADA -3
#define INTERROGACION -4
#define CANT_FILA 4
#define CANT_COL 4
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
void imprimir(int m[CANT_FILA][CANT_COL]){
	printf("BUSCAMINAS\n");
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
			}else if(m[i][j] == MARCADA || m[i][j] == MAL_MARCADA){
				printf("M ");
			}else if(m[i][j] == BOMBA_EXPLOTA){
				printf("X ");
			}else if(m[i][j] == BOMBA_MUESTRA){
				printf("* ");
			}else{
				printf("? ");
				//printf("%d",m[i][j]);
			}
		}
		printf("\n");
	}
}
/*
* Funcion que verifica si hay bombas en la celda, siempre
* que esta sea valida; si la coordenada es valida y hay bomba
* devuelve un 1, de lo contrario devuelve un 0.
*/
int hayBombas( int m[CANT_FILA][CANT_COL], int x, int y){
	if(x >= 0 && x < CANT_FILA && y >= 0 && y < CANT_COL){
		if(m[x][y] == BOMBA || m[x][y] == MARCADA){
			return 1;
		}
	}
	return 0;
}
/*
* Explora la casilla y verifica la cantidad de bombas
* adyacentes; si hay bomba se pierde, de lo contrario
* se asigna el numero de bombas cercanas.
*/
bool explorar(int m[CANT_FILA][CANT_COL], int x, int y){
	if(m[x][y] == BOMBA || m[x][y] == MARCADA){
		m[x][y] = BOMBA_EXPLOTA;
		for(int i = 0; i < CANT_COL; i++){
			for(int j = 0; j < CANT_FILA; j++){
				if(m[i][j] == BOMBA){
					m[i][j] = BOMBA_MUESTRA;
				}
			}
		}
		return false;
	}else{
		int suma_bombas = 0;
		suma_bombas+=hayBombas(m,x-1,y); //arriba
		suma_bombas+=hayBombas(m,x+1,y); //abajo
		suma_bombas+=hayBombas(m,x,y-1); //izquierda
		suma_bombas+=hayBombas(m,x,y+1); //derecha
		suma_bombas+=hayBombas(m,x-1,y-1); //arriba izquierda
		suma_bombas+=hayBombas(m,x+1,y+1); //abajo derecha
		suma_bombas+=hayBombas(m,x-1,y+1); //arriba derecha
		suma_bombas+=hayBombas(m,x+1,y-1); //abajo izquierda
		m[x][y] = suma_bombas;
		return true;
	}
}
/*
* Marca posible bomba en el tablero.
*/
int marcar(int m[CANT_FILA][CANT_COL], int f, int c){
	int cm = 0;
	if(m[f][c] < 0){
		if(m[f][c] == BOMBA){
			m[f][c] = MARCADA;
			cm++;
		}else{
			m[f][c] = MAL_MARCADA;
		}
	}
	return cm;
}
// hay que hacerla
bool buscar(int m[CANT_FILA][CANT_COL], int x, int y){
	if(m[x][y] >= 0){
		int cant = m[x][y]; //numero que tengo en ese lugar
		// repetir 8 veces por todas las casillas adyacentes
		if(x-1 >= 0){
			if(m[x-1][y] == MAL_MARCADA){
				return false;
			}else if(m[x-1][y] == MARCADA){
				cant--;
			}
		}
		//if (cant == 0)
	}
}
main () {
	int tablero[CANT_FILA][CANT_COL];
	bool fin = false;
	bool primerajugada = true;
	int fila,columna,cant_bombas,marcadas,exploradas;
	char op,esp,filachar,columnachar,enter;
	// Asigna varores iniciales sin bombas en el tablero.
	inicializar(tablero);
	printf("¡Bienvenido al BUSCAMINAS!\n");
	printf("Ingrese cantidad de bombas:\n");
	scanf("%d%c",&cant_bombas,&enter);
	marcadas = cant_bombas;
	exploradas = CANT_FILA * CANT_COL - cant_bombas;
	do{
		//system("clear");
		imprimir(tablero);
		printf("Marcadas: %d", marcadas);
		printf("Exploradas: %d", exploradas);
		printf("Realice su jugada: \n");
		scanf("%c%c%c%c%c", &op, &esp, &filachar, &columnachar, &enter);
		fila = filachar - 'A';
		columna = columnachar - 'A';
		switch(op){
			case 'E':{// Comprueba si es la primer jugada
				if(primerajugada == true){
					// Vuelve a iniciar el tablero por si el usuario marco casillas antes de empezar.
					inicializar(tablero);
					// Pone las bombas por primera y unica vez omitiendo la coordenada ingresada.
					poner_bombas(tablero,cant_bombas,fila,columna);
					explorar(tablero,fila,columna);
					exploradas-= 1;
					primerajugada = false;
					// Si la exploracion no es true, es porque exploto una bomba y se termina el juego.
				}else if((explorar(tablero,fila,columna) != true)){
					imprimir(tablero);
					printf("¡¡HAS PERDIDO LA PARTIDA!!\n");
					fin = true;
				}else{
					// No se exploto una bomba y se muestra el valor de bombas adyacentes.
					explorar(tablero,fila,columna);
					exploradas-= 1;
				}
				break;
				}
			case 'M':{
				marcar(tablero,fila,columna);
				marcadas-= 1;
				break;
				}
			case 'B':{
				break;
				}
			default:{
				}
		}
		if(marcadas == 0 && exploradas == 0){
			imprimir(tablero);
			printf("¡¡HAS GANADO LA PARTIDA!!\n");
			fin = true;
		}
	}while(fin == false);
	/*imprimir(tablero);
	if (marcadas == 0 && exploradas == 0){
		printf("¡¡HAS GANADO LA PARTIDA!!\n");
	}else{
		printf("¡¡HAS PERDIDO LA PARTIDA!!\n");
	}
	*/
}
