#pragma once
#include "common.h"

typedef unsigned int HASH;

#define MAX_NAME_SIZE 40 /* doesn't account for terminator character */
typedef char VAR_NAME[MAX_NAME_SIZE + 1];

// types of the variables; each of these custom types has its name starting with "t_"
typedef enum
{
    t_empty, // not declared yet
    t_int,   // basic integer value 32 bits // TODO: automatically extend variable size to avoid overflows ?
    t_float, // floating point real number; this one is decimal, unlike the usual binary ones // TODO: word this decently
    t_char,  // a single character
    t_str,   // a string; both length based and NULL-terminated for maximum compatibility
    t_fixed  // fixed point real number; works like an integer but a set (user defined) number of digits are considered decimals
} VAR_TYPE;

// strings are both length-based and NULL-terminated
typedef struct
{
    size_t length;
    char* data;
} VAR_STRING;

typedef struct
{
    int64_t coeff;
    int64_t value;
} VAR_FIXED;

typedef struct
{
    int64_t coeff;
    int64_t value;
} VAR_FLOAT;

typedef char VAR_CHAR[8]; // for all your unicode needs! TODO: keep or not depending on bman's encoding

typedef union
{
    void* t_empty; // TODO: unsure how to go about this one
    int64_t t_int; // TODO: resizing feature ?
    VAR_FLOAT t_float;
    VAR_CHAR t_char;
    VAR_STRING t_str;
    VAR_FIXED t_fixed;
} VAR_VALUE;

typedef struct VAR
{
    VAR_NAME name;
    VAR_TYPE type; // type of the variable, see enum
    VAR_VALUE value; // points to the actual value, will be allocated dynamically
    struct VAR* alternateValue;
    struct VAR* next; // variables are stored in linked lists in case of collision in the hashtable
} VAR;

// creates the hashtable and sets values that will be used internally by other variable-related functions
// must be called before doing anything
VAR* var_initHashTable(const unsigned int tableSize); // TODO: change return value to FUNCTION_RETURN_CODE and encapsulate ?

// value must be a pointer to an already set value of any VAR_TYPE;
// pointer type values will be copied, so be careful not to use a previously used address !
FUNCTION_RETURN_CODE var_declareVariable(const VAR_NAME name, const VAR_TYPE type, const VAR_VALUE value, VAR* alternateValue);

FUNCTION_RETURN_CODE var_setVariable(VAR* variable, VAR* newValue); // newValue contains the new value, the type to convert from and eventually an alternate value for additional conversion data

VAR* var_getVariable(const VAR_NAME name);


// internal functions
static HASH ivar_getHash(const VAR_NAME name);

static size_t ivar_sizeOf(const VAR_TYPE type); // TODO : useless ? delete ?

static FUNCTION_RETURN_CODE ivar_copyValue(VAR_VALUE* dest, VAR_VALUE src, VAR_TYPE type);

// internal variables
static VAR* var_hashTable;
static unsigned int var_hashTableSize;