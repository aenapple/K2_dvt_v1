/**********************************************************************************/
/**
 *  @file Eeprom.cpp
 *  @brief Driver for EEPROM emulator
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "Eeprom.hpp"


/**********************************************************************************/
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::Init()
{
	EOsResult result;


	result= this->Semaphore.CreateStatic();
	if(result != OsResult_Ok)
	{
		return(result);
	}

	this->Semaphore.Give();


	return(OsResult_Ok);
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::WriteProcessCounter(u32 data)
{
	EOsResult result;


//	taskENTER_CRITICAL();
	result = this->WriteVariable32bits(EEPROM_ADR_PROCESS_COUNTER, data);
//	taskEXIT_CRITICAL();
	if(result != OsResult_Ok)
	{
		return(result);
	}


	return(OsResult_Ok);
}
//=== end WriteProcessCounter ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadProcessCounter(u32* data)
{
	EOsResult result;


//	taskENTER_CRITICAL();
	result = this->ReadVariable32bits(EEPROM_ADR_PROCESS_COUNTER, data);
//	taskEXIT_CRITICAL();
	if(result != OsResult_Ok)
	{
		return(result);
	}


	return(OsResult_Ok);
}
//=== end ReadProcessCounter =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadVariable32bits(u32 address, u32* data)
{
	HAL_StatusTypeDef halResult;
	u16 tempDataAddress;
	u16 tempDevAddress;
	u32 tempData;
	u32 tempNotData;
	u8 writeBuffer[sizeof(tempDataAddress)];
	u8 readBuffer[sizeof(u32) * 2];


	if(address > EEPROM_16_BITS_ADDRESS)
	{
		tempDevAddress = (u16)(EEPROM_DEVICE_ADDRESS | 0x02);
	}
	else
	{
		tempDevAddress = (u16)EEPROM_DEVICE_ADDRESS;
	}

	tempDataAddress = (u16)address;

	memcpy((void*)writeBuffer, (void*)&tempDataAddress, sizeof(tempDataAddress));

	if(this->Semaphore.Take(100) == OsResult_Timeout)
	{
		return(OsResult_ResourceBusy);
	}

	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_RESET);
	halResult = HAL_I2C_Master_Transmit(
					&hi2c1,
					tempDevAddress,
					writeBuffer,
					sizeof(tempDataAddress),
					20  // timeout 20 mSec
					);
	if(halResult != HAL_OK)
	{
		HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
		this->Semaphore.Give();
		return(OsResult_ErrorI2c1);
	}


	halResult = HAL_I2C_Master_Receive(
					&hi2c1,
					tempDevAddress,
					readBuffer,
					sizeof(readBuffer),
					20  // timeout 20 mSec
					);
	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2c1);
	}

	memcpy((void*)&tempData, (void*)readBuffer, sizeof(tempData));
	memcpy((void*)data, (void*)readBuffer, sizeof(tempData));
	memcpy((void*)&tempNotData, (void*)(readBuffer + sizeof(tempData)), sizeof(tempNotData));

	if(tempData != (0xFFFFFFFF - tempNotData))
	{
		return(OsResult_ErrorI2c1);
	}


	return(OsResult_Ok);
}
//=== end ReadVariable32bits =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadVariable16bits(u32 address, u16* data)
{
	HAL_StatusTypeDef halResult;
	u16 tempDataAddress;
	u16 tempDevAddress;
	u16 tempData;
	u16 tempNotData;
	u8 writeBuffer[sizeof(tempDataAddress)];
	u8 readBuffer[sizeof(u32) * 2];


	if(address > EEPROM_16_BITS_ADDRESS)
	{
		tempDevAddress = (u16)(EEPROM_DEVICE_ADDRESS | 0x02);
	}
	else
	{
		tempDevAddress = (u16)EEPROM_DEVICE_ADDRESS;
	}

	tempDataAddress = (u16)address;

	memcpy((void*)writeBuffer, (void*)&tempDataAddress, sizeof(tempDataAddress));

	if(this->Semaphore.Take(100) == OsResult_Timeout)
	{
		return(OsResult_ResourceBusy);
	}

	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
	halResult = HAL_I2C_Master_Transmit(
					&hi2c1,
					tempDevAddress,
					writeBuffer,
					sizeof(tempDataAddress),
					20  // timeout 20 mSec
					);
	if(halResult != HAL_OK)
	{
		HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
		this->Semaphore.Give();
		return(OsResult_ErrorI2c1);
	}


	halResult = HAL_I2C_Master_Receive(
					&hi2c1,
					tempDevAddress,
					readBuffer,
					sizeof(readBuffer),
					20  // timeout 20 mSec
					);
	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2c1);
	}

	memcpy((void*)&tempData, (void*)readBuffer, sizeof(tempData));
	memcpy((void*)data, (void*)readBuffer, sizeof(tempData));
	memcpy((void*)&tempNotData, (void*)(readBuffer + sizeof(tempData)), sizeof(tempNotData));

	if(tempData != (0xFFFF - tempNotData))
	{
		return(OsResult_ErrorI2c1);
	}


	return(OsResult_Ok);
}
//=== end ReadVariable16bits =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadVariable8bits(u32 address, u8* data)
{
	HAL_StatusTypeDef halResult;
	u16 tempDataAddress;
	u16 tempDevAddress;
	u8 tempData;
	u8 tempNotData;
	u8 writeBuffer[sizeof(tempDataAddress)];
	u8 readBuffer[sizeof(u32) * 2];


	if(address > EEPROM_16_BITS_ADDRESS)
	{
		tempDevAddress = (u16)(EEPROM_DEVICE_ADDRESS | 0x02);
	}
	else
	{
		tempDevAddress = (u16)EEPROM_DEVICE_ADDRESS;
	}

	tempDataAddress = (u16)address;

	memcpy((void*)writeBuffer, (void*)&tempDataAddress, sizeof(tempDataAddress));

	if(this->Semaphore.Take(100) == OsResult_Timeout)
	{
		return(OsResult_ResourceBusy);
	}

	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_RESET);
	halResult = HAL_I2C_Master_Transmit(
					&hi2c1,
					tempDevAddress,
					writeBuffer,
					sizeof(tempDataAddress),
					20  // timeout 20 mSec
					);
	if(halResult != HAL_OK)
	{
		HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
		this->Semaphore.Give();
		return(OsResult_ErrorI2c1);
	}

	halResult = HAL_I2C_Master_Receive(
					&hi2c1,
					tempDevAddress,
					readBuffer,
					sizeof(readBuffer),
					20  // timeout 20 mSec
					);
	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2c1);
	}

	memcpy((void*)&tempData, (void*)readBuffer, sizeof(tempData));
	memcpy((void*)data, (void*)readBuffer, sizeof(tempData));
	memcpy((void*)&tempNotData, (void*)(readBuffer + sizeof(tempData)), sizeof(tempNotData));

	if(tempData != (0xFF - tempNotData))
	{
		return(OsResult_ErrorI2c1);
	}


	return(OsResult_Ok);
}
//=== end ReadVariable8bits ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::WriteVariable32bits(u32 address, u32 data)
{
	HAL_StatusTypeDef halResult;
	u16 tempDataAddress;
	u16 tempDevAddress;
	u32 tempData;
	u8 writeBuffer[sizeof(tempDataAddress) + (sizeof(data) * 2)];


	if(address > EEPROM_16_BITS_ADDRESS)
	{
		tempDevAddress = (u16)(EEPROM_DEVICE_ADDRESS | 0x02);
	}
	else
	{
		tempDevAddress = (u16)EEPROM_DEVICE_ADDRESS;
	}

	tempDataAddress = (u16)address;
	tempData = ~data;

	memcpy((void*)writeBuffer, (void*)&tempDataAddress, sizeof(tempDataAddress));
	memcpy((void*)(writeBuffer + sizeof(tempDataAddress)), (void*)&data, sizeof(data));
	memcpy((void*)(writeBuffer + sizeof(tempDataAddress) + sizeof(data)), (void*)&tempData, sizeof(tempData));

	if(this->Semaphore.Take(100) == OsResult_Timeout)
	{
		return(OsResult_ResourceBusy);
	}

	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_RESET);
	halResult = HAL_I2C_Master_Transmit(
					&hi2c1,
					tempDevAddress,
					writeBuffer,
					sizeof(tempDataAddress) + (sizeof(data) * 2),
					20  // timeout 20 mSec
					);
	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2c1);
	}


	return(OsResult_Ok);
}
//=== end WriteVariable32bits ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::WriteVariable16bits(u32 address, u16 data)
{
	HAL_StatusTypeDef halResult;
	u16 tempDataAddress;
	u16 tempDevAddress;
	u16 tempData;
	u8 writeBuffer[sizeof(tempDataAddress) + (sizeof(data) * 2)];


	if(address > EEPROM_16_BITS_ADDRESS)
	{
		tempDevAddress = (u16)(EEPROM_DEVICE_ADDRESS | 0x02);
	}
	else
	{
		tempDevAddress = (u16)EEPROM_DEVICE_ADDRESS;
	}

	tempDataAddress = (u16)address;
	tempData = ~data;

	memcpy((void*)writeBuffer, (void*)&tempDataAddress, sizeof(tempDataAddress));
	memcpy((void*)(writeBuffer + sizeof(tempDataAddress)), (void*)&data, sizeof(data));
	memcpy((void*)(writeBuffer + sizeof(tempDataAddress) + sizeof(data)), (void*)&tempData, sizeof(tempData));

	if(this->Semaphore.Take(100) == OsResult_Timeout)
	{
		return(OsResult_ResourceBusy);
	}

	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_RESET);
	halResult = HAL_I2C_Master_Transmit(
					&hi2c1,
					tempDevAddress,
					writeBuffer,
					sizeof(tempDataAddress) + (sizeof(data) * 2),
					20  // timeout 20 mSec
					);
	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2c1);
	}


	return(OsResult_Ok);
}
//=== end WriteVariable16bits ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::WriteVariable8bits(u32 address, u8 data)
{
	HAL_StatusTypeDef halResult;
	u16 tempDataAddress;
	u16 tempDevAddress;
	u8 tempData;
	u8 writeBuffer[sizeof(tempDataAddress) + (sizeof(data) * 2)];


	if(address > EEPROM_16_BITS_ADDRESS)
	{
		tempDevAddress = (u16)(EEPROM_DEVICE_ADDRESS | 0x02);
	}
	else
	{
			tempDevAddress = (u16)EEPROM_DEVICE_ADDRESS;
	}

	tempDataAddress = (u16)address;
	tempData = ~data;

	memcpy((void*)writeBuffer, (void*)&tempDataAddress, sizeof(tempDataAddress));
	memcpy((void*)(writeBuffer + sizeof(tempDataAddress)), (void*)&data, sizeof(data));
	memcpy((void*)(writeBuffer + sizeof(tempDataAddress) + sizeof(data)), (void*)&tempData, sizeof(tempData));

	if(this->Semaphore.Take(100) == OsResult_Timeout)
	{
		return(OsResult_ResourceBusy);
	}

	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_RESET);
	halResult = HAL_I2C_Master_Transmit(
					&hi2c1,
					tempDevAddress,
					writeBuffer,
					sizeof(tempDataAddress) + (sizeof(data) * 2),
					20  // timeout 20 mSec
					);
	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2c1);
	}


	return(OsResult_Ok);
}
//=== end WriteVariable8bits =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::Format()
{


	return(OsResult_Ok);
}
//=== end Format ===================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::WriteMinutes(u8 minutes)
{
	HAL_StatusTypeDef halResult;
	u8 writeBuffer[2];


	writeBuffer[0] = EEPROM_RTC_MINUTES_ADDRESS;
	writeBuffer[1] = minutes;
	halResult = HAL_I2C_Master_Transmit(
					&hi2c2,
					EEPROM_RTC_SLAVE_ADDRESS,
					writeBuffer,
					2,
					20  // timeout 20 mSec
					);
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2c2);
	}


	return(OsResult_Ok);
}
//=== end WriteMinutes ===================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadMinutes(u8* minutes)
{
	HAL_StatusTypeDef halResult;
	u8 writeBuffer[2];
	u8 readBuffer[2];


	writeBuffer[0] = EEPROM_RTC_MINUTES_ADDRESS;
	halResult = HAL_I2C_Master_Transmit(
					&hi2c2,
					EEPROM_RTC_SLAVE_ADDRESS,
					writeBuffer,
					1,
					20  // timeout 20 mSec
					);
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2c2);
	}

	halResult = HAL_I2C_Master_Receive(
					&hi2c2,
					EEPROM_RTC_SLAVE_ADDRESS,
					readBuffer,
					1,
					20  // timeout 20 mSec
					);
	if(halResult != HAL_OK)
	{
		*minutes = 0;
		return(OsResult_ErrorI2c2);
	}

	*minutes = readBuffer[0];


	return(OsResult_Ok);
}
//=== end ReadMinutes ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::WriteHours(u8 hours)
{
	HAL_StatusTypeDef halResult;
	u8 writeBuffer[2];


	writeBuffer[0] = EEPROM_RTC_SECONDS_ADDRESS;  // EEPROM_RTC_HOURS_ADDRESS;
	writeBuffer[1] = hours;
	halResult = HAL_I2C_Master_Transmit(
					&hi2c2,
					EEPROM_RTC_SLAVE_ADDRESS,
					writeBuffer,
					2,
					20  // timeout 20 mSec
					);
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2c2);
	}


	return(OsResult_Ok);
}
//=== end WriteHours ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadHours(u8* hours)
{
	HAL_StatusTypeDef halResult;
	u8 writeBuffer[2];
	u8 readBuffer[2];


	writeBuffer[0] = EEPROM_RTC_SECONDS_ADDRESS;  // EEPROM_RTC_HOURS_ADDRESS;
	halResult = HAL_I2C_Master_Transmit(
					&hi2c2,
					EEPROM_RTC_SLAVE_ADDRESS,
					writeBuffer,
					1,
					20  // timeout 20 mSec
					);
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2c2);
	}

	halResult = HAL_I2C_Master_Receive(
					&hi2c2,
					EEPROM_RTC_SLAVE_ADDRESS,
					readBuffer,
					1,
					20  // timeout 20 mSec
					);
	if(halResult != HAL_OK)
	{
		*hours = 0;
		return(OsResult_ErrorI2c2);
	}

	*hours = readBuffer[0];


	return(OsResult_Ok);
}
//=== end ReadHours ================================================================

/**********************************************************************************/
