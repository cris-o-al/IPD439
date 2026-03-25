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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
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

DMA_HandleTypeDef hdma_memtomem_dma1_channel1;
/* USER CODE BEGIN PV */

uint32_t mapa_error_transfer = 0;
//Arreglos de origen en memoria RAM

uint8_t src_sram_32[32];
uint8_t src_sram_64[64];
uint8_t src_sram_128[128];
uint8_t src_sram_256[256];
uint8_t src_sram_512[512];
uint8_t src_sram_1024[1024];

//Arreglos de origen en memoria flash
const uint8_t src_flash_32[32]   = { [0 ... 31]   = 0xAA };
const uint8_t src_flash_64[64]   = { [0 ... 63]   = 0xBB };
const uint8_t src_flash_128[128] = { [0 ... 127]  = 0xCC };
const uint8_t src_flash_256[256] = { [0 ... 255]  = 0xDD };
const uint8_t src_flash_512[512] = { [0 ... 511]  = 0xEE };
const uint8_t src_flash_1024[1024] = { [0 ... 1023] = 0xFF };

/* Origen en SRAM con memcpy */
uint8_t src_sram_32_memcpy[32];
uint8_t src_sram_64_memcpy[64];
uint8_t src_sram_128_memcpy[128];
uint8_t src_sram_256_memcpy[256];
uint8_t src_sram_512_memcpy[512];
uint8_t src_sram_1024_memcpy[1024];

/* Origen en Flash con memcpy */
const uint8_t src_flash_32_memcpy[32]   = { [0 ... 31]   = 0x11 };
const uint8_t src_flash_64_memcpy[64]   = { [0 ... 63]   = 0x22 };
const uint8_t src_flash_128_memcpy[128] = { [0 ... 127]  = 0x33 };
const uint8_t src_flash_256_memcpy[256] = { [0 ... 255]  = 0x44 };
const uint8_t src_flash_512_memcpy[512] = { [0 ... 511]  = 0x55 };
const uint8_t src_flash_1024_memcpy[1024] = { [0 ... 1023] = 0x66 };


//Arreglos de destino.
//Notar que aunque algunos esten etiquetados como dest_flash esto no implica que esten alojados en la flash, solo es para identificar que el arreglo de origen de los datos se encuentra en la flash
uint8_t dest_sram_32[32];
uint8_t dest_sram_64[64];
uint8_t dest_sram_128[128];
uint8_t dest_sram_256[256];
uint8_t dest_sram_512[512];
uint8_t dest_sram_1024[1024];

uint8_t dest_flash_32[32];
uint8_t dest_flash_64[64];
uint8_t dest_flash_128[128];
uint8_t dest_flash_256[256];
uint8_t dest_flash_512[512];
uint8_t dest_flash_1024[1024];

/* Destino en SRAM con memcpy */
uint8_t dest_sram_32_memcpy[32];
uint8_t dest_sram_64_memcpy[64];
uint8_t dest_sram_128_memcpy[128];
uint8_t dest_sram_256_memcpy[256];
uint8_t dest_sram_512_memcpy[512];
uint8_t dest_sram_1024_memcpy[1024];

