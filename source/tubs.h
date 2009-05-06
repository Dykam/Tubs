#include "standard.h"

typedef byte Tubs_BlockSignature[64]; 
typedef Tubs_BlockSignature *Tubs_BlockSignaturePtr;

typedef struct
{
	const string Type;
	const Tubs_BlockSignaturePtr Signature;
	const u32 DataLength;
	const byte const *Data;
} const Tubs_ReadOnlyBlock;
typedef Tubs_ReadOnlyBlock const *Tubs_ReadOnlyBlockPtr; // These are for regular use.

typedef struct
{
	string Type;
	Tubs_BlockSignaturePtr Signature;
	u32 DataLength;
	byte const *Data;
} Tubs_Block;
typedef Tubs_Block *Tubs_BlockPtr;


Tubs_BlockPtr Tubs_CopyBlock(Tubs_BlockPtr source);
Tubs_BlockPtr Tubs_GetWriteableBlock(Tubs_ReadOnlyBlockPtr source);

u8 Tubs_RegisterBytes(const byte * source, u32 length);
u8 Tubs_UnRegisterBytes(const byte const *source);

typedef Tubs_BlockPtr *(*Tubs_DataDefenitionRequestDelegate)(u32 *lenght);
