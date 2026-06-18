# Worklog libFem

Stato corrente della libreria dopo la pulizia dei moduli rimossi.

## Componenti attive

- [x] Stack: `stack.*`, ADT generico by-value con modalita' statica o dinamica.
- [x] Coda: `coda.*`, FIFO generica by-value su buffer circolare.
- [x] Hash table: `hash_table.*`, open addressing con linear probing e
  tombstone; usa `third_party/xxHash.*`.
- [x] AVL tree: `BST_AVL.*`, albero autobilanciato by-pointer.
- [x] Graph: `graph.*`, liste di adiacenza con BFS, DFS, Dijkstra e
  ordinamento topologico.
- [x] LinkedList: `LinkedList.*`, lista semplicemente collegata by-pointer.

## Build

- `CMakeLists.txt` compila solo i moduli attivi nel target `libFem`.
- `libfem.h` include solo gli header pubblici attivi.
- `tests/` contiene solo test dei moduli attivi e dell'header ombrello.

## Verifica richiesta

- Configurare CMake.
- Compilare il target di default.
- Eseguire CTest.
- Cercare riferimenti residui a moduli rimossi in build, test, README e docs.
