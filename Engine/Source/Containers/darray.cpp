// (C) 2026 madoodia.com
// ---------------------

#include "Containers/darray.h"
#include "Core/ge_memory.h"
#include "Core/logger.h"

void* _CreateDArray(u64 Length, u64 Stride)
{
    // NOTE: If DArrayField enum defined as struct, for multiplication
    // we have to implement operator*
    u64 HeaderSize = DArrayField::FIELD_LENGTH * sizeof(u64);
    u64 ArraySize = Length * Stride;
    u64* NewArray = (u64*)AllocateMemory(HeaderSize + ArraySize, MEMORY_TAG_DARRAY);
    SetMemory(NewArray, 0, HeaderSize + ArraySize);
    NewArray[DArrayField::CAPACITY] = Length;
    NewArray[DArrayField::LENGTH] = 0;
    NewArray[DArrayField::STRIDE] = Stride;
    return (void*)(NewArray + DArrayField::FIELD_LENGTH);
}

void _DestroyDArray(void* Array)
{
    u64* Header = (u64*)Array - DArrayField::FIELD_LENGTH;
    u64 HeaderSize = DArrayField::FIELD_LENGTH * sizeof(u64);
    u64 TotalSize = HeaderSize + Header[DArrayField::CAPACITY] * Header[DArrayField::STRIDE];
    FreeMemory(Header, TotalSize, MEMORY_TAG_DARRAY);
}

u64 _GetDArrayField(void* Array, u64 Field)
{
    u64* Header = (u64*)Array - DArrayField::FIELD_LENGTH;
    return Header[Field];
}

void _SetDArrayField(void* Array, u64 Field, u64 Value)
{
    u64* Header = (u64*)Array - DArrayField::FIELD_LENGTH;
    Header[Field] = Value;
}

void* _ResizeDArray(void* Array)
{
    u64 Length = GetDArrayLength(Array);
    u64 Stride = GetDArrayStride(Array);
    u64 Capacity = GetDArrayCapacity(Array);
    void* Temp = _CreateDArray(Capacity * DAARAY_RESIZE_FACTOR, Stride);
    CopyMemory(Temp, Array, Length * Stride);

    _SetDArrayField(Temp, LENGTH, Length);
    _DestroyDArray(Array);
    return Temp;
}

void* _PushDArray(void* Array, const void* ValuePtr)
{
    u64 Length = GetDArrayLength(Array);
    u64 Stride = GetDArrayStride(Array);
    u64 Capacity = GetDArrayCapacity(Array);
    if (Length >= Capacity)
    {
        Array = _ResizeDArray(Array);
    }
    u64 Address = (u64)Array + Length * Stride;
    CopyMemory((void*)Address, ValuePtr, Stride);
    _SetDArrayField(Array, LENGTH, Length + 1);
    return Array;
}

void _PopDArray(void* Array, void* Destination)
{
    u64 Length = GetDArrayLength(Array);
    u64 Stride = GetDArrayStride(Array);
    u64 Address = (u64)Array + (Length - 1) * Stride;
    CopyMemory(Destination, (void*)Address, Stride);
    _SetDArrayField(Array, LENGTH, Length - 1);
}

void* _PopDAtArray(void* Array, u64 Index, void* Destination)
{
    u64 Length = GetDArrayLength(Array);
    u64 Stride = GetDArrayStride(Array);
    if (Index >= Length)
    {
        GEERROR("Index out of bounds in this Array. Index: %i, Length: %i", Index, Length);
        return Array;
    }
    u64 Address = (u64)Array;
    CopyMemory(Destination, (void*)(Address + (Index * Stride)), Stride);
    if (Index != Length - 1)
    {
        CopyMemory((void*)(Address + (Index * Stride)), (void*)(Address + ((Index + 1) * Stride)), Stride * (Length - Index));
    }
    _SetDArrayField(Array, LENGTH, Length - 1);
    return Array;
}

void* _InsertAtDArray(void* Array, u64 Index, void* ValuePtr)
{
    u64 Length = GetDArrayLength(Array);
    u64 Stride = GetDArrayStride(Array);
    u64 Capacity = GetDArrayCapacity(Array);

    if (Index >= Length)
    {
        GEERROR("Index out of bounds in this Array. Index: %i, Length: %i", Index, Length);
        return Array;
    }

    if (Length >= Capacity)
    {
        Array = _ResizeDArray(Array);
    }

    u64 Address = (u64)Array;

    if (Index != Length - 1)
    {
        CopyMemory((void*)((u8*)Address + ((Index + 1) * Stride)), (void*)((u8*)Address + (Index * Stride)), Stride * (Length - Index));
    }

    CopyMemory((void*)((u8*)Address + (Index * Stride)), ValuePtr, Stride);
    _SetDArrayField(Array, LENGTH, Length + 1);
    return Array;
}
