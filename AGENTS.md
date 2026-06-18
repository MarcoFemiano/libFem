# AGENTS.md — Regole di stile C del progetto

Questo file descrive come Codex deve lavorare quando scrive o modifica codice C in questo repository.
Lo scopo non è produrre C “generico” o “più moderno”, ma codice il più possibile vicino allo stile dei file campione `BST_AVL.c/.h` e `hash_table.c/.h`.

## 1. Principio guida

Codex deve scrivere codice C pragmatico, esplicito, difensivo e organizzato come ADT.
La priorità è questa:

1. correttezza semantica;
2. robustezza contro input invalidi;
3. gestione chiara della memoria;
4. API leggibile e stabile;
5. stile coerente con i file già presenti;
6. solo dopo, micro-ottimizzazioni.

Non trasformare il codice in uno stile “da manuale”, “enterprise”, C++, pseudo-OOP avanzato o troppo astratto.
Il codice deve sembrare scritto dalla stessa persona che ha scritto i file campione.

## 2. Architettura generale dei moduli

Quando possibile, ogni componente deve essere un ADT C con struttura opaca.

Schema preferito:

```c
typedef struct strNomeModulo* NomeModulo;
```

Nel file `.h` si espone solo l'handle opaco.
Nel file `.c` si definisce la struttura concreta:

```c
struct strNomeModulo {
  /* campi interni */
};
```

Le strutture interne non devono essere manipolate direttamente dall'utilizzatore.
L'utente deve passare sempre dall'API pubblica.

Per strutture interne secondarie usare la stessa convenzione:

```c
typedef struct strNodo* Nodo;
struct strNodo {
        void* val;
        struct strNodo *leftChild;
        struct strNodo *rightChild;
};
```

## 3. Header `.h`

Gli header devono essere molto documentati.
Devono spiegare non solo le firme, ma anche il contratto d'uso dell'ADT.

Ogni header importante deve avere in alto un blocco Doxygen simile a questo:

```c
/**
 * @file nome_modulo.h
 * @brief Descrizione breve dell'ADT/API.
 *
 * Spiegazione pratica del modulo.
 *
 * -----------------------------------------------------------------------------
 * USO SEMPLICE DELL'ADT
 * -----------------------------------------------------------------------------
 *
 * 1) Creare la struttura:
 * @code
 * NomeModulo obj;
 * status_codes res = nomeModulo_create(&obj);
 * @endcode
 *
 * -----------------------------------------------------------------------------
 * GESTIONE DELLA MEMORIA
 * -----------------------------------------------------------------------------
 *
 * Specificare chiaramente chi possiede la memoria e chi deve liberarla.
 */
```

Negli header pubblici includere:

- `@file`;
- `@brief`;
- guida rapida all'utilizzo;
- esempio minimo completo quando il modulo è un ADT;
- regole sulla memoria;
- regole sui duplicati, se esistono;
- convenzioni importanti, per esempio altezza di un albero vuoto, stati degli slot, significato dei callback;
- documentazione Doxygen per ogni funzione pubblica.

Usare include guard in stile:

```c
#ifndef LIBFEM_NOME_MODULO_H
#define LIBFEM_NOME_MODULO_H

/* contenuto */

#endif /* LIBFEM_NOME_MODULO_H */
```

oppure, se il file esistente usa commento `//`, mantenere quello stile locale:

```c
#endif // LIBFEM_HASH_TABLE_H
```

Non sostituire automaticamente questo stile con `#pragma once`.

## 4. Tipo booleano

Nei file campione viene usato un booleano minimale, non `<stdbool.h>`.
Mantenere questo stile salvo richiesta esplicita diversa.

```c
typedef char bool;
#define true 1
#define false 0
```

Non introdurre `stdbool.h` in modo arbitrario.
Se in un progetto più grande nasce un conflitto tra più header che definiscono `bool`, segnalarlo e proporre una soluzione coerente, ma non riscrivere tutto senza richiesta.

## 5. `status_codes`

Le funzioni pubbliche devono restituire quasi sempre `status_codes`.
Non usare `errno`, `exit`, `abort`, `printf` di errore o valori magici come canale principale di errore.

Convenzione:

```c
typedef enum {
  OK = 0,
  ERROR_NULL_POINTER = -1,
  ERROR_ALLOCATION_FAILURE = -2,
  ERROR_ARITHMETIC_OVERFLOW = -3,
  ELEMENTO_NON_TROVATO = 1
} status_codes;
```

Regole:

