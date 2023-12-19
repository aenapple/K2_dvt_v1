/**********************************************************************************/
/**
 *  @file MotorChamber.cpp
 *  @brief Driver for chamber motors
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "MotorChamber.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotorChamber::Init(EMotorChamber motorChamber)
{
	if(motorChamber == MotorChamber_Left)
	{
		this->GPIOx_ON = ON_CHM1_MOT_GPIO_Port;
		this->GPIO_Pin_ON = ON_CHM1_MOT_Pin;
		this->GPIOx_DIR = DIR_CHM1_MOT_GPIO_Port;
		this->GPIO_Pin_DIR = DIR_CHM1_MOT_Pin;
	}
	else
	{
		this->GPIOx_ON = ON_CHM2_MOT_GPIO_Port;
		this->GPIO_Pin_ON = ON_CHM2_MOT_Pin;
		this->GPIOx_DIR = DIR_CHM2_MOT_GPIO_Port;
		this->GPIO_Pin_DIR = DIR_CHM2_MOT_Pin;
	}

}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotorChamber::StartForward()
{
	HAL_GPIO_WritePin(this->GPIOx_DIR, this->GPIO_Pin_DIR, GPIO_PIN_SET);
	HAL_GPIO_WritePin(this->GPIOx_ON, this->GPIO_Pin_ON, GPIO_PIN_SET);
}
//=== end StartForward =============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotorChamber::StartBackward()
{
	HAL_GPIO_WritePin(this->GPIOx_DIR, this->GPIO_Pin_DIR, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(this->GPIOx_ON, this->GPIO_Pin_ON, GPIO_PIN_SET);
}
//=== end StartBackward ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TMotorChamber::Stop()
{
	HAL_GPIO_WritePin(this->GPIOx_DIR, this->GPIO_Pin_DIR, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(this->GPIOx_ON, this->GPIO_Pin_ON, GPIO_PIN_RESET);
}
//=== end Stop =====================================================================

/**********************************************************************************/
