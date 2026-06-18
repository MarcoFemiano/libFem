# Contesto rapido libFem

Il progetto espone una libreria statica C11 chiamata `libFem`.

## Struttura

- Moduli pubblici compilati nel target `libFem`: `stack`, `coda`, `BST_AVL`,
  `hash_table`, `graph`, `LinkedList`.
- Dipendenza vendorizzata: `third_party/xxHash.*`, usata dalla hash table.
- Header ombrello: `libfem.h`, che include tutti i moduli pubblici correnti.
- `tests/` contiene solo test CTest per i moduli ancora presenti e per
  l'header ombrello.

## Build

`CMakeLists.txt` crea il target statico `libFem`, registra CTest e lascia i demo
storici come target separati esclusi dalla build di default.

La toolchain verificata in questo workspace e' quella di CLion:

```powershell
& 'C:\Program Files\JetBrains\CLion 2025.3.3\bin\cmake\win\x64\bin\cmake.exe' -S . -B cmake-build-debug
& 'C:\Program Files\JetBrains\CLion 2025.3.3\bin\cmake\win\x64\bin\cmake.exe' --build cmake-build-debug
& 'C:\Program Files\JetBrains\CLion 2025.3.3\bin\cmake\win\x64\bin\ctest.exe' --test-dir cmake-build-debug --output-on-failure
```

## Nota su AGENTS.md

La parte SAT/DIMACS presente in `AGENTS.md` non corrisponde a questo repository:
non esistono `SatFormula`, `quanteProp` o simboli `SAT_DIMACS`. Per libFem si
mantengono invece aggiornati questi documenti locali.
