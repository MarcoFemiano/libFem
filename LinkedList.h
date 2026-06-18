//
// Created by Spara on 16/06/2026.
//

/**
 * @file LinkedList.h
 * @brief ADT lista semplicemente collegata generica con dati salvati by pointer.
 *
 * Questo modulo implementa una lista collegata con inserimento in testa, in
 * coda e in posizione indicizzata. La struttura concreta della lista e dei nodi
 * resta privata nel file `.c`: l'utilizzatore lavora solo con l'handle opaco
 * `LinkedList`.
 *
 * --------------------------------------------------------------------------
 * GUIDA ALL'UTILIZZO DELL'API
 * --------------------------------------------------------------------------
 *
 * 1) Creare la lista.
 *
 *    `linkedList_create()` restituisce direttamente l'handle della lista.
 *    In caso di fallimento dell'allocazione restituisce `NULL`, quindi il
 *    controllo va fatto subito.
 * @code
 * LinkedList list = linkedList_create();
 * if (list == NULL) {
 *     return 1;
 * }
 * @endcode
 *
 * 2) Preparare i dati da inserire.
 *
 *    Questa lista e' by-pointer: ogni nodo salva esattamente il puntatore
 *    passato dall'utilizzatore. Il dato puntato deve quindi restare valido
 *    finche' il nodo rimane nella lista.
 * @code
 * int a = 10;
 * int b = 20;
 *
 * linkedList_add_head(list, &a);
 * linkedList_add_tail(list, &b);
 * @endcode
 *
 *    Sono rifiutati puntatori `NULL`: non e' possibile inserire un valore NULL
 *    come dato applicativo.
 *
 * 3) Inserire in una posizione precisa.
 *
 *    `linkedList_add_at()` accetta indici da 0 a `size` inclusi:
 *    - indice 0 significa inserimento in testa;
 *    - indice `size` significa inserimento in coda;
 *    - gli indici intermedi inseriscono il nodo prima dell'elemento che prima
 *      occupava quella posizione.
 * @code
 * linkedList_add_at(list, &a, 0);
 * linkedList_add_at(list, &b, (int)linkedList_size(list));
 * @endcode
 *
 * 4) Leggere stato e dimensione.
 *
 *    Le funzioni `linkedList_isEmpty()` e `linkedList_size()` sono difensive:
 *    se ricevono una lista NULL, restituiscono rispettivamente `true` e 0.
 * @code
 * if (!linkedList_isEmpty(list)) {
 *     size_t n = linkedList_size(list);
 * }
 * @endcode
 *
 * 5) Cercare valori.
 *
 *    La ricerca per valore richiede una callback `cmp`. La lista non e'
 *    ordinata: la callback serve solo a dire se il nodo corrente contiene il
 *    valore cercato. La funzione restituisce il puntatore salvato nel nodo.
 * @code
 * int key = 10;
 * int *found = linkedList_search(list, int_cmp, &key);
 * if (found != NULL) {
 *     // trovato //
 * }
 * @endcode
 *
 *    La lettura per indice restituisce il puntatore in posizione `index`.
 *    Gli indici validi vanno da 0 a `size - 1`.
 * @code
 * int *first = linkedList_search_index(list, 0);
 * @endcode
 *
 * 6) Rimuovere nodi.
 *
 *    Le rimozioni liberano il nodo interno, non il dato applicativo puntato.
 *    Se il dato era allocato dinamicamente, l'utilizzatore deve conservarne il
 *    puntatore e liberarlo al momento corretto.
 * @code
 * linkedList_remove_head(list);
 * linkedList_remove_tail(list);
 * linkedList_remove_at(list, 1);
 * linkedList_remove_val(list, int_cmp, &key);
 * @endcode
 *
 * 7) Svuotare o distruggere.
 *
 *    `linkedList_clear()` elimina tutti i nodi ma lascia la lista riutilizzabile.
 *    `linkedList_destroy()` elimina i nodi, elimina la struttura principale e
 *    imposta l'handle a `NULL`.
 * @code
 * linkedList_clear(list);
 * linkedList_add_tail(list, &a);
 * linkedList_destroy(&list);
 * @endcode
 *
 * 8) Gestire gli errori.
 *
 *    Le funzioni che modificano la lista restituiscono `LinkedList_statusCodes`.
 *    La funzione `linkedList_print_statusCodes()` e' pensata per debug/log e
 *    restituisce una stringa statica.
 * @code
 * LinkedList_statusCodes res = linkedList_add_tail(list, &a);
 * if (res != LINKED_LIST_OK) {
 *     const char *msg = linkedList_print_statusCodes(res);
 * }
 * @endcode
 *
 * --------------------------------------------------------------------------
 * INVARIANTI DELL'ADT
 * --------------------------------------------------------------------------
 *
 * Le seguenti regole devono restare vere dopo ogni funzione pubblica che
 * ritorna `LINKED_LIST_OK`.
 *
 * Stato vuoto:
 * - `size == 0`;
 * - `head == NULL`;
 * - `tail == NULL`.
 *
 * Stato non vuoto:
 * - `size > 0`;
 * - `head != NULL`;
 * - `tail != NULL`;
 * - partendo da `head` e seguendo `next` si visitano esattamente `size` nodi;
 * - l'ultimo nodo visitato e' `tail`;
 * - `tail->next == NULL`.
 *
 * Caso con un solo nodo:
 * - `size == 1`;
 * - `head == tail`;
 * - `head->next == NULL`.
 *
 * Nodi e valori:
 * - ogni nodo appartiene a una sola lista;
 * - ogni nodo contiene un `val != NULL`;
 * - la lista non possiede il dato puntato da `val`;
 * - non esiste alcun vincolo di ordinamento;
 * - i duplicati sono ammessi, sia come stesso puntatore sia come valori
 *   logicamente equivalenti secondo `cmp`.
 *
 * Fallimenti:
 * - un errore di allocazione non deve incrementare `size`;
 * - un indice invalido non deve modificare la lista;
 * - una ricerca fallita non modifica mai la lista;
 * - `linkedList_clear()` deve lasciare una lista vuota ma ancora valida;
 * - `linkedList_destroy()` deve impostare l'handle dell'utilizzatore a `NULL`.
 *
 * --------------------------------------------------------------------------
 * GESTIONE DELLA MEMORIA
 * --------------------------------------------------------------------------
 *
 * La lista possiede e libera solo i nodi interni.
 * I valori `void *val` sono puntatori applicativi salvati cosi' come vengono
 * ricevuti: non vengono copiati, duplicati o liberati da `linkedList_destroy`,
 * `linkedList_clear` o dalle funzioni di rimozione.
 *
 * Quindi l'utilizzatore deve garantire che i dati puntati restino validi finche'
 * sono presenti nella lista e deve liberarli autonomamente, se sono stati
 * allocati dinamicamente.
 *
 * --------------------------------------------------------------------------
 * DUPLICATI
 * --------------------------------------------------------------------------
 *
 * La lista non impone unicita': e' possibile inserire piu' volte puntatori o
 * valori logicamente equivalenti. Le funzioni di ricerca e rimozione per valore
 * agiscono sul primo nodo che la funzione `cmp` considera uguale.
 * L'inserimento non controlla mai la presenza di duplicati.
 *
 * --------------------------------------------------------------------------
 * CONTRATTO DELLA FUNZIONE cmp
 * --------------------------------------------------------------------------
 *
 * Le funzioni che ricevono `cmp` considerano trovato un valore quando:
 *
 * @code
 * cmp(valore_nel_nodo, valore_cercato) == 0
 * @endcode
 *
 * Gli altri valori restituiti da `cmp` non hanno significato ordinante per la
 * lista: servono solo a dire che il nodo corrente non corrisponde alla chiave.
 * La callback non deve liberare o invalidare i dati presenti nella lista mentre
 * la ricerca o la rimozione per valore e' in corso.
 *
 * --------------------------------------------------------------------------
 * REGOLE SUGLI INDICI
 * --------------------------------------------------------------------------
 *
 * Gli indici sono interi `int`.
 * - Per `linkedList_add_at`, sono validi gli indici da 0 a `size` inclusi.
 * - Per `linkedList_remove_at` e `linkedList_search_index`, sono validi gli
 *   indici da 0 a `size - 1`.
 * - Gli indici negativi sono rifiutati dalle funzioni che restituiscono
 *   `LinkedList_statusCodes`; nelle funzioni che restituiscono puntatore,
 *   producono `NULL`.
 *
 * --------------------------------------------------------------------------
 * CODICI DI STATO
 * --------------------------------------------------------------------------
 *
 * - `LINKED_LIST_OK`: operazione completata. Vale 0 come `OK` comune.
 * - `LINKED_LIST_VALUE_FOUND`: stato positivo disponibile per indicare valore
 *   trovato; l'implementazione attuale delle ricerche pubbliche restituisce
 *   direttamente un puntatore invece di questo codice.
 * - `LINKED_LIST_VALUE_NOT_FOUND`: valore non trovato.
 * - `LINKED_LIST_ITS_ALREADY_NULL`: stato interno gia' nullo, usato ad esempio
 *   quando la coda risulta NULL in rimozione.
 * - `LINKED_LIST_ERROR_NULL_POINTER`: parametro obbligatorio NULL o struttura
 *   non utilizzabile.
 * - `LINKED_LIST_ERROR_ALLOCATION_FAILURE`: allocazione di un nodo o struttura
 *   fallita.
 * - `LINKED_LIST_ARITHMETIC_OVERFLOW`: incremento di `size` non rappresentabile.
 * - `LINKED_LIST_NEGATIVE_INDEX`: indice negativo.
 * - `LINKED_LIST_SEMANTIC_CONTRADDICTION`: stato interno incoerente rispetto
 *   alle invarianti.
 * - `LINKED_LIST_INDEX_POINTING_TO_AN_UNCHAINED_NODE`: codice difensivo per un
 *   indice che punterebbe fuori dalla catena; non e' restituito dal flusso
 *   pubblico corrente.
 * - `LINKED_LIST_INVALID_INDEX`: indice fuori intervallo.
 * - `LINKED_LIST_EMPTY_LIST`: operazione non applicabile a lista vuota.
 *
 * --------------------------------------------------------------------------
 * ESEMPIO COMPLETO MINIMO
 * --------------------------------------------------------------------------
 * @code
 * #include "LinkedList.h"
 * #include <stdlib.h>
 *
 * static int int_cmp(const void *a, const void *b) {
 *     const int *x = a;
 *     const int *y = b;
 *
 *     if (x == NULL || y == NULL) return -1;
 *     if (*x == *y) return 0;
 *     return *x < *y ? -1 : 1;
 * }
 *
 * int main(void) {
 *     LinkedList list = linkedList_create();
 *     if (list == NULL) return 1;
 *
 *     int *value = malloc(sizeof(int));
 *     if (value == NULL) {
 *         linkedList_destroy(&list);
 *         return 1;
 *     }
 *
 *     *value = 42;
 *     if (linkedList_add_tail(list, value) != LINKED_LIST_OK) {
 *         free(value);
 *         linkedList_destroy(&list);
 *         return 1;
 *     }
 *
 *     int key = 42;
 *     int *found = linkedList_search(list, int_cmp, &key);
 *     int result = found == NULL ? 1 : 0;
 *
 *     linkedList_destroy(&list);
 *     free(value);
 *
 *     return result;
 * }
 * @endcode
 */

