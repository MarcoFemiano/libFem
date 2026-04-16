//
// Created by Spara on 16/04/2026.
//

/**
 * @file parking.c
 * @brief Implementazione dell'ADT Parcheggio.
 *
 * Il modulo implementa la logica di un parcheggio lineare a corsia singola.
 * Le auto vengono gestite tramite:
 * - uno stack per l'area di parcheggio principale;
 * - uno stack per lo spazio di manovra;
 * - una coda per le auto in attesa.
 *
 * Convenzione adottata:
 * il top dello stack `areaParcheggio` rappresenta l'auto più vicina
 * all'uscita del parcheggio.
 */

#include "parking.h"

#include <stdlib.h>

/**
 * @brief Struttura concreta del parcheggio.
 *
 * @var coda
 * Coda delle automobili in attesa di entrare nel parcheggio.
 *
 * @var areaParcheggio
 * Stack che rappresenta l'area principale di parcheggio.
 *
 * @var spazioManovra
 * Stack usato come area temporanea per spostare le auto che bloccano
 * l'uscita di un'altra auto.
 */
struct strParking {
    Coda coda;
    Stack areaParcheggio;
    Stack spazioManovra;
};

/* ========================================================================= */
/* Funzioni private                                                          */
/* ========================================================================= */

/**
 * @brief Inserisce un'automobile nella coda di attesa.
 *
 * @param parcheggio Parcheggio su cui operare.
 * @param automobile Identificativo dell'automobile da inserire.
 *
 * @retval ERROR_NULL_POINTER puntatore nullo.
 * @retval qualsiasi codice restituito da coda_push.
 */
static short parcheggio_inserisci_in_coda(Parcheggio parcheggio, int automobile) {
    // test di robustezza
    if (parcheggio == NULL || parcheggio->coda == NULL) return ERROR_NULL_POINTER;

    return coda_push(parcheggio->coda, &automobile);
}

/**
 * @brief Rimuove un'automobile dalla coda di attesa.
 *
 * @param parcheggio Parcheggio su cui operare.
 *
 * @note La firma attuale usa un parametro per valore. Questo significa che
 *       il chiamante non riceve il valore estratto. Se vuoi restituire davvero
 *       l'auto rimossa, conviene usare `int *automobile`.
 *
 * @retval ERROR_NULL_POINTER puntatore nullo.
 * @retval qualsiasi codice restituito da coda_pop.
 */
static short parcheggio_rimuovi_da_coda(Parcheggio parcheggio) {
    // test di robustezza
    if (parcheggio == NULL || parcheggio->coda == NULL) return ERROR_NULL_POINTER;

    int cestino;
    return coda_pop(parcheggio->coda, &cestino);
}

/**
 * @brief Inserisce un'automobile nello spazio di manovra.
 *
 * @param parcheggio Parcheggio su cui operare.
 * @param automobile Identificativo dell'automobile da inserire.
 *
 * @retval ERROR_NULL_POINTER puntatore nullo.
 * @retval qualsiasi codice restituito da stack_push.
 */
static short parcheggio_inserisci_in_spazioManovra(Parcheggio parcheggio, int automobile) {
    // test di robustezza
    if (parcheggio == NULL || parcheggio->spazioManovra == NULL) return ERROR_NULL_POINTER;

    return stack_push(parcheggio->spazioManovra, &automobile);
}

/**
 * @brief Rimuove un'automobile dallo spazio di manovra.
 *
 * @param parcheggio Parcheggio su cui operare.
 *
 * @note Anche qui, se vuoi propagare davvero al chiamante il valore rimosso,
 *       conviene usare `int *automobileRimossa`.
 *
 * @retval ERROR_NULL_POINTER puntatore nullo.
 * @retval qualsiasi codice restituito da stack_pop.
 */
static short parcheggio_rimuovi_da_spazioManovra(Parcheggio parcheggio) {
    // test di robustezza
    if (parcheggio == NULL || parcheggio->spazioManovra == NULL) return ERROR_NULL_POINTER;

    int cestino;
    return stack_pop(parcheggio->spazioManovra, &cestino);
}

/**
 * @brief Parcheggia un'automobile nell'area principale.
 *
 * @param parcheggio Parcheggio su cui operare.
 * @param automobile Identificativo dell'automobile da parcheggiare.
 *
 * @retval ERROR_NULL_POINTER puntatore nullo.
 * @retval qualsiasi codice restituito da stack_push.
 */
static short parcheggia_auto(Parcheggio parcheggio, int automobile) {
    // test di robustezza
    if (parcheggio == NULL || parcheggio->areaParcheggio == NULL) return ERROR_NULL_POINTER;

    return stack_push(parcheggio->areaParcheggio, &automobile);
}

