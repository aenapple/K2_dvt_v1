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

	this->indexRecord = 0;


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
EOsResult TEeprom::WriteTimestamp(TRtc* rtc)
{
	HAL_StatusTypeDef halResult;
	u16 tempDataAddress;
	u16 tempDevAddress;
	u8 writeBuffer[sizeof(tempDataAddress) + sizeof(TRtc)];


	tempDevAddress = (u16)EEPROM_DEVICE_ADDRESS;
	tempDataAddress = EEPROM_BETA_TEST_TIMESTAMP_ADR;

	memcpy((void*)writeBuffer, (void*)&tempDataAddress, sizeof(tempDataAddress));
	memcpy((void*)(writeBuffer + sizeof(tempDataAddress)), (void*)rtc, sizeof(TRtc));

	if(this->Semaphore.Take(100) == OsResult_Timeout)
	{
		return(OsResult_ResourceBusy);
	}

	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_RESET);
	halResult = HAL_I2C_Master_Transmit(
					&hi2c1,
					tempDevAddress,
					writeBuffer,
					sizeof(writeBuffer),
					50  // timeout 50 mSec
					);
	this->Delay(6);
	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2c1);
	}


	return(OsResult_Ok);
}
//=== end WriteTimestamp ===========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadTimestamp(TRtc* rtc)
{
	HAL_StatusTypeDef halResult;
	u16 tempDataAddress;
	u16 tempDevAddress;
	u8 writeBuffer[sizeof(tempDataAddress)];
	u8 readBuffer[sizeof(TRtc)];


	tempDevAddress = (u16)EEPROM_DEVICE_ADDRESS;
	tempDataAddress = EEPROM_BETA_TEST_TIMESTAMP_ADR;

	memcpy((void*)writeBuffer, (void*)&tempDataAddress, sizeof(tempDataAddress));

	if(this->Semaphore.Take(100) == OsResult_Timeout)
	{
		return(OsResult_ResourceBusy);
	}

	halResult = HAL_I2C_Master_Transmit(
					&hi2c1,
					tempDevAddress,
					writeBuffer,
					sizeof(writeBuffer),
					50  // timeout 50 mSec
					);
	if(halResult != HAL_OK)
	{
		this->Semaphore.Give();
		return(OsResult_ErrorI2c1);
	}

	halResult = HAL_I2C_Master_Receive(
					&hi2c1,
					tempDevAddress,
					readBuffer,
					sizeof(TRtc),
					50  // timeout 50 mSec
					);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		memset((void*)rtc, 0, sizeof(TRtc));
		return(OsResult_ErrorI2c1);
	}

	memcpy((void*)rtc, (void*)readBuffer, sizeof(TRtc));


	return(OsResult_Ok);
}
//=== end ReadTimestamp ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::WriteRecord(TBetaTestRecord* record)
{
	HAL_StatusTypeDef halResult;
	EOsResult result;
	u16 tempDataAddress;
	u16 tempDevAddress;
	u8 writeBuffer[sizeof(tempDataAddress) + sizeof(this->indexRecord)];
	u32 address;
//	u8 tempBuffer[8];
	u8* pRecord;


	if(this->indexRecord >= EEPROM_BETA_TEST_MAX_RECORDS)
	{
		return(OsResult_EepromFull);
	}

	pRecord = (u8*)record;
	address = (u32)this->indexRecord * EEPROM_BETA_TEST_RECORD_SIZE + EEPROM_BETA_TEST_FIRST_RECORD_ADR;
	for(u16 i = 0; i < (sizeof(TBetaTestRecord) / 8); i++)
	{
//		memcpy((void*)tempBuffer, (void*)pRecord, 8);

		result = this->WritePacket(address, pRecord);
		if(result != OsResult_Ok)
		{
			return(result);
		}

		address = address + 8;
		pRecord = pRecord + 8;

	}

	this->indexRecord++;
	tempDevAddress = (u16)EEPROM_DEVICE_ADDRESS;
	tempDataAddress = EEPROM_BETA_TEST_INDEX_RECORD_ADR;
	memcpy((void*)writeBuffer, (void*)&tempDataAddress, sizeof(tempDataAddress));
	memcpy((void*)(writeBuffer + sizeof(tempDataAddress)), (void*)&this->indexRecord, sizeof(this->indexRecord));

	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_RESET);
	halResult = HAL_I2C_Master_Transmit(
					&hi2c1,
					tempDevAddress,
					writeBuffer,
					sizeof(tempDataAddress) + sizeof(this->indexRecord),
					50  // timeout 50 mSec
					);
	this->Delay(6);
	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		return(OsResult_StopProcess/*OsResult_ErrorI2c1*/);
	}


	return(OsResult_Ok);
}
//=== end WriteRecord ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadRecord(TBetaTestRecord* record)
{
	HAL_StatusTypeDef halResult;
	u16 tempDataAddress;
	u16 tempDevAddress;
	u8 writeBuffer[sizeof(tempDataAddress)];
	u8 readBuffer[sizeof(TBetaTestRecord)];
	u32 address;


	address = (u32)this->indexRecord * EEPROM_BETA_TEST_RECORD_SIZE + EEPROM_BETA_TEST_FIRST_RECORD_ADR;
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
					sizeof(TBetaTestRecord),
					50  // timeout 50 mSec
					);
	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		memset((void*)record, 0, sizeof(TBetaTestRecord));
		return(OsResult_ErrorI2c1);
	}

	memcpy((void*)record, (void*)readBuffer, sizeof(TBetaTestRecord));


	return(OsResult_Ok);
}
//=== end ReadRecord ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::WritePacket(u32 address, u8* data)
{
	HAL_StatusTypeDef halResult;
	u16 tempDataAddress;
	u16 tempDevAddress;
	u32 tempData;
	u8 writeBuffer[sizeof(tempDataAddress) + 8];
//	u8 tempBuffer[8];


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
	memcpy((void*)(writeBuffer + sizeof(tempDataAddress)), (void*)data, 8);

	if(this->Semaphore.Take(100) == OsResult_Timeout)
	{
		return(OsResult_ResourceBusy);
	}

	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_RESET);
	halResult = HAL_I2C_Master_Transmit(
					&hi2c1,
					tempDevAddress,
					writeBuffer,
					sizeof(writeBuffer),
					50  // timeout 50 mSec
					);
	this->Delay(6);
	HAL_GPIO_WritePin(WC_EEPROM_GPIO_Port, WC_EEPROM_Pin, GPIO_PIN_SET);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2c1);
	}


	return(OsResult_Ok);
}
//=== end WritePacket ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadPacket(u32 address, u8* data)
{
	HAL_StatusTypeDef halResult;
	u16 tempDataAddress;
	u16 tempDevAddress;
	u8 writeBuffer[sizeof(tempDataAddress)];
	u8 readBuffer[8];


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

	halResult = HAL_I2C_Master_Transmit(
					&hi2c1,
					tempDevAddress,
					writeBuffer,
					sizeof(writeBuffer),
					50  // timeout 20 mSec
					);
	if(halResult != HAL_OK)
	{
		this->Semaphore.Give();
		return(OsResult_ErrorI2c1);
	}


	halResult = HAL_I2C_Master_Receive(
					&hi2c1,
					tempDevAddress,
					readBuffer,
					sizeof(readBuffer),
					50  // timeout 20 mSec
					);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		memset((void*)data, 0, sizeof(readBuffer));
		return(OsResult_ErrorI2c1);
	}

	memcpy((void*)data, (void*)readBuffer, sizeof(readBuffer));


	return(OsResult_Ok);
}
//=== end ReadPacket ===============================================================

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
	u8 readBuffer[sizeof(u8) * 2];


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
EOsResult TEeprom::WriteRtc(TRtc* rtc)
{
#ifdef __DEBUG_RTC_NOT_PRESENT
	return(OsResult_Ok);
#else
	HAL_StatusTypeDef halResult;
	u8 writeBuffer[1 + sizeof(TRtc)];


	writeBuffer[0] = EEPROM_RTC_SECONDS_ADDRESS;
	memcpy((void*)(writeBuffer + 1), (void*)rtc, sizeof(TRtc));

	if(this->Semaphore.Take(100) == OsResult_Timeout)
	{
		return(OsResult_ResourceBusy);
	}

	halResult = HAL_I2C_Master_Transmit(
					&hi2c2,
					EEPROM_RTC_SLAVE_ADDRESS,
					writeBuffer,
					sizeof(writeBuffer),
					50  // timeout 50 mSec
					);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2c2);
	}


	return(OsResult_Ok);
