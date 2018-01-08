/*
 * Juego.h
 * Autor: Juan Pablo García Plaz Pérez (jgarciapft)
 *
 * Descripción: TAD que maneja el la lógica del juego, la puntuación
 * 					y el entorno gráfico
 *
 */

#ifndef JUEGO_H_
#define JUEGO_H_

#include "Tablero.h"

const int FILAS_INICIALES = 2;
const int PTOS_PAREJA = 10; //ptos obtenidos al hacer pareja
const int PTOS_FINAL_TABLERO_VACIO = 50; //ptos de bonificación por vaciar el tablero
const float TIEMPO_TRANSICION_ESTADOS = 0.25; //retraso entre estados de la animación de voltear fichas
const float TIEMPO_RETRASO_SELECCION = 0.5; //tiempo que se le deja al usuario ver el anverso de ambas fichas

/*
 * PRE:
 * POST: Punto de entrada del programa. Llama a todas las pruebas y carga la configuración
 * 	inicial para iniciar el entorno gráfico. Si se realiza correctamente llama al
 * 	siguiente módulo 'manejadorJuego()'.
 * 	COMPLEJIDAD: O(1) //Sin contar las pruebas.
 */
int main();


/*
 * PRE:
 * POST: Módulo que maneja toda la lógica del juego: entrada del usuario, manejo del tablero,
 *  manejo del cronómetro, de la puntuación y de la finalización de la partida.
 * 	Una vez la partida acabe se llama al módulo 'terminarJuego()' para terminar
 * 	el entorno gráfico y liberar los recursos asociados.
 * 	COMPLEJIDAD:
 */
void manejadorJuego();


/*
 * PRE:
 * POST: Módulo que termina el entorno gráfico y libera los recursos asociados. También da
 * 	un mensaje de despedida con la puntuación obtenida.
 * 	COMPLEJIDAD: O(1)
 */
void terminarJuego();


/*
 * PRE:
 * POST: Módulo para actualizar el entorno gráfico del tablero
 * COMPLEJIDAD: n²
 */
void actualizarEntorno(Tablero &t);


/*
 * PRE: Cabe una fila más en el Tablero 't'
 * POST: Módulo que insertar una fila en el tablero y actualiza el entorno grafico
 * COMPLEJIDAD: O(n*n²) //por actualizarEntorno()
 */
void juegoInsertarFila(Tablero &t);


/*
 * PRE: 'pos_x' y 'pos_y' dentro de los límites del Tablero 't'
 * POST: Módulo que elimina una ficha y actualiza el entorno gráfico
 * COMPLEJIDAD: O(n*n²) //por actualizarEntorno()
 */
void juegoEliminarFicha(Tablero &t, int pos_x, int pos_y);


/*
 * PRE: pos_x' y 'pos_y' dentro de los límites del Tablero 't'
 * POST: Módulo que voltea una ficha y actualiza el entorno gráfico
 * COMPLEJIDAD: O(1*n²) //por actualizarEntorno()
 */

void juegoFichaVoltear(Tablero &t, int pos_x, int pos_y);

#endif /* JUEGO_H_ */
