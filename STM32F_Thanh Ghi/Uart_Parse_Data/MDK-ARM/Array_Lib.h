#include "stdio.h"
#include "stm32f1xx_hal.h"

/**
 *@brief Find max value in a array, ignore all alphabet character
 *
 *@param[in] Array_Address: address of array
 *@param[in] Array_Length: Actual length of array
 *
 *@return uint32_t: max value in array and ignore all alphabet character
 * */
uint32_t Array_Find_Max_Value(uint8_t *Array_Address, uint32_t Array_Length);

