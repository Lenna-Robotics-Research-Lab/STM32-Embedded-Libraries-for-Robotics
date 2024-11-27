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
#include "usart.h"
#include "gpio.h"

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
//static void MX_GPIO_Init(void);
//static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t data[256],test_data[256],test_dataa[8];
uint8_t response_array[256];
uint8_t response_array_full[256];
uint16_t exception_code;
int sending_byte_count;

uint32_t q=0;

unsigned int HoldingRegisters[] = {
5069,	7073,	52317,	58320,	53844,	7596,	50869,	64892,	58209,	18764,	//0-9
38145,	16852,	62343,	25973,	11963,	49434,	31745,	48278,	50955,	50027,	//10-19
62940,	38520,	51144,	54693,	60394,	14244,	62402,	62647,	65457,	56695,	//20-29
40971,	29608,	37659,	60486,	3321,	769,	52458,	61584,	55023,	61523,	//30-39
50744,	51336,	3649,	48678,	29801,	43147,	1325,	32661,	49277,	32353,	//40-49
48858,	41522,	32167,	13748,	24501,	2375,	6957,	45371,	53417,	34799,	//50-59
51057,	3290,	13510,	48962,	65080,	22597,	38672,	14315,	48621,	49290,	//60-69
30178,	1793,	13119,	54566,	1570,	31355,	1754,	21305,	5750,	25520,	//70-79
23688,	54689,	64511,	17349,	33911,	31220,	64245,	17176,	43586,	30999,	//80-89
12761,	42338,	62682,	7424,	6981,	36057,	63189,	4897,	52723,	61279,	//90-99
};

unsigned int input_registers[2000];

int len_of_regs = sizeof(HoldingRegisters)/sizeof(HoldingRegisters[0]);

//uint8_t coils[] = {0x1,0x5,0x6,0x8,0x9,0xC,0xD,0x4,0xA,0xD,0xF,0xC,0xE,0xD,0x6,0x9};
uint8_t coils[] = {1,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,0,1,0,1,1,1,0,0,0,0,0,1,1,1,1,0,1,0,1,0,0,0,1,0,1,0,1,1,0,1,0};

uint8_t discrete_inputs[2000];

uint8_t	exception_coils[8];

int len_of_coils = sizeof(coils)/sizeof(coils[0]);
int flag =0,flag2 = 0,flag3 = 0;
int ttt;
int test_1,test_2,test_3,test_4 = 0,test_5 = 0,test_6,test_7,test_8,test_9,test_10;
uint16_t slave_CRC;
uint16_t sent_CRC;

uint8_t response_array[256];
uint8_t response_array_full[256];
uint16_t num_of_coils_for_multi_coil;
uint16_t bytes_for_multi_coil;
uint16_t num_of_regs_for_multi_reg;
uint16_t bytes_for_multi_reg;
uint16_t start_addr;
uint16_t quantitiy_of_coils;
uint16_t quantitiy_of_regs;
uint16_t quantitiy_of_discrete_inputs;
uint16_t quantitiy_of_input_regs;
uint16_t slave_sendiing_crc;
uint16_t temp_coils[0x07B0];