/**
 * @brief Rimuove l'automobile attualmente in cima all'area di parcheggio.
 *
 * @param parcheggio Parcheggio su cui operare.
 *
 * @note Come per altre funzioni private analoghe, se vuoi restituire davvero
 *       il valore al chiamante è preferibile usare un puntatore a int.
 *
 * @retval ERROR_NULL_POINTER puntatore nullo.
 * @retval qualsiasi codice restituito da stack_pop.
 */
static short parcheggio_rimuovi_parcheggiata(Parcheggio parcheggio) {
    // test di robustezza
    if (parcheggio == NULL || parcheggio->areaParcheggio == NULL) return ERROR_NULL_POINTER;

    int cestino;
    return stack_pop(parcheggio->areaParcheggio, &cestino);
}

/* ========================================================================= */
/* API pubblica                                                              */
/* ========================================================================= */

Parcheggio parcheggio_create(void) {

    Parcheggio parcheggio = malloc(sizeof(struct strParking));
    // test di robustezza
    if (parcheggio == NULL) return NULL;

    parcheggio->coda = coda_create(DEFAULT_CODA_ATTESA_SIZE, sizeof(int));
    // test di robustezza
    if (parcheggio->coda == NULL) {
        free(parcheggio);
        return NULL;
    }

    parcheggio->areaParcheggio = stack_create(DEFAULT_STACK_AREA_PARCHEGGIO_SIZE, sizeof(int));
    // test di robustezza
    if (parcheggio->areaParcheggio == NULL) {
        coda_destroy(&parcheggio->coda);
        free(parcheggio);
        return NULL;
    }

    parcheggio->spazioManovra = stack_create(DEFAULT_STACK_SPAZIO_MANOVRA_SIZE, sizeof(int));
    // test di robustezza
    if (parcheggio->spazioManovra == NULL) {
        coda_destroy(&parcheggio->coda);
        stack_destroy(&parcheggio->areaParcheggio);
        free(parcheggio);
        return NULL;
    }

    return parcheggio;
}

/**
 * @brief Distrugge il parcheggio e libera tutte le strutture interne.
 *
 * @param parcheggio Indirizzo del puntatore al parcheggio da distruggere.
 *
 * @retval ERROR_NULL_POINTER puntatore nullo.
 * @retval OK distruzione eseguita con successo.
 */
short parcheggio_destroy(Parcheggio *parcheggio) {
    // test di robustezza
    if (parcheggio == NULL || *parcheggio == NULL) return ERROR_NULL_POINTER;

    if ((*parcheggio)->coda != NULL) coda_destroy(&(*parcheggio)->coda);
    if ((*parcheggio)->areaParcheggio != NULL) stack_destroy(&(*parcheggio)->areaParcheggio);
    if ((*parcheggio)->spazioManovra != NULL) stack_destroy(&(*parcheggio)->spazioManovra);

    free(*parcheggio);
    *parcheggio = NULL;

    return OK;
}

/**
 * @brief Gestisce l'arrivo di un'automobile nel sistema parcheggio.
 *
 * Se l'area di parcheggio non è piena, l'auto viene inserita direttamente.
 * In caso contrario, viene accodata nella coda di attesa.
 *
 * @param parcheggio Parcheggio su cui operare.
 * @param automobile Identificativo dell'automobile in arrivo.
 *
 * @retval ERROR_NULL_POINTER puntatore nullo.
 * @retval AUTO_PARCHEGGIATA auto parcheggiata con successo.
 * @retval AUTO_MESSA_IN_CODA auto inserita nella coda di attesa.
 * @retval qualsiasi codice di errore restituito dagli ADT interni.
 */
short parcheggio_autoArriva(Parcheggio parcheggio, int automobile) {
    // test di robustezza
    if (parcheggio == NULL) return ERROR_NULL_POINTER;

    if (!stack_is_full(parcheggio->areaParcheggio)) {
        short ris = parcheggia_auto(parcheggio, automobile);
        // se c'è un errore esco dalla funzione e lo propago al chiamante
        if (ris != OK) return ris;

        return AUTO_PARCHEGGIATA;
    } else {
        short ris = coda_push(parcheggio->coda, &automobile);
        // se c'è un errore esco dalla funzione e lo propago al chiamante
        if (ris != OK) return ris;

        return AUTO_MESSA_IN_CODA;
    }

    // return di sicurezza, teoricamente non raggiungibile
    return OK;
}

