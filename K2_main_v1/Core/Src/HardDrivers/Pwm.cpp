/**********************************************************************************/
/**
 *  @file Pwm.hpp
 *  @brief Driver PWM
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "Pwm.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TPwm::InitPwm(TIM_HandleTypeDef* pTimer, u32 channel)
{
	this->pwm = 0;
	this->pTimer = pTimer;
	this->channel = channel;

	this->pwmStarted = true;

	this->StopPwm();
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TPwm::StartPwm(u8 pwm)
{
	TIM_OC_InitTypeDef sConfigOC;
	u8 tempPwm;


	if(pwm > 99) tempPwm = 99;
	else tempPwm = pwm;

	if(this->pwmStarted)
	{
		if(this->pwm == tempPwm)
		{
			return;
		}

		this->StopPwm();

	}

	this->pwm = tempPwm;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = (u32)this->pwm;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
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
void TPwm::StopPwm(void)
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