#ifndef LIBFEM_LINKEDLIST_H
#define LIBFEM_LINKEDLIST_H

#include "libfem_common.h"

/**
 * @brief Handle opaco della lista collegata.
 *
 * La struttura concreta e i nodi sono definiti solo in `LinkedList.c`.
 */
typedef struct strLinkedList* LinkedList;

/**
 * @brief Codici di stato specifici della lista.
 *
 * `LINKED_LIST_OK` vale 0 come `OK` comune. I valori negativi rappresentano
 * errori veri. I valori positivi rappresentano stati particolari non
 * necessariamente fatali, come valore non trovato o struttura gia' nulla.
 */
typedef enum {
    //status
    LINKED_LIST_OK = 0,
    LINKED_LIST_VALUE_FOUND = 10,
    LINKED_LIST_VALUE_NOT_FOUND = 20,
    LINKED_LIST_ITS_ALREADY_NULL = 21,
    //errors
    LINKED_LIST_ERROR_NULL_POINTER = -1,
    LINKED_LIST_ERROR_ALLOCATION_FAILURE = -2,
    LINKED_LIST_ARITHMETIC_OVERFLOW = -3,
    LINKED_LIST_NEGATIVE_INDEX = -4,
    LINKED_LIST_SEMANTIC_CONTRADDICTION = -5,
    LINKED_LIST_INDEX_POINTING_TO_AN_UNCHAINED_NODE = -6,
    LINKED_LIST_INVALID_INDEX = -7,
    LINKED_LIST_EMPTY_LIST = -8


}LinkedList_statusCodes;

