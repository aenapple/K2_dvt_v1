/**********************************************************************************/
/**
 *  @file Motor.cpp
 *  @brief ...
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "Motor.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotor::Init()
{
	this->Stop();
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotor::StartForward()
{
	HAL_GPIO_WritePin(MOTOR_INA_GPIO_Port, MOTOR_INA_Pin, GPIO_PIN_SET);
}
//=== end StartForward =============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotor::StartBackward()
{
	HAL_GPIO_WritePin(MOTOR_INB_GPIO_Port, MOTOR_INB_Pin, GPIO_PIN_SET);
}
//=== end StartBackward ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotor::Stop()
{
	HAL_GPIO_WritePin(MOTOR_INA_GPIO_Port, MOTOR_INA_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_INB_GPIO_Port, MOTOR_INB_Pin, GPIO_PIN_RESET);
}
//=== end Stop =====================================================================

/**********************************************************************************/
