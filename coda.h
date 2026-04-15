//
// Created by Spara on 15/04/2026.
//

#ifndef LIBFEM_CODA_H
#define LIBFEM_CODA_H

#include <stddef.h>

/**
 * @file coda.h
 * @brief Interfaccia pubblica per un ADT Coda generico a memoria contigua.
 *
 * Questo modulo implementa una coda FIFO (First In, First Out) generica
 * tramite un buffer contiguo allocato dinamicamente.
 *
 * Gli elementi non vengono memorizzati come puntatori esterni, ma come copie
 * raw dei loro byte tramite memcpy().
 *
 * La dimensione di ciascun elemento viene fissata in fase di creazione
 * della coda tramite il parametro sizeOfEachElement.
 *
 * Il modulo supporta due modalità operative:
 * - coda a capacità fissa
 * - coda dinamica con espansione automatica
 *
 * La modalità viene scelta in fase di creazione:
 * - se capacity > 0, la coda è a capacità fissa
 * - se capacity == 0, la coda è dinamica e viene inizializzata con DEFAULT_SIZE
 *
 * Internamente la coda è implementata come array circolare.
 *
 * @warning La copia degli elementi è superficiale (shallow copy).
 *          Se un elemento contiene puntatori interni, vengono copiati
 *          solo i valori dei puntatori, non i dati puntati.
 *          La gestione dell'eventuale memoria dinamica interna agli elementi
 *          resta quindi responsabilità dell'utilizzatore.
 */

/**
 * @brief Capacità iniziale usata per le code dinamiche.
 *
 * Se in coda_create() viene passato capacity == 0, la coda viene creata
 * in modalità dinamica con questa capacità iniziale.
 */
#define DEFAULT_SIZE 10

/**
 * @brief Tipo opaco che rappresenta una coda generica.
 *
 * La struttura concreta è nascosta nel file coda.c.
 * L'utente può manipolare la coda esclusivamente tramite le funzioni
 * pubbliche esposte da questo header.
 */
typedef struct strCoda* Coda;

/**
 * @brief Codice di successo.
 */
#define OK 0

/**
 * @brief Errore: puntatore nullo.
 *
 * Restituito quando un parametro puntatore richiesto è NULL.
 */
#define ERROR_NULL_POINTER (-1)

/**
 * @brief Errore: coda vuota.
 *
 * Restituito quando si tenta di leggere o rimuovere un elemento
 * da una coda priva di elementi.
 */
#define ERROR_CODA_EMPTY (-2)

/**
 * @brief Errore: coda piena.
 *
 * Restituito solo nel caso di coda a capacità fissa quando si tenta
 * di inserire un elemento oltre la capacità massima.
 */
#define ERROR_CODA_FULL (-3)

/**
 * @brief Errore: puntatore ai dati nullo.
 *
 * Può essere usato da implementazioni che distinguono il caso di
 * puntatore all'ADT nullo dal caso di puntatore ai dati nullo.
 *
 * @note Nella implementazione attuale questo codice non è utilizzato.
 */
#define ERROR_NULL_DATA_POINTER (-4)

/**
 * @brief Errore: fallimento di riallocazione o impossibilità di espansione.
 *
 * Restituito nel caso di coda dinamica quando l'espansione fallisce
 * oppure quando un controllo di overflow impedisce di allocare
 * ulteriore memoria in sicurezza.
 */
#define ERROR_REALLOC_FAIL (-5)

/**
 * @brief Errore: funzione non valida nel caso d'uso corrente.
 *
 * Restituito quando una funzione non ha significato per la modalità
 * operativa della coda.
 *
 * Esempio: coda_is_full() su una coda dinamica.
 */
#define ERROR_FUNCTION_NOT_VALID_IN_THIS_USE_CASE (-6)

/**
 * @brief Crea una nuova coda generica.
 *
 * Se capacity è maggiore di zero, viene creata una coda a capacità fissa.
 * Se capacity è uguale a zero, viene creata una coda dinamica con capacità
 * iniziale pari a DEFAULT_SIZE.
 *
 * Ogni elemento inserito nella coda viene copiato per byte, secondo la
 * dimensione specificata da sizeOfEachElement.
 *
 * @param capacity Capacità iniziale della coda.
 *                 - Se > 0: coda statica
 *                 - Se == 0: coda dinamica
 * @param sizeOfEachElement Dimensione in byte di ciascun elemento.
 *
 * @return Puntatore alla coda appena creata in caso di successo.
 * @return NULL in caso di parametri non validi o fallimento di allocazione.
 */