- `OK` deve valere `0`.
- Gli errori veri devono essere negativi.
- Gli stati particolari non necessariamente fatali possono essere positivi, se lo stile del modulo lo richiede.
- Ogni funzione pubblica deve documentare i codici restituiti.
- Non restituire direttamente puntatori o valori calcolati se l'operazione può fallire: usare parametro di output.

Esempio:

```c
status_codes nomeModulo_size(NomeModulo obj, size_t* result);
status_codes nomeModulo_search(NomeModulo obj, void* value, bool* result);
```

## 6. Parametri di output

Lo stile preferito è:

```c
status_codes funzione(Input input, Tipo* result);
```

Regole:

- controllare sempre `result == NULL`;
- scrivere in `*result` solo dopo i controlli minimi;
- se la funzione ha successo, `*result` deve essere sempre inizializzato;
- per ricerche booleane, inizializzare spesso a `false` prima del ciclo o della ricorsione.

Esempio:

```c
status_codes hashTable_search(HashTable hashTable,int (*cmp)(const void*, const void*) ,void* value, bool *result) {
  if (hashTable == NULL || value == NULL || result == NULL || cmp == NULL) return ERROR_NULL_POINTER;

  *result = false;

  /* ricerca */

  return OK;
}
```

## 7. Controlli di robustezza

Ogni funzione pubblica e ogni helper non banale deve iniziare con i controlli di robustezza.
Usare commenti nello stile dei file campione:

```c
//tests di robustezza
if (tree == NULL) return ERROR_NULL_POINTER;
if (cmp == NULL) return ERROR_NULL_POINTER;
if (value == NULL) return ERROR_NULL_POINTER;
```

Non usare `assert` come sostituto dei controlli runtime dell'API.
`assert` può essere usato solo per invarianti interne, ma le funzioni pubbliche devono comunque restituire `status_codes`.

## 8. Gestione della memoria

La memoria deve essere gestita in modo esplicito con `malloc`, `calloc`, `realloc` se davvero serve, `free`.

Regole obbligatorie:

- controllare sempre il risultato di `malloc/calloc/realloc`;
- in caso di errore dopo allocazioni parziali, liberare tutto ciò che è già stato allocato;
- le funzioni `destroy` devono liberare tutta la memoria interna posseduta dall'ADT;
- dopo `destroy`, impostare l'handle a `NULL`;
- documentare chiaramente se i dati dell'utente vengono copiati by value o se viene salvato solo un puntatore.

Esempio stile by pointer:

```c
/* L'albero salva il puntatore. Non libera il dato applicativo. */
newNode->val = value;
```

Esempio stile by value:

```c
memcpy(targetSlot, value, hashTable->sizeOfelements);
```

Quando si ridimensionano strutture dati, preferire una strategia con rollback:

1. salvare i vecchi puntatori;
2. allocare i nuovi buffer;
3. spostare temporaneamente la struttura sui nuovi buffer;
4. migrare gli elementi;
5. se qualcosa fallisce, liberare i nuovi buffer e ripristinare i vecchi;
6. se tutto riesce, liberare i vecchi buffer.

Questo schema è parte importante dello stile.

## 9. Overflow e dimensioni

Per capacità, indici e dimensioni usare `size_t`.
Prima di moltiplicare capacità e dimensione elemento, controllare overflow:

```c
if (defaultCapacity > SIZE_MAX / sizeOfElements) {
  free(newHashTable);
  return ERROR_ARITHMETIC_OVERFLOW;
}
```

Non fare moltiplicazioni di memoria senza controllo se il valore dipende dall'input.

## 10. Naming

### Tipi

Usare nomi opachi leggibili:

```c
typedef struct strAVLTree* AVLTree;
typedef struct strHashTable* HashTable;
typedef struct strNodo* Nodo;
```

La struttura concreta deve avere prefisso `str`:

```c
struct strHashTable {
  /* ... */
};
```

### Funzioni pubbliche

Le funzioni pubbliche devono avere prefisso del modulo:

```c
avl_create
avl_destroy
avl_insert
avl_remove
hashTable_create
hashTable_destroy
hashTable_push
hashTable_search
```

Quando si lavora in un modulo esistente, copiare esattamente il prefisso già usato.
Non rinominare `hashTable_` in `hash_table_` e non rinominare `avl_` in altro modo.

### Helper privati

Gli helper devono essere `static` e avere nome descrittivo con prefisso del modulo:

```c
static status_codes avl_insert_node(...)
static void avl_update_balance(...)
static status_codes hashTable_pushWithoutResize(...)
static status_codes hashTable_adjustCapacity(...)
```