/**
 * @brief Gestisce l'uscita di un'automobile dal parcheggio.
 *
 * L'algoritmo funziona così:
 * - si esplora l'area di parcheggio partendo dall'auto più vicina all'uscita;
 * - se l'auto in cima non è quella richiesta, viene spostata nello spazio di manovra;
 * - se lo spazio di manovra si riempie, l'operazione fallisce e lo stato precedente
 *   viene ripristinato;
 * - quando l'auto richiesta viene trovata, viene rimossa;
 * - le auto temporaneamente spostate vengono rimesse nel parcheggio nello stesso ordine logico;
 * - se esiste un'auto in coda, la prima entra nel parcheggio.
 *
 * Se l'auto non viene trovata, viene effettuato il rollback completo delle auto
 * eventualmente già spostate nello spazio di manovra.
 *
 * @param parcheggio Parcheggio su cui operare.
 * @param automobile Identificativo dell'automobile da far uscire.
 *
 * @retval ERROR_NULL_POINTER puntatore nullo.
 * @retval OK auto uscita correttamente.
 * @retval AUTO_SPAZIO_DI_MANOVRA_PIENO impossibile eseguire la manovra per insufficienza di spazio.
 * @retval AUTO_NON_ESISTE_NEL_PARCHEGGIO auto non trovata nell'area di parcheggio.
 * @retval qualsiasi codice di errore restituito dagli ADT interni.
 */
short parcheggio_autoEsce(Parcheggio parcheggio, int automobile) {
    if (parcheggio == NULL) return ERROR_NULL_POINTER;

    int numeroAutoLetto;
    short ris0;

    // itero finché ci sono auto nell'area di parcheggio
    while (1) {
        ris0 = stack_is_empty(parcheggio->areaParcheggio);
        if (ris0 == ERROR_NULL_POINTER) return ris0;
        if (ris0 == 1) break;

        // leggo l'auto attualmente più vicina all'uscita
        short ris = stack_peek(parcheggio->areaParcheggio, &numeroAutoLetto);
        if (ris != OK) return ris;

        // se l'auto in cima è quella richiesta, può uscire
        if (numeroAutoLetto == automobile) {

            // rimuovo davvero l'auto dal parcheggio
            short ris5 = stack_pop(parcheggio->areaParcheggio, &numeroAutoLetto);
            if (ris5 != OK) return ris5;

            // ripristino le auto spostate nello spazio di manovra
            short ris2;
            while (1) {
                ris2 = stack_is_empty(parcheggio->spazioManovra);
                if (ris2 == ERROR_NULL_POINTER) return ris2;
                if (ris2 == 1) break;

                short ris3 = stack_pop(parcheggio->spazioManovra, &numeroAutoLetto);
                if (ris3 != OK) return ris3;

                short ris4 = stack_push(parcheggio->areaParcheggio, &numeroAutoLetto);
                if (ris4 != OK) return ris4;
            }

            // se c'è almeno un'auto in coda, la faccio entrare
            short ris6 = coda_is_empty(parcheggio->coda);
            if (ris6 == ERROR_NULL_POINTER) return ris6;

            if (ris6 == 1) return OK;

            short ris7 = coda_pop(parcheggio->coda, &numeroAutoLetto);
            if (ris7 != OK) return ris7;

            short ris8 = stack_push(parcheggio->areaParcheggio, &numeroAutoLetto);
            if (ris8 != OK) return ris8;

            return OK;
        }

        // l'auto in cima non è quella richiesta: provo a spostarla nello spazio di manovra
        short ris5 = stack_is_full(parcheggio->spazioManovra);
        if (ris5 == ERROR_NULL_POINTER) return ris5;

        // se lo spazio di manovra è pieno, annullo l'operazione e ripristino lo stato
        if (ris5 == 1) {
            short ris9;

            while (1) {
                ris9 = stack_is_empty(parcheggio->spazioManovra);
                if (ris9 == ERROR_NULL_POINTER) return ris9;
                if (ris9 == 1) break;

                short ris10 = stack_pop(parcheggio->spazioManovra, &numeroAutoLetto);
                if (ris10 != OK) return ris10;

                short ris11 = stack_push(parcheggio->areaParcheggio, &numeroAutoLetto);
                if (ris11 != OK) return ris11;
            }

            return AUTO_SPAZIO_DI_MANOVRA_PIENO;
        }

        // sposto l'auto corrente nello spazio di manovra
        ris = stack_pop(parcheggio->areaParcheggio, &numeroAutoLetto);
        if (ris != OK) return ris;

        short res6 = stack_push(parcheggio->spazioManovra, &numeroAutoLetto);
        if (res6 != OK) return res6;
    }

    // rollback finale: auto non trovata, ripristino le auto temporaneamente spostate
    short ris2;
    while (1) {
        ris2 = stack_is_empty(parcheggio->spazioManovra);
        if (ris2 == ERROR_NULL_POINTER) return ris2;
        if (ris2 == 1) break;

        short ris3 = stack_pop(parcheggio->spazioManovra, &numeroAutoLetto);
        if (ris3 != OK) return ris3;

        short ris4 = stack_push(parcheggio->areaParcheggio, &numeroAutoLetto);
        if (ris4 != OK) return ris4;
    }

    return AUTO_NON_ESISTE_NEL_PARCHEGGIO;
}