#endif
}
//=== end WriteRtc =================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadRtc(TRtc* rtc)
{
#ifdef __DEBUG_RTC_NOT_PRESENT
	memset((void*)rtc, 0, sizeof(TRtc));
	return(OsResult_Ok);
#else
	HAL_StatusTypeDef halResult;
	u8 writeBuffer[2];
	u8 readBuffer[sizeof(TRtc)];


	writeBuffer[0] = EEPROM_RTC_SECONDS_ADDRESS;

	if(this->Semaphore.Take(100) == OsResult_Timeout)
	{
		return(OsResult_ResourceBusy);
	}

	halResult = HAL_I2C_Master_Transmit(
					&hi2c2,
					EEPROM_RTC_SLAVE_ADDRESS,
					writeBuffer,
					1,
					50  // timeout 50 mSec
					);
	if(halResult != HAL_OK)
	{

		return(OsResult_ErrorI2c2);
	}

	halResult = HAL_I2C_Master_Receive(
					&hi2c2,
					EEPROM_RTC_SLAVE_ADDRESS,
					readBuffer,
					sizeof(TRtc),
					50  // timeout 20 mSec
					);
	this->Semaphore.Give();
	if(halResult != HAL_OK)
	{
		memset((void*)rtc, 0, sizeof(TRtc));
		return(OsResult_ErrorI2c2);
	}

	memcpy((void*)rtc, (void*)readBuffer, sizeof(TRtc));


	return(OsResult_Ok);
#endif
}
//=== end WriteRtc =================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::WriteMinutes(u8 minutes)
{
#ifdef __DEBUG_RTC_NOT_PRESENT
	return(OsResult_Ok);
#else
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
#endif
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
#ifdef __DEBUG_RTC_NOT_PRESENT
	*minutes = 30;

	return(OsResult_Ok);
#else
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
#endif
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
#ifdef __DEBUG_RTC_NOT_PRESENT
	return(OsResult_Ok);
#else
	HAL_StatusTypeDef halResult;
	u8 writeBuffer[2];


	writeBuffer[0] = EEPROM_RTC_HOURS_ADDRESS;
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
#endif
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
#ifdef __DEBUG_RTC_NOT_PRESENT
	*hours = 0x10;

	return(OsResult_Ok);
#else
	HAL_StatusTypeDef halResult;
	u8 writeBuffer[2];
	u8 readBuffer[2];


	writeBuffer[0] = EEPROM_RTC_HOURS_ADDRESS;
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
#endif
}
//=== end ReadHours ================================================================

/**********************************************************************************/