### Variabili

Sono accettati nomi misti italiano/inglese quando rendono più chiaro il ragionamento algoritmico:

```c
size_t tentativi;
size_t firstDeleted;
status_codes res;
status_codes res2;
int cmpResult;
Nodo nodeToDelete;
```

Non trasformare tutto in nomi inglesi solo per uniformità.

## 11. Formattazione

Non applicare formatter automatici aggressivi.
Il codice campione ha una formattazione manuale e pragmatica.

Regole:

- mantenere lo stile locale del file che si sta modificando;
- se un file usa indentazione a 2 spazi, continuare con 2 spazi;
- se un file usa indentazione larga, continuare con quella;
- non riallineare interi file solo perché si modifica una funzione;
- non cambiare le firme esistenti per “abbellire” gli spazi;
- mantenere brace K&R:

```c
status_codes funzione(...) {
  if (condizione) {
    return OK;
  }else {
    return ERROR_INVALID_ARGUMENT;
  }
}
```

Nel codice esistente si trovano spesso firme compatte:

```c
status_codes avl_search(AVLTree tree,const void* value,int (*cmp)(const void*,const void*), bool* result)
```

e firme più spaziate:

```c
status_codes hashTable_create(HashTable* hashTable, size_t defaultCapacity, size_t sizeOfElements);
```

Preferisci le spaziate.

Quando si aggiunge codice a un file esistente, imitare quel file, non imporre uno stile unico.

## 12. Commenti

I commenti devono essere in italiano, pratici e vicini al ragionamento algoritmico.
Devono parlare ad alto livello.

Stile preferito nei `.c`:

```c
//tests di robustezza
//base case
//caso in cui il nodo è stato trovato
//caso non ha figli
//caso ha solo il figlio destro
//vado a dx
//rollback
```

Per le API pubbliche usare Doxygen:

```c
/**
 * @brief Inserisce un nuovo elemento nella struttura.
 *
 * Spiegare cosa succede, come vengono gestiti duplicati e memoria.
 *
 * @param[in] obj Struttura di destinazione.
 * @param[in] value Valore da inserire.
 * @return `OK` in caso di successo, altrimenti un opportuno codice di errore.
 */
```

Non riempire il codice di commenti inutili, ma commentare bene:

- casi algoritmici;
- ownership della memoria;
- rollback;
- stati speciali;
- invarianti;
- motivi di una scelta non ovvia.
Un buon commento, esprime il codice ad Alto Livello.

## 13. Organizzazione interna dei `.c`

Nei `.c` mettere:

1. commento iniziale `Created by Spara on ...` se il file è nuovo e il progetto lo usa;
2. include del proprio header per primo;
3. include standard necessari;
4. definizioni delle struct concrete;
5. helper statici;
6. funzioni pubbliche;
7. sezioni logiche con commenti `//region` se utili.

Esempio:

```c
//
// Created by Spara on 20/04/2026.
//

#include "nome_modulo.h"

#include <stdlib.h>
#include <string.h>

struct strNomeModulo {
  /* campi */
};

//region helpers
static int helper(...) {
  /* ... */
}
//endregion
```

## 14. Flusso di controllo

Preferire codice esplicito.
Non nascondere troppa logica in macro, funzioni ultra-generiche o costrutti difficili da seguire.

Sono coerenti con lo stile:

- early return sui controlli di errore;
- ricorsione per alberi e strutture naturalmente ricorsive;
- `goto` per loop manuali tipo linear probing, se rende il flusso più diretto;
- `for` con `size_t` per scansioni di array;
- `if/else` espliciti per casi algoritmici.

Esempio `goto` accettabile:

```c
LINEAR_PROBING:

if (tentativi == hashTable->capacity) return ELEMENTO_NON_TROVATO;

/* ... */

tentativi++;
goto LINEAR_PROBING;
```

Non usare `goto` per saltare a caso dentro funzioni complesse.
Usarlo solo quando il pattern è semplice e leggibile.

## 15. Comparatori e callback

Le strutture generiche devono ricevere callback dall'utente.
Per confronto usare firme simili:

```c
int (*cmp)(const void*, const void*)
```

Documentare bene il contratto della `cmp`.
Nel codice esistente ci sono due varianti:

- contratto classico: valore `< 0`, `0`, `> 0`;
- contratto stretto: `-1`, `0`, `1`, con valori `< -1` per errori.

Quando lavori in un modulo esistente, rispetta il contratto già scritto nell'header.
Quando crei un nuovo modulo, preferisci il contratto classico `<0 / 0 / >0`, ma se devi propagare errori della `cmp`, documenta esplicitamente quali valori indicano errore.

