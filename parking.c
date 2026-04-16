//
// Created by Spara on 16/04/2026.
//

#include "parking.h"

#include <stdlib.h>

struct strParking {
    Coda coda;
    Stack areaParcheggio;
    Stack spazioManovra;
};


Parcheggio parcheggio_create() {

    Parcheggio parcheggio = malloc(sizeof(struct strParking));
    //test di robustezza
    if (parcheggio == NULL) return NULL;

    parcheggio->coda = coda_create(DEFAULT_CODA_ATTESA_SIZE,sizeof(int));
    //test di robustezza
    if (parcheggio->coda == NULL) {
        free(parcheggio);
        return NULL;
    }

    parcheggio->areaParcheggio = stack_create(DEFAULT_STACK_AREA_PARCHEGGIO_SIZE,sizeof(int));
    //test di robustezza
    if (parcheggio->areaParcheggio == NULL) {
        coda_destroy(&parcheggio->coda);
        free(parcheggio);
        return NULL;
    }

    parcheggio->spazioManovra = stack_create(DEFAULT_STACK_SPAZIO_MANOVRA_SIZE,sizeof(int));
    //test di robustezza
    if (parcheggio->spazioManovra == NULL) {
        coda_destroy(&parcheggio->coda);
        stack_destroy(&parcheggio->areaParcheggio);
        free(parcheggio);
        return NULL;
    }

    return parcheggio;

}



short parcheggio_destroy(Parcheggio *parcheggio) {
    //tests di robustezza
    if (parcheggio == NULL || *parcheggio == NULL) return ERROR_NULL_POINTER;

    if ((*parcheggio)->coda != NULL ) coda_destroy(&(*parcheggio)->coda);
    if ((*parcheggio)->areaParcheggio != NULL) stack_destroy(&(*parcheggio)->areaParcheggio);
    if ((*parcheggio)->spazioManovra != NULL) stack_destroy(&(*parcheggio)->spazioManovra);

    free(*parcheggio);

    *parcheggio = NULL;

    return OK;
}




static short parcheggio_inserisci_in_coda(Parcheggio parcheggio, int automobile) {
    //tests di robustezza
    if (parcheggio == NULL || parcheggio->coda == NULL) return ERROR_NULL_POINTER;

    return coda_push(parcheggio->coda, &automobile);
}

static short parcheggio_rimuovi_da_coda(Parcheggio parcheggio, int automobile) {
    //tests di robustezza
    if (parcheggio == NULL || parcheggio->coda == NULL) return ERROR_NULL_POINTER;

    return coda_pop(parcheggio->coda, &automobile);
}


static short parcheggio_inserisci_in_spazioManovra(Parcheggio parcheggio, int automobile) {
    //tests di robustezza
    if (parcheggio == NULL || parcheggio->spazioManovra == NULL) return ERROR_NULL_POINTER;

    return stack_push(parcheggio->spazioManovra, &automobile);
}

static short parcheggio_rimuovi_da_spazioManovra(Parcheggio parcheggio, int automobileRimossa) {
    //tests di robustezza
    if (parcheggio == NULL || parcheggio->spazioManovra == NULL) return ERROR_NULL_POINTER;

    return stack_pop(parcheggio->spazioManovra, &automobileRimossa);
}

static short parcheggia_auto(Parcheggio parcheggio, int automobile) {
    //tests di robustezza
    if (parcheggio == NULL || parcheggio->areaParcheggio == NULL) return ERROR_NULL_POINTER;

    return stack_push(parcheggio->areaParcheggio, &automobile);
}

static short parcheggio_rimuovi_parcheggiata(Parcheggio parcheggio, int automobileRimossa) {
    //tests di robustezza
    if (parcheggio == NULL || parcheggio->areaParcheggio == NULL) return ERROR_NULL_POINTER;

    return stack_pop(parcheggio->areaParcheggio, &automobileRimossa);
}



