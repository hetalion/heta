//
// Created by jhnos on 25-05-2025.
//

#ifndef HETA_H
#define HETA_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#define HETA_VERSION_MAJOR 0
#define HETA_VERSION_MINOR 1
#define HETA_VERSION_PATCH 0

#define heta_stringify(x) #x
#define heta_tostring(x) heta_stringify(x)

#define HETA_VERSION_STRING heta_tostring(HETA_VERSION_MAJOR) "." heta_tostring(HETA_VERSION_MINOR) "." heta_tostring(HETA_VERSION_PATCH)

#define HETA_VESION_NUMBER (HETA_VERSION_MAJOR * 10000 + HETA_VERSION_MINOR * 100 + HETA_VERSION_PATCH)

#ifndef HETA_API
#if defined(_MSC_VER) && defined(HETA_API_DLLEXPORT)
#define HETA_API __declspec(dllexport)
#else
#define HETA_API
#endif
#endif

// A single virtual machine for executing Heta code.
// Heta has no global state, so all state stored by a running interpreter lives here.
typedef struct Heta_VM Heta_VM;

// A handle to a Heta object.
// This lets code outside of the VM hold a persistent reference to an object.
// After a handle is acquired and until it is released, this ensures the
// garbage collector will not reclaim the object it references.
typedef struct HetaHandle HetaHandle;

typedef void * (*HetaReallocateFn)(void *memory, size_t new_size, void *user_data);

typedef void (*HetaForeignMethodFn)(Heta_VM *vm);

typedef void (*HetaFinalizerFn)(void *data);

typedef const char * (*HetaResolveModuleFn)(Heta_VM *vm,
                                            const char *importer, const char *name);

struct HetaLoadModuleResult;

typedef void (*HetaLoadModuleCompleteFn)(Heta_VM *vm, const char *name, struct HetaLoadModuleResult result);

typedef struct HetaLoadModuleResult {
    const char *source;
    HetaLoadModuleCompleteFn on_complete;
    void *user_data;
} HetaLoadModuleResult;

typedef HetaLoadModuleResult (*HetaLoadModuleFn)(Heta_VM *vm, const char *name);

typedef HetaForeignMethodFn (*HetaBindForeignMethodFn)(Heta_VM *vm,
                                                       const char *module, const char *class_name, bool is_static,
                                                       const char *signature);

typedef void (*HetaWriteFn)(Heta_VM *vm, const char *text);

typedef enum {
    HETA_ERROR_COMPILE,
    HETA_ERROR_RUNTIME,
    HETA_ERROR_STACK_TRACE
} HetaErrorType;

typedef void (*HetaErrorFn)(Heta_VM *vm,
                            HetaErrorType error_type, const char *module, int line, const char *message);

typedef struct {
    HetaForeignMethodFn allocate;
    HetaFinalizerFn finalize;
} HetaForeignClassMethods;

typedef HetaForeignClassMethods (*HetaBindForeignClassFn)(Heta_VM *vm,
                                                          const char *module, const char *class_name);

typedef struct {
    HetaReallocateFn reallocate_fn;
    HetaResolveModuleFn resolve_module_fn;
    HetaLoadModuleFn load_module_fn;
    HetaBindForeignMethodFn bind_foreign_method_fn;
    HetaBindForeignClassFn bind_foreign_class_fn;
    HetaWriteFn write_fn;
    size_t initial_heap_size;
    size_t min_heap_size;
    int heap_growth_percent;
    void *user_data;
} HetaConfiguration;

typedef enum {
    HETA_RESULT_SUCCESS,
    HETA_RESULT_COMPILE_ERROR,
    HETA_RESULT_RUNTIME_ERROR
} HetaInterpretResult;

typedef enum {
    HETA_TYPE_BOOL,
    HETA_TYPE_NUM,
    HETA_TYPE_FOREIGN,
    HETA_TYPE_LIST,
    HETA_TYPE_MAP,
    HETA_TYPE_NULL,
    HETA_TYPE_STRING,
    HETA_TYPE_UNKNOWN
} HetaType;

