/*
 * mcp4725.h
 *
 *  Created on: Jun 27, 2023
 *      Author: Rubin
 */

#ifndef INC_MCP4725_H_
#define INC_MCP4725_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef MCP4725_ADDRESS
#define MCP4725_ADDRESS	0x60
#endif

#define MCP4725_TIMEOUT 1

#define MCP4725_DEFAULT_REF_VOLTAGE 5.5F
#define MCP4725_RESOLUTION 4096

#define MCP4725_OPTION_READ 0x00
#define MCP4725_OPTION_WRITE 0x01

#define MCP4725_READ_DAC 0x00
#define MCP4752_READ_EEPROM 0x01

#define MCP4725_WRITE_DAC_FAST_COMMAND 0x01
#define MCP4725_WRITE_DAC_COMMAND 0x02
#define MCP4725_WRITE_DAC_EEPROM_COMMAND 0x03

#define MCP4725_PD_NORMAL 0x00
#define MCP4725_PD_1KOM 0x01
#define MCP4725_PD_100KOM 0x02
#define MCP4725_PD_500KOM 0x03

#define MCP4725_ERROR 0xFFFF

void printI2CError(uint8_t errorCode);

void MCP4725_Write_DAC_Fast(I2C_HandleTypeDef *I2CHandler, uint8_t address, uint8_t pdSelect, uint16_t dacRegValue);
void MCP4725_Write_DAC(I2C_HandleTypeDef *I2CHandler, uint8_t address, uint8_t pdSelect, uint16_t dacRegValue);
void MCP4725_Write_DAC_EEPROM(I2C_HandleTypeDef *I2CHandler, uint8_t address, uint8_t pdSelect, uint16_t dacRegValue);

uint16_t MCP4725_Read_DAC(I2C_HandleTypeDef *I2CHandler, uint8_t address);
uint16_t MCP4725_Read_EEPROM(I2C_HandleTypeDef *I2CHandler, uint8_t address);

void MCP4725_Reset(I2C_HandleTypeDef *I2CHandler, uint8_t address);
void MCP4725_WakeUp(I2C_HandleTypeDef *I2CHandler, uint8_t address);

void MCP4725_SetOutputVoltage(I2C_HandleTypeDef *I2CHandler, uint8_t address, uint8_t pdSelect, double outVoltage, double refVoltage, uint8_t commandType);

#endif /* INC_MCP4725_H_ */
