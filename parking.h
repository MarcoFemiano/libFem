//
// Created by Spara on 16/04/2026.
//

/**
 * @file parking.h
 * @brief Interfaccia pubblica dell'ADT Parcheggio.
 *
 * Questo modulo modella un parcheggio lineare a corsia singola con:
 * - una zona di parcheggio principale;
 * - una coda di attesa per le auto che non trovano posto;
 * - uno spazio di manovra temporaneo necessario per far uscire
 *   un'auto bloccata da altre auto.
 *
 * Il modello è basato sui seguenti ADT:
 * - Stack per l'area di parcheggio;
 * - Stack per lo spazio di manovra;
 * - Coda per le auto in attesa.
 *
 * Ogni automobile è identificata da un valore intero.
 */

#ifndef LIBFEM_PARKING_H
#define LIBFEM_PARKING_H

#include "stack.h"
#include "coda.h"

/* ========================================================================= */
/* Costanti di configurazione                                                */
/* ========================================================================= */

/**
 * @brief Capacità di default della coda di attesa.
 */
#define DEFAULT_CODA_ATTESA_SIZE 10

/**
 * @brief Capacità di default dell'area principale di parcheggio.
 */
#define DEFAULT_STACK_AREA_PARCHEGGIO_SIZE 10

/**
 * @brief Capacità di default dello spazio di manovra.
 *
 * In base alla traccia, se servono più di 4 spostamenti temporanei
 * l'auto non può uscire.
 */
#define DEFAULT_STACK_SPAZIO_MANOVRA_SIZE 4

/* ========================================================================= */
/* Codici di ritorno specifici del modulo                                    */
/* ========================================================================= */

/**
 * @brief L'auto è stata parcheggiata con successo nell'area principale.
 */
#define AUTO_PARCHEGGIATA 1

/**
 * @brief L'auto non è stata parcheggiata ed è stata inserita nella coda.
 */
#define AUTO_MESSA_IN_CODA 2

/**
 * @brief Codice disponibile per eventuali estensioni legate allo spazio di manovra.
 *
 * @note Attualmente non risulta utilizzato dalle funzioni pubbliche mostrate.
 */
#define AUTO_MESSA_IN_SPAZIO_MANOVRA 3

/**
 * @brief L'auto richiesta non è presente nel parcheggio.
 */
#define AUTO_NON_ESISTE_NEL_PARCHEGGIO 4

/**
 * @brief L'auto non può uscire perché servirebbe uno spazio di manovra oltre il limite.
 */
#define AUTO_SPAZIO_DI_MANOVRA_PIENO 5

/* ========================================================================= */
/* Tipo opaco                                                                */
/* ========================================================================= */

/**
 * @brief Tipo opaco che rappresenta un parcheggio.
 *
 * La struttura concreta è nascosta all'utilizzatore del modulo.
 */
typedef struct strParking* Parcheggio;

/* ========================================================================= */
/* API pubblica                                                              */
/* ========================================================================= */

/**
 * @brief Crea e inizializza un nuovo parcheggio.
 *
 * Il parcheggio viene creato con:
 * - una coda di attesa;
 * - un'area di parcheggio principale;
 * - uno spazio di manovra.
 *
 * Tutte le strutture interne vengono allocate dinamicamente.
 *
 * @return Un nuovo oggetto Parcheggio se la creazione va a buon fine,
 *         NULL in caso di errore di allocazione o inizializzazione.
 */
Parcheggio parcheggio_create(void);

/**
 * @brief Gestisce l'arrivo di un'automobile.
 *
 * Se l'area principale di parcheggio non è piena, l'auto viene parcheggiata.
 * In caso contrario, l'auto viene inserita nella coda di attesa.
 *
 * @param parcheggio Parcheggio su cui operare.
 * @param automobile Identificativo dell'automobile.
 *
 * @retval ERROR_NULL_POINTER puntatore al parcheggio nullo.
 * @retval AUTO_PARCHEGGIATA auto parcheggiata con successo.
 * @retval AUTO_MESSA_IN_CODA auto inserita nella coda di attesa.
 * @retval qualsiasi codice di errore propagato dagli ADT interni.
 */
short parcheggio_autoArriva(Parcheggio parcheggio, int automobile);

/**
 * @brief Gestisce l'uscita di un'automobile dal parcheggio.
 *
 * Se l'automobile è bloccata da altre auto, queste vengono spostate
 * temporaneamente nello spazio di manovra. Se il numero di auto da spostare
 * supera la capacità dello spazio di manovra, l'operazione fallisce e lo
 * stato del parcheggio viene ripristinato.
 *
 * Se l'uscita va a buon fine e nella coda di attesa è presente almeno
 * un'automobile, la prima auto in coda entra nel parcheggio.
 *
 * @param parcheggio Parcheggio su cui operare.
 * @param automobile Identificativo dell'automobile da far uscire.
 *
 * @retval ERROR_NULL_POINTER puntatore nullo.
 * @retval OK auto uscita con successo.
 * @retval AUTO_NON_ESISTE_NEL_PARCHEGGIO auto non presente nell'area di parcheggio.
 * @retval AUTO_SPAZIO_DI_MANOVRA_PIENO auto non estraibile per insufficienza dello spazio di manovra.
 * @retval qualsiasi codice di errore propagato dagli ADT interni.
 */
short parcheggio_autoEsce(Parcheggio parcheggio, int automobile);

/**
 * @brief Distrugge il parcheggio e libera tutte le risorse associate.
 *
 * Dopo la distruzione, il puntatore viene impostato a NULL.
 *
 * @param parcheggio Indirizzo del puntatore al parcheggio da distruggere.
 *
 * @retval ERROR_NULL_POINTER puntatore nullo.
 * @retval OK distruzione completata con successo.
 */
short parcheggio_destroy(Parcheggio *parcheggio);

/**
 * @brief Rimuove un'automobile dalla coda di attesa.
 *
 * Funzione extra non richiesta esplicitamente dalla traccia.
 *
 * @param parcheggio Parcheggio su cui operare.
 * @param automobile Identificativo dell'automobile da rimuovere dalla coda.
 *
 * @warning Funzione dichiarata ma non implementata nel file mostrato.
 *
 * @retval codici dipendenti dall'implementazione.
 */
short parcheggio_auto_esce_da_coda_di_attesa(Parcheggio parcheggio, int automobile);

#endif //LIBFEM_PARKING_H