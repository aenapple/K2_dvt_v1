/**********************************************************************************/
/**
 *  @file HalDcFans.cpp
 *  @brief ...
 *
 *  @copyright (c) 2024 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include <HalDcFans.hpp>


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
THalDcFans::THalDcFans()
{
	this->FanPtcLeft.Init();
	this->FanPtcRight.Init();
	this->FanAirLeft.Init();
	this->FanAirRight.Init();
}
//=== end THalDcFans ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
THalDcFans& THalDcFans::GetInstance(void)
{
	static THalDcFans HalDcFans;
	return(HalDcFans);
}
//=== end GetInstance ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalDcFans::StartFanPtcLeft(u8 pwm)
{
	this->FanPtcLeft.StartPwm(pwm);
}
//=== end StartFanPtcLeft ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalDcFans::StopFanPtcLeft()
{
	this->FanPtcLeft.StartPwm(0);
}
//=== end StopFanPtcLeft ===========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u8 THalDcFans::GetPwmFanPtcLeft()
{
	return(this->FanPtcLeft.GetPwm());
}
//=== end GetPwmFanPtcLeft =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u16 THalDcFans::GetRpmFanPtcLeft(void)
{
	return(this->FanPtcLeft.GetRpm());
}
//=== end GetRpmFanPtcLeft =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalDcFans::StartFanPtcRight(u8 pwm)
{
	this->FanPtcRight.StartPwm(pwm);
}
//=== end StartFanPtcRight =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalDcFans::StopFanPtcRight()
{
	this->FanPtcRight.StartPwm(0);
}
//=== end StopFanPtcRight ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u8 THalDcFans::GetPwmFanPtcRight()
{
	return(this->FanPtcRight.GetPwm());
}
//=== end GetPwmFanPtcRight ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u16 THalDcFans::GetRpmFanPtcRight(void)
{
	return(this->FanPtcRight.GetRpm());
}
//=== end GetRpmFanPtcRight ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalDcFans::StartFanAirLeft(u8 pwm)
{
	this->FanAirLeft.StartPwm(pwm);
}
//=== end StartFanAirLeft ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalDcFans::StopFanAirLeft()
{
	this->FanAirLeft.StartPwm(0);
}
//=== end StopFanAirLeft ===========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u8 THalDcFans::GetPwmFanAirLeft()
{
	return(this->FanAirLeft.GetPwm());
}
//=== end GetPwmFanAirLeft =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u16 THalDcFans::GetRpmFanAirLeft(void)
{
	return(this->FanAirLeft.GetRpm());
}
//=== end GetRpmFanAirLeft =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalDcFans::StartFanAirRight(u8 pwm)
{
	this->FanAirRight.StartPwm(pwm);
}
//=== end StartFanAirRight =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalDcFans::StopFanAirRight()
{
	this->FanAirRight.StartPwm(0);
}
//=== end StopFanAirRight ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u8 THalDcFans::GetPwmFanAirRight()
{
	return(this->FanAirRight.GetPwm());
}
//=== end GetPwmFanAirRight ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u16 THalDcFans::GetRpmFanAirRight(void)
{
	return(this->FanAirRight.GetRpm());
}
//=== end GetRpmFanAirRight ========================================================

/**********************************************************************************/
