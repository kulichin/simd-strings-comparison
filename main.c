#include "helpers.h"

#include <stdint.h>
#include <intrin.h>
#include <stdio.h>

#define DEBUG_PRINT 			 0
#define USE_AVX2_INSTRUCTIONS	 0
#define NUM_OF_GENERATED_STRINGS 100000 // Must be a multiple of 2

#if USE_AVX2_INSTRUCTIONS
	#define STRING_SIZE 32
#else
	#define STRING_SIZE 16
#endif

inline char *GenerateStrings()
{	
	char *Strings = (char *)malloc(NUM_OF_GENERATED_STRINGS * STRING_SIZE * sizeof(char *));
	const char CharSet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK";
	
	int Offset = 0;
	for(int CurrentString = 0; CurrentString <= NUM_OF_GENERATED_STRINGS; CurrentString++)
	{
		srand(__rdtsc());
		for(int Index = 0; Index < STRING_SIZE - 1; Index++)
		{
			Strings[Index + Offset] = CharSet[rand() % 37];
		}
		
		Strings[Offset + STRING_SIZE - 1] = '\0';
		Offset += STRING_SIZE;
	}
	
	return Strings;
}

inline void PrintMask(uint32_t Mask)
{
#if USE_AVX2_INSTRUCTIONS
	printf(
		"Mask: ""%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
		(Mask & 0x800000000 ? '1' : '0'),
		(Mask & 0x400000000 ? '1' : '0'),
		(Mask & 0x200000000 ? '1' : '0'),
		(Mask & 0x100000000 ? '1' : '0'),
		(Mask & 0x080000000 ? '1' : '0'),
		(Mask & 0x040000000 ? '1' : '0'),
		(Mask & 0x020000000 ? '1' : '0'),
		(Mask & 0x010000000 ? '1' : '0'),
		(Mask & 0x008000000 ? '1' : '0'),
		(Mask & 0x004000000 ? '1' : '0'),
		(Mask & 0x002000000 ? '1' : '0'),
		(Mask & 0x001000000 ? '1' : '0'),
		(Mask & 0x00080000 ? '1' : '0'),
		(Mask & 0x00040000 ? '1' : '0'),
		(Mask & 0x00020000 ? '1' : '0'),
		(Mask & 0x00010000 ? '1' : '0'),
		(Mask & 0x00008000 ? '1' : '0'),
		(Mask & 0x00004000 ? '1' : '0'),
		(Mask & 0x00002000 ? '1' : '0'),
		(Mask & 0x00001000 ? '1' : '0'),
		(Mask & 0x00000800 ? '1' : '0'),
		(Mask & 0x00000400 ? '1' : '0'),
		(Mask & 0x00000200 ? '1' : '0'),
		(Mask & 0x00000100 ? '1' : '0'),
		(Mask & 0x00000080 ? '1' : '0'),
		(Mask & 0x00000040 ? '1' : '0'),
		(Mask & 0x00000020 ? '1' : '0'),
		(Mask & 0x00000010 ? '1' : '0'),
		(Mask & 0x00000008 ? '1' : '0'),
		(Mask & 0x00000004 ? '1' : '0'),
		(Mask & 0x00000002 ? '1' : '0'),
		(Mask & 0x00000001 ? '1' : '0'));

#else // USE_SSE_INSTRUCTIONS
	printf(
		"Mask: ""%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
		(Mask & 0x8000 ? '1' : '0'),
		(Mask & 0x4000 ? '1' : '0'),
		(Mask & 0x2000 ? '1' : '0'),
		(Mask & 0x1000 ? '1' : '0'),
		(Mask & 0x0800 ? '1' : '0'),
		(Mask & 0x0400 ? '1' : '0'),
		(Mask & 0x0200 ? '1' : '0'),
		(Mask & 0x0100 ? '1' : '0'),
		(Mask & 0x0080 ? '1' : '0'),
		(Mask & 0x0040 ? '1' : '0'),
		(Mask & 0x0020 ? '1' : '0'),
		(Mask & 0x0010 ? '1' : '0'),
		(Mask & 0x0008 ? '1' : '0'),
		(Mask & 0x0004 ? '1' : '0'),
		(Mask & 0x0002 ? '1' : '0'),
		(Mask & 0x0001 ? '1' : '0'));
#endif
}

inline
#if USE_AVX2_INSTRUCTIONS
	__m256i
#else // USE_SSE_INSTRUCTIONS
	__m128i
#endif

InitializeVectorizedString(char *String)
{
#if USE_AVX2_INSTRUCTIONS
	   return _mm256_set_epi8(
        String[0],  String[1],  String[2],  String[3],  String[4],
		String[5],  String[6],  String[7],  String[8],  String[9],
        String[10], String[11], String[12], String[13], String[14],
		String[15], String[16], String[17], String[18], String[19],
		String[20], String[21], String[22], String[23], String[24],
		String[25], String[26], String[27], String[28], String[29],
		String[30], '\0');
		
#else // USE_SSE_INSTRUCTIONS
	return _mm_set_epi8(
        String[0],  String[1],  String[2],  String[3],  String[4],
		String[5],  String[6],  String[7],  String[8],  String[9],
        String[10], String[11], String[12], String[13], String[14],
		'\0');

#endif
}

int main()
{
	// Generating strings
	printf("Generating %i random strings ...\n", NUM_OF_GENERATED_STRINGS);
	char *Strings = GenerateStrings();
	
	// Initializing variables outside of "START_COUNTING_CPU_CYCLES", because this may affect the cycles counter.
	uint32_t Mask = 0;
	uint32_t CurrentString = 0;
	uint32_t Offset = 0;
	
	// Comparing SSE strings
	printf("Comparing strings ...\n\n");
	START_COUNTING_CPU_CYCLES();

	while(CurrentString < (NUM_OF_GENERATED_STRINGS / 2))
	{
		// Moving to the next string in a sequenced array
		Offset += STRING_SIZE;
		char *String1 = &Strings[Offset];
		
		// Moving to the next string in a sequenced array
		Offset += STRING_SIZE;
		char *String2 = &Strings[Offset];
		
		// Next string
		CurrentString++;
		
#if USE_AVX2_INSTRUCTIONS
	// Initializing and comparing AVX2 strings
	__m256i VectorString1 = InitializeVectorizedString(String1);
	__m256i VectorString2 = InitializeVectorizedString(String2);
	Mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(VectorString1, VectorString2));

#else // USE_SSE_INSTRUCTIONS
	// Initializing and comparing SSE strings
	__m128i VectorString1 = InitializeVectorizedString(String1);
	__m128i VectorString2 = InitializeVectorizedString(String2);
	Mask = _mm_movemask_epi8(_mm_cmpeq_epi8(VectorString1, VectorString2));
#endif	
	
#if DEBUG_PRINT
		if(Mask > 1)
		{
			printf("Found characters at the same place: %s | %s: ", String1, String2);
			PrintMask(Mask);
		}
#endif
	}
	END_COUNTING_CPU_CYCLES();
	
	// IMPORTANT!!
	printf("LastMask: %i\n", Mask);
	
	// Free strings
	printf("Free strings ...\n");
	// TODO: check this
	free(Strings);
	
	//uint32_t CurrentSymbol = 0;
	//while(CurrentSymbol < STRINGS_ARRAY_SIZE_OF)
	//{
	//}
}
