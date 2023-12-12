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
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;


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
		this->pTimer = &htim16;
	}
	else
	{
		this->pTimer = &htim17;
	}

	this->channel = TIM_CHANNEL_1;

	this->pwmStarted = true;

	this->Stop();

}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TPtcFan::Start(u8 pwm)
{
	TIM_OC_InitTypeDef sConfigOC;
	u8 tempPwm;


	if(pwm > 99)
	{
		tempPwm = 99;
	}
	else
	{
		if(pwm < 10)
		{
			tempPwm = 10;
		}
		else
		{
			tempPwm = pwm;
		}
	}

	if(this->pwmStarted)
	{
		if(this->pwm == tempPwm)
		{
			return;
		}

		this->Stop();

	}

	this->pwm = tempPwm;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = (u32)this->pwm;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_SET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	HAL_TIM_PWM_ConfigChannel(this->pTimer, &sConfigOC, this->channel);
	HAL_TIM_PWM_Start(this->pTimer, this->channel);

	this->pwmStarted = true;
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
	if(this->pwmStarted)
	{
		HAL_TIM_PWM_Stop(this->pTimer, this->channel);
		this->pwm = 0;
		this->pwmStarted = false;
	}
}
//=== end Stop =====================================================================

/**********************************************************************************/
