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
#define DEFAULT_STACK_PARCHEGGIO_SIZE 10
#define DEFAULT_STACK_SPAZIO_MANOVRA_SIZE 4


typedef struct strParking* Parcheggio;
/** Funzioni pubbliche
 *
 *
 */
Parcheggio parcheggio_create();
short parcheggio_autoArriva(Parcheggio parcheggio, int automobile);
short parcheggio_autoEsce(Parcheggio parcheggio, int automobile);
short parcheggio_auto_esce_da_coda_di_attesa(Parcheggio parcheggio, int automobile);
void parcheggio_destroy(Parcheggio *parcheggio);





static short parcheggia_auto(Parcheggio parcheggio, Stack areaParcheggio, int automobile);
static short parcheggio_rimuovi_parcheggiata(Parcheggio parcheggio, Stack areaParcheggio, Stack spazioManovra, int automobileRimossa);

static short parcheggio_inserisci_in_coda(Parcheggio parcheggio, Coda codaVeicoli, int automobile);
static short parcheggio_rimuovi_da_coda(Parcheggio parcheggio, Coda codaVeicoli, int automobile);

static short parcheggio_inserisci_in_spazioManovra(Parcheggio parcheggio, Stack areaParcheggio, Stack spazioManovra, int automobile);
static short parcheggio_rimuovi_da_spazioManovra(Parcheggio parcheggio, Stack areaParcheggio, int automobileRimossa);


#endif //LIBFEM_PARKING_H