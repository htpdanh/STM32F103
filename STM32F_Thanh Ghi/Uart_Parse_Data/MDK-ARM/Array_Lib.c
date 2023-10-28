#include "stdint.h"
#include "Array_Lib.h"

static void Array_Ignore_Alphabet_Char(uint8_t *Array, uint32_t Array_Length)
{
	uint32_t array_index = 0;
	for(array_index=0; array_index<Array_Length; array_index++)
	{
		if (Array[array_index]<48 || Array[array_index]>57)
		{
			Array[array_index]=0xFF;
		}
	}
}

uint32_t Array_Find_Max_Value(uint8_t *Array_Address, uint32_t Array_Length)
{
	uint32_t array_index = 0;
	uint32_t max = 0;
	Array_Ignore_Alphabet_Char(Array_Address,Array_Length);
	for(array_index=0; array_index<Array_Length; array_index++)
	{
		if(max < Array_Address[array_index] && Array_Address[array_index]!=0xFF)
		{
			max = Array_Address[array_index];
		}
	}
}
