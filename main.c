//
// Created by Spara on 16/04/2026.
//
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "coda.h"
#include "stack.h"


//Esercitazione programmazione procedurale II. Prof. Rocco Oliveto

/** Esercizio 1.
 * 1) Per la coda delle auto in attesa che si liberi un posto utilizzerò una coda con array circolare allocato
 * dinamicamente. Partirà con una capienza DEFAULT_CODA_ATTESA_SIZE di 10, definita tramite parametro #define, che verrà incrementata
 * geometricamente ogni volta che sarà riempita.
 *
 * 2) Per definire il parcheggio dove le auto si andranno a posizionare utilizzerò uno stack a capacità statica
 * di DEFAULT_STACK_PARCHEGGIO_SIZE, definita tramite parametro #define, di valore 10 come richiesto dal problema
 *
 * 3) Per definire l'area di manovra dove le auto si sposteranno per permettere l'uscita alle auto parcheggiate utilizzerò
 * di nuovo uno Stack a capacità statica di DEFAULT_STACK_SPAZIO_MANOVRA_SIZE, definita tramite parametro #define,
 * di valore 4 come richiesto dal problema
 *
 *
 * Motivazioni:
 * 1° punto => La scelta più sensata è appunto una coda, il primo ad arrivare è logico sia anche il primo
 * a trovare posto. Un'altra scelta sensata sarebbe potuta essere quella della coda a priorità, ma questo
 * dipende esclusivamente dal tipo di sistema e dai casi di utilizzo che si vogliono realizzare. In questo
 * problema chiaramente non è necessaria questa soluzione.
 *
 * 2° punto => Per motivi fisici legati alla definizione del problema sarebbe irrealistico utilizzare una coda
 * per modellare il parcheggio in una fila. Affinchè le auto sono ancora incapaci di teletrasportarsi o attraversare le pareti
 * l'unica soluzione realistica è l'utilizzo di uno Stack
 *
 * 3° punto => Questo è il punto più delicato del problema. L'esercitazione chiede espressamente e in modo diretto
 * che le auto vengano riposizionate mantenendo l'ordine iniziale. Questo Vincolo è realizzabile utilizzando uno Stack.
 * Se utilizzassi una coda nel riposizionare le auto avrei un ordine invertito delle auto movimentate.
 *
 *
 */

/** Esercizio 2.
 * Per il secondo punto guardare l'interno del file parking.h
 *
 *
 */

int main(void) {









    return 0;
}