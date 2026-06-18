# libFem

`libFem` e' una libreria C11 di strutture dati classiche scritte come ADT C:
header pubblici, strutture opache quando possibile, memoria gestita
esplicitamente e codici di stato documentati.

## Componenti incluse

| Componente | Modulo principale | Ownership dati |
| --- | --- | --- |
| Stack | `stack.h` / `stack.c` | by value |
| Coda FIFO | `coda.h` / `coda.c` | by value |
| Hash table | `hash_table.h` / `hash_table.c` | by value |
| AVL tree | `BST_AVL.h` / `BST_AVL.c` | by pointer |
| Graph adjacency list | `graph.h` / `graph.c` | memoria interna |
| LinkedList | `LinkedList.h` / `LinkedList.c` | by pointer |

`third_party/xxHash.c` e `third_party/xxHash.h` sono usati dalla hash table.

## Header ombrello

`libfem.h` include le API pubbliche della libreria:

```c
#include "libfem.h"
```

I moduli possono anche essere inclusi singolarmente quando serve un solo ADT.

## Build e test

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Nel workspace corrente e' disponibile anche la toolchain CLion configurata:

```powershell
& 'C:\Program Files\JetBrains\CLion 2025.3.3\bin\cmake\win\x64\bin\cmake.exe' -S . -B cmake-build-debug
& 'C:\Program Files\JetBrains\CLion 2025.3.3\bin\cmake\win\x64\bin\cmake.exe' --build cmake-build-debug
& 'C:\Program Files\JetBrains\CLion 2025.3.3\bin\cmake\win\x64\bin\ctest.exe' --test-dir cmake-build-debug --output-on-failure
```

Il target principale e' la libreria statica `libFem`. I demo storici sono
target separati esclusi dalla build di default.

## Mini esempi

Stack by value:

```c
#include "stack.h"

int main(void) {
  Stack stack = stack_create(2, sizeof(int));
  int value = 42;
  int out = 0;

  if (stack == NULL) return 1;

  stack_push(stack, &value);
  stack_pop(stack, &out);
  stack_destroy(&stack);

  return out == 42 ? 0 : 1;
}
```

Hash table by value:

```c
#include "hash_table.h"

static int int_cmp(const void* a, const void* b) {
  int x = *(const int*)a;
  int y = *(const int*)b;
  if (x < y) return -1;
  if (x > y) return 1;
  return 0;
}

int main(void) {
  HashTable table = NULL;
  int value = 42;
  bool found = false;

  if (hashTable_create(&table, 4, sizeof(int)) != HT_OK) return 1;

  hashTable_push(table, int_cmp, &value);
  hashTable_search(table, int_cmp, &value, &found);
  hashTable_destroy(&table);

  return found ? 0 : 1;
}
```

LinkedList by pointer:

```c
#include "LinkedList.h"

int main(void) {
  LinkedList list = linkedList_create();
  int value = 42;
  int* first = NULL;

  if (list == NULL) return 1;

  linkedList_add_tail(list, &value);
  first = linkedList_search_index(list, 0);
  linkedList_destroy(&list);

  return first == &value ? 0 : 1;
}
```

## Note di ownership

- `stack`, `coda` e `hash_table` copiano i dati by value. Se un elemento
  contiene puntatori interni, la copia e' superficiale.
- `BST_AVL` e `LinkedList` salvano puntatori applicativi e non liberano i dati
  puntati.
- `graph` possiede solo la propria rappresentazione interna: liste di
  adiacenza, archi e buffer temporanei allocati dagli algoritmi.

Per dettagli di contesto e invarianti vedere `docs/`.
