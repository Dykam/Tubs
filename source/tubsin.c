#include "tubsin.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

ushort Tubs_In_MeasureString(const byte *source, char delimeter)
{
	ushort length = 0;
	// Get the length
	while(*(source)++ != delimeter) length++;
	return length;
}

string Tubs_In_ExtractString(const byte *source, char delimeter, ushort *length)
{
	string str;
	*length = Tubs_In_MeasureString(source, delimeter);
	str = malloc(sizeof(char) * *length);
	strncpy(str, source, *length);
	return str;
}

uint Tubs_In_ExtractProvidedDataLength(const byte *source)
{
	return source[0] << 24 | source[1] << 16 | source[2] << 8 | source[3];
}

Tubs_BlockSignaturePtr Tubs_In_ExtractBlockSignature(const byte *source)
{
	Tubs_BlockSignaturePtr sigPtr = malloc(sizeof(Tubs_BlockSignature));
	u8 length = source[0] << 8 | source[1];
	source += 2;
	sigPtr->Length = length;
	while(length-- != 0)
		sigPtr->Data[length] = source[length];
	return sigPtr;
}

bool Tubs_In_Compare_Block_Signatures(Tubs_BlockSignaturePtr a, Tubs_BlockSignaturePtr b)
{
	if(a->Length == b->Length) {
		u8 i = b->Length;
		while(i-- != 0)
			if(a->Data[i] != b->Data[i])
				return false;
	}
	return true;
}

uint Tubs_In_ExtractDataLengthByBlockSignature(const byte *source, Tubs_BlockSignaturePtr signature)
{
	
}

ushort Tubs_In_CountBlocks(const byte *source, uint length)
{
	
}

Tubs_ReadOnlyBlockPtr *Tubs_In_ReadAllBlocks(const byte *source, uint length, ushort *count)
{
	*count = min(Tubs_In_CountBlocks(source, length), *count);
	Tubs_ReadOnlyBlockPtr * blockPtrArray = malloc(sizeof(Tubs_ReadOnlyBlockPtr) * *count);
	ushort current = 0;
	do {
		blockPtrArray[current] = Tubs_In_ReadBlock(source);
		source += blockPtrArray[current]->DataLength;
	} while(++current != *count);
	return blockPtrArray;
}

Tubs_ReadOnlyBlockPtr Tubs_In_ReadBlock(const byte *source)
{
	Tubs_BlockPtr blockPtr = malloc(sizeof(Tubs_Block));
	ushort typeLength;
	string type = Tubs_In_ExtractString(source, '\n', &typeLength);
	source += typeLength;
	typeLength++;
	type = realloc(type, sizeof(char) * typeLength);
	type[typeLength - 1] = '\0';
	blockPtr->Type = type;
	uint dataLength = Tubs_In_ExtractProvidedDataLength(source);
	source += 4;
	if(!dataLength) {
		Tubs_BlockSignaturePtr sigPtr = Tubs_In_ExtractBlockSignature(source);
		blockPtr->Signature->Data = sigPtr->Data;
		dataLength = Tubs_In_ExtractDataLengthByBlockSignature(source, sigPtr);
		source += sigPtr->Length;
	}
	blockPtr->DataLength = dataLength;
	byte* data = malloc(sizeof(byte) * dataLength);
	while(dataLength-- != 0)
		data[dataLength] = source[dataLength];
	blockPtr->Data = data;
	return (Tubs_ReadOnlyBlockPtr)blockPtr;
}