/**********************************************************************************/
/**
 *  @file PtcFan.cpp
 *  @brief Driver for PTC fans
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "PtcFan.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TPtcFan::Init(EPtcFan ptcFan)
{

	if(ptcFan == PtcFan_Left)
	{
		this->GPIOx = PTC1_FAN2_GPIO_Port;
		this->GPIO_Pin = PTC1_FAN2_Pin;
	}
	else
	{
		this->GPIOx = PTC2_FAN2_GPIO_Port;
		this->GPIO_Pin = PTC2_FAN2_Pin;
	}

	this->pwm = PtcFanPwm_0;
	this->maxPwm = PtcFanMaxPwm_66_100;
	this->ClearCounterPwm();

}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TPtcFan::Start(EPtcFanPwm ptcFanPwm, EPtcFanMaxPwm ptcFanMaxPwm)
{
	this->pwm = ptcFanPwm;
	this->maxPwm = ptcFanMaxPwm;
}
//=== end Start ====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TPtcFan::Stop()
{
	this->pwm = PtcFanPwm_0;
}
//=== end Stop =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TPtcFan::PulseOn()
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
void TPtcFan::PulseOff()
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
u8 TPtcFan::GetPwm()
{
	return(this->pwm);
}
//=== end GetPwm ===================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u8 TPtcFan::GetMaxPwm()
{
	return(this->maxPwm);
}
//=== end GetMaxPwm ================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u8 TPtcFan::GetCounterPwm()
{
	return(this->counterPwm);
}
//=== end GetCounterPwm ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TPtcFan::IncrementCounterPwm()
{
	this->counterPwm++;
}
//=== end IncrementCounterPwm ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TPtcFan::ClearCounterPwm()
{
	this->counterPwm = 0;
}
//=== end ClearCounterPwm ==========================================================

/**********************************************************************************/
