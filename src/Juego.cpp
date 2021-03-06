/*
 * Juego.cpp
 * Autor: Juan Pablo García Plaza Pérez (jgarciapft)
 *
 */

#include "Juego.h"
#include "Tablero.h"
#include "entorno.h"
#include "pruebas.h"
#include <time.h>

int tamanoTablero;  				//Guardará el número de filas y columnas del tablero, rol: valor fijo
int tiempoJugada;   				//Guardará el tiempo de una jugada, rol: valor fijo
int puntosPista; 					//Guardara el número de puntos que se restan en algunas ampliaciones, rol: valor fijo
int seg;	  						//Almacena los segundos del cronometro, rol: más reciente
int puntuacion;						//Almacena la puntuación, rol: transformación
string mensaje;  					//Almacena los diferentes mensajes que aparecen durante la ejecucion, rol: valor fijo
bool salir;							//Controla la salida del bucle, rol: bandera
TipoTecla tecla;					//Almacena las teclas pulsadas, rol: mas reciente
int pos_x, pos_y;					//Almacena la posición de la celda seleccionada, rol: transformación
int sel_x1, sel_y1, sel_x2, sel_y2; //Almacenan las posiciones de las dos fichas seleccionadas, rol: transformación
int seleccionadas;					//Almacena el numero de fichas seleccionadas, rol: contador
Tablero t;							//Almacena el tablero en memoria, rol: transformación

void cargarJuego(){
	//Carga la configuración e inicia el tablero
	if(entornoCargarConfiguracion(tamanoTablero, tiempoJugada, puntosPista)){
		if(entornoIniciar(tamanoTablero)){
			iniciarTablero(t, tamanoTablero);
			manejadorJuego(); //Inicia la partida
		}
	}
}

void manejadorJuego(){
	/*
	 * INICIALIZACIÓN
	 */
	pos_x = 0;
	pos_y = tamanoTablero-1;
	entornoActivarCelda(pos_y, pos_x);

	puntuacion = 0;
	entornoPonerPuntuacion(puntuacion);

	salir = false;

	seleccionadas = 0;
	seg = 0;

	//Inserción de filas iniciales
	for(int i=0; i<FILAS_INICIALES; i++){
		insertarFila(t, time(NULL)+i); //El tiempo + i como semilla para que se generen 2 filas distintas en un mismo segundo
	}
	actualizarEntorno(t);

	/*
	 * Bucle que mantiene el procesamiento de la lógica del juego
	 */
	while(!salir){
		/*
		 * Manejador de tiempo
		 */
		entornoTiempo(seg, tiempoJugada); //Se actualizan y muestran los segundos del cronómetro
		if(seg == tiempoJugada){ //Al llegar a 'tiempoJugada' segundos, el cronómetro se vuelve a iniciar
			seg = 0;
			if(cabeFila(t)) //Comprueba si cabe una fila adicional en el tablero
				juegoInsertarFila(t);
			else
				salir = true;
		}

		/*
		 * Manejador de entrada de usuario
		 */
		tecla = entornoLeerTecla();
		switch(tecla) {
			case TEnter:
				switch(seleccionadas){
				case 0:
					if(!celdaObtenerEstaVacia(t, pos_x, pos_y)){ //Se comprueba que la celda no esté vacia
						juegoFichaVoltear(t, pos_x, pos_y);

						sel_x1 = pos_x;
						sel_y1 = pos_y;

						seleccionadas++;
					}
					break;
				case 1:
					if(!celdaObtenerEstaVacia(t, pos_x, pos_y)){ //Se comprueba que la celda no esté vacia
						if(sel_x1 != pos_x || sel_y1 != pos_y){ //Se comprueba que no se seleccione dos veces la misma ficha
							juegoFichaVoltear(t, pos_x, pos_y);

							sel_x2 = pos_x;
							sel_y2 = pos_y;

							seleccionadas++;
						}
					}
					break;
				default:
					break;
				}
				break;
			case TDerecha:
				entornoDesactivarCelda(pos_y, pos_x);
				if (pos_x < tamanoTablero-1)
					pos_x++;
				else
					pos_x = 0;
				entornoActivarCelda(pos_y, pos_x);
				break;
			case TIzquierda:
				entornoDesactivarCelda(pos_y, pos_x);
				if (pos_x > 0)
					pos_x--;
				else
					pos_x = tamanoTablero-1;
				entornoActivarCelda(pos_y, pos_x);
				break;
			case TArriba:
				entornoDesactivarCelda(pos_y, pos_x);
				if (pos_y > 0)
					pos_y--;
				else
					pos_y = tamanoTablero-1;
				entornoActivarCelda(pos_y, pos_x);
				break;
			case TAbajo:
				entornoDesactivarCelda(pos_y, pos_x);
				if (pos_y < tamanoTablero-1)
					pos_y++;
				else
					pos_y = 0;
				entornoActivarCelda(pos_y, pos_x);
				break;
			case TX:
				break;
			case TY:
				break;
			case TZ:
				break;
			case TSalir:
				salir = true;
				break;
			case TNada:
				break;
		}

		/*
		 * Manejador de comparación de fichas
		 */
		if(seleccionadas == 2){
			entornoPausa(TIEMPO_RETRASO_SELECCION); //Pausa para que el jugador vea la comparación

			if(compararFichas(t, sel_x1, sel_y1, sel_x2, sel_y2)){ //Son iguales
				if(sel_y1 < sel_y2){ //Si se elimina primero la ficha inferior la superior cae y se desplaza, por lo que su posicion varia
					juegoEliminarFicha(t, sel_x1, sel_y1);
					juegoEliminarFicha(t, sel_x2, sel_y2);
				}else{
					juegoEliminarFicha(t, sel_x2, sel_y2);
					juegoEliminarFicha(t, sel_x1, sel_y1);
				}

				puntuacion = puntuacion + PTOS_PAREJA; //Se suman los puntos
				entornoPonerPuntuacion(puntuacion);

				/*
				 * Manejador de final por vaciar tablero
				 */
				if(tableroEstaVacio(t))
					salir = true;
			}else{ //Son distintas
				juegoFichaVoltear(t, sel_x1, sel_y1); //Se vuelven a voltear las fichas
				juegoFichaVoltear(t, sel_x2, sel_y2);
			}
			seleccionadas = 0; //Se reinicia el contador
		}
	}

	/*
	 * Manejador de final de juego
	 */
	if(salir)
		terminarJuego(); //Finaliza la partida
}

