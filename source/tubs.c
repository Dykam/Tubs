#include "tubs.h"
#include <stdlib.h>
#include <stdbool.h>

Tubs_ReadOnlyBlockPtr *registeredBlocks = NULL;
uint registeredBlocksCount = 0;

Tubs_BlockPtr Tubs_CopyBlock(Tubs_BlockPtr source)
{
	Tubs_BlockPtr blockPtr = malloc(sizeof(Tubs_Block));
	byte *array = malloc(sizeof(byte) * source->DataLength);
	uint length = source->DataLength;
	while(length-- != 0) {
		array[length] = source->Data[length]; // offcourse won't work
	}
	return blockPtr;
}

Tubs_BlockPtr Tubs_GetWriteableBlock(Tubs_ReadOnlyBlockPtr source)
{
	return Tubs_CopyBlock((Tubs_BlockPtr)source);
}

/**
 * 0: Succeeded;
 * ~0: Not Implemented, unknown error.
 */
ushort Tubs_RegisterBlock(Tubs_ReadOnlyBlockPtr source)
{
	Tubs_BlockPtr blockPtr = Tubs_CopyBlock((Tubs_BlockPtr)source);
	if(!registeredBlocksCount++) {
		registeredBlocks = malloc(sizeof(Tubs_ReadOnlyBlockPtr));
	} else {
		registeredBlocks = realloc(registeredBlocks, sizeof(Tubs_ReadOnlyBlockPtr) * registeredBlocksCount);
	}
	registeredBlocks[registeredBlocksCount] = (Tubs_ReadOnlyBlockPtr)blockPtr;
	return 0;
}

/**
 * 0: Succeeded;
 * 1: Not found
 * ~0: Not Implemented, unknown error.
 */
ushort Tubs_UnRegisterBlock(Tubs_ReadOnlyBlockPtr source)
{
	uint i = 0;
	bool jump = false;
	Tubs_BlockPtr blockPtr = (Tubs_BlockPtr)source;
	free((void*)blockPtr->Data);
	// Filter out the const byte * to unregister
	while(i != registeredBlocksCount) {
		if(registeredBlocks[i] == source) {
			jump == true;
		}
		if(jump)
			registeredBlocks[i] = registeredBlocks[++i];
		else
			++i;
	}
	// Reallocate a smaller piece of memory.
	registeredBlocks = realloc(registeredBlocks, sizeof(byte*) * --registeredBlocksCount);
	free((void*)source);
	return !jump;
}



