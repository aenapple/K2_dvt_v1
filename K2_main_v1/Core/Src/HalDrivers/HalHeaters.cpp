/**********************************************************************************/
/**
 *  @file HalHeaters.cpp
 *  @brief ...
 *
 *  @copyright (c) 2024 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "HalHeaters.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
THalHeaters::THalHeaters()
{
	this->HeaterPtcLeft.Init();
	this->HeaterPtcRight.Init();
	this->HeaterPadLeft.Init();
	this->HeaterPadRight.Init();
}
//=== end THalHeaters ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
THalHeaters& THalHeaters::GetInstance(void)
{
	static THalHeaters HalHeaters;
	return(HalHeaters);
}
//=== end GetInstance ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::StartHeaterPtcLeft(u8 pwm)
{
	this->HeaterPtcLeft.TurnOn((EHeaterPwm)(pwm / 10));
}
//=== end StartHeaterPtcLeft =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::StopHeaterPtcLeft()
{
	this->HeaterPtcLeft.TurnOff();
}
//=== end StopHeaterPtcLeft ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EHeaterPwm THalHeaters::GetPwmHeaterPtcLeft()
{
	return(this->HeaterPtcLeft.GetPwm());
}
//=== end GetPwmHeaterPtcLeft ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::PulseOnHeaterPtcLeft()
{
	this->HeaterPtcLeft.PulseOn();
}
//=== end PulseOnHeaterPtcLeft =====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::PulseOffHeaterPtcLeft()
{
	this->HeaterPtcLeft.PulseOff();
}
//=== end PulseOffHeaterPtcLeft ====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::StartHeaterPtcRight(u8 pwm)
{
	this->HeaterPtcRight.TurnOn((EHeaterPwm)(pwm / 10));
}
//=== end StartHeaterPtcRight ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::StopHeaterPtcRight()
{
	this->HeaterPtcRight.TurnOff();
}
//=== end StopHeaterPtcRight =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EHeaterPwm THalHeaters::GetPwmHeaterPtcRight()
{
	return(this->HeaterPtcRight.GetPwm());
}
//=== end GetPwmHeaterPtcRight =====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::PulseOnHeaterPtcRight()
{
	this->HeaterPtcRight.PulseOn();
}
//=== end PulseOnHeaterPtcRight ====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::PulseOffHeaterPtcRight()
{
	this->HeaterPtcRight.PulseOff();
}
//=== end PulseOffHeaterPtcRight ===================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::StartHeaterPadLeft(u8 pwm)
{
	this->HeaterPadLeft.TurnOn((EHeaterPwm)(pwm / 10));
}
//=== end StartHeaterPadLeft =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::StopHeaterPadLeft()
{
	this->HeaterPadLeft.TurnOff();
}
//=== end StopHeaterPadLeft ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EHeaterPwm THalHeaters::GetPwmHeaterPadLeft()
{
	return(this->HeaterPadLeft.GetPwm());
}
//=== end GetPwmHeaterPadLeft ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::PulseOnHeaterPadLeft()
{
	this->HeaterPadLeft.PulseOn();
}
//=== end PulseOnHeaterPadLeft =====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::PulseOffHeaterPadLeft()
{
	this->HeaterPadLeft.PulseOff();
}
//=== end PulseOffHeaterPadLeft ====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::StartHeaterPadRight(u8 pwm)
{
	this->HeaterPadRight.TurnOn((EHeaterPwm)(pwm / 10));
}
//=== end StartHeaterPadRight ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::StopHeaterPadRight()
{
	this->HeaterPadRight.TurnOff();
}
//=== end StopHeaterPadRight =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EHeaterPwm THalHeaters::GetPwmHeaterPadRight()
{
	return(this->HeaterPadRight.GetPwm());
}
//=== end GetPwmHeaterPadRight =====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::PulseOnHeaterPadRight()
{
	this->HeaterPadRight.PulseOn();
}
//=== end PulseOnHeaterPadRight ====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalHeaters::PulseOffHeaterPadRight()
{
	this->HeaterPadRight.PulseOff();
}
//=== end PulseOffHeaterPadRight ===================================================

/**********************************************************************************/
