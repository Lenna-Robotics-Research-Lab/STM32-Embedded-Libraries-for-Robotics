/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define max_frame_size 255u
#define Slave_Address 0x01
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);//calculating the CRC
void read_coils(uint16_t starting_Address,uint16_t num_of_coils,uint8_t coils[],uint8_t x[]);//command 0x01
void read_descrete_data(uint16_t starting_Address,uint16_t num_of_inputs,uint8_t data[],uint8_t x[]);//command 0x02
void read_holding_registers(uint16_t starting_Address,uint16_t num_of_regs,unsigned int regs[],uint8_t x[]);//command 0x03
void read_input_registers(uint16_t starting_Address,uint16_t num_of_regs,unsigned int input_regs[],uint8_t x[]);//command 0x04
void write_single_coil(uint16_t Address,uint16_t cmd, uint8_t coils[],uint8_t x[]);//command 0x05
void write_single_register(uint16_t Address,uint16_t cmd, unsigned int regs[],uint8_t x[]);//command 0x06
void read_exception_status(uint8_t exp_coils[],uint8_t x[]);//command 0x07
void write_multiple_coils(uint16_t starting_Address, uint16_t num_of_coils,uint8_t bytes_count, uint16_t output_value[],uint8_t coils[],uint8_t x[]);//command 0x0F
void write_multiple_registers(uint16_t starting_Address, uint16_t bytes,uint16_t num_of_regs, unsigned int input_regs[], unsigned int regs[],uint8_t x[]);//command 0x10
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
