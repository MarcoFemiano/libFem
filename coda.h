//
// Created by Spara on 15/04/2026.
//

#ifndef LIBFEM_CODA_H
#define LIBFEM_CODA_H

#define DEFAULT_SIZE 10
#include <stddef.h>

/**
 * @file stack.h
 * @brief Interfaccia pubblica per un ADT Stack generico a memoria contigua.
 *
 * Questo modulo implementa uno stack generico tramite un buffer contiguo
 * allocato dinamicamente. Gli elementi non vengono memorizzati come
 * puntatori esterni, ma come copie raw dei loro byte tramite memcpy().
 *
 * La dimensione di ciascun elemento viene fissata in fase di creazione
 * dello stack tramite il parametro sizeOfEachElement.
 *
 * Il modulo supporta due modalità operative:
 * - stack a capacità fissa
 * - stack dinamico con riallocazione automatica
 *
 * @warning La copia degli elementi è superficiale (shallow copy).
 *          Se un elemento contiene puntatori interni, vengono copiati
 *          solo i valori dei puntatori, non i dati puntati.
 *          Quindi se gli elementi sono allocati dinamicamente la responsabilità
 *          di fare la free sugli elementi è dell'utilizzatore
 */

/**
 * @brief Tipo opaco che rappresenta uno stack generico.
 *
 * La struttura concreta è nascosta nel file stack.c.
 * L'utente può manipolare l'oggetto solo tramite le funzioni pubbliche
 * esposte da questo header.
 */
typedef struct strCoda* Coda;

/**
 * @brief Codice di successo.
 */
#define OK 0

/**
 * @brief Errore: puntatore allo stack nullo.
 */
#define ERROR_NULL_POINTER (-1)

/**
 * @brief Errore: stack vuoto.
 */
#define ERROR_STACK_EMPTY (-2)

/**
 * @brief Errore: stack pieno.
 *
 * Restituito solo nel caso di stack a capacità fissa.
 */
#define ERROR_STACK_FULL (-3)

/**
 * @brief Errore: puntatore ai dati nullo.
 */
#define ERROR_NULL_DATA_POINTER (-4)

/**
 * @brief Errore: fallimento di realloc() o impossibilità di espansione.
 */
#define ERROR_REALLOC_FAIL (-5)

/**
 * @brief Errore: funzione non valida nel caso d'uso corrente.
 */
#define ERROR_FUNCTION_NOT_VALID_IN_THIS_USE_CASE (-6)


Coda coda_create(long long int capacity, size_t sizeOfEachElement);


void coda_destroy(Coda* coda);

short coda_push(Coda coda, const void* datoInput);

short coda_pop(Coda coda, void* datoOutput);


short coda_peek(Coda coda, void* datoOutput);

long long int coda_size(Coda coda);

short coda_is_empty(Coda coda);


short coda_is_full(Coda coda);










#endif // LIBFEM_CODA_H
