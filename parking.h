//
// Created by Spara on 16/04/2026.
//

#ifndef LIBFEM_PARKING_H
#define LIBFEM_PARKING_H
#include "stack.h"
#include "coda.h"

/** Parametri
 *
 */
#define DEFAULT_CODA_ATTESA_SIZE 10
#define DEFAULT_STACK_AREA_PARCHEGGIO_SIZE 10
#define DEFAULT_STACK_SPAZIO_MANOVRA_SIZE 4
#define AUTO_PARCHEGGIATA 1
#define AUTO_MESSA_IN_CODA 2
#define AUTO_MESSA_IN_SPAZIO_MANOVRA 3
#define AUTO_NON_ESISTE_NEL_PARCHEGGIO 4
#define AUTO_SPAZIO_DI_MANOVRA_PIENO 5




typedef struct strParking* Parcheggio;
/** Funzioni pubbliche
 *
 *
 */
Parcheggio parcheggio_create();
short parcheggio_autoArriva(Parcheggio parcheggio, int automobile);
short parcheggio_autoEsce(Parcheggio parcheggio, int automobile);
short parcheggio_destroy(Parcheggio *parcheggio);


//extra Personale, non necessaria nel progetto
short parcheggio_auto_esce_da_coda_di_attesa(Parcheggio parcheggio, int automobile);


static short parcheggia_auto(Parcheggio parcheggio, int automobile);
static short parcheggio_rimuovi_parcheggiata(Parcheggio parcheggio, int automobileRimossa);

static short parcheggio_inserisci_in_coda(Parcheggio parcheggio, int automobile);
static short parcheggio_rimuovi_da_coda(Parcheggio parcheggio, int automobile);

static short parcheggio_inserisci_in_spazioManovra(Parcheggio parcheggio, int automobile);
static short parcheggio_rimuovi_da_spazioManovra(Parcheggio parcheggio, int automobileRimossa);


#endif //LIBFEM_PARKING_H