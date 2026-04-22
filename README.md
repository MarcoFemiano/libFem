# libFem

`libFem` e` un progetto in linguaggio C che raccoglie una serie di strutture dati generiche e piccoli programmi dimostrativi a supporto dello studio e della sperimentazione con ADT, algoritmi e gestione della memoria.

Il repository include implementazioni di `stack`, `coda`, `AVL tree` e `hash table`, oltre ad alcuni esempi d'uso come un simulatore di parcheggio lineare e una demo interattiva per la hash table.

## Funzionalita` principali

- `Stack` generico con gestione statica o dinamica degli elementi.
- `Coda` generica FIFO basata su array circolare.
- `AVL tree` generico con inserimento, ricerca, rimozione, min, max, altezza e stampa.
- `Hash table` generica con open addressing, linear probing, tombstone e ridimensionamento automatico.
- Simulazione di un parcheggio lineare che usa `stack` e `coda`.
- Esempi e test manuali per verificare il comportamento dei moduli.

## Stack tecnologico

- Linguaggio: `C`
- Standard: `C11`
- Build system: `CMake`
- Dipendenze incluse nel repository: `xxHash`
- Toolchain verificata nella directory di build esistente: `MinGW GCC` su Windows

## Requisiti

- `CMake` 4.0 o superiore
- Compilatore C compatibile con lo standard `C11`
- Ambiente locale con supporto alla compilazione da riga di comando oppure IDE compatibile con `CMake`
- `TODO`: elenco ufficiale dei sistemi operativi supportati

## Installazione

1. Clonare o scaricare il repository in locale.
   `TODO`: URL ufficiale del repository remoto
2. Posizionarsi nella cartella del progetto.
3. Generare la directory di build con `CMake`.
4. Compilare il target `libFem`.

Esempio:

```bash
cmake -S . -B build
cmake --build build
```

## Installazione solo di ciò che può interessare
1. Copiare e incollare i file .c e .h dell'API che può essere utile nella cartella dei propri progetti
2. includerla dove necessario con #include "nomeAPI"

## Avvio locale

Dopo la compilazione, l'eseguibile generato e` `libFem`.

Esempio su Windows:

```powershell
.\build\libFem.exe
```

Nota: l'entrypoint attualmente collegato al target principale e` [main.c](./main.c), che avvia una demo interattiva della `hash table`.

## Struttura del progetto

```text
libFem/
|-- BST_AVL.c
|-- BST_AVL.h
|-- coda.c
|-- coda.h
|-- hash_table.c
|-- hash_table.h
|-- main.c
|-- mainBSTAVL.c
|-- mainEsercitazioneParking.c
|-- parking.c
|-- parking.h
|-- stack.c
|-- stack.h
|-- esercizioValidatoreParentesi.c
|-- third_party/
|   |-- xxHash.c
|   `-- xxHash.h
|-- CMakeLists.txt
|-- LICENSE
`-- README.md
```

### Note sulla struttura

- I moduli principali sono attualmente organizzati direttamente nella root del repository.
- La cartella `third_party/` contiene la dipendenza esterna `xxHash`.
- I file `main*.c` raccolgono demo, test manuali e piccoli esercizi.
- `TODO`: separare in modo piu` netto libreria, esempi e test automatici.

## Esempio d'uso

Esempio minimale di utilizzo della `hash table` generica:

```c
#include <stdio.h>
#include "hash_table.h"

static int int_cmp(const void *a, const void *b) {
    if (a == NULL || b == NULL) return -2;

    if (*(const int *)a < *(const int *)b) return -1;
    if (*(const int *)a > *(const int *)b) return 1;
    return 0;
}

int main(void) {
    HashTable ht = NULL;
    bool found = false;
    int value = 42;

    if (hashTable_create(&ht, 10, sizeof(int)) != OK) return 1;
    if (hashTable_push(ht, int_cmp, &value) != OK) return 1;
    if (hashTable_search(ht, int_cmp, &value, &found) != OK) return 1;

    if (found) {
        printf("Valore trovato\n");
    }

    hashTable_destroy(&ht);
    return 0;
}
```

## Roadmap futura
- Migliorare la documentazione pubblica dei moduli e dei contratti di ownership della memoria
- Separare i programmi dimostrativi in target distinti
- Aggiungere test automatici eseguibili tramite `CMake`
- Valutare la generazione di una vera libreria riutilizzabile oltre all'eseguibile demo

## Licenza

Il progetto e` distribuito con licenza `MIT`. Per il testo completo consultare [LICENSE](./LICENSE).
