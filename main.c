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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "mcp4725.h"

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
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

char user_inp[64];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void selectReadCommand(uint8_t commandType){

	char* header0 = "DAC Value: %d\r\n";
	char* header1 = "EEPROM Value: %d\r\n";

	if (commandType == MCP4725_READ_DAC){
		uint16_t dacValue = MCP4725_Read_DAC(&hi2c1, MCP4725_ADDRESS << 1);
		char value0[20];
		sprintf(value0, header0, dacValue);
		CDC_Transmit_FS(value0, strlen(value0));
		HAL_Delay(200);
	} else if(commandType == MCP4752_READ_EEPROM){
		uint16_t eepromValue = MCP4725_Read_EEPROM(&hi2c1, MCP4725_ADDRESS << 1);
		char value1[20];
		sprintf(value1, header1, eepromValue);
		CDC_Transmit_FS(value1, strlen(value1));
		HAL_Delay(200);
	}

}

void selectWriteCommand(uint8_t commandType, uint8_t pdSelect, double refVoltage, double outVoltage){

	switch (commandType) {
		case MCP4725_WRITE_DAC_FAST_COMMAND:
			MCP4725_SetOutputVoltage(&hi2c1,  MCP4725_ADDRESS << 1, pdSelect, outVoltage, refVoltage, MCP4725_WRITE_DAC_FAST_COMMAND);
			break;
		case MCP4725_WRITE_DAC_COMMAND:
			MCP4725_SetOutputVoltage(&hi2c1,  MCP4725_ADDRESS << 1, pdSelect, outVoltage, refVoltage, MCP4725_WRITE_DAC_COMMAND);
			break;
		case MCP4725_WRITE_DAC_EEPROM_COMMAND:
			MCP4725_SetOutputVoltage(&hi2c1,  MCP4725_ADDRESS << 1, pdSelect, outVoltage, refVoltage, MCP4725_WRITE_DAC_EEPROM_COMMAND);
			break;
		default:
			break;
	}

}

void printMsg(){

	const char* header0 = "\r\n\r\n[MCP4752] Enter the command in next format:\r\n";
	const char* header1 = "Read/Write_select Command_type [PD_select Reference_Voltage Output_Voltage]\r\n";
	const char* header2 = "     [0 1]         [0 1 2 3]    [0 1 2 3]     [double]         [double]    \r\n";

	CDC_Transmit_FS(header0, strlen(header0));
	HAL_Delay(200);
	CDC_Transmit_FS(header1, strlen(header1));
	HAL_Delay(200);
	CDC_Transmit_FS(header2, strlen(header2));
	HAL_Delay(200);

}



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
  MX_I2C1_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  HAL_Delay(1000);

  printMsg();

  HAL_Delay(1000);

  //General WAKE_UP

//  uint8_t sentTable[2];
//  sentTable[0] = 0x00;
//  sentTable[1] = 0x09;
//  HAL_I2C_Master_Transmit(&hi2c1, MCP4725_ADDRESS << 1, sentTable, 2, MCP4725_TIMEOUT);
//  HAL_Delay(1000);


  // General RESET
//  uint8_t sentTable2[2];
//   sentTable2[0] = 0x00;
//   sentTable2[1] = 0x06;
//   HAL_I2C_Master_Transmit(&hi2c1, MCP4725_ADDRESS << 1, sentTable2, 2, MCP4725_TIMEOUT);
//    HAL_Delay(1000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */


	  if (user_inp[0] == '1' || user_inp[0] == '0'){

	  char received[64];
	  strcpy(received, user_inp);

	  uint8_t rwSelect = atoi(strtok (received, " "));
	  uint8_t commandType = atoi(strtok (NULL, " "));
	  uint8_t pdSelect = atoi(strtok (NULL, " "));
	  double refVoltage = atof(strtok (NULL, " "));
	  double outVoltage = atof(strtok (NULL, " "));

	  const char* header0 = "RW: %d, Command: %d, pdSelect: %d, refVoltage: %.4f, outVoltage: %.4f\r\n";

	  char values[100];
	  sprintf(values, header0, rwSelect, commandType, pdSelect, refVoltage, outVoltage);
	  CDC_Transmit_FS(values, strlen(values));
	  HAL_Delay(200);

	  switch(rwSelect){

	  	  case MCP4725_OPTION_READ:
	  		  selectReadCommand(commandType);
	  		  break;

	  	  case MCP4725_OPTION_WRITE:
	  		  selectWriteCommand(commandType, pdSelect, refVoltage, outVoltage);
	  		  break;

	  	  default:
	  		  break;

	  }

	  memset(received, '\0', 64);
	  memset(user_inp, '\0', 64);
	}
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
