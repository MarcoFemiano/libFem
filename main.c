//
// main.c - Test per la libreria coda
//

#include <stdio.h>
#include <string.h>
#include "coda.h"

typedef struct {
    int id;
    char nome[32];
} Persona;

static void stampa_esito(const char* operazione, short codice) {
    printf("%s -> %d\n", operazione, codice);
}

static void test_coda_statica_int(void) {
    printf("========================================\n");
    printf("TEST 1 - Coda statica di int\n");
    printf("========================================\n");

    Coda coda = coda_create(3, sizeof(int));
    if (coda == NULL) {
        printf("coda_create fallita\n\n");
        return;
    }

    int x;

    stampa_esito("is_empty iniziale", coda_is_empty(coda));
    printf("size iniziale = %lld\n", coda_size(coda));
    stampa_esito("is_full iniziale", coda_is_full(coda));

    x = 10;
    stampa_esito("push 10", coda_push(coda, &x));

    x = 20;
    stampa_esito("push 20", coda_push(coda, &x));

    x = 30;
    stampa_esito("push 30", coda_push(coda, &x));

    printf("size dopo 3 push = %lld\n", coda_size(coda));
    stampa_esito("is_empty dopo 3 push", coda_is_empty(coda));
    stampa_esito("is_full dopo 3 push", coda_is_full(coda));

    x = 999;
    stampa_esito("peek", coda_peek(coda, &x));
    printf("valore peek = %d\n", x);

    x = 40;
    stampa_esito("push 40 su coda piena", coda_push(coda, &x));

    x = -1;
    stampa_esito("pop 1", coda_pop(coda, &x));
    printf("valore pop 1 = %d\n", x);

    x = -1;
    stampa_esito("pop 2", coda_pop(coda, &x));
    printf("valore pop 2 = %d\n", x);

    printf("size dopo 2 pop = %lld\n", coda_size(coda));
    stampa_esito("is_full dopo 2 pop", coda_is_full(coda));

    x = 40;
    stampa_esito("push 40 dopo liberazione spazio", coda_push(coda, &x));

    x = 50;
    stampa_esito("push 50 dopo liberazione spazio", coda_push(coda, &x));

    printf("size finale prima dello svuotamento = %lld\n", coda_size(coda));

    while (!coda_is_empty(coda)) {
        x = -1;
        stampa_esito("pop", coda_pop(coda, &x));
        printf("valore estratto = %d\n", x);
    }

    stampa_esito("pop su coda vuota", coda_pop(coda, &x));
    stampa_esito("peek su coda vuota", coda_peek(coda, &x));
    printf("size finale = %lld\n", coda_size(coda));

    stampa_esito("destroy", coda_destroy(&coda));
    printf("coda dopo destroy = %p\n\n", (void*)coda);
}

static void test_coda_dinamica_int(void) {
    printf("========================================\n");
    printf("TEST 2 - Coda dinamica di int\n");
    printf("========================================\n");

    Coda coda = coda_create(0, sizeof(int));
    if (coda == NULL) {
        printf("coda_create fallita\n\n");
        return;
    }

    printf("size iniziale = %lld\n", coda_size(coda));
    stampa_esito("is_empty iniziale", coda_is_empty(coda));
    stampa_esito("is_full su dinamica", coda_is_full(coda));

    for (int i = 1; i <= 25; i++) {
        int valore = i * 100;
        short esito = coda_push(coda, &valore);
        printf("push %d -> %d | size = %lld\n", valore, esito, coda_size(coda));
    }

    int x = -1;

    stampa_esito("peek dopo molti push", coda_peek(coda, &x));
    printf("valore peek = %d\n", x);

    printf("\nEstraggo i primi 10 elementi:\n");
    for (int i = 0; i < 10; i++) {
        x = -1;
        stampa_esito("pop", coda_pop(coda, &x));
        printf("valore estratto = %d | size = %lld\n", x, coda_size(coda));
    }

    printf("\nInserisco altri 5 elementi per testare il wrap-around:\n");
    for (int i = 1000; i < 1005; i++) {
        short esito = coda_push(coda, &i);
        printf("push %d -> %d | size = %lld\n", i, esito, coda_size(coda));
    }

    printf("\nSvuotamento completo:\n");
    while (!coda_is_empty(coda)) {
        x = -1;
        stampa_esito("pop", coda_pop(coda, &x));
        printf("valore estratto = %d | size = %lld\n", x, coda_size(coda));
    }

    stampa_esito("destroy", coda_destroy(&coda));
    printf("\n");
}