/**
 * @brief Restituisce una stringa descrittiva per un codice di stato.
 *
 * La stringa restituita e' statica e non deve essere liberata dall'utilizzatore.
 *
 * @param[in] statusCodes codice di stato da convertire in testo
 * @return stringa statica descrittiva del codice ricevuto
 */
const char* linkedList_print_statusCodes(LinkedList_statusCodes statusCodes);

/**
 * @brief Crea una nuova lista vuota.
 *
 * Alloca la struttura principale, inizializza testa e coda a `NULL` e imposta
 * la dimensione logica a 0.
 *
 * @return handle della lista in caso di successo, `NULL` in caso di errore di
 *         allocazione
 */
LinkedList linkedList_create(void);

/**
 * @brief Distrugge la lista e tutti i suoi nodi interni.
 *
 * La funzione libera i nodi e la struttura principale, ma NON libera i dati
 * applicativi puntati dai nodi. Dopo la distruzione, `*linkedList` viene
 * impostato a `NULL`.
 *
 * @param[in,out] linkedList indirizzo dell'handle della lista da distruggere
 *
 * @retval LINKED_LIST_OK distruzione completata con successo
 * @retval LINKED_LIST_ERROR_NULL_POINTER `linkedList` o `*linkedList` sono NULL
 */
LinkedList_statusCodes linkedList_destroy(LinkedList *linkedList);

