#include "application.h"
#define UINT32_T_MAX 4294967295

volatile uint32_t ms_ticks = 0;

void HAL_IncTick(void)
{
    ms_ticks++;
}

void delay_ms(uint32_t delay_time_ms)
{
	uint32_t expected_ticks = ms_ticks + delay_time_ms;

    if (expected_ticks >= UINT32_T_MAX)
    {
        ms_ticks = 0;
        expected_ticks = delay_time_ms;
    }

	while (ms_ticks < expected_ticks)
	{
		__asm("nop");
	}
}

void app_main(void)
{
    GPIO_InitTypeDef my_GPIO_InitStruct = {0};

    // GPIO Ports Clock Enable
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Configure GPIO pin Output Level
    // Since PC13 LED is source current so we set PC13 default at 1
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

    // Configure GPIO pin : PC13
    my_GPIO_InitStruct.Pin = GPIO_PIN_13;
    my_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    my_GPIO_InitStruct.Pull = GPIO_PULLUP;
    my_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &my_GPIO_InitStruct);

    /*
    * Do ham HAL_SYSTICK_config luon mac dinh lay clock la AHB khong chia cho 8
    * nen, T = 1 / 8*10^6 = 1.25*10^-7, de co tick cho 1ms thi SysTick_load = 1*10^-3/1.25*10-7
    */
    // 1tick = 1Mhz = 1ns, 1ms = 1khz = 1000 tick
    HAL_SYSTICK_Config(8000);

    while(1)
    {
        delay_ms(1000);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    }
}