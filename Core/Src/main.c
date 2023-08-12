/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Definitions for light1 */
osThreadId_t light1Handle;
const osThreadAttr_t light1_attributes = {
  .name = "light1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for light2 */
osThreadId_t light2Handle;
const osThreadAttr_t light2_attributes = {
  .name = "light2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for BinarySem1 */
osSemaphoreId_t BinarySem1Handle;
const osSemaphoreAttr_t BinarySem1_attributes = {
  .name = "BinarySem1"
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartLight1(void *argument);
void StartLight2(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of BinarySem1 */
  BinarySem1Handle = osSemaphoreNew(1, 1, &BinarySem1_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of light1 */
  light1Handle = osThreadNew(StartLight1, NULL, &light1_attributes);

  /* creation of light2 */
  light2Handle = osThreadNew(StartLight2, NULL, &light2_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|BLUE_LIGHT_Pin|GREEN_LIGHT_Pin|RED_LIGHT2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, YELLOW_LIGHT_Pin|RED_LIGHT_Pin|GREEN_LIGHT2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(YELLOW_LIGHT2_GPIO_Port, YELLOW_LIGHT2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin BLUE_LIGHT_Pin GREEN_LIGHT_Pin RED_LIGHT2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|BLUE_LIGHT_Pin|GREEN_LIGHT_Pin|RED_LIGHT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : YELLOW_LIGHT_Pin RED_LIGHT_Pin GREEN_LIGHT2_Pin */
  GPIO_InitStruct.Pin = YELLOW_LIGHT_Pin|RED_LIGHT_Pin|GREEN_LIGHT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : YELLOW_LIGHT2_Pin */
  GPIO_InitStruct.Pin = YELLOW_LIGHT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(YELLOW_LIGHT2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int buttonPress = 0;
/* USER CODE END 4 */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */
	buttonPress = 1;
  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(B1_Pin);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}
void StartCrossing() {
	  HAL_GPIO_WritePin(GPIOA, BLUE_LIGHT_Pin, GPIO_PIN_SET);		// BLUE turned on
	  osDelay(2000);
	  HAL_GPIO_WritePin(GPIOA, BLUE_LIGHT_Pin, GPIO_PIN_RESET);		// BLUE turned off

	  /* Flashes on and off for 2s */
	  int count = 4;
	  while(count > 0) {
		  HAL_GPIO_WritePin(GPIOA, BLUE_LIGHT_Pin, GPIO_PIN_SET);	// BLUE turned on
		  osDelay(250);												// 0.25s delay
		  HAL_GPIO_WritePin(GPIOA, BLUE_LIGHT_Pin, GPIO_PIN_RESET);	// BLUE turned off
		  osDelay(250);
		  count--;
	  }
}
/* USER CODE BEGIN Header_StartLight1 */
/**
  * @brief  Function implementing the light1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartLight1 */
void StartLight1(void *argument)
{
  /* USER CODE BEGIN 5 */
	HAL_GPIO_WritePin(GPIOA, RED_LIGHT2_Pin, GPIO_PIN_SET); // initially sets RED2 to be on
		  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreAcquire(BinarySem1Handle, osWaitForever); 	// waits forever for semaphore to become available

	  HAL_GPIO_WritePin(GPIOB, RED_LIGHT_Pin, GPIO_PIN_RESET);		// RED1 turned off

	  HAL_GPIO_WritePin(GPIOA, GREEN_LIGHT_Pin, GPIO_PIN_SET);		// GREEN1 turned on
	  if (buttonPress == 1){
	  	StartCrossing();
	  	buttonPress = 0;
	  	HAL_GPIO_WritePin(GPIOA, GREEN_LIGHT_Pin, GPIO_PIN_RESET);	// GREEN1 turned off
	  } else {
		  osDelay(4000);												// 4s delay
		  HAL_GPIO_WritePin(GPIOA, GREEN_LIGHT_Pin, GPIO_PIN_RESET);	// GREEN1 turned off
	  }

	  HAL_GPIO_WritePin(GPIOB, YELLOW_LIGHT_Pin, GPIO_PIN_SET);		// YELLOW1 turned on
	  osDelay(1500);												// 1.5s delay
	  HAL_GPIO_WritePin(GPIOB, YELLOW_LIGHT_Pin, GPIO_PIN_RESET);	// YELLOW1 turned off

	  HAL_GPIO_WritePin(GPIOB, RED_LIGHT_Pin, GPIO_PIN_SET);		// RED1 turned on
	  osDelay(1000);												// 1s delay

	  osSemaphoreRelease(BinarySem1Handle);		//releases semaphore for next task
	  osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartLight2 */
/**
* @brief Function implementing the light2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLight2 */
void StartLight2(void *argument)
{
  /* USER CODE BEGIN StartLight2 */
  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreAcquire(BinarySem1Handle, osWaitForever); 	// waits forever for semaphore to become available

	  HAL_GPIO_WritePin(GPIOA, RED_LIGHT2_Pin, GPIO_PIN_RESET);		// RED2 turned off

	  HAL_GPIO_WritePin(GPIOB, GREEN_LIGHT2_Pin, GPIO_PIN_SET);		// GREEN2 turned on
	  if (buttonPress == 1){
			StartCrossing();
			buttonPress = 0;
			HAL_GPIO_WritePin(GPIOA, GREEN_LIGHT_Pin, GPIO_PIN_RESET);	// GREEN1 turned off
	  } else {
			osDelay(4000);												// 4s delay
			HAL_GPIO_WritePin(GPIOA, GREEN_LIGHT_Pin, GPIO_PIN_RESET);	// GREEN1 turned off
	  }											// 4s delay
	  HAL_GPIO_WritePin(GPIOB, GREEN_LIGHT2_Pin, GPIO_PIN_RESET);	// GREEN2 turned off

	  HAL_GPIO_WritePin(YELLOW_LIGHT2_GPIO_Port, YELLOW_LIGHT2_Pin, GPIO_PIN_SET);	// YELLOW2 turned on
	  osDelay(1500);												// 1.5s delay
	  HAL_GPIO_WritePin(YELLOW_LIGHT2_GPIO_Port, YELLOW_LIGHT2_Pin, GPIO_PIN_RESET);	// YELLOW2 turned off

	  HAL_GPIO_WritePin(GPIOA, RED_LIGHT2_Pin, GPIO_PIN_SET);		// RED2 turned on
	  osDelay(1000);												// 7.5s delay to give other task time to complete

	  osSemaphoreRelease(BinarySem1Handle);		//releases semaphore for next task
	  osDelay(1);
  }
  /* USER CODE END StartLight2 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM5 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM5) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
