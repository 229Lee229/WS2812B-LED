#include "stm32f1xx_hal.h"
#include "driver_ws2812b.h"
#include <stdarg.h>
#include <stdio.h>
#include "stm32f1xx_hal_spi.h"

extern SPI_HandleTypeDef hspi2; // created by CubeMX

static uint8_t ws_spi_inited = 0;


uint8_t ws_spi_10mhz_init(void)
{
    if (ws_spi_inited) return 0;

    // If CubeMX already initializes hspi1 in MX_SPI1_Init(), just call it once.
    // Ensure Prescaler => ~9 MHz (72/8) and other settings per above.
    if (HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_RESET)
    {
        // If you have MX_SPI1_Init(), call it here.
        extern void MX_SPI2_Init(void);
        MX_SPI2_Init();
    }

    ws_spi_inited = 1;
    return 0;
}

uint8_t ws_spi_deinit(void)
{
    ws_spi_inited = 0;
    return (HAL_SPI_DeInit(&hspi2) == HAL_OK) ? 0 : 1;
}

uint8_t ws_spi_write_cmd(uint8_t *buf, uint16_t len)
{
    // Blocking transmit. For long strips, switch to HAL_SPI_Transmit_DMA.
    return (HAL_SPI_Transmit(&hspi2, buf, len, 1000) == HAL_OK) ? 0 : 1;
}

void ws_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

void ws_debug_print(const char *const fmt, ...)
{
    char out[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(out, sizeof(out), fmt, args);
    va_end(args);
    // Route to ITM/SEGGER RTT/UART as you prefer; here we use SWO ITM.
    // ITM_SendChar not always available; replace with your UART print if needed.
    // Example placeholder:
    // printf("%s", out);
}
