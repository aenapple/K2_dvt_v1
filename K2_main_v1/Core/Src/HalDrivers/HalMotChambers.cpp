/**********************************************************************************/
/**
 *  @file HalMotChambers.cpp
 *  @brief Driver for chamber motors
 *
 *  @copyright (c) 2024 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "HalMotChambers.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
THalMotChambers::THalMotChambers()
{
	this->MotorChamberLeft.Init();
	this->MotorChamberRight.Init();
}
//=== end THalMotChambers ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
THalMotChambers& THalMotChambers::GetInstance()
{
	static THalMotChambers HalMotChambers;
	return(HalMotChambers);
}
//=== end GetInstance ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalMotChambers::StartForwardMotorLeft()
{
	this->MotorChamberLeft.StartForward();
}
//=== end StartForwardMotorLeft ====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalMotChambers::StartBackwardMotorLeft(void)
{
	this->MotorChamberLeft.StartBackward();
}
//=== end StartBackwardMotorLeft ===================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalMotChambers::StopMotorLeft(void)
{
	this->MotorChamberLeft.Stop();
}
//=== end StopMotorLeft ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u8 THalMotChambers::GetPwmMotorLeft(void)
{
	return(this->MotorChamberLeft.GetPwm());
}
//=== end GetPwmMotorLeft ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
bool THalMotChambers::GetDirMotorLeft(void)
{
	return(this->MotorChamberLeft.GetDirection());
}
//=== end GetDirMotorLeft ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalMotChambers::StartForwardMotorRight()
{
	this->MotorChamberRight.StartForward();
}
//=== end StartForwardMotorRight ===================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalMotChambers::StartBackwardMotorRight(void)
{
	this->MotorChamberRight.StartBackward();
}
//=== end StartBackwardMotorRight ==================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THalMotChambers::StopMotorRight(void)
{
	this->MotorChamberRight.Stop();
}
//=== end StopMotorRight ===========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u8 THalMotChambers::GetPwmMotorRight(void)
{
	return(this->MotorChamberRight.GetPwm());
}
//=== end GetPwmMotorRight =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
bool THalMotChambers::GetDirMotorRight(void)
{
	return(this->MotorChamberRight.GetDirection());
}
//=== end GetDirMotorRight =========================================================

/**********************************************************************************/