/**
 * @brief Svuota la lista liberando tutti i nodi interni.
 *
 * I dati applicativi puntati dai nodi non vengono liberati. Dopo la pulizia la
 * lista resta valida e riutilizzabile, con dimensione 0.
 *
 * @param[in,out] linkedList lista da svuotare
 *
 * @retval LINKED_LIST_OK lista svuotata correttamente
 * @retval LINKED_LIST_ERROR_NULL_POINTER `linkedList` e' NULL
 */
LinkedList_statusCodes linkedList_clear(LinkedList linkedList);

/**
 * @brief Inserisce un valore in testa alla lista.
 *
 * Il puntatore `val` viene salvato cosi' com'e', senza copiare il dato puntato.
 * Se la lista era vuota, il nuovo nodo diventa sia testa sia coda.
 *
 * @param[in,out] linkedList lista di destinazione
 * @param[in] val puntatore applicativo da memorizzare
 *
 * @retval LINKED_LIST_OK inserimento completato con successo
 * @retval LINKED_LIST_ERROR_NULL_POINTER `linkedList` o `val` sono NULL
 * @retval LINKED_LIST_ARITHMETIC_OVERFLOW incremento della dimensione non sicuro
 * @retval LINKED_LIST_ERROR_ALLOCATION_FAILURE impossibile allocare il nuovo nodo
 */