Non confrontare puntatori al posto del contenuto logico, a meno che l'API lo dica chiaramente.

## 16. Funzioni create/destroy

Ogni ADT che alloca memoria deve avere almeno:

```c
status_codes nomeModulo_create(NomeModulo* obj, ...);
status_codes nomeModulo_destroy(NomeModulo* obj);
```

`create`:

- controlla il puntatore all'handle;
- controlla argomenti semanticamente invalidi;
- alloca la struttura principale;
- inizializza tutti i campi;
- alloca eventuali buffer interni;
- in caso di fallimento libera ciò che ha già allocato;
- assegna `*obj` solo quando la struttura è valida.

`destroy`:

- controlla `obj` e `*obj`;
- libera buffer/nodi interni;
- libera la struttura principale;
- imposta `*obj = NULL`;
- ritorna `OK`.

## 17. Inserimento, rimozione, ricerca

Per funzioni come insert/push/remove/search:

- fare controlli di robustezza all'inizio;
- delegare la logica complessa a helper statici se serve;
- aggiornare `size` solo dopo successo reale;
- non alterare `size` se l'operazione fallisce;
- gestire duplicati con codice di stato esplicito;
- gestire non trovato con codice di stato o `bool result`, secondo l'API.

Esempio stile:

```c
status_codes nomeModulo_insert(NomeModulo obj,int (*cmp)(const void*,const void*), void* value) {
  if (obj == NULL) return ERROR_NULL_POINTER;
  if (value == NULL) return ERROR_NULL_POINTER;
  if (cmp == NULL) return ERROR_NULL_POINTER;

  status_codes res = nomeModulo_insert_node(&(obj->root),cmp,value);

  if (res != OK) return res;

  obj->size++;
  return OK;
}
```

## 18. Stati interni semplici

Per stati interni compatti è accettabile usare `char` con convenzione documentata.
Esempio:

```c
char* slotStates; //2=> deleted ; 1 => occupied   ; 0 => free
```

Se il nuovo modulo usa stati simili, documentarli vicino al campo o tramite macro uppercase.
Non introdurre enum pubblici se gli stati sono puramente interni e non servono all'utente.

## 19. Stile algoritmico

Il codice deve mostrare il ragionamento.
Per algoritmi con casi distinti, scrivere i casi uno alla volta.

Esempio:

```c
//caso non ha figli
if ((*node)->leftChild == NULL && (*node)->rightChild == NULL) {
  free(*node);
  *node = NULL;
  return OK;
}

//caso ha solo il figlio sinistro
if ((*node)->leftChild != NULL && (*node)->rightChild == NULL) {
  Nodo nodeToDelete = *node;
  *node = (*node)->leftChild;
  free(nodeToDelete);
  return OK;
}
```

Non comprimere questi casi in codice troppo “furbo”.
La chiarezza del caso algoritmico è più importante della brevità.

## 20. Dipendenze

Non aggiungere dipendenze esterne se non sono necessarie.
Se una dipendenza è già nello stile del progetto, come `third_party/xxHash.h`, usarla coerentemente.

Regole:

- includere nel `.h` solo ciò che serve all'API pubblica;
- includere nel `.c` ciò che serve all'implementazione;
- non introdurre librerie “comode” se bastano C standard, callback e memoria manuale;
- niente C++, niente STL, niente macro framework.

## 21. Cose da non fare

Codex non deve:

- riscrivere tutto il file per cambiare stile;
- sostituire gli ADT opachi con struct pubbliche;
- trasformare `status_codes` in `int`, `bool`, `errno` o eccezioni inesistenti in C;
- usare `exit()` dentro funzioni di libreria;
- stampare errori dentro funzioni che non sono esplicitamente funzioni di print/debug;
- cambiare ownership della memoria senza aggiornare header, esempi e commenti;
- usare `<stdbool.h>` se il modulo segue il bool custom;
- rinominare funzioni pubbliche esistenti;
- cambiare il contratto della `cmp` senza richiesta;
- eliminare controlli di robustezza perché “ridondanti”;
- rimuovere rollback o cleanup;
- introdurre astrazioni troppo generiche;
- nascondere la logica algoritmica in macro complesse;
- usare formattatori automatici sull'intero file.

## 22. Workflow obbligatorio per Codex

Prima di modificare o creare codice, Codex deve:

1. leggere gli header collegati;
2. capire ownership della memoria;
3. capire il contratto dei callback;
4. capire i codici `status_codes` già presenti;
5. scegliere nomi coerenti col modulo;
6. scrivere prima l'API o verificare che l'API esista;
7. implementare i controlli di robustezza;
8. implementare la logica con helper statici se serve;
9. aggiornare documentazione Doxygen;
10. compilare almeno con warning base, se possibile;
11. verificare casi limite: `NULL`, struttura vuota, duplicati, non trovato, allocazione fallita dove simulabile, resize/rollback se presente.

## 22-bis. Documentazione obbligatoria del funzionamento

Questo progetto deve avere documentazione locale aggiornata per rendere rapido
il recupero del contesto nelle modifiche future.

Codex deve leggere, quando pertinenti:

- `docs/CONTESTO_RAPIDO.md`, per capire mappa dei file e flusso generale;
- `docs/INVARIANTI.md`, per capire le regole semantiche da non violare;
- `docs/CHECKLIST_MODIFICHE.md`, per verificare i casi limite prima di chiudere.

Regole obbligatorie:

- se cambia il flusso del progetto, aggiornare `docs/CONTESTO_RAPIDO.md`;
- se cambia una rappresentazione interna, aggiornare `docs/INVARIANTI.md`;
- se cambia il contratto di un modulo pubblico, aggiornare sia Doxygen
  nell'header sia la documentazione in `docs/`, se pertinente;
- se si aggiunge un modulo importante, documentarlo nella mappa dei file;
- se si aggiunge un nuovo caso limite importante, inserirlo nella checklist;
- non lasciare documentazione che contraddice il codice.

In particolare, per questo repository devono restare documentate:

- ownership e ciclo di vita di `SatFormula`;
- significato di `quanteProp`;
- uso di `SAT_DIMACS_PROP_INVALIDA`;
- gestione di clausole vuote, unit clause, clausole binarie, ternarie e lunghe;
- regole della lettura DIMACS da file, stdin e `argc/argv`;
- cosa fa e cosa NON fa il preprocessing banale;
- invarianti di rinumerazione e deduplicazione.

## 23. Stile di risposta quando Codex lavora

Quando Codex completa una modifica, deve spiegare in modo breve:

- quali file ha toccato;
- quali funzioni ha aggiunto o cambiato;
- quali contratti di memoria valgono;
- quali casi limite sono gestiti;
- come compilare/testare.

Non deve produrre spiegazioni lunghissime se il codice è già chiaro.

## 24. Template minimo per un nuovo ADT

### Header

```c
#ifndef LIBFEM_NOME_MODULO_H
#define LIBFEM_NOME_MODULO_H

#include <stddef.h>

typedef char bool;
#define true 1
#define false 0

typedef struct strNomeModulo* NomeModulo;

typedef enum {
  OK = 0,
  ERROR_NULL_POINTER = -1,
  ERROR_ALLOCATION_FAILURE = -2,
  ERROR_ARITHMETIC_OVERFLOW = -3,
  ERROR_INVALID_ARGUMENT = -4,
  ELEMENTO_NON_TROVATO = 1
} status_codes;

status_codes nomeModulo_create(NomeModulo* obj);
status_codes nomeModulo_destroy(NomeModulo* obj);

#endif /* LIBFEM_NOME_MODULO_H */
```

### Source

```c
//
// Created by Spara on GG/MM/AAAA.
//

#include "nome_modulo.h"

#include <stdlib.h>

struct strNomeModulo {
  size_t size;
};

status_codes nomeModulo_create(NomeModulo* obj) {
  //tests di robustezza
  if (obj == NULL) return ERROR_NULL_POINTER;

  NomeModulo temp = malloc(sizeof(struct strNomeModulo));
  if (temp == NULL) return ERROR_ALLOCATION_FAILURE;

  temp->size = 0;

  *obj = temp;
  return OK;
}

status_codes nomeModulo_destroy(NomeModulo* obj) {
  //tests di robustezza
  if (obj == NULL || *obj == NULL) return ERROR_NULL_POINTER;

  free(*obj);
  *obj = NULL;

  return OK;
}
```

Questo template va adattato al modulo reale, ma conserva la pragmatica di base.

## 25. Obiettivo finale

Il codice prodotto deve dare questa impressione:

- è C scritto a mano;
- è difensivo;
- non si fida degli input;
- espone API pulite;
- nasconde i dettagli interni;
- documenta bene il contratto;
- gestisce memoria ed errori in modo esplicito;
- preferisce chiarezza algoritmica a soluzioni troppo compatte;
- rispetta il modo di ragionare dei file campione.
