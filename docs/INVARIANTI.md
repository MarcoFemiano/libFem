# Invarianti libFem

## Regole comuni

- `OK` vale 0 nei moduli che usano `status_codes`.
- Gli errori sono negativi; gli stati non fatali possono essere positivi.
- Ogni `destroy` libera la memoria interna posseduta dall'ADT e imposta
  l'handle a `NULL`, quando l'API riceve l'indirizzo dell'handle.
- Le funzioni pubbliche controllano i puntatori obbligatori prima di usarli.
- Le allocazioni dipendenti dall'input devono controllare overflow prima di
  moltiplicare capacita' e dimensione elemento.

## Ownership

- `stack`, `coda` e `hash_table` sono container by-value.
- `BST_AVL` e `LinkedList` sono container by-pointer: salvano puntatori
  applicativi e non liberano i dati puntati.
- `graph` possiede la propria rappresentazione interna, inclusi nodi di
  adiacenza e archi.

## Invarianti specifiche

- `stack`: se statico non cresce oltre la capacita'; se dinamico puo'
  riallocare. `size` non deve superare `capacity`.
- `coda`: usa buffer circolare; `head`, `tail`, `size` e `capacity` devono
  restare coerenti anche dopo wrap-around.
- `hash_table`: usa stati slot `0 free`, `1 occupied`, `2 deleted`; le
  tombstone non devono interrompere il linear probing.
- `BST_AVL`: albero vuoto con altezza -1, foglia con altezza 0; i duplicati
  secondo `cmp` sono rifiutati.
- `graph`: i vertici validi sono `0..vertices-1`; Dijkstra accetta solo pesi
  non negativi; `graph_topological_sort` segnala `ERROR_GRAPH_HAS_CYCLE` se il
  grafo diretto non e' aciclico.
- `LinkedList`: lista vuota con `size == 0`, `head == NULL`, `tail == NULL`;
  lista non vuota con `head != NULL`, `tail != NULL`, `tail->next == NULL` e
  catena `next` di esattamente `size` nodi. Ogni nodo contiene `val != NULL`.

## Regole `LinkedList`

- `linkedList_add_at` accetta indici da 0 a `size` inclusi.
- `linkedList_remove_at` e `linkedList_search_index` accettano indici da 0 a
  `size - 1`.
- La ricerca e la rimozione per valore usano il primo nodo per cui
  `cmp(nodo->val, chiave) == 0`.
- Errori di allocazione, indici invalidi e ricerche fallite non devono cambiare
  la lista.