short parcheggio_autoArriva(Parcheggio parcheggio, int automobile) {
    //tests di robustezza
    if (parcheggio == NULL) return ERROR_NULL_POINTER;

    if (!stack_is_full(parcheggio->areaParcheggio)) {
       short ris = parcheggia_auto(parcheggio,automobile);
        //se c'è un errore esco dalla funzione e riporto l'errore
        if (ris != OK) return ris;

        return AUTO_PARCHEGGIATA;
    }else {
        short ris = coda_push(parcheggio->coda, &automobile);
        //se c'è un errore esco dalla funzione e riporto l'errore
        if (ris != OK) return ris;

        return AUTO_MESSA_IN_CODA;
    }

    //questa return serve al compilatore
    return OK;
}

short parcheggio_autoEsce(Parcheggio parcheggio, int automobile) {
    if (parcheggio == NULL) return ERROR_NULL_POINTER;

    int numeroAutoLetto;

    short ris0;
    //itero affinchè ci sono auto nell'area del parcheggio
    while (1) {
        ris0 = stack_is_empty(parcheggio->areaParcheggio);
        if (ris0 == ERROR_NULL_POINTER) return ris0;

        if (ris0 == 1) break;

        //qui utilizzo la peek per una questione di gestione degli errori [*]
        short ris = stack_peek(parcheggio->areaParcheggio, &numeroAutoLetto);
        //se c'è un errore esco dalla funzione e riporto l'errore, ripeterò
        //questo pattern in tutto il codice
        //se non lo faccio è un'errore di distrazione o
        //una mancanza non voluta (!! Da segnalare se succede !!)
        if (ris != OK) return ris;
        //se l'auto nel parcheggio può uscire perchè non ci sono auto che la bloccano
        if (numeroAutoLetto == automobile) {

            //auto uscita con successo
            short ris5 = stack_pop(parcheggio->areaParcheggio, &numeroAutoLetto);
            if (ris5 != OK) return ris5;

            short ris2;
            //svuoto lo spazio di manovra (se ci sono auto)
            while (1) {

                ris2 = stack_is_empty(parcheggio->spazioManovra);
                if (ris2 == ERROR_NULL_POINTER) return ris2;
                else if (ris2 == 1) break;

                short ris3 = stack_pop(parcheggio->spazioManovra, &numeroAutoLetto);
                if (ris3 != OK) return ris3;

                short ris4 = stack_push(parcheggio->areaParcheggio, &numeroAutoLetto);
                if (ris4 != OK) return ris4;

            }//2nd while



            //Adesso vedo se c'è un'auto in coda
            short ris6 = coda_is_empty(parcheggio->coda);
            if (ris6 == ERROR_NULL_POINTER) return ris6;

            //se la coda è vuota, posso uscire
            if (ris6 == 1) return OK;

            short ris7 = coda_pop(parcheggio->coda, &numeroAutoLetto);
            if (ris7 != OK) return ris7;

            short ris8 = stack_push(parcheggio->areaParcheggio, &numeroAutoLetto);
            if (ris8 != OK) return ris8;

            return OK;

        }

            //non combacia, porto le macchine nell'area di manovra
            short ris5 = stack_is_full(parcheggio->spazioManovra);
            if (ris5 == ERROR_NULL_POINTER) return ris5;

            if (ris5 == 1) {
                short ris9;

                while (1) {
                    ris9 = stack_is_empty(parcheggio->spazioManovra);
                    if (ris9 == ERROR_NULL_POINTER) return ris9;

                    if (ris9 == 1) break;

                    short ris10 = stack_pop(parcheggio->spazioManovra,&numeroAutoLetto);
                    if (ris10 != OK) return ris10;

                    short ris11 = stack_push(parcheggio->areaParcheggio,&numeroAutoLetto);
                    if (ris11 != OK) return ris11;

                }
                return AUTO_SPAZIO_DI_MANOVRA_PIENO;
            }

            ris = stack_pop(parcheggio->areaParcheggio, &numeroAutoLetto);
            if (ris != OK) return ris;

            short res6 =  stack_push(parcheggio->spazioManovra, &numeroAutoLetto);
            if (res6 != OK) return res6;

    }//1st while

    short ris2;
    //svuoto lo spazio di manovra (se ci sono auto)
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