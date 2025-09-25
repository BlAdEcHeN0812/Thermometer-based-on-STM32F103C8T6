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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
#include "oled.h"

#include "stm32f1xx.h"                 
#include "Delay.h"
#include "MyIIC.h"
#include "MLX90614.h"
#include "MLX90614_Address.h"
#include "bmp.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
char ambient_temp_str[20]; 
char object_temp_str[20];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//printf

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

	Delay_ms(1000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
/* ???? */
/* USER CODE BEGIN WHILE */
MLX90614_Init();
OLED_Init();
Delay_ms(20);

float ambient_temperature = 0;
float object_temperature = 0;
int mode = 0; // 0=???, 1=OLED????, 2=????

// ????
for(int i=0;i<2;i++){
	OLED_DrawGIF(32,0,96,8,18,512,Rocket);
  Delay_ms(100);
}

// ?????,???????
OLED_Clear();
OLED_ShowString(1, 1, "1:OLED Mode");
OLED_ShowString(2, 1, "2:Bluetooth Mode");

while (1)
{
  if (mode == 0) // ---------- ???????? ----------
  {
    // ?????,????????????
    
    // ??PA1,????????
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
    {
      Delay_ms(20);
      if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
      {
        mode = 1; // ?????1
        // ????1?,??????
        OLED_Clear();
        OLED_ShowString(1, 1, "OLED Mode Ready");
        OLED_ShowString(2, 1, "1:Read temp");
        OLED_ShowString(3, 1, "2:Menu");
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET); // ??????
      }
    }
    
    // ??PA2,????????
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET)
    {
      Delay_ms(20);
      if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET)
      {
        mode = 2; // ?????2
        OLED_Clear();
        OLED_ShowString(1, 1, "Bluetooth Mode");
        OLED_ShowString(2, 1, "Sending data...");
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET); // ??????
      }
    }
  }
  else if (mode == 1) // ---------- ???OLED???? ----------
  {
    // ???1?,???????????????????
    
    // ??PA1,??????????
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
    {
      Delay_ms(20);
      if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
      {
        // --- ??????????? ---
        MLX90614_TA();
        Delay_ms(20);
        MLX90614_TO();
        Delay_ms(20);
        
        ambient_temperature = a_temp;
        object_temperature = o_temp;

        snprintf(ambient_temp_str, sizeof(ambient_temp_str), "Amb: %.2f C", ambient_temperature*1.08);
        snprintf(object_temp_str, sizeof(object_temp_str), "Obj: %.2f C", object_temperature*1.05);
        
        OLED_Clear();
        OLED_ShowString(1, 1, "Reading:");
        OLED_ShowString(2, 1, ambient_temp_str);
        OLED_ShowString(3, 1, object_temp_str);
        // --- ??????? ---
        
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET); // ??????
      }
    }
    
    // ??PA2,?????????
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET)
    {
      Delay_ms(20);
      if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET)
      {
        mode = 0; // ????????
        OLED_Clear();
        OLED_ShowString(1, 1, "1:OLED Mode");
        OLED_ShowString(2, 1, "2:Bluetooth Mode");
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET); // ??????
      }
    }
  }
  else if (mode == 2) // ---------- ??????? ----------
  {
				OLED_Clear();
        MLX90614_TA();
        Delay_ms(20);
        MLX90614_TO();
        Delay_ms(20);
        
        object_temperature = o_temp;
        snprintf(object_temp_str, sizeof(object_temp_str), "temp: %.2f C\r\n", object_temperature);

        HAL_UART_Transmit(&huart1, (uint8_t *)object_temp_str, strlen(object_temp_str), HAL_MAX_DELAY);

        OLED_ShowString(1, 1, "Data Sent!");
        Delay_ms(500); // ??????
  }

  // ?????
  Delay_ms(50); 
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