void terminarJuego() {
	if(tableroEstaVacio(t))
		puntuacion = puntuacion + PTOS_FINAL_TABLERO_VACIO;//Si la partida termina por vaciar el tablero se dan ptos extra

	mensaje = "             Puntuación final : " + toString(puntuacion) + " ptos";
	entornoMostrarMensajeFin(mensaje);

	entornoPausa(TIEMPO_PAUSA_FINAL);
	entornoTerminar();
}

void actualizarEntorno(Tablero &t){
	for(int i=0; i<obtenerTamanoTablero(t); i++){
		for(int j=0; j<obtenerTamanoTablero(t); j++){
			if(!celdaObtenerEstaVacia(t, j, i)){ //Actualiza el estado de la ficha en función de su miembro 'mostrandoAnverso'
				if(celdaObtenerMostrandoAnverso(t, j, i))
					entornoFichaAnverso(i, j, celdaObtenerValor(t, j, i));
				else
					entornoFichaReves(i, j);
			}else if(celdaObtenerValor(t, j, i) != VALOR_PREDEFINIDO){ //Elimina las fichas que fueron borradas de la memoria del tablero
				entornoBorrarCelda(i, j);
			}
		}
	}
}

void juegoInsertarFila(Tablero &t){
	insertarFila(t, time(NULL)); //El timepo como semilla

	/*
	 * Hace que aparezca una fila de fichas mostrando el reverso en la parte superior del tablero
	 * 		y después de un pequeño retraso desaparece para hacer la ilusión de que cae hacia abajo
	 */
	for(int i=0; i<tamanoTablero; i++){
		entornoFichaReves(0, i);
	}
	entornoPausa(TIEMPO_INSERCION_FILA); //Retraso antes del borrado de la fila
	for(int i=0; i<tamanoTablero; i++){
		entornoBorrarCelda(0, i);
	}

	actualizarEntorno(t);
}

void juegoEliminarFicha(Tablero &t, int pos_x, int pos_y){
	if(!celdaObtenerEstaVacia(t, pos_x, pos_y)){ //Si la celda esta vacia no hace nada
		eliminarFicha(t, pos_x, pos_y);
		actualizarEntorno(t);
	}
}

void juegoFichaVoltear(Tablero &t, int pos_x, int pos_y){
	if(!celdaObtenerEstaVacia(t, pos_x, pos_y)){ //Si la celda esta vacia no hace nada
		if(fichaVoltear(t, pos_x, pos_y)){ //Animación de volteado de la ficha
			entornoFichaCanto(pos_y, pos_x);
			entornoPausa(TIEMPO_TRANSICION_ESTADOS);
			entornoFichaAnverso(pos_y, pos_x, celdaObtenerValor(t, pos_x, pos_y));
		}else{
			entornoFichaCanto(pos_y, pos_x);
			entornoPausa(TIEMPO_TRANSICION_ESTADOS-0.15); //La animación de ANVERSO a REVERSO es más corta
			entornoFichaReves(pos_y, pos_x);
		}
	}
}

int main(){
	//Llamada a todas las pruebas
	//pruebas();
	cargarJuego();
}
