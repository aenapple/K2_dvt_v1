/**********************************************************************************/
/**
 *  @file I2c.cpp
 *  @brief ...
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "I2c.hpp"


/**********************************************************************************/
extern I2C_HandleTypeDef hi2c1;


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TI2c::Init()
{
/*	EOsResult result;
	u8 regData;


	result = this->Bme688_WriteRegister(BME688_REG_RESET, BME688_RESET_CMD);
	if(result != OsResult_Ok)
	{
		return(result);
	}

	this->Delay(4);

	result = this->Bme688_ReadRegister(BME688_REG_CHIP_ID, &regData);
	if(result != OsResult_Ok)
	{
		return(result);
	}


	if(regData != BME688_CHIP_ID)
	{
		return(OsResult_Error);
	}

	result = this->Bme688_GetCalibrationData();
	if(result != OsResult_Ok)
	{
		return(result);
	} */


	return(OsResult_Ok);
}
//=== end Init =====================================================================

/**********************************************************************************/
