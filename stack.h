//
// Created by Spara on 13/04/2026.
//

#ifndef LIBFEM_STACK_H
#define LIBFEM_STACK_H

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
typedef struct strStack* Stack;

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

/**
 * @brief Crea e inizializza un nuovo stack generico.
 *
 * Lo stack memorizza elementi tutti della stessa dimensione, espressa in byte
 * tramite @p sizeOfEachElement.
 *
 * Modalità di creazione:
 * - Se @p capacity > 0, viene creato uno stack a capacità fissa.
 * - Se @p capacity == 0, viene creato uno stack dinamico con capacità iniziale
 *   interna predefinita e riallocazione automatica in caso di saturazione.
 *
 * Ogni elemento inserito viene copiato nel buffer interno dello stack.
 *
 * @param capacity Capacità iniziale dello stack.
 *        - > 0: stack statico a capacità fissa
 *        - = 0: stack dinamico
 * @param sizeOfEachElement Dimensione, in byte, di ciascun elemento
 *        memorizzato nello stack. In genere si passa sizeof(T).
 *
 * @return Stack Puntatore allo stack creato in caso di successo.
 * @return NULL Se i parametri non sono validi, se si verifica overflow
 *         nel calcolo della memoria richiesta o se l'allocazione fallisce.
 *
 * @pre sizeOfEachElement > 0
 * @pre capacity >= 0
 *
 * @post In caso di successo, lo stack è vuoto.
 *
 * @note Lo stack esegue una copia raw dei dati tramite memcpy().
 * @see stack_push(), stack_destroy()
 */
Stack stack_create(long long int capacity, unsigned int sizeOfEachElement);

/**
 * @brief Distrugge uno stack e libera tutta la memoria associata.
 *
 * Libera:
 * - il buffer interno degli elementi
 * - la struttura dello stack
 *
 * Dopo la distruzione, il puntatore passato viene posto a NULL per evitare
 * dangling pointer.
 *
 * Se il parametro è NULL o se *stack è NULL, la funzione non esegue alcuna azione.
 *
 * @param stack Indirizzo del puntatore allo stack da distruggere.
 *
 * @post Se la funzione termina correttamente, *stack == NULL.
 *
 * @warning La funzione non esegue alcuna logica di distruzione profonda
 *          sugli eventuali puntatori contenuti negli elementi memorizzati.
 */
void stack_destroy(Stack* stack);

/**
 * @brief Inserisce un elemento in cima allo stack.
 *
 * La funzione copia @p sizeOfEachElement byte a partire da @p datoInput
 * nel buffer interno dello stack.
 *
 * Se lo stack è dinamico e risulta pieno, viene tentata una riallocazione
 * con crescita geometrica della capacità.
 *
 * @param stack Stack di destinazione.
 * @param datoInput Puntatore ai dati da inserire.
 *
 * @retval OK Inserimento completato con successo.
 * @retval ERROR_NULL_POINTER Puntatore allo stack nullo.
 * @retval ERROR_NULL_DATA_POINTER Puntatore ai dati nullo.
 * @retval ERROR_STACK_FULL Stack statico pieno.
 * @retval ERROR_REALLOC_FAIL Fallimento della riallocazione o overflow
 *         nel calcolo della nuova dimensione del buffer.
 *
 * @pre stack != NULL
 * @pre datoInput != NULL
 *
 * @note L'elemento viene copiato, non referenziato.
 */
short stack_push(Stack stack, const void* datoInput);

/**
 * @brief Estrae l'elemento in cima allo stack.
 *
 * L'elemento rimosso viene copiato in @p datoOutput.
 *
 * @param stack Stack sorgente.
 * @param datoOutput Buffer di destinazione dove copiare l'elemento estratto.
 *
 * @retval OK Estrazione completata con successo.
 * @retval ERROR_NULL_POINTER Puntatore allo stack nullo.
 * @retval ERROR_NULL_DATA_POINTER Puntatore di output nullo.
 * @retval ERROR_STACK_EMPTY Stack vuoto.
 *
 * @pre stack != NULL
 * @pre datoOutput != NULL
 * @pre lo stack non deve essere vuoto
 *
 * @post In caso di successo, la dimensione logica dello stack diminuisce di 1.
 */
short stack_pop(Stack stack, void* datoOutput);

/**
 * @brief Legge l'elemento in cima allo stack senza rimuoverlo.
 *
 * L'elemento viene copiato in @p datoOutput, ma lo stack non viene modificato.
 *
 * @param stack Stack sorgente.
 * @param datoOutput Buffer di destinazione dove copiare l'elemento letto.
 *
 * @retval OK Lettura completata con successo.
 * @retval ERROR_NULL_POINTER Puntatore allo stack nullo.
 * @retval ERROR_NULL_DATA_POINTER Puntatore di output nullo.
 * @retval ERROR_STACK_EMPTY Stack vuoto.
 *
 * @pre stack != NULL
 * @pre datoOutput != NULL
 * @pre lo stack non deve essere vuoto
 *
 * @post La dimensione logica dello stack rimane invariata.
 */
short stack_peek(Stack stack, void* datoOutput);

/**
 * @brief Restituisce il numero di elementi attualmente presenti nello stack.
 *
 * @param stack Stack da interrogare.
 *
 * @return long long int Numero di elementi presenti nello stack.
 * @return ERROR_NULL_POINTER Se il puntatore allo stack è nullo.
 *
 * @note Il valore restituito è la dimensione logica dello stack, non la capacità.
 */
long long int stack_size(Stack stack);

/**
 * @brief Verifica se lo stack è vuoto.
 *
 * @param stack Stack da interrogare.
 *
 * @retval 1 Lo stack è vuoto.
 * @retval 0 Lo stack non è vuoto.
 * @retval ERROR_NULL_POINTER Puntatore allo stack nullo.
 */
short stack_is_empty(Stack stack);

/**
 * @brief Verifica se lo stack è pieno.
 *
 * Per uno stack a capacità fissa, la funzione restituisce 1 quando
 * size == capacity.
 *
 * Per uno stack dinamico, la funzione restituisce sempre 0, perché
 * la struttura può espandersi automaticamente.
 *
 * @param stack Stack da interrogare.
 *
 * @retval 1 Lo stack è pieno.
 * @retval 0 Lo stack non è pieno.
 * @retval ERROR_NULL_POINTER Puntatore allo stack nullo.
 */
short stack_is_full(Stack stack);

#endif //LIBFEM_STACK_H