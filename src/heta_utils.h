//
// Created by jhnos on 25-05-2025.
//

#ifndef HETA_UTILS_H
#define HETA_UTILS_H

#include "heta_common.h"
#include "heta.h"

typedef struct sObjString ObjString;

// We need buffers of a few different types. To avoid lots of casting between
// void* and back, we'll use the preprocessor as a poor man's generics and let
// it generate a few type-specific ones.
#define DECLARE_BUFFER(name, type)                                             \
  typedef struct                                                             \
  {                                                                          \
    type* data;                                                              \
    int count;                                                               \
    int capacity;                                                            \
  } name##Buffer;                                                            \
  void heta##name##BufferInit(name##Buffer* buffer);                         \
  void heta##name##BufferClear(Heta_VM* vm, name##Buffer* buffer);            \
  void heta##name##BufferFill(Heta_VM* vm, name##Buffer* buffer, type data,   \
                                int count);                                    \
  void heta##name##BufferWrite(Heta_VM* vm, name##Buffer* buffer, type data)

// This should be used once for each type instantiation, somewhere in a .c file.
#define DEFINE_BUFFER(name, type)                                              \
    void heta##name##BufferInit(name##Buffer* buffer)                          \
    {                                                                          \
      buffer->data = NULL;                                                     \
      buffer->capacity = 0;                                                    \
      buffer->count = 0;                                                       \
    }                                                                          \
                                                                               \
    void heta##name##BufferClear(Heta_VM* vm, name##Buffer* buffer)             \
    {                                                                          \
      heta_reallocate(vm, buffer->data, 0, 0);                                  \
      heta##name##BufferInit(buffer);                                          \
    }                                                                          \
                                                                               \
    void heta##name##BufferFill(Heta_VM* vm, name##Buffer* buffer, type data,   \
                                int count)                                     \
    {                                                                          \
      if (buffer->capacity < buffer->count + count)                            \
      {                                                                        \
        int capacity = heta_power_of_2_ceil(buffer->count + count);                \
        buffer->data = (type*)heta_reallocate(vm, buffer->data,                 \
            buffer->capacity * sizeof(type), capacity * sizeof(type));         \
        buffer->capacity = capacity;                                           \
      }                                                                        \
                                                                               \
      for (int i = 0; i < count; i++)                                          \
      {                                                                        \
        buffer->data[buffer->count++] = data;                                  \
      }                                                                        \
    }                                                                          \
                                                                               \
    void heta##name##BufferWrite(Heta_VM* vm, name##Buffer* buffer, type data)  \
    {                                                                          \
      heta##name##BufferFill(vm, buffer, data, 1);                             \
    }

DECLARE_BUFFER(Byte, uint8_t);
DECLARE_BUFFER(Int, int);
DECLARE_BUFFER(String, ObjString*);

typedef StringBuffer SymbolTable;

// Initializes the symbol table.
void heta_symbol_table_init(SymbolTable* symbols);

// Frees all dynamically allocated memory used by the symbol table, but not the
// SymbolTable itself.
void heta_symbol_table_clear(Heta_VM* vm, SymbolTable* symbols);

// Adds name to the symbol table. Returns the index of it in the table.
int heta_symbol_table_add(Heta_VM* vm, SymbolTable* symbols,
                       const char* name, size_t length);

// Adds name to the symbol table. Returns the index of it in the table. Will
// use an existing symbol if already present.
int heta_symbol_table_ensure(Heta_VM* vm, SymbolTable* symbols,
                          const char* name, size_t length);

// Looks up name in the symbol table. Returns its index if found or -1 if not.
int heta_symbol_table_find(const SymbolTable* symbols,
                        const char* name, size_t length);

void heta_blacken_symbol_table(Heta_VM* vm, SymbolTable* symbolTable);

// Returns the number of bytes needed to encode [value] in UTF-8.
//
// Returns 0 if [value] is too large to encode.
int heta_utf8_encode_num_bytes(int value);

// Encodes value as a series of bytes in [bytes], which is assumed to be large
// enough to hold the encoded result.
//
// Returns the number of written bytes.
int heta_utf8_encode(int value, uint8_t* bytes);

// Decodes the UTF-8 sequence starting at [bytes] (which has max [length]),
// returning the code point.
//
// Returns -1 if the bytes are not a valid UTF-8 sequence.
int heta_utf8_decode(const uint8_t* bytes, uint32_t length);

// Returns the number of bytes in the UTF-8 sequence starting with [byte].
//
// If the character at that index is not the beginning of a UTF-8 sequence,
// returns 0.
int heta_utf8_decode_num_bytes(uint8_t byte);

// Returns the smallest power of two that is equal to or greater than [n].
int heta_power_of_2_ceil(int n);

// Validates that [value] is within `[0, count)`. Also allows
// negative indices which map backwards from the end. Returns the valid positive
// index value. If invalid, returns `UINT32_MAX`.
uint32_t heta_validate_index(uint32_t count, int64_t value);
#endif //HETA_UTILS_H
