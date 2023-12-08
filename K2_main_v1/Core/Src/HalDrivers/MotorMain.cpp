/**********************************************************************************/
/**
 *  @file MotorMain.cpp
 *  @brief Driver for main motor
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "MotorMain.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TMotorMain::Init()
{
	HAL_GPIO_WritePin(PULSE_ON_GPIO_Port, PULSE_ON_Pin, GPIO_PIN_SET);

	return(OsResult_Ok);
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotorMain::StartForward()
{
	HAL_GPIO_WritePin(SW_STATOR2_GPIO_Port, SW_STATOR2_Pin, GPIO_PIN_SET);
	this->Delay(100);
	HAL_GPIO_WritePin(MOTOR_ON_GPIO_Port, MOTOR_ON_Pin, GPIO_PIN_SET);
}
//=== end StartForward =============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotorMain::StartBackward()
{
	HAL_GPIO_WritePin(SW_STATOR2_GPIO_Port, SW_STATOR2_Pin, GPIO_PIN_RESET);
	this->Delay(100);
	HAL_GPIO_WritePin(MOTOR_ON_GPIO_Port, MOTOR_ON_Pin, GPIO_PIN_SET);
}
//=== end StartBackward ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotorMain::Stop()
{
	HAL_GPIO_WritePin(MOTOR_ON_GPIO_Port, MOTOR_ON_Pin, GPIO_PIN_RESET);
//	this->Delay(100);
//	HAL_GPIO_WritePin(SW_STATOR2_GPIO_Port, SW_STATOR2_Pin, GPIO_PIN_RESET);
}
//=== end Stop =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotorMain::BrakeOn()
{
	HAL_GPIO_WritePin(BREAK_ON_GPIO_Port, BREAK_ON_Pin, GPIO_PIN_SET);
}
//=== end BrakeOn ==================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotorMain::BrakeOff()
{
	HAL_GPIO_WritePin(BREAK_ON_GPIO_Port, BREAK_ON_Pin, GPIO_PIN_SET);
}
//=== end BrakeOff =================================================================

/**********************************************************************************/
