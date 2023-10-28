#include "Uart_Middleware.h"
#include <stdbool.h>
#include <string.h>
#include "Array_lib.h"
#include "stm32f1xx_hal_uart.h"

#define RX_BUFFER_LINE 4U
#define RX_BUFFER_SIZE 80U

static uint8_t RX_Buffer[RX_BUFFER_LINE][RX_BUFFER_SIZE] = {0};