unsigned int temp_regs[0x007B];
int num;

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
  HAL_UART_Receive_IT(&huart1, data, 8);
  HAL_UART_Transmit_IT(&huart1, response_array, sending_byte_count);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		int temp_count = 0;
	  unsigned int temp_regs[256];// sending register
		uint16_t temp_cmd;//writing single register
    if(data[0] == Slave_Address || data[0] == 0x00){
		  test_1 = 1;
			if(data[1] == 0x01||data[1] == 0x02 || data[1] == 0x03 || data[1] == 0x04 || data[1] == 0x05 || data[1] == 0x06){
			  sent_CRC = (data[7]<<8|data[6]);//-1 and -2 change for the simply modbus test
        num	= 6;//for	calculating the slave crc
		  }else if(data[1] == 0x0F||data[1] == 0x10){
			  sent_CRC = (data[8 + data[6]]<<8|data[7+data[6]]);
        num = 7 + data[6];	//for	calculating the slave crc
			}
			slave_CRC = crc16(data,num);

			if(sent_CRC == slave_CRC){
				//CRC calculation is the same
				test_2 = 1;
				switch(data[1]){
					case 0x01:
						test_3 = 1;
						start_addr = (data[2]<<8 | data[3]);
					  quantitiy_of_coils = (data[4]<<8 | data[5]);
					  read_coils(start_addr,quantitiy_of_coils,coils,response_array);
					  flag2 = 1;
					  break;
					case 0x02:
						start_addr = (data[2]<<8 | data[3]);
					  quantitiy_of_discrete_inputs = (data[4]<<8 | data[5]);
					  read_descrete_data(start_addr,quantitiy_of_discrete_inputs,discrete_inputs,response_array);
					  flag2 = 1;
					  break;
					case 0x03:
						start_addr = (data[2]<<8 | data[3]);
					  quantitiy_of_regs = (data[4]<<8 | data[5]);
            temp_count =0;
					  for(int i=6;i<sizeof(data)/sizeof(data[0]);i+=2){
							temp_regs[temp_count] = (data[i]<<8|data[i+1]);
              temp_count++;
						}
						read_holding_registers(start_addr,quantitiy_of_regs,HoldingRegisters,response_array);
					  flag2 = 1;
						break;
					case 0x04:
						start_addr = (data[2]<<8 | data[3]);
					  quantitiy_of_input_regs = (data[4]<<8 | data[5]);
            temp_count =0;
					  for(int i=6;i<sizeof(data)/sizeof(data[0]);i+=2){
							temp_regs[temp_count] = (data[i]<<8|data[i+1]);
              temp_count++;
						}
						read_input_registers(start_addr,quantitiy_of_input_regs,input_registers,response_array);
					  flag2 = 1;
						break;
					case 0x05:
						start_addr = (data[2]<<8 | data[3]);
				    temp_cmd = (data[4]<<8 | data[5]);
					  write_single_coil(start_addr,temp_cmd,coils,response_array);
					  flag2 = 1;
					  break;
					case 0x06:
						start_addr = (data[2]<<8 | data[3]);
					  temp_cmd = (data[4]<<8 | data[5]);
					  write_single_register(start_addr,temp_cmd,HoldingRegisters,response_array);
					  flag2 = 1;
					  break;
					case 0x07:
						read_exception_status(exception_coils,response_array);
					  flag2 = 1;
						break;
					case 0x0F:
						start_addr = (data[2]<<8 | data[3]);
					  num_of_coils_for_multi_coil = (data[4]<<8 | data[5]);
					  bytes_for_multi_coil = data[6];
					  temp_count =0;
					  for(int i=7;i<num;i+=2){
						  temp_coils[temp_count] =  (data[i]<<8|data[i+1]);
							temp_count++;
						}
						write_multiple_coils(start_addr,num_of_coils_for_multi_coil,bytes_for_multi_coil,temp_coils,coils,response_array);
					  flag2 = 1;
					  break;
					case 0x10:
						start_addr = (data[2]<<8 | data[3]);
					  num_of_regs_for_multi_reg = (data[4]<<8 | data[5]);
					  bytes_for_multi_reg = data[6];
					  temp_count =0;
					  for(int i=7;i<num;i+=2){
						  temp_regs[temp_count] =  (data[i]<<8|data[i+1]);
							temp_count++;
						}
						write_multiple_registers(start_addr,num_of_regs_for_multi_reg,bytes_for_multi_reg,temp_regs,HoldingRegisters,response_array);
					  flag2 = 1;
					  break;
					default:
						flag = 1;
						exception_code = 0x01;
					  response_array[0] = 0x81;
					  response_array[1] = exception_code;
					  flag2 = 1;
					  break;
					}
				if(flag2 == 1 && flag3 == 1){
					HAL_UART_Transmit_IT(&huart1,response_array,sending_byte_count);
					flag2 = 0;
					flag3 = 0;
				}
				//sending data
			}else{
			  //CRC error
				//HAL_Delay(1000);
			}

		}
		q++;
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Transmit one byte with 100 ms timeout */
   // HAL_UART_Transmit(&huart1, &byte, 1, 100);

    /* Receive one byte in interrupt mode */
		if(data[1] == 0x01||data[1] == 0x02 || data[1] == 0x03 || data[1] == 0x04 || data[1] == 0x05 || data[1] == 0x06){
			// for 8 byte frame
		  test_4 ++;
		  flag3 = 1;
      HAL_UART_Receive_IT(&huart1, data, 8);
	  }else if(data[1] == 0x0F||data[1] == 0x10){
			// for more than 8 byte frame
		  HAL_UART_Receive_IT(&huart1, data, 9 + data[6]);

			}else{
				HAL_UART_Receive_IT(&huart1, data, 8);
				flag3 ++;
			}

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
   flag2 = 0;
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
