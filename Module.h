/* Owned by Fernando Zaragoza - January 2020 */
#ifndef _MODULE_H
#define _MODULE_H

/******************************************************************
 *                         I N C L U D E S                        *
 ******************************************************************/

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

 /******************************************************************
  *                         D E F I N E S                          *
  ******************************************************************/
/**
* Utilities
*/
#define SNAKE4(A, B, C, D)  SNAKE(A, SNAKE3(B, C, D))
#define SNAKE3(A, B, C)     SNAKE(SNAKE(A, B), C)
#define SNAKE(A, B)         JOIN3(A, _, B)
#define JOIN4(A, B, C, D)   JOIN(JOIN3(A, B, C), D)
#define JOIN3(A, B, C)      JOIN(JOIN(A, B), C)
#define JOIN(A, B)          DO_JOIN(A, B)
#define DO_JOIN(A, B)       DO_JOIN2(A, B)
#define DO_JOIN2(A, B)      A##B 
#define COUNTOF(x)  ((uint8_t) (sizeof(x) / sizeof((x)[0])) ) // Get number of elements dividing the size of complete thing, by the size of the first element

/**
 * Atomic bit writes
 */
#define setBitAtomic(bit)       ((bit) = 1)
#define clearBitAtomic(bit)     ((bit) = false)
#define assignBitAtomic(bit, condition)         \
    do {                                        \
        if (condition) {                        \
            (bit) = true;                       \
        } else {                                \
            (bit) = 0;                          \
        }                                       \
    } while (zero())

#define SERIAL_DEBUG 1

/******************************************************************
 *               I N L I N E   F U N C T I O N S                  *
 ******************************************************************/
static inline bool zero() { return false; }

/******************************************************************
 *                      T Y P E D E F S                           *
 ******************************************************************/

typedef struct s_moduleDesc
{
    void (*periodicX1_CLK)(void);    // Not-defined periodic processing in CLK
} moduleDesc_S;


#endif