LinkedList_statusCodes linkedList_add_head(LinkedList linkedList, void* val);

/**
 * @brief Inserisce un valore in coda alla lista.
 *
 * Il puntatore `val` viene salvato senza copiare il dato puntato. Se la lista
 * era vuota, il nuovo nodo diventa sia testa sia coda.
 *
 * @param[in,out] linkedList lista di destinazione
 * @param[in] val puntatore applicativo da memorizzare
 *
 * @retval LINKED_LIST_OK inserimento completato con successo
 * @retval LINKED_LIST_ERROR_NULL_POINTER `linkedList` o `val` sono NULL
 * @retval LINKED_LIST_ARITHMETIC_OVERFLOW incremento della dimensione non sicuro
 * @retval LINKED_LIST_ERROR_ALLOCATION_FAILURE impossibile allocare il nuovo nodo
 */
LinkedList_statusCodes linkedList_add_tail(LinkedList linkedList,void* val);

/**
 * @brief Inserisce un valore alla posizione indicata.
 *
 * Sono validi gli indici da 0 a `linkedList_size(linkedList)` inclusi.
 * L'inserimento in posizione 0 equivale a `linkedList_add_head`; l'inserimento
 * in posizione `size` equivale a `linkedList_add_tail`.
 *
 * Il puntatore `val` viene salvato senza copiare il dato puntato.
 *
 * @param[in,out] linkedList lista di destinazione
 * @param[in] val puntatore applicativo da memorizzare
 * @param[in] index posizione in cui inserire il nuovo nodo
 *
 * @retval LINKED_LIST_OK inserimento completato con successo
 * @retval LINKED_LIST_ERROR_NULL_POINTER `linkedList` o `val` sono NULL
 * @retval LINKED_LIST_NEGATIVE_INDEX `index` e' negativo
 * @retval LINKED_LIST_INVALID_INDEX `index` e' maggiore della dimensione
 * @retval LINKED_LIST_SEMANTIC_CONTRADDICTION stato interno incoerente
 * @retval LINKED_LIST_ARITHMETIC_OVERFLOW incremento della dimensione non sicuro
 * @retval LINKED_LIST_ERROR_ALLOCATION_FAILURE impossibile allocare il nuovo nodo
 */
LinkedList_statusCodes linkedList_add_at(LinkedList linkedList,void* val, int index);

/**
 * @brief Rimuove il nodo in testa alla lista.
 *
 * Viene liberato solo il nodo interno. Il dato applicativo puntato dal nodo non
 * viene liberato ne' restituito.
 *
 * @param[in,out] linkedList lista da modificare
 *
 * @retval LINKED_LIST_OK rimozione completata con successo
 * @retval LINKED_LIST_ERROR_NULL_POINTER `linkedList` e' NULL oppure la lista
 *         non ha una testa valida
 */
LinkedList_statusCodes linkedList_remove_head(LinkedList linkedList);

/**
 * @brief Rimuove il nodo in coda alla lista.
 *
 * Viene liberato solo il nodo interno. Il dato applicativo puntato dal nodo non
 * viene liberato ne' restituito.
 *
 * @param[in,out] linkedList lista da modificare
 *
 * @retval LINKED_LIST_OK rimozione completata con successo
 * @retval LINKED_LIST_ERROR_NULL_POINTER `linkedList` e' NULL oppure la lista
 *         non ha una testa valida
 * @retval LINKED_LIST_ITS_ALREADY_NULL la coda risulta gia' NULL
 * @retval LINKED_LIST_SEMANTIC_CONTRADDICTION stato interno incoerente
 */
LinkedList_statusCodes linkedList_remove_tail(LinkedList linkedList);

