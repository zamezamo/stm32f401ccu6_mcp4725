/*
 * mcp4725.c
 *
 *  Created on: Jun 27, 2023
 *      Author: Rubin
 */

#include "mcp4725.h"

void printI2CError(uint8_t errorCode){

	const char* header0 = "I2C Error! HAL Code: 0x%X\r\n";

	char textError[29];

	sprintf(textError, header0, errorCode);

	CDC_Transmit_FS(textError, strlen(textError));
	HAL_Delay(200);

}

void printI2CCompleted(){

	const char* header0 = "Operation completed!\r\n";

	CDC_Transmit_FS(header0, strlen(header0));
	HAL_Delay(200);

}

void MCP4725_Write_DAC_Fast(I2C_HandleTypeDef *I2CHandler, uint8_t address, uint8_t pdSelect, uint16_t dacRegValue){

	uint8_t sentTable[2];

	sentTable[0] = (MCP4725_WRITE_DAC_FAST_COMMAND >> 1 << 6) | (pdSelect << 4) | (uint8_t)(dacRegValue >> 8);
	sentTable[1] = (dacRegValue & 0x00FF);

	uint8_t errorCode = HAL_I2C_Master_Transmit(I2CHandler, address, sentTable, 2, MCP4725_TIMEOUT);

	if (errorCode != HAL_OK)
		printI2CError(errorCode);
	else
		printI2CCompleted();

}

void MCP4725_Write_DAC(I2C_HandleTypeDef *I2CHandler, uint8_t address, uint8_t pdSelect, uint16_t dacRegValue){

	uint8_t sentTable[3];

	sentTable[0] = (MCP4725_WRITE_DAC_COMMAND << 5) | (pdSelect << 1);
	sentTable[1] = (uint8_t)(dacRegValue >> 4);
	sentTable[2] = (uint8_t)(dacRegValue & 0x000F) << 4;

	uint8_t errorCode = HAL_I2C_Master_Transmit(I2CHandler, address, sentTable, 3, MCP4725_TIMEOUT);

	if (errorCode != HAL_OK)
		printI2CError(errorCode);
	else
		printI2CCompleted();

}

void MCP4725_Write_DAC_EEPROM(I2C_HandleTypeDef *I2CHandler, uint8_t address, uint8_t pdSelect, uint16_t dacRegValue){

	uint8_t sentTable[3];

	sentTable[0] = (MCP4725_WRITE_DAC_EEPROM_COMMAND << 5) | (pdSelect << 1);
	sentTable[1] = (uint8_t)(dacRegValue >> 4);
	sentTable[2] = (uint8_t)(dacRegValue & 0x000F) << 4;

	uint8_t errorCode = HAL_I2C_Master_Transmit(I2CHandler, address, sentTable, 3, MCP4725_TIMEOUT);

	if (errorCode != HAL_OK)
		printI2CError(errorCode);
	else
		printI2CCompleted();

}

uint16_t MCP4725_Read_DAC(I2C_HandleTypeDef *I2CHandler, uint8_t address){

	uint8_t sentTable[1] = {address | 0x0001};

	uint8_t errorCode = HAL_I2C_Master_Transmit(I2CHandler, address, sentTable, 1, MCP4725_TIMEOUT);

	if (errorCode != HAL_OK)
		printI2CError(errorCode);

	uint8_t receivedTable[5];

	errorCode = HAL_I2C_Master_Receive(I2CHandler, address, receivedTable, 5, MCP4725_TIMEOUT);

	if (errorCode != HAL_OK) {
		printI2CError(errorCode);
		return MCP4725_ERROR;
	}
	else
		return ((uint16_t)receivedTable[1] << 4 | (receivedTable[2] & 0x00F0));

}

uint16_t MCP4725_Read_EEPROM(I2C_HandleTypeDef *I2CHandler, uint8_t address){

	uint8_t sentTable[1] = {address | 0x0001};

	uint8_t errorCode = HAL_I2C_Master_Transmit(I2CHandler, address, sentTable, 1, MCP4725_TIMEOUT);

	if (errorCode != HAL_OK)
		printI2CError(errorCode);

	uint8_t receivedTable[5];

	errorCode = HAL_I2C_Master_Receive(I2CHandler, address, receivedTable, 5, MCP4725_TIMEOUT);

	if (errorCode != HAL_OK) {
		printI2CError(errorCode);
		return MCP4725_ERROR;
	}
	else
		return (((uint16_t)receivedTable[3] & 0x000F) << 8 | receivedTable[4]);

}

void MCP4725_SetOutputVoltage(I2C_HandleTypeDef *I2CHandler, uint8_t address, uint8_t pdSelect, double outVoltage, double refVoltage, uint8_t commandType){

	uint16_t dacRegValue = (uint16_t)round(outVoltage * MCP4725_RESOLUTION / refVoltage);

	const char* header0 = "Digital Value: %d\r\n";

	char value[20];

	sprintf(value, header0, dacRegValue);

	CDC_Transmit_FS(value, strlen(value));
	HAL_Delay(200);

	switch(commandType){

	case MCP4725_WRITE_DAC_FAST_COMMAND:
		MCP4725_Write_DAC_Fast(I2CHandler, address, pdSelect, dacRegValue);
		break;

	case MCP4725_WRITE_DAC_COMMAND:
		MCP4725_Write_DAC(I2CHandler, address, pdSelect, dacRegValue);
		break;

	case MCP4725_WRITE_DAC_EEPROM_COMMAND:
		MCP4725_Write_DAC_EEPROM(I2CHandler, address, pdSelect, dacRegValue);
		break;

	default:
		break;

	}

}
