/*
 * Tablero.cpp
 * Autor: Juan Pablo García Plaza Pérez (jgarciapft)
 *
 */

#include "Tablero.h"
#include "Celda.h"
#include <stdlib.h>

using namespace std;

void iniciarTablero(Tablero &t, int tamano) {
	t.tamanoTablero = tamano;

	for(int i=0; i<tamano; i++){ //Bucles for anidados para iniciar cada celda del tablero
		for(int j=0; j<tamano; j++){
			iniciarCelda(t.v[i][j]);
		}
	}
}

void insertarFicha(Tablero &t, int pos_x, int pos_y, int valor) {
	bool obs = false; //bandera

	celdaPonerValor(t, pos_x, pos_y, valor);

	for(int i=0; i<obtenerTamanoTablero(t)-1 && !obs; i++){ //Deja caer la ficha
		if(celdaObtenerEstaVacia(t, pos_x, pos_y+i+1)){
			celdaPonerValor(t, pos_x, pos_y+i+1, celdaObtenerValor(t, pos_x, pos_y+i));
			celdaVaciarCelda(t, pos_x, pos_y+i);
		}else{
			obs = true;
		}
	}
}

void eliminarFicha(Tablero &t, int pos_x, int pos_y) {
	bool vacio = false; //bandera

	celdaVaciarCelda(t, pos_x, pos_y);

	for(int i=0; i<obtenerTamanoTablero(t)-1 && !vacio; i++){ //Deja caer las fichas superiores
		if(!celdaObtenerEstaVacia(t, pos_x, pos_y-i-1)){
			celdaPonerValor(t, pos_x, pos_y-i, celdaObtenerValor(t, pos_x, pos_y-i-1));
			celdaVaciarCelda(t, pos_x, pos_y-i-1);
		}
		else{
			vacio = true;
		}
	}
}

bool fichaVoltear(Tablero &t, int pos_x, int pos_y) {
	bool mostrandoAnverso;

	if(!celdaObtenerMostrandoAnverso(t, pos_x, pos_y)){ //Estado presente: ANVERSO
		celdaPonerMostrandoAnverso(t, pos_x , pos_y, true);
		mostrandoAnverso = true; //Estado siguiente: REVERSO
	}else{ //Estado presente: REVERSO
		celdaPonerMostrandoAnverso(t, pos_x, pos_y, false);
		mostrandoAnverso = false; //Estado siguiente: ANVERSO
	}
	return mostrandoAnverso;
}

bool cabeFila(Tablero t) {
	bool cabeFila = true; //bandera

	if(!tableroEstaVacio(t)){
		for(int i=0; i<obtenerTamanoTablero(t) && cabeFila; i++){ //Si hay alguna ficha en la fila superior no caben más filas. Comprueba solo la fila superior
			if(!celdaObtenerEstaVacia(t, i, 0))
				cabeFila = false;
		}
	}
	return cabeFila;
}

void insertarFila(Tablero &t, int semilla) {
	vValores valores; //Vector de valores
	vValores posiciones; //Vector de posiciones de cada valor

	generarVectorUnico(valores, obtenerTamanoTablero(t)/2, MAX_VALOR_FICHA+1, semilla); //Vector de valores únicos que representan una fila nueva. Solo se rellena hasta la mitad
	generarVectorUnico(posiciones, obtenerTamanoTablero(t), obtenerTamanoTablero(t), semilla); //Vector de posiciones que deben ocupar cada ficha

	for(int i=obtenerTamanoTablero(t)/2; i<obtenerTamanoTablero(t); i++){ //Replico la mitad del vector valores sobre sí mismo
		valores[i] = valores[i-obtenerTamanoTablero(t)/2];
	}

	for(int i=0; i<obtenerTamanoTablero(t); i++){ //Inserción de fichas
		insertarFicha(t, i, 0, valores[posiciones[i]]);
	}
}

bool tableroEstaVacio(Tablero t) {
	bool estaVacio = true; //bandera

	for(int i=0; i<obtenerTamanoTablero(t) && estaVacio; i++){ //Si no hay nada en la ultima fila significa que el tablero está vacio
		if(!celdaObtenerEstaVacia(t, i, obtenerTamanoTablero(t)-1))
			estaVacio = false;
	}
	return estaVacio;
}

bool compararFichas(Tablero t, int pos_x1, int pos_y1, int pos_x2, int pos_y2){
	bool iguales = false;

	if(!celdaObtenerEstaVacia(t, pos_x1, pos_y1) && !celdaObtenerEstaVacia(t, pos_x2, pos_y2)) //Si las celdas están vacias devuelve false
		iguales = sonIguales(t.v[pos_y1][pos_x1], t.v[pos_y2][pos_x2]);
	return iguales;
}

int obtenerTamanoTablero(Tablero t) {
	return t.tamanoTablero;
}

void celdaVaciarCelda(Tablero &t, int pos_x, int pos_y){
	vaciarCelda(t.v[pos_y][pos_x]);
}

void celdaPonerValor(Tablero &t, int pos_x, int pos_y, int valor){
	ponerValor(t.v[pos_y][pos_x], valor);
}

void celdaPonerMostrandoAnverso(Tablero &t, int pos_x, int pos_y, bool estado){
	ponerMostrandoAnverso(t.v[pos_y][pos_x], estado);
}

bool celdaObtenerEstaVacia(Tablero t, int pos_x, int pos_y) {
	return obtenerEstaVacia(t.v[pos_y][pos_x]);
}

int celdaObtenerValor(Tablero t, int pos_x, int pos_y) {
	return obtenerValor(t.v[pos_y][pos_x]);
}

bool celdaObtenerMostrandoAnverso(Tablero t, int pos_x, int pos_y) {
	return obtenerMostrandoAnverso(t.v[pos_y][pos_x]);
}

void generarVectorUnico(vValores &vector, int tamano, int rang, int semilla){
	/*
	 *Algoritmo de generación de vectores de valores únicos para la generación de filas de valores emparejados
	 */
	srand(semilla); //Semilla que se actualiza con el tiempo del sistema

	bool enc = false; //bandera
	int valor; //valor generado
	int j; //indice

	for(int i=0; i<tamano; i++){ //Popula vector[] hasta 'tamano'
		valor = rand()%rang;
		for(j=0; j<i && !enc; j++){ //Comprobamos que el valor no esté repetido
			if(vector[j] == valor)
				enc = true;
		}
		if(!enc)
			vector[i] = valor;
		else
			i--; //Si el valor está repetido no cuenta la iteración
		enc = false;
	}
}
