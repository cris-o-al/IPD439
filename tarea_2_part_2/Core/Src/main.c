/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
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
ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart2;

/* Definitions for ADCReadTask */
osThreadId_t ADCReadTaskHandle;
const osThreadAttr_t ADCReadTask_attributes = {
  .name = "ADCReadTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for consumerTask */
osThreadId_t consumerTaskHandle;
const osThreadAttr_t consumerTask_attributes = {
  .name = "consumerTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for LedStatusTask */
osThreadId_t LedStatusTaskHandle;
const osThreadAttr_t LedStatusTask_attributes = {
  .name = "LedStatusTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LedHandlerTask */
osThreadId_t LedHandlerTaskHandle;
const osThreadAttr_t LedHandlerTask_attributes = {
  .name = "LedHandlerTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for QueueSerial */
osMessageQueueId_t QueueSerialHandle;
const osMessageQueueAttr_t QueueSerial_attributes = {
  .name = "QueueSerial"
};
/* Definitions for QueueLedStatus */
osMessageQueueId_t QueueLedStatusHandle;
const osMessageQueueAttr_t QueueLedStatus_attributes = {
  .name = "QueueLedStatus"
};
/* Definitions for QueueADC */
osMessageQueueId_t QueueADCHandle;
const osMessageQueueAttr_t QueueADC_attributes = {
  .name = "QueueADC"
};
/* USER CODE BEGIN PV */

uint8_t byte_recibido;
char buffer_recepcion[20]; //Se pueden recibir números de hasta 19 carácteres. (Sin contar salto de línea)
uint8_t indice_recepcion;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
void StartADCReadTask(void *argument);
void StartConsumerTask(void *argument);
void StartLedStatusTask(void *argument);
void StartLedHandlerTask(void *argument);

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
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2, &byte_recibido, 1);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of QueueSerial */
  QueueSerialHandle = osMessageQueueNew (100, sizeof(uint32_t), &QueueSerial_attributes);

  /* creation of QueueLedStatus */
  QueueLedStatusHandle = osMessageQueueNew (16, sizeof(uint8_t), &QueueLedStatus_attributes);

  /* creation of QueueADC */
  QueueADCHandle = osMessageQueueNew (16, sizeof(uint32_t), &QueueADC_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ADCReadTask */
  ADCReadTaskHandle = osThreadNew(StartADCReadTask, NULL, &ADCReadTask_attributes);

  /* creation of consumerTask */
  consumerTaskHandle = osThreadNew(StartConsumerTask, NULL, &consumerTask_attributes);

  /* creation of LedStatusTask */
  LedStatusTaskHandle = osThreadNew(StartLedStatusTask, NULL, &LedStatusTask_attributes);

  /* creation of LedHandlerTask */
  LedHandlerTaskHandle = osThreadNew(StartLedHandlerTask, NULL, &LedHandlerTask_attributes);

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
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
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_Low_Pin|LED_Mid_Pin|LED_High_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_Low_Pin LED_Mid_Pin LED_High_Pin */
  GPIO_InitStruct.Pin = LED_Low_Pin|LED_Mid_Pin|LED_High_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        if (byte_recibido == '\n') {
                buffer_recepcion[indice_recepcion] = '\0'; // Fin del string

                float valor_recibido = atof(buffer_recepcion); // Convertimos texto a número
                osMessageQueuePut(QueueSerialHandle, &valor_recibido, 0,0); //Se envía a la cola
                indice_recepcion = 0;
            }
        else {
            if (indice_recepcion < 19) { // Evitamos que explote el buffer
            	buffer_recepcion[indice_recepcion++] = byte_recibido;
            }
        }
        HAL_UART_Receive_IT(&huart2, &byte_recibido, 1);
    }
}



//Redireccionamiento de printf

int __io_putchar(int ch)

{
  // Mandamos un caracter a la vez por la UART2
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartADCReadTask */
/**
  * @brief  Function implementing the ADCReadTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartADCReadTask */
void StartADCReadTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	uint32_t start_tick = osKernelGetTickCount(); // Tiempo de referencia
	uint32_t adc_val = 0;
  /* Infinite loop */
  for(;;)
  {
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
    	adc_val = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);
    osMessageQueuePut(QueueADCHandle,&adc_val,0,0);
    osDelayUntil(start_tick+100);
    start_tick +=100;
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartConsumerTask */
/**
* @brief Function implementing the consumerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartConsumerTask */
void StartConsumerTask(void *argument)
{
  /* USER CODE BEGIN StartConsumerTask */
	float datos[100];
	float dato_recibido = 0;
	osStatus_t status;
	int contador = 0;
  /* Infinite loop */
  for(;;)
  {
	status = osMessageQueueGet(QueueSerialHandle, &dato_recibido, NULL, osWaitForever);
	if(status == osOK){
		datos[contador] = dato_recibido;
		contador++;
	}
	if(contador == 100){
		float suma = 0;
		float promedio = 0;
		float varianza = 0;
		float desv_std = 0;

		for(int i = 0;i<100;i++){
			suma += datos[i];
		}
		promedio = suma/100.0f;

		for(int i = 0;i<100;i++){
			varianza += (datos[i]-promedio)*(datos[i]-promedio);
		}

		varianza = varianza/100.0f;
		desv_std = sqrtf(varianza);
		contador = 0;

		//Mandar los datos por uart!
		printf("Resultados: Promedio = %.2f, Desv. Est. = %.2f\r\n", promedio, desv_std);
	}
  }
  /* USER CODE END StartConsumerTask */
}

/* USER CODE BEGIN Header_StartLedStatusTask */
/**
* @brief Function implementing the LedStatusTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLedStatusTask */
void StartLedStatusTask(void *argument)
{
  /* USER CODE BEGIN StartLedStatusTask */
	uint32_t raw_value;
	float voltaje_medido;
	uint8_t led_status;
	osStatus_t status;
  /* Infinite loop */
  for(;;)
  {
	  status = osMessageQueueGet(QueueADCHandle, &raw_value, NULL, osWaitForever);
	  if(status==osOK){
		  voltaje_medido = (float) raw_value*3.3f/4095.0f;

		  if(voltaje_medido > 2.2f){
			  //Tercer led encendido
			  led_status = 0x04; //0000 0100
		  }
		  else if(voltaje_medido > 1.1f){
			  //Segundo led encendido
			  led_status = 0x02; //0000 0010
		  }
		  else {
			  //Tercer led encendido
			  led_status = 0x01; //0000 0001
		  }
		  osMessageQueuePut(QueueLedStatusHandle,&led_status,0,0);
	  }
  }
  /* USER CODE END StartLedStatusTask */
}

/* USER CODE BEGIN Header_StartLedHandlerTask */
/**
* @brief Function implementing the LedHandlerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLedHandlerTask */
void StartLedHandlerTask(void *argument)
{
  /* USER CODE BEGIN StartLedHandlerTask */
	osStatus_t status;
	uint8_t led_status;
  /* Infinite loop */
  for(;;)
  {
	  status = osMessageQueueGet(QueueLedStatusHandle, &led_status, NULL, osWaitForever);
	  if(status == osOK){
		  HAL_GPIO_WritePin(LED_Low_GPIO_Port, LED_Low_Pin, (led_status & 0x01));
		  HAL_GPIO_WritePin(LED_Mid_GPIO_Port, LED_Mid_Pin, (led_status & 0x02));
		  HAL_GPIO_WritePin(LED_High_GPIO_Port, LED_High_Pin, (led_status & 0x04));
	  }
  }
  /* USER CODE END StartLedHandlerTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
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
#ifdef USE_FULL_ASSERT
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
