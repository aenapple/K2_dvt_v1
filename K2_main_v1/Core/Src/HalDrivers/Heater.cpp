/**********************************************************************************/
/**
 *  @file Heater.cpp
 *  @brief Driver for PTC and Pad heaters
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "Heater.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THeater::Init(EHeater heater)
{

//	this->heater = heater;

	switch(heater)
	{
		case Heater_PtcHeaterLeft:
			this->GPIOx = PTC1_ON_GPIO_Port;
			this->GPIO_Pin = PTC1_ON_Pin;
			break;

		case Heater_PtcHeaterRight:
			this->GPIOx = PTC2_ON_GPIO_Port;
			this->GPIO_Pin = PTC2_ON_Pin;
			break;

		case Heater_PadHeaterLeft:
			this->GPIOx = PAD1_ON_GPIO_Port;
			this->GPIO_Pin = PAD1_ON_Pin;
			break;

		default:  // Heater_PadHeaterRight
			this->GPIOx = PAD2_ON_GPIO_Port;
			this->GPIO_Pin = PAD2_ON_Pin;
			break;
	}

	this->pwm = 0;

}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THeater::TurnOn(EHeaterPwm heaterPwm)
{
	this->pwm = heaterPwm;
}
//=== end TurnOn ===================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THeater::TurnOff()
{
	this->pwm = 0;
}
//=== end TurnOff ==================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THeater::PulseOn()
{
	HAL_GPIO_WritePin(this->GPIOx, this->GPIO_Pin, GPIO_PIN_RESET);
}
//=== end PulseOn ==================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THeater::PulseOff()
{
	HAL_GPIO_WritePin(this->GPIOx, this->GPIO_Pin, GPIO_PIN_SET);
}
//=== end PulseOff =================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
s8 THeater::GetTemperature()
{
	return(this->temperature);
}
//=== end GetTemperature ===========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THeater::SetTemperature(s8 t)
{
	this->temperature = t;
}
//=== end SetTemperature ===========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u8 THeater::GetPwm()
{
	return(this->pwm);
}
//=== end GetPwm ===================================================================

/**********************************************************************************/