/* Destino para flash con memcpy */
uint8_t dest_flash_32_memcpy[32];
uint8_t dest_flash_64_memcpy[64];
uint8_t dest_flash_128_memcpy[128];
uint8_t dest_flash_256_memcpy[256];
uint8_t dest_flash_512_memcpy[512];
uint8_t dest_flash_1024_memcpy[1024];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void HAL_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma);
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
  //Se llenan los arreglos de la ram con datos
  for(int i = 0;i<1024;i++){
	  if(i<32){
		  src_sram_32[i] = (uint8_t)i;
		  src_sram_32_memcpy[i] = (uint8_t)i;
	  }
	  if(i<64){
		  src_sram_64[i] = (uint8_t)i;
		  src_sram_64_memcpy[i] = (uint8_t)i;
	  }
	  if(i<128){
		  src_sram_128[i] = (uint8_t)i;
		  src_sram_128_memcpy[i] = (uint8_t)i;
	  }
	  if(i<256){
	  		  src_sram_256[i] = (uint8_t)i;
	  		  src_sram_256_memcpy[i] = (uint8_t)i;
	  }
	  if(i<512){
	  		  src_sram_512[i] = (uint8_t)i;
	  		  src_sram_512_memcpy[i] = (uint8_t)i;
	  }
	  if(i<1024){
	  		  src_sram_1024[i] = (uint8_t)i;
	  		  src_sram_1024_memcpy[i] = (uint8_t)i;
	  }
  }
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  hdma_memtomem_dma1_channel1.XferCpltCallback = HAL_DMA_XferCpltCallback;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */


    /* USER CODE BEGIN 3 */

	  //RAM A RAM

	  //Ram a Ram 32 bytes 0 y 1. 7.4us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)src_sram_32, (uint32_t)dest_sram_32, 32);
	  HAL_Delay(1000);

	  //Validación de copia
	  if (memcmp(src_sram_32, dest_sram_32, 32) != 0) {
		  mapa_error_transfer |= (1 << 0); // Bit 0 en alto si falla la de 32 bytes de ram a ram con dma
	  }
	  //Ram a Ram 64 bytes 2. 9.1us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)src_sram_64, (uint32_t)dest_sram_64, 64);
	  HAL_Delay(1000);
	  if (memcmp(src_sram_64, dest_sram_64, 64) != 0) {
		  mapa_error_transfer |= (1 << 1); // Bit 1 en alto si falla la de 64 bytes de ram a ram con dma
	  }
	  //Ram a Ram 128 bytes 3 y 4. 13.0us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)src_sram_128, (uint32_t)dest_sram_128, 128);
	  HAL_Delay(1000);
	  if (memcmp(src_sram_128, dest_sram_128, 128) != 0) {
		  mapa_error_transfer |= (1 << 2); // Bit 2 en alto si falla la de 32 bytes de ram a ram con dma
	  }
	  //Ram a Ram 256 bytes 5. 21.8us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)src_sram_256, (uint32_t)dest_sram_256, 256);
	  HAL_Delay(1000);
	  if (memcmp(src_sram_256, dest_sram_256, 256) != 0) {
		  mapa_error_transfer |= (1 << 3);
	  }
	  //Ram a Ram 512 bytes 6. 38.4us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)src_sram_512, (uint32_t)dest_sram_512, 512);
	  HAL_Delay(1000);
	  if (memcmp(src_sram_512, dest_sram_512, 512) != 0) {
		  mapa_error_transfer |= (1 << 4);
	  }
	  //Ram a Ram 1024 bytes 7. 71.6us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)src_sram_1024, (uint32_t)dest_sram_1024, 1024);
	  HAL_Delay(1000);
	  if (memcmp(src_sram_1024, dest_sram_1024, 1024) != 0) {
		  mapa_error_transfer |= (1 << 5);
	  }




	  //FLASH A RAM

	  //Flash a Ram 32 bytes 8. 7.5us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)src_flash_32, (uint32_t)dest_flash_32, 32);
	  HAL_Delay(1000);
	  if (memcmp(src_flash_32, dest_flash_32, 32) != 0) {
		  mapa_error_transfer |= (1 << 6); // Bit 6 en alto si falla la de 32 bytes de flash a ram con dma
	  }
	  //Flash a Ram 64 bytes 9 y 10. 10.7us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)src_flash_64, (uint32_t)dest_flash_64, 64);
	  HAL_Delay(1000);
	  if (memcmp(src_flash_64, dest_flash_64, 64) != 0) {
		  mapa_error_transfer |= (1 << 7); // Bit 7 en alto si falla la de 64 bytes de flash a ram con dma
	  }
	  //Flash a Ram 128 bytes 11. 15.9us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)src_flash_128, (uint32_t)dest_flash_128, 128);
	  HAL_Delay(1000);
	  if (memcmp(src_flash_128, dest_flash_128, 128) != 0) {
		  mapa_error_transfer |= (1 << 8); // Bit 8 en alto si falla la de 128 bytes de flash a ram con dma
	  }
	  //Flash a Ram 256 bytes 12. 26.2us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)src_flash_256, (uint32_t)dest_flash_256, 256);
	  HAL_Delay(1000);
	  if (memcmp(src_flash_256, dest_flash_256, 256) != 0) {
		  mapa_error_transfer |= (1 << 9);
	  }
	  //Flash a Ram 512 bytes 13. 47.1us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)src_flash_512, (uint32_t)dest_flash_512, 512);
	  HAL_Delay(1000);
	  if (memcmp(src_flash_512, dest_flash_512, 512) != 0) {
		  mapa_error_transfer |= (1 << 10);
	  }
	  //Flash a Ram 1024 bytes 14. 88.8us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)src_flash_1024, (uint32_t)dest_flash_1024, 1024);
	  HAL_Delay(1000);
	  if (memcmp(src_flash_1024, dest_flash_1024, 1024) != 0) {
		  mapa_error_transfer |= (1 << 11);
	  }




	  //RAM A RAM (memcpy)
	  //Ram a Ram 32 bytes 15. 950ns
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  memcpy(dest_sram_32_memcpy, src_sram_32_memcpy, 32);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  if (memcmp(src_sram_32_memcpy, dest_sram_32_memcpy, 32) != 0) {
		  mapa_error_transfer |= (1 << 12); // Bit 12 en alto si falla la de 32 bytes de ram a ram con memcpy
	  }
	  //Ram a Ram 64 bytes 16. 1.07us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  memcpy(dest_sram_64_memcpy, src_sram_64_memcpy, 64);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  if (memcmp(src_sram_64_memcpy, dest_sram_64_memcpy, 64) != 0) {
		  mapa_error_transfer |= (1 << 13); // Bit 13 en alto si falla la de 64 bytes de ram a ram con memcpy
	  }

	  //Ram a Ram 128 bytes 17. 10.6us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  memcpy(dest_sram_128_memcpy, src_sram_128_memcpy, 128);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  if (memcmp(src_sram_128_memcpy, dest_sram_128_memcpy, 128) != 0) {
		  mapa_error_transfer |= (1 << 14);
	  }

	  //Ram a Ram 256 bytes 18. 20.2us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  memcpy(dest_sram_256_memcpy, src_sram_256_memcpy, 256);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  if (memcmp(src_sram_256_memcpy, dest_sram_256_memcpy, 256) != 0) {
		  mapa_error_transfer |= (1 << 15);
	  }

	  //Ram a Ram 512 bytes 19. 39.6us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  memcpy(dest_sram_512_memcpy, src_sram_512_memcpy, 512);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  if (memcmp(src_sram_512_memcpy, dest_sram_512_memcpy, 512) != 0) {
		  mapa_error_transfer |= (1 << 16);
	  }

	  //Ram a Ram 1024 bytes 20. 77.8us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  memcpy(dest_sram_1024_memcpy, src_sram_1024_memcpy, 1024);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  if (memcmp(src_sram_1024_memcpy, dest_sram_1024_memcpy, 1024) != 0) {
		  mapa_error_transfer |= (1 << 17);
	  }



	  //FLASH A RAM (memcpy)
	  //Flash a Ram 32 bytes 21. 1.22us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  memcpy(dest_flash_32_memcpy, src_flash_32_memcpy, 32);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  if (memcmp(src_flash_32_memcpy, dest_flash_32_memcpy, 32) != 0) {
		  mapa_error_transfer |= (1 << 18); // Bit 18 en alto si falla la de 32 bytes de flash a ram con memcpy
	  }

	  //Flash a Ram 64 bytes 22. 1.54us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  memcpy(dest_flash_64_memcpy, src_flash_64_memcpy, 64);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  if (memcmp(src_flash_64_memcpy, dest_flash_64_memcpy, 64) != 0) {
		  mapa_error_transfer |= (1 << 19); // Bit 19 en alto si falla la de 64 bytes de flash a ram con memcpy
	  }

	  //Flash a Ram 128 bytes 23. 13.0us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  memcpy(dest_flash_128_memcpy, src_flash_128_memcpy, 128);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  if (memcmp(src_flash_128_memcpy, dest_flash_128_memcpy, 128) != 0) {
		  mapa_error_transfer |= (1 << 20);
	  }

	  //Flash a Ram 256 bytes 24. 25.0us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  memcpy(dest_flash_256_memcpy, src_flash_256_memcpy, 256);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  if (memcmp(src_flash_256_memcpy, dest_flash_256_memcpy, 256) != 0) {
		  mapa_error_transfer |= (1 << 21);
	  }

	  //Flash a Ram 512 bytes 25. 49.0us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  memcpy(dest_flash_512_memcpy, src_flash_512_memcpy, 512);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  if (memcmp(src_flash_512_memcpy, dest_flash_512_memcpy, 512) != 0) {
		  mapa_error_transfer |= (1 << 22);
	  }

	  //Flash a Ram 1024 bytes 26. 96.8us
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  memcpy(dest_flash_1024_memcpy, src_flash_1024_memcpy, 1024);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  if (memcmp(src_flash_1024_memcpy, dest_flash_1024_memcpy, 1024) != 0) {  // Bit 23 en alto si falla la de 1024 bytes de flash a ram con memcpy
		  mapa_error_transfer |= (1 << 23);
	  }
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
  * Enable DMA controller clock
  * Configure DMA for memory to memory transfers
  *   hdma_memtomem_dma1_channel1
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure DMA request hdma_memtomem_dma1_channel1 on DMA1_Channel1 */
  hdma_memtomem_dma1_channel1.Instance = DMA1_Channel1;
  hdma_memtomem_dma1_channel1.Init.Request = DMA_REQUEST_0;
  hdma_memtomem_dma1_channel1.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma_memtomem_dma1_channel1.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma_memtomem_dma1_channel1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_memtomem_dma1_channel1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_memtomem_dma1_channel1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_memtomem_dma1_channel1.Init.Mode = DMA_NORMAL;
  hdma_memtomem_dma1_channel1.Init.Priority = DMA_PRIORITY_LOW;
  if (HAL_DMA_Init(&hdma_memtomem_dma1_channel1) != HAL_OK)
  {
    Error_Handler( );
  }

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

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

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
{
	if (hdma->Instance == DMA1_Channel1) {
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	}
}
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
