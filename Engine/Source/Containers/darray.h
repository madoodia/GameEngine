// (c) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

enum DArrayField
{
    CAPACITY,
    LENGTH,
    STRIDE,
    FIELD_LENGTH
};

GEAPI void* _CreateDArray(u64 Lenght, u64 Stride);
GEAPI void _DestroyDArray(void* Array);

GEAPI u64 _GetDArrayField(void* Array, u64 Field);
GEAPI void _SetDArrayField(void* Array, u64 Field, u64 Value);

GEAPI void* _ResizeDArray(void* Array);

GEAPI void* _PushDArray(void* Array, const void* ValuePtr);
GEAPI void _PopDArray(void* Array, void* Destination);

GEAPI void* _InsertAtDArray(void* Array, u64 Index, void* Destination);
GEAPI void* _PopDAtArray(void* Array, u64 Index, void* ValuePtr);

#define DAARAY_DEFAULT_CAPACITY 1
#define DAARAY_RESIZE_FACTOR 2

// Used as calling function to replace the private function
#define CreateDArray(Type) \
    _CreateDArray(DAARAY_DEFAULT_CAPACITY, sizeof(Type));

#define ReserveDArray(Type, Capacity) \
    _CreateDArray(Capacity, sizeof(Type));

#define DestroyDArray(Array) _DestroyDArray(Array);

#define PushDArray(Array, Value)                                  \
    {                                                             \
        decltype(Value) Temp = Value;                             \
        auto PurePtr = Array;                                     \
        PurePtr = (decltype(PurePtr))_PushDArray(PurePtr, &Temp); \
        Array = PurePtr;                                          \
    }

#define PopDArray(Array, ValuePtr) \
    _PopDArray(Array, ValuePtr);

#define InsertAtDArray(Array, Index, Value)                            \
    {                                                                  \
        decltype(Value) Temp = Value;                                  \
        Array = (decltype(Array))_InsertAtDArray(Array, Index, &Temp); \
    }

#define PopAtDArray(Array, Index, ValuePtr) \
    _PopDAtArray(Array, Index, ValuePtr);

#define ClearDArray(Array) \
    _SetDArrayField(Array, LENGTH, 0);

#define GetDArrayCapacity(Array) _GetDArrayField(Array, CAPACITY);
#define GetDArrayLength(Array) _GetDArrayField(Array, LENGTH);
#define GetDArrayStride(Array) _GetDArrayField(Array, STRIDE);

#define SetDArrayLength(Array, Value) _SetDArrayField(Array, LENGTH, Value);
