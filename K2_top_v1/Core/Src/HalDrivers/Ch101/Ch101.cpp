/**********************************************************************************/
/**
 *  @file Ch101.cpp
 *  @brief Driver for CH101 sensors
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "Ch101.hpp"


/**********************************************************************************/



/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::Init()
{
	u8 sigBytes[2];


	chirp_group.num_ports = CHIRP_MAX_NUM_SENSORS;
	chirp_group.num_i2c_buses = CHIRP_NUM_I2C_BUSES;
	chirp_group.rtc_cal_pulse_ms = CH101_RTC_CAL_PULSE_MS;

	HAL_GPIO_WritePin(RESET1_GPIO_Port, RESET1_Pin, GPIO_PIN_SET);  // reset = '1'
	HAL_GPIO_WritePin(RESET2_GPIO_Port, RESET2_Pin, GPIO_PIN_SET);  // reset = '1'
	HAL_GPIO_WritePin(RESET3_GPIO_Port, RESET3_Pin, GPIO_PIN_SET);  // reset = '1'

	HAL_GPIO_WritePin(PROG1_GPIO_Port, PROG1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PROG2_GPIO_Port, PROG2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PROG3_GPIO_Port, PROG3_Pin, GPIO_PIN_RESET);

	///// check sensor 1 //////
	HAL_GPIO_WritePin(PROG1_GPIO_Port, PROG1_Pin, GPIO_PIN_SET);
	sigBytes[0] = 0;
	sigBytes[1] = 0;



}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult ReadRegisterI2c1(u8 devAddress, u8 regAddress, u16 regLength, u8* regValue)
{

}
//=== end ReadRegisterI2c1 =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult ReadRawRegisterI2c1(u8 devAddress, u16 regLength, u8* regValue)
{

}
//=== end ReadRawRegisterI2c1 ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult WriteRegisterI2c1(u8 devAddress, u8 regAddress, u16 regLength, u8* regValue)
{

}
//=== end WriteRegisterI2c1 ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult WriteRawRegisterI2c1(u8 devAddress, u16 regLength, u8* regValue)
{

}
//=== end WriteRawRegisterI2c1 =====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult ReadRegisterI2c2(u8 devAddress, u8 regAddress, u16 regLength, u8* regValue)
{

}
//=== end ReadRegisterI2c2 =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult ReadRawRegisterI2c2(u8 devAddress, u16 regLength, u8* regValue)
{

}
//=== end ReadRawRegisterI2c2 ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult WriteRegisterI2c2(u8 devAddress, u8 regAddress, u16 regLength, u8* regValue)
{

}
//=== end WriteRegisterI2c2 ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult WriteRawRegisterI2c2(u8 devAddress, u16 regLength, u8* regValue)
{

}


/**********************************************************************************/
