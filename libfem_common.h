/**
 * @file libfem_common.h
 * @brief Tipi comuni leggeri per i nuovi moduli di libFem.
 *
 * Questo header raccoglie il booleano minimale e i codici di stato usati dai
 * nuovi ADT della libreria. I moduli storici mantengono i loro header per non
 * rompere le API gia' presenti.
 */

#ifndef LIBFEM_COMMON_H
#define LIBFEM_COMMON_H

#include <stddef.h>

/** @brief Tipo booleano minimale usato dal progetto. */
typedef char bool;

/** @brief Valore booleano vero. */
#ifndef true
#define true 1
#endif

/** @brief Valore booleano falso. */
#ifndef false
#define false 0
#endif

/**
 * @brief Codici di stato comuni.
 *
 * `OK` vale sempre 0. Gli errori reali sono negativi. Gli stati positivi
 * rappresentano condizioni non fatali, come elemento non trovato.
 */
typedef enum {
  OK = 0,
  ERROR_NULL_POINTER = -1,
  ERROR_ALLOCATION_FAILURE = -2,
  ERROR_ARITHMETIC_OVERFLOW = -3,
  ERROR_INVALID_ARGUMENT = -4,
  ERROR_EMPTY_STRUCTURE = -5,
  ERROR_OUT_OF_BOUNDS = -6,
  ERROR_NOT_FOUND = -7,
  ERROR_FULL_STRUCTURE = -8,
  ERROR_VALUE_ALREADY_EXISTS = -9,
  ERROR_GRAPH_HAS_CYCLE = -10,
  ERROR_STACK_EMPTY = -2,
  ERROR_STACK_FULL = -3,
  ERROR_NULL_DATA_POINTER = -4,
  ERROR_REALLOC_FAIL = -5,
  ERROR_FUNCTION_NOT_VALID_IN_THIS_USE_CASE = -6,
  ERROR_CODA_EMPTY = -2,
  ERROR_CODA_FULL = -3,
  ERROR_EMPTY_TREE = -5,
  ERROR_REALLOC_FAILURE = -3,
  HT_OK = 0,
  HT_ERROR_NULL_POINTER = -1,
  HT_ERROR_ALLOCATION_FAILURE = -2,
  HT_ERROR_REALLOCATION_FAILURE = -3,
  HT_ERROR_ARITHMETIC_OVERFLOW = -4,
  HT_ERROR_WRONG_ELEMENTS_SIZE = -5,
  HT_ERROR_SEARCHING_FOR_ZERO = -6,
  HT_ERROR_VALUE_ALREADY_EXISTS = -7,
  HT_ERROR_SEMANTIC_BUG_IT_SHOULDNT_HAPPEN = -8,
  HT_ELEMENTO_NON_TROVATO = 1,
  ELEMENTO_NON_TROVATO = 1
} status_codes;

#define NODE_ALREADY_EXISTS ELEMENTO_NON_TROVATO

#endif /* LIBFEM_COMMON_H */