HETA_API int heta_get_version_number();

HETA_API void heta_init_configuration(HetaConfiguration *configuration);

HETA_API Heta_VM *heta_new_VM(HetaConfiguration *configuration);

HETA_API void heta_free_VM(Heta_VM *vm);

HETA_API void heta_collect_garbage(Heta_VM *vm);

HETA_API HetaInterpretResult heta_interpret(Heta_VM *vm, const char *module, const char *source);

HETA_API HetaHandle *heta_make_call_handle(Heta_VM *vm, const char *signature);

HETA_API HetaInterpretResult heta_call(Heta_VM *vm, HetaHandle *method);

HETA_API void heta_release_handle(Heta_VM *vm, HetaHandle *handle);

HETA_API int heta_get_slot_count(Heta_VM *vm);

HETA_API void heta_ensure_slots(Heta_VM *vm, int num_slots);

HETA_API HetaType heta_get_slot_type(Heta_VM *vm, int slot);

HETA_API bool heta_get_slot_bool(Heta_VM *vm, int slot);

HETA_API const char *heta_get_slot_bytes(Heta_VM *vm, int slot, int *length);

HETA_API double heta_get_slot_double(Heta_VM *vm, int slot);

HETA_API void *heta_get_slot_foreign(Heta_VM *vm, int slot);

HETA_API const char *heta_get_slot_string(Heta_VM *vm, int slot);

HETA_API HetaHandle *heta_get_slot_handle(Heta_VM *vm, int slot);

HETA_API void heta_set_slot_bool(Heta_VM *vm, int slot, bool value);

HETA_API void heta_set_slot_bytes(Heta_VM *vm, int slot, const char *bytes, size_t length);

HETA_API void heta_set_slot_double(Heta_VM *vm, int slot, double value);

HETA_API void *heta_set_slot_new_foreign(Heta_VM *vm, int slot, int class_slot, size_t size);

HETA_API void heta_set_slot_new_list(Heta_VM *vm, int slot);

HETA_API void heta_set_slot_new_map(Heta_VM *vm, int slot);

HETA_API void heta_set_slot_null(Heta_VM *vm, int slot);

HETA_API void heta_set_slot_string(Heta_VM *vm, int slot, const char *text);

HETA_API void heta_set_slot_handle(Heta_VM *vm, int slot, HetaHandle *handle);

HETA_API int heta_get_list_count(Heta_VM *vm, int slot);

HETA_API void heta_get_list_element(Heta_VM *vm, int list_slot, int index, int element_slot);

HETA_API void heta_set_list_element(Heta_VM *vm, int list_slot, int index, int element_slot);

HETA_API void heta_insert_in_list(Heta_VM *vm, int list_slot, int index, int element_slot);

HETA_API int heta_get_map_count(Heta_VM *vm, int slot);

HETA_API bool heta_get_map_contains_key(Heta_VM *vm, int map_slot, int key_slot);

HETA_API void heta_get_map_value(Heta_VM *vm, int map_slot, int key_slot, int value_slot);

HETA_API void heta_set_map_value(Heta_VM *vm, int map_slot, int key_slot, int value_slot);

HETA_API void heta_remove_map_value(Heta_VM *vm, int map_slot, int key_slot, int remove_value_slot);

HETA_API void heta_get_variable(Heta_VM *vm, const char *module, const char *name, int slot);

HETA_API bool heta_has_variable(Heta_VM *vm, const char *module, const char *name);

HETA_API bool heta_has_module(Heta_VM *vm, const char *module);

HETA_API void heta_abort_fiber(Heta_VM *vm, int slot);

HETA_API void heta_get_user_data(Heta_VM *vm);

HETA_API void heta_set_user_data(Heta_VM *vm, void *user_data);
#endif //HETA_H
