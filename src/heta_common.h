//
// Created by jhnos on 25-05-2025.
//

#ifndef HETA_COMMON_H
#define HETA_COMMON_H

#define __STDC_LIMIT_MACROS
#include <stdint.h>

#ifndef HETA_NAN_TAGGING
#define HETA_NAN_TAGGING 1
#endif

#ifndef HETA_COMPUTED_FOTO
#if defined(_MSC_VER) && !defined(__clang__)
#define HETA_COMPUTED_FOTO 0
else
#define HETA_COMPUTED_FOTO 1
#endif
#endif

#define HETA_DEBUG_GC_STRESS 0

#define HETA_DEBUG_TRACE_MEMORY 0

#define HETA_DEBUG_TRACE_GC 0

#define HETA_DEBUG_DUMP_COMPILED_CODE 0

#define HETA_DEBUG_TRACE_INSTRUCTIONS 0

#define MAX_MODULE_VARS 65536

#define MAX_PARAMETERS 16

#define MAX_METHOD_NAME 64

#define MAX_METHOD_SIGNATURE (MAX_METHOD_NAME + (MAX_PARAMETERS * 2) + 6)

#define MAX_VARIABLE_NAME 64

#define MAX_FIELDS 255

#define ALLOCATE(vm, type) \
    ((type*)heta_reallocate(vm, NULL, 0, sizeof(type)))

#define ALLOCATE_FLEX(vm, main_type, array_type, count) \
    ((main_type*)heta_reallocate(vm, NULL, 0, sizeof(main_type) + sizeof(array_type) * (count)))

#define ALLOCATE_ARRAY(vm, type, count) \
    ((type*)heta_reallocate(vm, NULL, 0, sizeof(type)* (count)))

#define DEALLOCATE(vm, pointer) \
    heta_reallocate(vm, pointer, 0, 0)

#if defined(_MSC_VER) && !defined(__cplusplus)
    #define inline _inline
#endif

#if __STDC_VERSION__ >= 199901L
#define FLEXIBLE_ARRAY
#else
#define FLEXIBLE_ARRAY 0
#endif

#ifdef DEBUG
#include <stdio.h>
#define ASSERT(condition, message)                                           \
do                                                                       \
{                                                                        \
    if (!(condition))                                                      \
    {                                                                      \
        fprintf(stderr, "[%s:%d] Assert failed in %s(): %s\n",               \
        __FILE__, __LINE__, __func__, message);                          \
        abort();                                                             \
    }                                                                      \
} while (false)

#define UNREACHABLE()                                                        \
do                                                                       \
{                                                                        \
    fprintf(stderr, "[%s:%d] This code should not be reached in %s()\n",   \
    __FILE__, __LINE__, __func__);                                     \
    abort();                                                               \
} while (false)
#else
#define ASSERT(condition, message) do { } while (false)

// Tell the compiler that this part of the code will never be reached.
#if defined( _MSC_VER )
#define UNREACHABLE() __assume(0)
#elif (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5))
#define UNREACHABLE() __builtin_unreachable()
#else
#define UNREACHABLE()
#endif
#endif
#endif //HETA_COMMON_H
