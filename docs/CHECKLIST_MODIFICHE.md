# Checklist modifiche libFem

Prima di chiudere una modifica:

- Leggere l'header del modulo coinvolto.
- Verificare ownership: by-value, by-pointer o memoria interna.
- Verificare contratto della callback `cmp`, se presente.
- Controllare `NULL`, struttura vuota, indici fuori range e duplicati.
- Controllare overflow prima di allocazioni `capacity * sizeOfEachElement`.
- Su resize, mantenere rollback o lasciare lo stato vecchio valido se
  l'allocazione fallisce.
- Aggiornare Doxygen nell'header quando cambia il contratto pubblico.
- Aggiornare `README.md` e `docs/WORKLOG_LIBFEM.md` se cambia l'elenco API.
- Aggiungere o aggiornare il test CTest del modulo.
- Per funzioni concorrenti, testare thread count invalidi, output condiviso,
  ripetizioni stress e casi in cui la struttura di output non accetta tutti gli
  elementi.
- Verificare che nessun include locale punti a file assenti.
- Eseguire build e CTest.
