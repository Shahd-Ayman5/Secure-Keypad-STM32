#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#include <stdint.h>

/* Unsigned types */
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

/* Signed types */
typedef int8_t   sint8;
typedef int16_t  sint16;
typedef int32_t  sint32;

/* Boolean type */
typedef uint8       bool_t;
#define TRUE     ((bool_t)1U)
#define FALSE    ((bool_t)0U)

/* Generic pointer for raw memory access */
// typedef void*    GenericPtr_t;

/* Standard return type */
typedef uint8     Std_ReturnType;
#define STD_SUCCESS  ((Std_ReturnType)1U)
#define STD_FAILURE  ((Std_ReturnType)0U)

/* Generic pointer type */
typedef void*       Ptr_t;          /* Generic pointer */
typedef const void* CPtr_t;         /* Generic const pointer */

#define NULL_PTR ((void*)0)

#endif /* STD_TYPES_H_ */