Coda coda_create(long long int capacity, size_t sizeOfEachElement);

/**
 * @brief Distrugge una coda e libera tutta la memoria associata.
 * MA NON GLI ELEMENTI INSERITI!
 *
 * Dopo la distruzione, il puntatore viene posto a NULL.
 *
 * @param coda Indirizzo del puntatore alla coda da distruggere.
 *
 * @return OK in caso di successo.
 * @return ERROR_NULL_POINTER se il puntatore al puntatore è NULL
 *         oppure se la coda è già NULL.
 */
short coda_destroy(Coda* coda);

/**
 * @brief Inserisce un elemento in coda.
 *
 * L'elemento viene copiato nella posizione di tail secondo il principio FIFO.
 *
 * Nel caso di coda dinamica, se il buffer è pieno, la funzione tenta
 * automaticamente di espandere la capacità.
 *
 * Nel caso di coda statica, se la coda è piena, la funzione fallisce.
 *
 * @param coda Coda di destinazione.
 * @param datoInput Puntatore all'elemento da inserire.
 *
 * @return OK in caso di successo.
 * @return ERROR_NULL_POINTER se coda o datoInput sono NULL.
 * @return ERROR_CODA_FULL se la coda è statica ed è piena.
 * @return ERROR_REALLOC_FAIL se la coda è dinamica ma l'espansione fallisce.
 */
short coda_push(Coda coda, const void* datoInput);

/**
 * @brief Rimuove l'elemento in testa alla coda e lo copia in output.
 *
 * L'elemento estratto è il più vecchio tra quelli presenti nella coda,
 * secondo la semantica FIFO.
 *
 * @param coda Coda da cui estrarre l'elemento.
 * @param datoOutput Buffer di destinazione in cui copiare l'elemento rimosso.
 *
 * @return OK in caso di successo.
 * @return ERROR_NULL_POINTER se coda o datoOutput sono NULL.
 * @return ERROR_CODA_EMPTY se la coda è vuota.
 */
short coda_pop(Coda coda, void* datoOutput);

/**
 * @brief Legge l'elemento in testa alla coda senza rimuoverlo.
 *
 * La funzione copia l'elemento corrente di testa nel buffer di output
 * senza modificare lo stato interno della coda.
 *
 * @param coda Coda da ispezionare.
 * @param datoOutput Buffer di destinazione in cui copiare l'elemento letto.
 *
 * @return OK in caso di successo.
 * @return ERROR_NULL_POINTER se coda o datoOutput sono NULL.
 * @return ERROR_CODA_EMPTY se la coda è vuota.
 */
short coda_peek(Coda coda, void* datoOutput);

/**
 * @brief Restituisce il numero di elementi attualmente presenti nella coda.
 *
 * @param coda Coda da interrogare.
 *
 * @return Numero di elementi presenti nella coda.
 * @return ERROR_NULL_POINTER se coda è NULL.
 *
 * @note Questa funzione restituisce un valore di dimensione oppure un codice
 *       di errore negativo. L'utilizzatore deve quindi distinguere i due casi.
 */
long long int coda_size(Coda coda);

/**
 * @brief Verifica se la coda è vuota.
 *
 * @param coda Coda da interrogare.
 *
 * @return 1 se la coda è vuota.
 * @return 0 se la coda non è vuota.
 * @return ERROR_NULL_POINTER se coda è NULL.
 */
short coda_is_empty(Coda coda);

/**
 * @brief Verifica se la coda è piena.
 *
 * Questa operazione ha significato solo per una coda a capacità fissa.
 *
 * Per una coda dinamica la funzione non è valida, poiché la struttura
 * può espandersi automaticamente.
 *
 * @param coda Coda da interrogare.
 *
 * @return 1 se la coda statica è piena.
 * @return 0 se la coda statica non è piena.
 * @return ERROR_NULL_POINTER se coda è NULL.
 * @return ERROR_FUNCTION_NOT_VALID_IN_THIS_USE_CASE se la coda è dinamica.
 */
short coda_is_full(Coda coda);

/**
 * @brief Svuota logicamente la coda senza liberare il buffer.
 *
 * Dopo questa operazione la coda risulta vuota, ma la memoria già allocata
 * resta disponibile per successivi inserimenti.
 *
 * @param coda Coda da azzerare.
 *
 * @return OK in caso di successo.
 * @return ERROR_NULL_POINTER se coda è NULL.
 */
short coda_clear(Coda coda);

#endif // LIBFEM_CODA_H