/**
 * @brief Rimuove il nodo alla posizione indicata.
 *
 * Sono validi gli indici da 0 a `size - 1`. Viene liberato solo il nodo interno;
 * il dato applicativo non viene liberato ne' restituito.
 *
 * @param[in,out] linkedList lista da modificare
 * @param[in] index posizione del nodo da rimuovere
 *
 * @retval LINKED_LIST_OK rimozione completata con successo
 * @retval LINKED_LIST_ERROR_NULL_POINTER `linkedList` e' NULL
 * @retval LINKED_LIST_NEGATIVE_INDEX `index` e' negativo
 * @retval LINKED_LIST_INVALID_INDEX `index` e' fuori dalla lista
 * @retval LINKED_LIST_SEMANTIC_CONTRADDICTION stato interno incoerente
 */
LinkedList_statusCodes linkedList_remove_at(LinkedList linkedList, int index);

/**
 * @brief Rimuove il primo nodo che contiene un valore equivalente a `val`.
 *
 * La ricerca usa `cmp(nodo->val, val) == 0`. Viene rimosso al massimo un nodo:
 * il primo incontrato partendo dalla testa. Il dato applicativo puntato dal nodo
 * non viene liberato.
 *
 * @param[in,out] linkedList lista da modificare
 * @param[in] cmp funzione di confronto usata per trovare il valore
 * @param[in] val chiave logica da cercare
 *
 * @retval LINKED_LIST_OK rimozione completata con successo
 * @retval LINKED_LIST_ERROR_NULL_POINTER `linkedList`, `cmp` o `val` sono NULL
 * @retval LINKED_LIST_EMPTY_LIST la lista non contiene nodi
 * @retval LINKED_LIST_VALUE_NOT_FOUND nessun valore equivalente e' stato trovato
 */
LinkedList_statusCodes linkedList_remove_val(LinkedList linkedList,int (*cmp)(const void*, const void*), void* val);

/**
 * @brief Cerca il primo valore equivalente a `val`.
 *
 * La ricerca usa `cmp(nodo->val, val) == 0`. La funzione restituisce il
 * puntatore salvato nel nodo, non una copia del dato.
 *
 * @param[in] linkedList lista in cui cercare
 * @param[in] cmp funzione di confronto usata per trovare il valore
 * @param[in] val chiave logica da cercare
 *
 * @return puntatore al valore trovato, oppure `NULL` se gli argomenti non sono
 *         validi, la lista e' vuota o nessun nodo corrisponde
 */
void* linkedList_search(LinkedList linkedList, int (*cmp)(const void*, const void*), void* val);

/**
 * @brief Restituisce il valore salvato alla posizione indicata.
 *
 * La funzione restituisce il puntatore applicativo presente nel nodo, non una
 * copia del dato.
 *
 * @param[in] linkedList lista da interrogare
 * @param[in] index posizione da leggere, valida da 0 a `size - 1`
 *
 * @return puntatore al valore in posizione `index`, oppure `NULL` se gli
 *         argomenti non sono validi o l'indice e' fuori range
 */
void* linkedList_search_index(LinkedList linkedList, int index);

/**
 * @brief Verifica se la lista e' vuota.
 *
 * Per convenzione difensiva, una lista NULL viene considerata vuota.
 *
 * @param[in] linkedList lista da controllare
 * @return `true` se la lista e' NULL o non contiene nodi, `false` altrimenti
 */
bool linkedList_isEmpty(LinkedList linkedList);

/**
 * @brief Restituisce il numero di nodi presenti nella lista.
 *
 * Per convenzione difensiva, una lista NULL ha dimensione 0.
 *
 * @param[in] linkedList lista da interrogare
 * @return numero di nodi presenti, oppure 0 se `linkedList` e' NULL
 */
size_t linkedList_size(LinkedList linkedList);



#endif /* LIBFEM_LINKEDLIST_H */
