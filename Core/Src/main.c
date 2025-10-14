/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ws2812b_port.h"
#include "driver_ws2812b.h"
#include "stdbool.h"
// #include "driver_ws2812b_basic.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
struct soft_timer {
	uint32_t timeout;
	void * args;
	void (*func)(void *);
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define		BUS_SET_BIT	 	1		
#define		BUS_RESET_BIT	0		
#define		TOGGLE_LED()  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_5);

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int g_key_cnt = 0;
 uint8_t Flash = 0;

void key_timeout_func(void *args);

struct soft_timer key_timer = {~0, NULL, key_timeout_func};
struct soft_timer dimmer_1_timer = {~0, NULL, key_timeout_func};

void key_timeout_func(void *args)
{
	g_key_cnt++;
	key_timer.timeout = ~0;
}

void mod_timer(struct soft_timer *pTimer, uint32_t timeout)
{
	pTimer->timeout = HAL_GetTick() + timeout;
}

void check_timer(void)
{
	if (key_timer.timeout <= HAL_GetTick())
	{
		Flash = 1;
		key_timer.func(key_timer.args);
	}
	if (dimmer_1_timer.timeout <= HAL_GetTick()){
		Flash = 1;
		dimmer_1_timer.func(dimmer_1_timer.args);
	}
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t pulse1 = 0;
static bool toggle_strobe = false;
static bool toggle_breath = false;
static uint8_t breath_color = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM2) {
       TOGGLE_LED(); //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_5); // ??PA5??(LED)
////		TIM_OC_InitTypeDef sConfigOC = {0};
////		sConfigOC.OCMode = TIM_OCMODE_PWM1;
////		sConfigOC.Pulse = 
////		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
////		sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
////		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
////		HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, TIM_CHANNEL_1);
////		HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);  // ?? PWM
//		
//		if(pulse1 == 0){
//			mod_timer(&dimmer_1_timer,2000);
//			pulse1 = 1;
//						WS2812_SetPulse(&htim1,pulse1);	

//			Flash = 2;
//		}
//		
//		
//		if(pulse1 == 20){
//			mod_timer(&key_timer,2000);
//									WS2812_SetPulse(&htim1,pulse1);	

//			// pulse1 = 0;
//			Flash = 0;
//			// return;
//			// pulse1 = 1;
////			if (key_timer.timeout <= HAL_GetTick())
////			{
////				pulse1 = 1;
////			}
////			else return;
//		}
////		if (key_timer.timeout <= HAL_GetTick()){
////			;
////		}
////		else return;
////		if(Flash == 1){
////			WS2812_SetPulse(&htim1,10);		
////		}else	WS2812_SetPulse(&htim1,0);		

////		Flash = !Flash;
//		if(Flash == 1){
//			WS2812_SetPulse(&htim1,pulse1);	
//			pulse1++;	
//			if(pulse1 == 20)	pulse1 = 0;
//		}

/*
		if(Flash == 0){
			WS2812_SetPulse(&htim1,pulse1++);
		}
		if(pulse1 == 150)
			Flash = 1;
		
		if(Flash == 1){
			WS2812_SetPulse(&htim1,pulse1--);
		}
		
		if(pulse1 == 0){
			WS2812_SetPulse(&htim1,0);
			Flash = 0;
		}
		
*/

	// Red
	
	if(toggle_breath == false){
					if(breath_color == 0){
						LED_PWM_OUT(&htim1,pulse1++,TIM_CHANNEL_1);
						
					}else if(breath_color == 1){
						LED_PWM_OUT(&htim3,pulse1++,TIM_CHANNEL_2);
						
					}else if(breath_color == 2){
						LED_PWM_OUT(&htim3,pulse1++,TIM_CHANNEL_3);
						
					}else if(breath_color == 3){
						LED_PWM_OUT(&htim3,pulse1++,TIM_CHANNEL_4);
					
					}		
	}else if(toggle_breath == true){
					if(breath_color == 0){
						LED_PWM_OUT(&htim1,pulse1--,TIM_CHANNEL_1);
						
					}else if(breath_color == 1){
						LED_PWM_OUT(&htim3,pulse1--,TIM_CHANNEL_2);
						
					}else if(breath_color == 2){
						LED_PWM_OUT(&htim3,pulse1--,TIM_CHANNEL_3);
						
					}else if(breath_color == 3){
						LED_PWM_OUT(&htim3,pulse1--,TIM_CHANNEL_4);
					
					}			
		
	}
	
	if(toggle_breath == true && pulse1 == 0){
		breath_color++;
		if(breath_color == 4){
			breath_color = 0;
		}
		toggle_breath = false;
	}
	
	
	if(breath_color == 0){
		LED_PWM_OUT(&htim1,pulse1++,TIM_CHANNEL_1);
		
	}else if(breath_color == 1){
		LED_PWM_OUT(&htim3,pulse1++,TIM_CHANNEL_2);
		
	}else if(breath_color == 2){
		LED_PWM_OUT(&htim3,pulse1++,TIM_CHANNEL_3);
		
	}else if(breath_color == 3){
		LED_PWM_OUT(&htim3,pulse1++,TIM_CHANNEL_4);
	}
	
	if(pulse1 == 10){
		breath_color++;
		if(breath_color == 4)
			breath_color = 0;
		toggle_breath = true;
	}
	
	
	

    }
	
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == Key_1_Pin) {  // ? GPIO_PIN_0
        // ??????,?? HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		
		// Send pulse
		WS2812_SetPulse(&htim1,BUS_SET_BIT);
    }
}

// define TIM2-IRQ Function
//void TIM2_IRQHandler(void)
//{
//  HAL_TIM_IRQHandler(&htim2);
//}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	// Enable TIM3
//	HAL_TIM_Base_Start_IT(&htim3);				// Frequency : 1kHz ; Pulse : 50%
	 HAL_TIM_Base_Start_IT(&htim2);				// Enable TIM2 Timing 1s
	 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

//	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // WS2812_SetPulse(&htim1,BUS_SET_BIT);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
