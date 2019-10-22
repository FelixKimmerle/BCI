#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type *)allocateObject(sizeof(type), objectType)

static Obj *allocateObject(size_t size, ObjType type)
{
    Obj *object = (Obj *)reallocate(NULL, 0, size);
    object->type = type;
    object->next = vm.objects;
    vm.objects = object;

    return object;
}

static ObjString *allocateString(const char *chars, int length, uint32_t hash)
{
    ObjString *string = (ObjString *)allocateObject(sizeof(ObjString) + (length + 1) * sizeof(char), OBJ_STRING);
    string->length = length;
    memcpy(&string->chars, chars, length);
    string->chars[length] = '\0';
    string->hash = hash;

    return string;
}

static uint32_t hashString(const char *key, int length)
{
    uint32_t hash = 2166136261u;

    for (int i = 0; i < length; i++)
    {
        hash ^= key[i];
        hash *= 16777619;
    }
    return hash;
    
}

ObjString *emptyString(int length)
{
    ObjString *string = (ObjString *)allocateObject(sizeof(ObjString) + (length + 1) * sizeof(char), OBJ_STRING);
    string->length = length;
    return string;
}

void UpdateHash(ObjString *str)
{
    str->hash = hashString(str->chars, str->length);
}

ObjString *copyString(const char *chars, int length)
{
    uint32_t hash = hashString(chars, length);
    return allocateString(chars, length, hash);
}

void printObject(Value value)
{
    switch (OBJ_TYPE(value))
    {
    case OBJ_STRING:
        printf("%s", AS_CSTRING(value));
        break;
    }
}