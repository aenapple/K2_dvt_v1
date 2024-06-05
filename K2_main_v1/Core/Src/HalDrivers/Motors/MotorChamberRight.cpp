/**********************************************************************************/
/**
 *  @file MotorChamberRight.cpp
 *  @brief Driver for Right chamber motors
 *
 *  @copyright (c) 2024 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "MotorChamberRight.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotorChamberRight::StartForward()
{
//	HAL_GPIO_WritePin(DIR_CHM2_MOT_GPIO_Port, DIR_CHM2_MOT_Pin, GPIO_PIN_SET);    // old
	HAL_GPIO_WritePin(DIR_CHM2_MOT_GPIO_Port, DIR_CHM2_MOT_Pin, GPIO_PIN_RESET);  // new
	HAL_GPIO_WritePin(ON_CHM2_MOT_GPIO_Port, ON_CHM2_MOT_Pin, GPIO_PIN_SET);
	this->pwm = 100;
	this->direction = true;  // forward
}
//=== end StartForward =============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotorChamberRight::StartBackward()
{
//	HAL_GPIO_WritePin(DIR_CHM2_MOT_GPIO_Port, DIR_CHM2_MOT_Pin, GPIO_PIN_RESET);  // old
	HAL_GPIO_WritePin(DIR_CHM2_MOT_GPIO_Port, DIR_CHM2_MOT_Pin, GPIO_PIN_SET);    // new
	HAL_GPIO_WritePin(ON_CHM2_MOT_GPIO_Port, ON_CHM2_MOT_Pin, GPIO_PIN_SET);
	this->pwm = 100;
	this->direction = false;  // backward
}
//=== end StartBackward ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotorChamberRight::Stop()
{
	HAL_GPIO_WritePin(DIR_CHM2_MOT_GPIO_Port, DIR_CHM2_MOT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ON_CHM2_MOT_GPIO_Port, ON_CHM2_MOT_Pin, GPIO_PIN_RESET);
	this->pwm = 0;
}
//=== end Stop =====================================================================

/**********************************************************************************/
