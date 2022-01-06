#include "variables.h"

VAR* var_initHashTable(const unsigned int tableSize)
{
    // create hashtable
    var_hashTable = malloc(sizeof(VAR) * tableSize);
    var_hashTableSize = tableSize;

    // init values
    for (int i = 0; i < var_hashTableSize; i++)
    {
        var_hashTable[i].name[0] = '\0';
        var_hashTable[i].type = t_empty;
        var_hashTable[i].value.t_empty = NULL;
        var_hashTable[i].alternateValue = NULL;
        var_hashTable[i].next = NULL;
    }

    return var_hashTable;
}

FUNCTION_RETURN_CODE var_declareVariable(const VAR_NAME name, const VAR_TYPE type, const VAR_VALUE value, VAR* alternateValue)
{
    VAR* var = &var_hashTable[ivar_getHash(name)];

    // if there is no linked list for this hash yet, create one
    if (var->type == t_empty)
    { // TODO: check length ? (performance...)
        strcpy(var->name, name);
        var->type = type;
        var->value.t_empty = 0;
        if (type == t_str) // initialize pointer if it's a string
        {
            var->value.t_str.data = malloc(sizeof(char) * value.t_str.length);
        }
        ivar_copyValue(&var->value, value, type); // TODO: can be optimized ?
        var->alternateValue = alternateValue;
        var->next = NULL;
    }
    else
    {
        // looking for end of linked list
        while (var->next != NULL)
        {
            var = var->next;
        }

        // adding new variable
        VAR* newVar = malloc(sizeof(VAR));
        strcpy(newVar->name, name);
        newVar->type = type;
        newVar->value.t_empty = 0;
        if (type == t_str) // initialize pointer if it's a string
        {
            newVar->value.t_str.data = malloc(sizeof(char) * value.t_str.length);
        }
        ivar_copyValue(&newVar->value, value, type); // TODO: can be optimized ?
        newVar->alternateValue = alternateValue;
        newVar->next = NULL;

        // appending it to the linked list
        var->next = newVar;
    }

    return FUNCTION_RETURN_EVERYTHING_OK;
}

FUNCTION_RETURN_CODE var_setVariable(VAR* variable, VAR* newValue)
{
    if (variable->type == newValue->type)
    {
        ivar_copyValue(&variable->value, newValue->value, newValue->type);
        variable->alternateValue = newValue->alternateValue;

        return FUNCTION_RETURN_EVERYTHING_OK;
    }
    else
    {
        // TODO: implement the whole thing lmao; switch statement calling dedicated casting functions
        return -1;
    }
}

VAR* var_getVariable(const VAR_NAME name)
{
    VAR* var = &var_hashTable[ivar_getHash(name)];

    do
    {
        if (strcmp(name, var->name) == 0)
        {
            return var;
        }
        var = var->next;
    } while (var != NULL);

    return NULL;
}

HASH ivar_getHash(const VAR_NAME name)
{
    HASH hash = 0;

    for (int i = 0; name[i]; i++)
    {
        hash += name[i] * i;
    }

    return hash % var_hashTableSize;
}

size_t ivar_sizeOf(const VAR_TYPE type)
{
    switch (type)
    {
    case t_char:
        return sizeof(char);

    case t_int:
        return sizeof(int_fast64_t);

    case t_fixed:
        return (size_t) 8; // TODO: implement fixed

    case t_float:
        return (size_t) 8; // TODO: implement float

    case t_str:
        return sizeof(VAR_STRING);

    case t_empty:
    default:
        return 0; // TODO: error code using ssize_t ?
    };
}

FUNCTION_RETURN_CODE ivar_copyValue(VAR_VALUE* dest, VAR_VALUE src, VAR_TYPE type)
{
    switch (type)
    {
    case t_str: {
        dest->t_str.length = src.t_str.length;
        memcpy(dest->t_str.data, src.t_str.data, src.t_str.length);
        dest->t_str.data[dest->t_str.length] = '\0';

        return FUNCTION_RETURN_EVERYTHING_OK;
    }

    default:
        memcpy(dest, &src, sizeof(VAR_VALUE));
        return FUNCTION_RETURN_EVERYTHING_OK;
    }
}