static void test_coda_struct(void) {
    printf("========================================\n");
    printf("TEST 3 - Coda di struct Persona\n");
    printf("========================================\n");

    Coda coda = coda_create(2, sizeof(Persona));
    if (coda == NULL) {
        printf("coda_create fallita\n\n");
        return;
    }

    Persona p;

    p.id = 1;
    strncpy(p.nome, "Marco", sizeof(p.nome) - 1);
    p.nome[sizeof(p.nome) - 1] = '\0';
    stampa_esito("push Persona 1", coda_push(coda, &p));

    p.id = 2;
    strncpy(p.nome, "Luca", sizeof(p.nome) - 1);
    p.nome[sizeof(p.nome) - 1] = '\0';
    stampa_esito("push Persona 2", coda_push(coda, &p));

    Persona out;

    stampa_esito("peek Persona", coda_peek(coda, &out));
    printf("peek -> id=%d nome=%s\n", out.id, out.nome);

    stampa_esito("pop Persona 1", coda_pop(coda, &out));
    printf("pop -> id=%d nome=%s\n", out.id, out.nome);

    stampa_esito("pop Persona 2", coda_pop(coda, &out));
    printf("pop -> id=%d nome=%s\n", out.id, out.nome);

    stampa_esito("destroy", coda_destroy(&coda));
    printf("\n");
}

static void test_clear(void) {
    printf("========================================\n");
    printf("TEST 4 - coda_clear\n");
    printf("========================================\n");

    Coda coda = coda_create(5, sizeof(int));
    if (coda == NULL) {
        printf("coda_create fallita\n\n");
        return;
    }

    for (int i = 1; i <= 3; i++) {
        int x = i * 7;
        stampa_esito("push", coda_push(coda, &x));
    }

    printf("size prima di clear = %lld\n", coda_size(coda));
    stampa_esito("clear", coda_clear(coda));
    printf("size dopo clear = %lld\n", coda_size(coda));
    stampa_esito("is_empty dopo clear", coda_is_empty(coda));

    int x = -1;
    stampa_esito("pop dopo clear", coda_pop(coda, &x));

    stampa_esito("destroy", coda_destroy(&coda));
    printf("\n");
}

static void test_errori(void) {
    printf("========================================\n");
    printf("TEST 5 - Errori e robustezza\n");
    printf("========================================\n");

    int x = 123;

    stampa_esito("push su coda NULL", coda_push(NULL, &x));
    stampa_esito("pop su coda NULL", coda_pop(NULL, &x));
    stampa_esito("peek su coda NULL", coda_peek(NULL, &x));
    printf("size su coda NULL = %lld\n", coda_size(NULL));
    stampa_esito("is_empty su coda NULL", coda_is_empty(NULL));
    stampa_esito("is_full su coda NULL", coda_is_full(NULL));
    stampa_esito("clear su coda NULL", coda_clear(NULL));

    Coda coda = coda_create(3, sizeof(int));
    if (coda == NULL) {
        printf("coda_create fallita\n\n");
        return;
    }

    stampa_esito("push con datoInput NULL", coda_push(coda, NULL));
    stampa_esito("pop con datoOutput NULL", coda_pop(coda, NULL));
    stampa_esito("peek con datoOutput NULL", coda_peek(coda, NULL));

    stampa_esito("destroy", coda_destroy(&coda));
    stampa_esito("destroy con coda già NULL", coda_destroy(&coda));
    printf("\n");
}

int main(void) {
    test_coda_statica_int();
    test_coda_dinamica_int();
    test_coda_struct();
    test_clear();
    test_errori();

    return 0;
}